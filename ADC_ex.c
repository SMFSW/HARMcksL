/*!\file ADC_ex.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Simple extension for ADCs
** \details ADC_ex is meant to automate ADC conversions using DMA.
** 			- DMA must be configured for ADC peripherals:
** 				- Peripheral to Memory
** 				- Circular Mode
** 				- Increment Memory
** 				- Data width Word for both peripheral and memory
** 			- ADC configuration (no need to active ADC global interrupt, DMA will handle conversion complete callback):
** 				- Scan conversion mode across channels enabled
** 				- Continuous conversion is optional (as DMA continuous request handles the behavior)
** 				- DMA continuous request when available (if disabled, user has to start conversions manually when needed)
** 				- Interrupt generated at end of sequence of conversion
** \note Define \c USE_ADC_EX symbol at project level to use ADC_ex functionalities
** \note Configuration done including ADC_cfg.h file
**/
/****************************************************************/
#include "sarmfsw.h"
#include "ADC_ex.h"

#if defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ADC_EX)
/****************************************************************/

#if defined(STM32C0) || defined(STM32H5) || defined(STM32MP1) || defined(STM32WBA)
#warning "Current STM32 family (C0/H5/MP11/WBA) is probably not fully compliant! Some symbols may need to be defined manually."
#endif


#if (	(	defined(STM32F0)		\
		 || defined(STM32F2)		\
		 || defined(STM32F4)		\
		 || defined(STM32F7)		\
		 || defined(STM32L1))		\
	 && defined(ADC_CALIBRATION))
#undef ADC_CALIBRATION
#warning "Current STM32 family (F0/F2/F4/F7/L1) doesn't support ADC peripheral calibration. Undef ADC_CALIBRATION for you!"
#endif

#if defined(STM32F1) && defined(ADC_COMPENSATION)
#undef ADC_COMPENSATION
#warning "Current STM32 family (F1) doesn't have VREF_CAL register. Undef ADC_COMPENSATION for you!"
#endif

#if defined(TEMP_CALC_VTEMP) && (defined(STM32L0) || defined(STM32L1))
#undef TEMP_CALC_VTEMP
#warning "Current STM32 family (L0/L1) only use VREF_CAL registers for temperature computation. Undef TEMP_CALC_VTEMP for you!"
#endif

#if !defined(ADC_USE_VREF) && defined(ADC_COMPENSATION)
#error "ADC_USE_VREF shall be defined when using ADC_COMPENSATION."
#endif


#if !defined(Def_VCal)
	#if defined(STM32F0)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#elif defined(STM32F1)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#elif defined(STM32F2)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#elif defined(STM32F3)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#elif defined(STM32F4)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#elif defined(STM32F7)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#elif defined(STM32G0)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32G4)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32H7)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#elif defined(STM32L0)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32L1)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32L4)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32L5)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32U5)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32WB)
		#define Def_VCal			(3000)		//!< Calibration values performed at 3V
	#elif defined(STM32WL)
		#define Def_VCal			(3300)		//!< Calibration values performed at 3.3V
	#else
	#error "You shall define Def_VCal with parameters given in datasheet!"
	#endif
#endif


#if !defined(Def_AvgSlope)
	#warning "You should define Def_AvgSlope with parameters given in datasheet, using STM32 family common default values instead!"

	#if defined(STM32F0)
		#define Def_AvgSlope		(4.3f)		//!< Average Slope (in mV)
	#elif defined(STM32F1)
		#define Def_AvgSlope		(4.3f)		//!< Average Slope (in mV)
	#elif defined(STM32F2)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32F3)
		#define Def_AvgSlope		(4.3f)		//!< Average Slope (in mV)
	#elif defined(STM32F4)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32F7)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32G0)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32G4)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32H7)
		#define Def_AvgSlope		(2.0f)		//!< Average Slope (in mV)
	#elif defined(STM32L0)
		#define Def_AvgSlope		(1.48f)		//!< Average Slope (in mV)
	#elif defined(STM32L1)
		#define Def_AvgSlope		(1.61f)		//!< Average Slope (in mV)
	#elif defined(STM32L4)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32L5)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32U5)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32WB)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#elif defined(STM32WL)
		#define Def_AvgSlope		(2.5f)		//!< Average Slope (in mV)
	#endif
#endif


