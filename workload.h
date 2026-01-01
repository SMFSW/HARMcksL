/*!\file workload.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Workload estimation utilities
** \note Define \c IT_WORKLOAD symbol at project level to use workload estimation module functionalities
** \note Loop period values will only be relevant in case of sequential code (you will get task period for RTOS based code)
** \details Handler has to be called in the main sequential loop (or in a periodic task).
** 			Init function has to be called prior to main loop.
** 			\c WORLOAD_IT_IN macro has to be put at the start of every interrupt, \c WORLOAD_IT_OUT macro before return of every interrupt
** \warning M0/M0+ cores doesn't have core debug trace peripheral, time count shall be generated using a dedicated TIM peripheral.
** \note With cores other than M0/M0+, core debug trace timer is used by default.
** \note In case of M0/M0+ core or willing to use a TIM peripheral on purpose, \c WORKLOAD_TIM_INST symbol has to be defined with proper TIM instance at project level.
**/
/****************************************************************/
#ifndef WORKLOAD_H__
	#define WORKLOAD_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


#ifdef IT_WORKLOAD

#if defined(DWT) && !defined(WORKLOAD_TIM_INST)
#define GET_COUNT()			DWT->CYCCNT
#else
#define GET_COUNT()			__HAL_TIM_GET_COUNTER(WORKLOAD_TIM_INST)
#endif

extern volatile uint32_t	it_ticks_accumulator;
extern volatile uint32_t	it_ticks_start;
extern volatile bool		it_counts_ongoing;

#define WORKLOAD_IT_IN()	bool already_counting = it_counts_ongoing ? true : false;	\
							it_counts_ongoing = true;									\
							if (!already_counting)	{ it_ticks_start = GET_COUNT(); }

#define WORKLOAD_IT_OUT()	if (!already_counting) {									\
								it_ticks_accumulator += (GET_COUNT() - it_ticks_start);	\
								it_counts_ongoing = false; }

#else

#define WORKLOAD_IT_IN()	//!< Dummy macro for standard targets
#define WORKLOAD_IT_OUT()	//!< Dummy macro for standard targets

#endif


/*!\enum eWorkload_val
** \brief Workload type value enum
**/
typedef enum {
	Workload_average = 0,	//!< Average value
	Workload_worst,			//!< Worst value
	Workload_best			//!< Best value
} eWorkload_val;


/*!\brief Get interrupts load percentage
** \param[in] type - Type to get
** \return Interrupts load percentage
**/
float get_Workload_load_perc(const eWorkload_val type);


/*!\brief Get main loop period
** \note Loop period values will only be relevant in case of sequential code (you will get task period for RTOS based code)
** \param[in] type - Type to get
** \return Loop period
**/
uint32_t get_Workload_loop_period(const eWorkload_val type);


/*!\brief Init Workload functionalities
** \weak Function declared as weak, can be customly implemented in project if special timer is required
** \return FctERR - Error code
**/
FctERR Workload_init(void);

/*!\brief Set number of timer ticks in a µs
** \weak Function declared as weak, can be customly implemented in project if special timer is required
** \note Useful in case of \ref Workload_init custom implementation
** \param[in] ticks - Ticks in a µs
**/
void set_Workload_ticks_us(const float ticks);


/*!\brief Set workload estimation time window (in ms)
** \param[in] per - Time window in ms
**/
void set_Workload_window(const uint32_t per);


/*!\brief Workload handler
** \note This function shall be called in main loop or periodic task
**/
void Workload_handler(void);


/*!\brief Print workload results
**/
void Workload_show_results(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif /* WORKLOAD_H__ */
/****************************************************************/
