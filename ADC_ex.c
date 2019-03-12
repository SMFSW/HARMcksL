/*!\file ADC_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2019, SMFSW
** \brief Simple extension for ADCs
** \note Define USE_ADC_EX symbol at project level to use ADC_ex functionalities
** \note Configuration done calling ADC_cfg.h file
**/
/****************************************************************/
#include "sarmfsw.h"
#include "ADC_ex.h"

#if defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ADC_EX)
/****************************************************************/


#if (	(	defined(STM32F0)		\
		 || defined(STM32F2)		\
		 || defined(STM32F4))		\
	 && defined(ADC_CALIBRATION))
// TODO: see all families supporting calibration
#error "STM32F4 family doesn't support ADC peripherals calibration."
#endif

#if !defined(ADC_USE_VREF) && defined(ADC_COMPENSATION)
#error "ADC_USE_VREF shall be defined when using ADC_COMPENSATION."
#endif


#if !defined(Def_V25) || !defined(Def_AvgSlope)
#if defined(TEMP_CALC_V25)
	#if defined(Def_V25)
	#undef Def_V25
	#endif
	#if defined(Def_AvgSlope)
	#undef Def_AvgSlope
	#endif

	#warning "You should define both Def_V25 & Def_AvgSlope with parameters given in datasheet, using STM32 family presumed default instead!"

	#if defined(STM32F0)
		#define Def_V30				(1430.0f)	//!< Temperature at 30°c (in mV)
		//!\warning	Beware F0 family has reference at 30°C
		#define Def_AvgSlope		(4.3f)		//!< Average Slope (in mV)
	#elif defined(STM32F1)
		// TODO: add F1 temperature constants symbols
	#elif defined(STM32F2)
		#define Def_V25				(760.0f)	//!< Temperature at 25°c (in mV)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32F3)
		#define Def_V25				(1430.0f)	//!< Temperature at 25°c (in mV)
		#define Def_AvgSlope		(4.3f)		//!< Average Slope (in mV)
	#elif defined(STM32F4)
		#define Def_V25				(760.0f)	//!< Temperature at 25°c (in mV)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32F7)
		#define Def_V25				(760.0f)	//!< Temperature at 25°c (in mV)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32G0)
		#define Def_V30				(760.0f)	//!< Temperature at 30°c (in mV)
		//!\warning	Beware G0 family has reference at 30°C
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32H7)
		// TODO: add H7 temperature constants symbols
	#elif defined(STM32L0)
		#define Def_V130			(670.0f)	//!< Temperature at 130°c (in mV)
		//!\warning	Beware L0 family has reference at 130°C
		#define Def_AvgSlope		(1.48f)		//!< Average Slope (in mV)
	#elif defined(STM32L1)
		#define Def_V110			(628.8f)	//!< Temperature at 110°c (in mV)
		//!\warning	Beware L1 family has reference at 110°C
		#define Def_AvgSlope		(1.61f)		//!< Average Slope (in mV)
	#elif defined(STM32L4)
		#define Def_V30				(760.0f)	//!< Temperature at 30°c (in mV)
		//!\warning	Beware L4 family has reference at 30°C
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#endif
#endif
#endif


#if !defined(STM32F2)
#if defined(ADC_COMPENSATION) && !defined(STM32_VREF_CAL)
#define STM32_VREF_CAL		(VAL_AT(VREF_CAL_ADDR, uint16_t))		//!< VRef (ADC) calibration address content
#endif
#if defined(ADC_USE_TEMP) && !defined(TEMP_CALC_V25)
#ifndef STM32_TS_CAL1
#define STM32_TS_CAL1		(VAL_AT(TS_CAL1_ADDR, uint16_t))		//!< Temp sensor ADC raw data acquired at 30°C, VDDA=3.3V address content
#endif
#ifndef STM32_TS_CAL2
#define STM32_TS_CAL2		(VAL_AT(TS_CAL2_ADDR, uint16_t))		//!< Temp sensor raw data acquired at 110°C, VDDA=3.3V address content
#endif
#endif
#endif


#ifndef Def_VIn
#define Def_VIn				(3.3f)		//!< Vin voltage (in V)
//!< Def_VIn can be defined in globals.h or at project level if power supply is not 3.3VDC
#endif