#if !defined(Def_VTemp)
	#warning "You should define Def_VTemp with parameters given in datasheet, using STM32 family common default values instead!"

	#if defined(STM32F0)
		#define Def_VTemp			(1430.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32F1)
		#define Def_VTemp			(1430.0f)	//!< Temperature at 25°C (in mV)
	#elif defined(STM32F2)
		#define Def_VTemp			(760.0f)	//!< Temperature at 25°C (in mV)
	#elif defined(STM32F3)
		#define Def_VTemp			(1430.0f)	//!< Temperature at 25°C (in mV)
	#elif defined(STM32F4)
		#define Def_VTemp			(760.0f)	//!< Temperature at 25°C (in mV)
	#elif defined(STM32F7)
		#define Def_VTemp			(760.0f)	//!< Temperature at 25°C (in mV)
	#elif defined(STM32G0)
		#define Def_VTemp			(760.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32G4)
		#define Def_VTemp			(760.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32H7)
		#define Def_VTemp			(620.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32L0)
		//!< \note For reference only, not used in code
		#define Def_VTemp			(670.0f)	//!< Temperature at 130°C (in mV)
	#elif defined(STM32L1)
		//!< \note For reference only, not used in code
		#define Def_VTemp			(628.8f)	//!< Temperature at 110°C (in mV)
	#elif defined(STM32L4)
		#define Def_VTemp			(760.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32L5)
		#define Def_VTemp			(760.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32U5)
		#define Def_VTemp			(752.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32WB)
		#define Def_VTemp			(760.0f)	//!< Temperature at 30°C (in mV)
	#elif defined(STM32WL)
		#define Def_VTemp			(760.0f)	//!< Temperature at 30°C (in mV)
	#endif
#endif

#if !defined(Def_VBatFactor)
	#if		defined(STM32F4) || defined(STM32F7) || defined(STM32H5) || defined(STM32H7) || defined(STM32U5)
		#define Def_VBatFactor		(4.0f)		//!< 4x factor (\warning *2.0f for STM32F40xx and STM32F41xx devices)
	#elif	defined(STM32G0) || defined(STM32G4) || defined(STM32L4) || defined(STM32L5) || \
			defined(STM32WB) || defined(STM32WBA) || defined(STM32WL)
		#define Def_VBatFactor		(3.0f)		//!< 3x factor
	#else
		#define Def_VBatFactor		(2.0f)		//!< 2x factor
	#endif
#endif


#if !defined(STM32F1)
#if !defined(STM32F2)

#if !defined(STM32_VREF_CAL)
#ifndef VREF_CAL_ADDR
#error "Refer to datasheet to find VREF_CAL address!"
#endif
#define STM32_VREF_CAL		(VAL_AT(VREF_CAL_ADDR, uint16_t))		//!< VRef (ADC) calibration address content
#endif

#if !defined(TEMP_CALC_VTEMP)
#if !defined(STM32_TS_CAL1)
#ifndef TS_CAL1_ADDR
#error "Refer to datasheet to find TS_CAL1 address!"
#endif
#define STM32_TS_CAL1		(VAL_AT(TS_CAL1_ADDR, uint16_t))		//!< Temp sensor ADC raw data acquired at Lower temperature address content
#endif
#if !defined(STM32_TS_CAL2)
#ifndef TS_CAL2_ADDR
#error "Refer to datasheet to find TS_CAL2 address!"
#endif
#define STM32_TS_CAL2		(VAL_AT(TS_CAL2_ADDR, uint16_t))		//!< Temp sensor ADC raw data acquired at Higher temperature address content
#endif
#endif

#endif
#endif

#ifndef ADC_NB
#define	ADC_NB				1UL			//!< Number of ADC peripherals used
#warning "You should define number of ADC peripheral used with ADC_NB macro. Assuming 1 ADC peripheral in use."
#endif

#ifndef ADC_NB_CHAN
#define	ADC_NB_CHAN			2UL			//!< Number of ADC channels used (per peripheral)
#warning "You should define number of ADC channels per peripheral used with ADC_NB_CHAN macro. Assuming 2 ADC channels per peripheral in use."
#endif

#define TOTAL_ADC_CHANS		(ADC_NB * ADC_NB_CHAN)	//!< Total number of ADC channels


#ifndef ADC_SAMP_BUF_SIZE
#define ADC_SAMP_BUF_SIZE	4UL			//!< Size of the input buffer per analog input
//! \note ADC_SAMP_BUFF_SIZE can be defined in adc_cfg.h, globals.h or at project level if higher amount of samples required
#endif


#ifndef ADC_REFRESH_PERIOD
#define ADC_REFRESH_PERIOD	20UL		//!< ADC conversions results refreshing period (default value)
#endif


#if (ADC_NB == 2)
	typedef uint16_t	DMA_sz_t;	//!< Size of DMA buffer cells (special case: 1 cell for 2 ADC values)
#else
	typedef uint32_t	DMA_sz_t;	//!< Size of DMA buffer cells (1 cell per ADC value)
#endif

static __IO DMA_sz_t	bufferDMA[TOTAL_ADC_CHANS];	//!< DMA buffer


