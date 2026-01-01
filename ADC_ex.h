/*!\file ADC_ex.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
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
** \note Define \c USE_ADC_EX symbol at project level to use ADC_ex functionalities
** \note Configuration done including ADC_cfg.h file
**/
/****************************************************************/
#ifndef ADC_EX_H__
	#define	ADC_EX_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"

#if defined(HAL_ADC_MODULE_ENABLED) && defined(USE_ADC_EX)

#include "ADC_cfg.h"
/****************************************************************/


#if !defined(TEMP_CALC_VTEMP)
#if defined(TEMP_CALC_V25) || defined(TEMP_CALC_V30)	//!< Alias for legacy code using TEMP_CALC_V25 or TEMP_CALC_V30
#define TEMP_CALC_VTEMP
#endif
#endif


#if !defined(Def_VTemp)
#if defined(Def_V25)
#define Def_VTemp		Def_V25		//!< Alias for legacy code using Def_V25 (not accurate following families, leading to name change)
#endif
#if defined(Def_V30)
#define Def_VTemp		Def_V30		//!< Alias for legacy code using Def_V30 (not accurate following families, leading to name change)
#endif
#endif


#if !defined(Def_VAlim)
#define Def_VAlim		VDD_VALUE	//!< Def_VAlim needs to be defined (in mV) if special power supply voltage used (eg. U5 variable power supply range)
#endif


#if !defined(ADC_RESOLUTION)
#define ADC_RESOLUTION	12			//!< ADC resolution in bits (defaults to 12 if not set)
#endif

#define	DefRangeADC		(LSHIFT(1U, ADC_RESOLUTION) - 1)		//!< ADC digital range

#define Def_ADCStep(mv)	((mv) / (float) (DefRangeADC * 1000))	//!< Step value (in mV) following given \p mv


/*!\enum eADCidx
** \brief ADC peripheral instance indexes
**/
typedef enum {
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


//typedef enum {
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
typedef struct {
	const eADCidx	adc;							//!< ADC instance index
	const uint8_t	chan;							//!< Channel index
	float			(* const conv) (const float);	//!< Pointer to conversion function
} ADC_Cfg;


/*!\brief Get ADC channel raw value
** \param[in] input - Input index
** \return Channel raw value
**/
uint16_t ADC_GetRawVal(const eAnalogInput input);

/*!\brief Get ADC channel converted value
** \param[in] input - Input index
** \return Channel converted value
**/
float ADC_GetConvertedVal(const eAnalogInput input);


/*!\brief Start ADC conversions
** \return FctERR - Error code
**/
FctERR ADC_Start(void);

/*!\brief Stop ADC conversions
** \return FctERR - Error code
**/
FctERR ADC_Stop(void);


/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif /* defined(HAL_ADC_MODULE_ENABLED) */
#endif /* ADC_EX_H__ */
/****************************************************************/

