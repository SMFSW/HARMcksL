/*!\file ADC_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2020, SMFSW
** \brief Simple extension for ADCs
** \details ADC_ex is meant to automate ADC conversions using DMA.
** 			- DMA must be configured for ADC peripherals:
** 				- Peripheral to Memory
** 				- Circular Mode
** 				- Increment Memory
** 				- Data width Word for both peripheral and memory
** 			- ADC configuration:
** 				- Scan conversion mode across channels enabled
** 				- Continuous conversion is optional (if disabled, user has to start conversions manually when needed)
** 				- DMA continuous request when available
** 				- Interrupt generated at end of conversion
** \note Define USE_ADC_EX symbol at project level to use ADC_ex functionalities
** \note Configuration done calling ADC_cfg.h file
**/
/****************************************************************/
#ifndef __ADC_EX_H
	#define	__ADC_EX_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ADC_EX)

#include "ADC_cfg.h"
/****************************************************************/


#define TEMP_CALC_V25	TEMP_CALC_VTEMP		//!< Alias for legacy code using TEMP_CALC_V25 (not accurate following families, leading to name change)
#define TEMP_CALC_V30	TEMP_CALC_VTEMP		//!< Alias for legacy code using TEMP_CALC_V30 (not accurate following families, leading to name change)

#define Def_V25			Def_VTemp			//!< Alias for legacy code using Def_V25 (not accurate following families, leading to name change)
#define Def_V30			Def_VTemp			//!< Alias for legacy code using Def_V30 (not accurate following families, leading to name change)


/*!\enum eADCidx
** \brief ADC peripheral instance indexes
**/
typedef enum eADCidx {
	#if defined(ADC1)
	ADC_1 = 0,	//!< ADC1 index
	#endif
	#if defined(ADC2)
	ADC_2,		//!< ADC2 index
	#endif
	#if defined(ADC3)
	ADC_3,		//!< ADC3 index
	#endif
	ADC_MAX		//!< Adc MAX index
} eADCidx;


// TODO: declare weak if possible with breaking legacy compatibility
//typedef enum eAnalogInput {
//	#if defined(ADC_USE_VREF)
//	Adc_Vref,		//!< Adc VRef (converted value in V)
//	#endif
//	#if defined(ADC_USE_TEMP)
//	Adc_Temp,		//!< Adc Temperature (converted value in °C)
//	#endif
//	#if defined(ADC_USE_VBAT)
//	Adc_Vbat,		//!< Adc VBat (converted value in V)
//	#endif
//	Adc_MAX			//!< Adc MAX
//} eAnalogInput;


/*!\struct ADC_Cfg
** \brief ADC channel declaration config structure
**/
typedef struct ADC_Cfg {
	const eADCidx	adc;							//!< ADC instance index
	const uint8_t	chan;							//!< Channel index
	float			(* const conv) (const float);	//!< Pointer to conversion function
} ADC_Cfg;


/**!\brief Get ADC channel raw value
**	\param[in] input - Input index
**	\return Channel raw value
**/
uint16_t ADC_GetRawVal(const eAnalogInput input);

/**!\brief Get ADC channel converted value
**	\param[in] input - Input index
**	\return Channel converted value
**/
float ADC_GetConvertedVal(const eAnalogInput input);


/**!\brief Start ADC conversions
**	\return Error code
**/
FctERR ADC_Start(void);

/**!\brief Stop ADC conversions
**	\return Error code
**/
FctERR ADC_Stop(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* defined(HAL_ADC_MODULE_ENABLED) */
#endif /* __ADC_EX_H */
/****************************************************************/