/*!\struct ADCex_t
** \brief ADCex structure
**/
typedef struct {
	__IO uintCPU_t				idxMeasure;										//!< Current measure index (for array filling)
	__IO uint16_t				bufferADC[TOTAL_ADC_CHANS][ADC_SAMP_BUF_SIZE];	//!< ADC buffer
#if !defined(ADC_CONTINUOUS_SAMPLING) || defined(ADC_AUTO_RAW_VAL_CONV)
	uint32_t					hUpdate;										//!< Last ADC sampling or conversion time
#endif
#if defined(ADC_AUTO_RAW_VAL_CONV)
	float						bufferConv[Adc_MAX];							//!< ADC converted values array
	bool						conv;											//!< Conversion to perform flag
#endif
	struct {
		uint32_t				period;											//!< Sampling or Conversion period
		const ADC_Cfg * const	aADCCfg;										//!< Pointer to ADCConfig array
	} cfg;
} ADCex_t;


extern const ADC_Cfg	ADCConfig[Adc_MAX];		//!< ADCConfig external declaration

ADCex_t ADC_ex = {	0, {{ 0 }},
					#if !defined(ADC_CONTINUOUS_SAMPLING) || defined(ADC_AUTO_RAW_VAL_CONV)
					0,
					#endif
					#if defined(ADC_AUTO_RAW_VAL_CONV)
					{ 0 }, false,
					#endif
					{ ADC_REFRESH_PERIOD, ADCConfig } };


/**!\brief Get ADC num and channel corresponding to input
**	\param[in,out] adc - Pointer to ADC instance result
**	\param[in,out] chan - Pointer to channel result
**	\param[in] input - Input index
**	\return Error code
**/
static FctERR NONNULL__ ADC_GetChan(uintCPU_t * const adc, uintCPU_t * const chan, const eAnalogInput input)
{
	FctERR err = ERROR_OK;

	if (input >= Adc_MAX)	{ err = ERROR_VALUE; }
	else
	{
		*adc = ADC_ex.cfg.aADCCfg[input].adc;
		*chan = ADC_ex.cfg.aADCCfg[input].chan;
	}

	return err;
}


/*!\brief Convert ADC channel raw value
** \param[in] input - Input index
** \return Channel converted value
**/
static float ADC_ConvertRAW(const eAnalogInput input)
{
	float val = 0.0f;

	if (input < Adc_MAX)
	{
		#if defined(ADC_COMPENSATION)

		const uint16_t	vrefint_cal = STM32_VREF_CAL;									// read Vref calibration from flash
		const uint16_t	vrefint_dat = ADC_GetRawVal(Adc_Vref);							// read Vref data
		if (vrefint_dat == 0)	{ goto ret; }											// Preventing HW faults when MCU starts with slow configured ADC acquisition

		const int16_t	in_raw = (ADC_GetRawVal(input) * vrefint_cal) / vrefint_dat;	// read input value and apply compensation
		const float		in_v = in_raw * Def_ADCStep(Def_VCal);							// Convert input value in Volts (in regard of calibration voltage)

		#else

		const int16_t	in_raw = ADC_GetRawVal(input);									// read input value
		const float		in_v = in_raw * Def_ADCStep(Def_VAlim);							// Convert input value in Volts (in regard of power supply voltage)

		#endif

		if (ADCConfig[input].conv != NULL)
		{
			val = ADCConfig[input].conv(in_v);
		}
		else
		{
			switch (input)
			{
				#if defined(ADC_USE_VREF)
				case Adc_Vref:
				{
					#if defined(ADC_COMPENSATION)
						val = vrefint_dat * Def_ADCStep(Def_VAlim);		// Always using full scale power supply voltage
					#else
						val = in_v;
					#endif
				}
				break;
				#endif

				#if defined(ADC_USE_TEMP)
				case Adc_Temp:
				{
					#ifdef TEMP_CALC_VTEMP
						// With raw converted value to mV
						#if defined(STM32F0) || defined(STM32G0) || defined(STM32G4) || defined(STM32H5) || defined(STM32H7) || \
							defined(STM32L4) || defined(STM32L5) || defined(STM32U5) || defined(STM32WB) || defined(STM32WBA) || defined(STM32WL)
							val = ((Def_VTemp - (in_v * 1000.0f)) / Def_AvgSlope) + 30.0f;
						#elif defined(STM32F1) || defined(STM32F3)
							val = ((Def_VTemp - (in_v * 1000.0f)) / Def_AvgSlope) + 25.0f;
						#elif defined(STM32F2) || defined(STM32F4) || defined(STM32F7)
							val = (((in_v * 1000.0f) - Def_VTemp) / Def_AvgSlope) + 25.0f;
						#endif
					#else
						// Temp = (TC2 - TC1) / (ValC2 - ValC1) * (ValTS - ValC1) + TC1
						#if defined(STM32G0) || defined(STM32G4) || defined(STM32L0) || defined(STM32L5) || \
							defined(STM32U5) || defined(STM32WB) || defined(STM32WBA) || defined(STM32WL)
							// Temp = 100 / (TS_CAL2 - TS_CAL1) * (ValTS - TS_CAL1) + 30
							val = ((float) (100 * (in_raw - STM32_TS_CAL1)) / (float) (STM32_TS_CAL2 - STM32_TS_CAL1)) + 30.0f;
						#else
							// Temp = 80 / (TS_CAL2 - TS_CAL1) * (ValTS - TS_CAL1) + 30
							val = ((float) (80 * (in_raw - STM32_TS_CAL1)) / (float) (STM32_TS_CAL2 - STM32_TS_CAL1)) + 30.0f;
						#endif
					#endif
				}
				break;
				#endif

				#if defined(ADC_USE_VBAT)
				case Adc_Vbat:
				{
					val = in_v * Def_VBatFactor;
				}
				break;
				#endif

				default:
				{
					val = in_v;
				}
				break;
			}
		}
	}

	#if defined(ADC_COMPENSATION)
	ret:
	#endif
	return val;
}