#define Def_Step			(Def_VIn / 4095.0f)		//!< Step value (in V)


#ifndef ADC_NB
#define	ADC_NB				1			//!< Number of ADC peripherals used
#warning "You should define number of ADC peripheral used with ADC_NB macro. Assuming 1 ADC peripheral in use."
#endif

#ifndef ADC_NB_CHAN
#define	ADC_NB_CHAN			2			//!< Number of ADC channels used (per peripheral)
#warning "You should define number of ADC channels per peripheral used with ADC_NB_CHAN macro. Assuming 2 ADC channels per peripheral in use."
#endif

#define TOTAL_ADC_CHANS		(ADC_NB * ADC_NB_CHAN)	//!< Total number of ADC channels


#ifndef ADC_SAMP_BUF_SIZE
#define ADC_SAMP_BUF_SIZE	4			//!< Size of the input buffer per analog input
//!< ADC_SAMP_BUFF_SIZE can be defined in globals.h or at project level if higher amount of samples required
#endif

typedef struct AnalogTab {
	uint16_t	Array[ADC_SAMP_BUF_SIZE];	//!< Samples
	//uint16_t	Average;					//!< Averaged samples
} AnalogTab;


#if (ADC_NB == 2)
	typedef uint16_t	DMA_sz_t;	//!< Size of DMA buffer cells (special case: 1 cell for 2 ADC values)
#else
	typedef uint32_t	DMA_sz_t;	//!< Size of DMA buffer cells (1 cell per ADC value)
#endif


extern const ADC_Cfg	ADCConfig[Adc_MAX];

static __IO uint32_t	ADCChan = 0, ADCMeasureIdx = 0;
static __IO DMA_sz_t	ADCbuffer_DMA[TOTAL_ADC_CHANS];	//!< DMA ADC buffer
static __IO AnalogTab	ADCbuffer[TOTAL_ADC_CHANS];		//!< ADC buffer


/**!\brief Compute converted value for given channel
**	\param[in] input - Input index
**	\return Channel converted value
**/
static float ADC_ConvertVal(const eAnalogInput input)
{
	#if defined(ADC_COMPENSATION)
		const uint16_t	vrefint_cal = STM32_VREF_CAL;									// read Vref calibration from flash
		const uint16_t	vrefint_dat = ADC_GetRawVal(Adc_Vref);							// read Vref data
		const int16_t	in_raw = (ADC_GetRawVal(input) * vrefint_cal) / vrefint_dat;	// read input value and apply compensation
	#else
		const int16_t	in_raw = ADC_GetRawVal(input);									// read input value
	#endif
	const float			in_v = in_raw * Def_Step;										// Convert input value in Volts
	float				val = 0.0f;

	if (input < Adc_MAX)
	{
		if (ADCConfig[input].conv != NULL)		{ val = ADCConfig[input].conv(in_v); }
		else
		{
			switch (input)
			{
				#if defined(ADC_USE_VREF)
					case Adc_Vref:
						#if defined(ADC_COMPENSATION)
							val = vrefint_dat * Def_Step;
						#else
							val = in_v;
						#endif
						break;
				#endif

				#if defined(ADC_USE_TEMP)
					case Adc_Temp:
						// TODO: code temperature following families (F3/F4 handled so far)
						#ifdef TEMP_CALC_V25
							// With raw converted value to mV
							#if defined(STM32F3)
								val = ((Def_V25 - (in_v * 1000.0f)) / Def_AvgSlope) + (float) 25;
							#elif defined(STM32F4)
								val = (((in_v * 1000.0f) - Def_V25) / Def_AvgSlope) + (float) 25;
							#endif
						#else
							// Temp = (TC2 - TC1) / (ValC2 - ValC1) * (ValTS - ValC1) + TC1
							// Temp = 80 / (TS_CAL2 - TS_CAL1) * (ValTS - TS_CAL1) + 30
							val = ((80 * (in_raw - STM32_TS_CAL1)) / (float) (STM32_TS_CAL2 - STM32_TS_CAL1)) + (float) 30;
						#endif
						break;
				#endif

				#if defined(ADC_USE_VBAT)
					case Adc_Vbat:
						val = in_v * 2.0f;
						break;
				#endif

				default:
					val = in_v;
					break;
			}
		}
	}

	return val;
}


