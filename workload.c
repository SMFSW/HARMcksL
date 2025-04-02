/*!\file workload.c
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
** \brief Workload estimation utilities
** \note Define IT_WORKLOAD symbol at project level to use workload estimation module functionalities
** \note Loop period values will only be relevant in case of sequential code (you will get task period for RTOS based code)
** \details Handler has to be called in the main sequential loop (or in a periodic task).
** 			Init function has to be called prior to main loop.
** 			WORLOAD_IT_IN macro has to be put at the start of every interrupt, WORLOAD_IT_OUT macro before return of every interrupt
** \warning M0/M0+ cores doesn't have core debug trace peripheral, time count shall be generated using a dedicated TIM peripheral.
** \note With cores other than M0/M0+, core debug trace timer is used by default.
** \note In case of M0/M0+ core or willing to use a TIM peripheral on purpose, WORKLOAD_TIM_INST symbol has to be defined with proper TIM instance at project level.
**/
/****************************************************************/
#include <stdio.h>

#include "sarmfsw.h"
#include "TIM_ex.h"
#include "tick_utils.h"
#include "workload.h"

#ifdef IT_WORKLOAD
/****************************************************************/


volatile uint32_t	it_ticks_accumulator = 0, it_ticks_start;
volatile bool		it_counts_ongoing = false;

static uint32_t		workload_window;	//!< Workload measurement period (in ms)
static float		ticks_us;			//!< Counter ticks for a microsecond

static uint32_t		avg_load_perc = 0, wst_load_perc = 0, bst_load_perc = 10000;				// in hundreds of % (2 decimals included as integer)
static uint32_t		avg_loop_period = 0, wst_loop_period = 0, bst_loop_period = (uint32_t) -1;	// in µs


float get_Workload_load_perc(const eWorkload_val type)
{
	float val = 0.0f;

	if (type == Workload_average)		{ val = avg_load_perc; }
	else if (type == Workload_worst)	{ val = wst_load_perc; }
	else if (type == Workload_best)		{ val = bst_load_perc; }

	return val / 100.0f;
}


uint32_t get_Workload_loop_period(const eWorkload_val type)
{
	uint32_t val = 0;

	if (type == Workload_average)		{ val = avg_loop_period; }
	else if (type == Workload_worst)	{ val = wst_loop_period; }
	else if (type == Workload_best)		{ val = bst_loop_period; }

	return val;
}


__WEAK FctERR Workload_init(void)
{
	HAL_StatusTypeDef err;

#if defined(DWT) && !defined(WORKLOAD_TIM_INST)
	err = init_Delay_Generator();
	if (err)	{ goto ret; }

	set_Workload_ticks_us(get_TicksPerMicrosecond());
#else
	err = set_TIM_Tick_Freq(WORKLOAD_TIM_INST, 1000000);
	if (err)	{ goto ret; }
	err = HAL_TIM_Base_Start(WORKLOAD_TIM_INST);
	if (err)	{ goto ret; }

	set_Workload_ticks_us(1.0f);
#endif

	set_Workload_window(100);

	ret:
	return HALERRtoFCTERR(err);
}


__WEAK void set_Workload_ticks_us(const float ticks) {
	ticks_us = ticks; }

void set_Workload_window(const uint32_t per) {
	workload_window = per; }


void Workload_handler(void)
{
	static uint32_t nb_loops = 0;
	static uint32_t hStart = 0;

	nb_loops++;

	if (TPSSUP_MS(hStart, workload_window))
	{
		diInterrupts();
		const uint32_t acc = it_ticks_accumulator;
		it_ticks_accumulator = 0;
		enInterrupts();

		// Compute real time spent in loops (in case any loop lasts too long for proper computations with time window value)
		const uint32_t real_time_spent = OVF_DIFF(HALTicks(), hStart);

		// Time spent in interrupt (in us)
		const uint32_t time_it = acc / ticks_us;

		// 10000 is for percent with 2 decimals coded as integer, 1000 is for us to ms
		//const uint32_t load_perc = (time_it * 10000) / (real_time_spent * 1000);
		const uint32_t load_perc = (time_it * 10) / real_time_spent;

		// Subtract time spent in interrupt from time window, to get better approximation of loop period
		const uint32_t loop_period = ((real_time_spent * 1000) - time_it) / nb_loops;	// 1000 to get result in µs

		avg_load_perc = (!avg_load_perc) ? load_perc : (avg_load_perc + load_perc) / 2;
		bst_load_perc = min(load_perc, bst_load_perc);
		wst_load_perc = max(load_perc, wst_load_perc);

		avg_loop_period = (!avg_loop_period) ? loop_period : (avg_loop_period + loop_period) / 2;
		bst_loop_period = min(loop_period, bst_loop_period);
		wst_loop_period = max(loop_period, wst_loop_period);

		nb_loops = 0;
		hStart = HALTicks();
	}
}


void Workload_show_results(void)
{
	printf("Workload estimations: %ldms time window\r\n", workload_window);
	printf("\tloop period (us)\tload (%%)\r\n");
	printf("avg:\t%ld\t\t\t%ld.%02ld\r\n", avg_loop_period, avg_load_perc / 100, avg_load_perc % 100);
	printf("best:\t%ld\t\t\t%ld.%02ld\r\n", bst_loop_period, bst_load_perc / 100, bst_load_perc % 100);
	printf("worst:\t%ld\t\t\t%ld.%02ld\r\n", wst_loop_period, wst_load_perc / 100, wst_load_perc % 100);
}


#endif