uint16_t ADC_GetRawVal(const eAnalogInput input)
{
	uint16_t raw = 0;
	uintCPU_t adc_num;
	uintCPU_t chan;

	if (ADC_GetChan(&adc_num, &chan, input) == ERROR_OK)
	{
		const uintCPU_t idx = adc_num + (chan * ADC_NB);
		raw = RestrictedAverage_WORD(ADC_ex.bufferADC[idx], SZ_OBJ(ADC_ex.bufferADC[idx], uint16_t));
	}

	return raw;
}

float ADC_GetConvertedVal(const eAnalogInput input)
{
	#if defined(ADC_AUTO_RAW_VAL_CONV)
	return ADC_ex.bufferConv[input];
	#else
	return ADC_ConvertRAW(input);
	#endif
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

		err |= HALERRtoFCTERR(HAL_ADC_Start_DMA(&hadc1, (uint32_t *) bufferDMA, TOTAL_ADC_CHANS));
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
		err |= HALERRtoFCTERR(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t *) bufferDMA, TOTAL_ADC_CHANS / 2U));
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
		err |= HALERRtoFCTERR(HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t *) bufferDMA, TOTAL_ADC_CHANS));
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


void ADC_handler(void)
{
	#if !defined(ADC_CONTINUOUS_SAMPLING) || defined(ADC_AUTO_RAW_VAL_CONV)
	if (TPSSUP_MS(ADC_ex.hUpdate, ADC_ex.cfg.period))
	{
		ADC_ex.hUpdate = HAL_GetTick();

		#if !defined(ADC_CONTINUOUS_SAMPLING)
		ADC_Start();
		#elif defined(ADC_AUTO_RAW_VAL_CONV)
		ADC_ex.conv = true;
		#endif
	}

	#if defined(ADC_AUTO_RAW_VAL_CONV)
	if (ADC_ex.conv)
	{
		ADC_ex.conv = false;

		for (eAnalogInput in = Adc_Vref ; in < Adc_MAX ; in++)
		{
			ADC_ex.bufferConv[in] = ADC_ConvertRAW(in);
		}
	}
	#endif
	#endif
}


void ADC_SetPeriod(const uint32_t period)
{
	 ADC_ex.cfg.period = period;
}


/**!\brief Conversion complete callback in non blocking mode
**	\strong Strong implementation of HAL_ADC_ConvCpltCallback in the library (will get precedence over HAL function)
**	\warning If DMA used, same number of channels to convert have to be declared between ADCs for proper handling
**	\param[in] hadc - ADC handle
**/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc)
{
	UNUSED(hadc);

#if !defined(ADC_CONTINUOUS_SAMPLING)
	ADC_Stop();

	#if defined(ADC_AUTO_RAW_VAL_CONV)
	ADC_ex.conv = true;
	#endif
#endif

	// Store value in buffers
	for (uintCPU_t chan = 0 ; chan < TOTAL_ADC_CHANS ; chan++)
	{
		ADC_ex.bufferADC[chan][ADC_ex.idxMeasure] = (uint16_t) bufferDMA[chan];
	}

	if (++ADC_ex.idxMeasure >= ADC_SAMP_BUF_SIZE)	{ ADC_ex.idxMeasure = 0; }
}


/******************************************************************/
#endif	/* defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ADC_EX) */
/******************************************************************/