/**!\brief Get ADC num and channel corresponding to input
**	\param[in,out] adc - Pointer to ADC instance result
**	\param[in,out] chan - Pointer to channel result
**	\return Error code
**/
static FctERR NONNULL__ ADC_GetChan(uint8_t * const adc, uint8_t * const chan, const eAnalogInput input)
{
	if (input >= Adc_MAX)	{ return ERROR_VALUE; }

	*adc = ADCConfig[input].adc;
	*chan = ADCConfig[input].chan;

	return ERROR_OK;
}


uint16_t ADC_GetRawVal(const eAnalogInput input)
{
	uint8_t adc_num, chan;

	if (ADC_GetChan(&adc_num, &chan, input) != ERROR_OK)	{ return 0; }

	const uint8_t idx = adc_num + (chan * ADC_NB);
	return RestrictedAverage_WORD(ADCbuffer[idx].Array, SZ_OBJ(ADCbuffer[idx].Array, uint16_t));
}

float ADC_GetConvertedVal(const eAnalogInput input)
{
	if (input >= Adc_MAX)	{ return 0; }

	return ADC_ConvertVal(input);
}


FctERR ADC_Start(void)
{
	FctERR err = ERROR_OK;
	#if defined(ADC_CALIBRATION)
		static bool calib_done = false;
	#endif

	#if (ADC_NB == 0)
		err = ERROR_NOTAVAIL;
	#elif (ADC_NB == 1)
		#if defined(ADC_CALIBRATION)
			if (!calib_done)
			{
				err |= HALERRtoFCTERR(HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED));
				calib_done = true;
			}
		#endif

		err |= HALERRtoFCTERR(HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADCbuffer_DMA, TOTAL_ADC_CHANS));
	#elif (ADC_NB == 2)
		#if defined(ADC_CALIBRATION)
			if (!calib_done)
			{
				err |= HALERRtoFCTERR(HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED));
				err |= HALERRtoFCTERR(HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED));
				calib_done = true;
			}
		#endif

		err |= HALERRtoFCTERR(HAL_ADC_Start(&hadc2));
		err |= HALERRtoFCTERR(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t *) ADCbuffer_DMA, TOTAL_ADC_CHANS / 2));
	#elif (ADC_NB == 3)
		#if defined(ADC_CALIBRATION)
			if (!calib_done)
			{
				err |= HALERRtoFCTERR(HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED));
				err |= HALERRtoFCTERR(HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED));
				err |= HALERRtoFCTERR(HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED));
				calib_done = true;
			}
		#endif

		err |= HALERRtoFCTERR(HAL_ADC_Start(&hadc3));
		err |= HALERRtoFCTERR(HAL_ADC_Start(&hadc2));
		err |= HALERRtoFCTERR(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t *) ADCbuffer_DMA, TOTAL_ADC_CHANS));
	#endif

		return err;
}

FctERR ADC_Stop(void)
{
	FctERR err;

	#if (ADC_NB == 1)
		err = HALERRtoFCTERR(HAL_ADC_Stop_DMA(&hadc1));
	#elif (ADC_NB > 1)
		err = HALERRtoFCTERR(HAL_ADCEx_MultiModeStop_DMA(&hadc1));
		err |= HALERRtoFCTERR(HAL_ADC_Stop(&hadc2));
	#endif
	#if (ADC_NB > 2)
		err |= HALERRtoFCTERR(HAL_ADC_Stop(&hadc3));
	#endif

	return err;
}


/**!\brief Conversion complete callback in non blocking mode
**	\strong Strong implementation of HAL_ADC_ConvCpltCallback in the library (will get precedence over HAL function)
**	\warning If DMA used, same number of channels to convert have to be declared between ADCs for proper handling
**	\param[in] hadc - ADC handle
**/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	// Store value in buffers
	ADCbuffer[ADCChan].Array[ADCMeasureIdx] = (uint16_t) ADCbuffer_DMA[ADCChan];

	if (++ADCChan >= TOTAL_ADC_CHANS)
	{
		ADCChan = 0;

		if (++ADCMeasureIdx >= ADC_SAMP_BUF_SIZE)	{ ADCMeasureIdx = 0; }
	}
}


/******************************************************************/
#endif	/* defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ADC_EX) */
/******************************************************************/
