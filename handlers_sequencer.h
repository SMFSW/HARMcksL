/*!\file handlers_sequencer.h
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Simple task sequencer handling
**/
/****************************************************************/
#ifndef HANDLERS_SEQUENCER_H__
	#define HANDLERS_SEQUENCER_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "sarmfsw.h"
/****************************************************************/


// *****************************************************************************
// Section: Types
// *****************************************************************************
/*!\enum eSeqCallback
** \brief Callback id
**/
typedef enum {
	cback_preSeq = 0,
	cback_postSeq,
	cback_preHandle,
	cback_postHandle,
	cback_max,
} eSeqCallback;


typedef struct sequencer	Sequencer;					//!< Typedef for StruSequencer used by function pointers included in struct

typedef void (*pfHandler_t)(void);						//!< Basic handler function pointer typedef
typedef void (*pfSeq_cback_t)(Sequencer const *);


typedef struct {
	const pfHandler_t *	aHandlers;
	uintCPU_t			nb_handlers;
	pfSeq_cback_t		pf_callback[cback_max];
	uint32_t			handler_en;
} Sequencer_cfg;


/*!\struct sequencer
** \brief Sequencer structure
**/
struct sequencer {
	Sequencer_cfg	cfg;
	uintCPU_t		current_handler;
	bool			sequencer_en;
};


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
__INLINE void NONNULL_INLINE__ handler_disable(Sequencer * const pSeq, const uint8_t idx) {
	CLR_BITS(pSeq->cfg.handler_en, LSHIFT32(1UL, idx)); }

__INLINE void NONNULL_INLINE__ handler_enable(Sequencer * const pSeq, const uint8_t idx) {
	SET_BITS(pSeq->cfg.handler_en, LSHIFT32(1UL, idx)); }

__INLINE void NONNULL_INLINE__ handler_set_mask(Sequencer * const pSeq, const uint32_t mask) {
	pSeq->cfg.handler_en = mask; }

__INLINE void NONNULL_INLINE__ sequencer_stop(Sequencer * const pSeq) {
	pSeq->sequencer_en = false; }

__INLINE void NONNULL_INLINE__ sequencer_start(Sequencer * const pSeq) {
	pSeq->sequencer_en = true; }

FctERR NONNULL__ sequencer_init(Sequencer * const pSeq, const pfHandler_t * const aHandlers, const uintCPU_t nb_handlers, const bool start);
FctERR NONNULL__ sequencer_init_all(Sequencer * const pSeq, const Sequencer_cfg * const pCfg, const bool start);
FctERR NONNULL__ sequencer_deinit(Sequencer * const pSeq);

// Note: sequencer_init has to be called prior registering any callback
FctERR NONNULL__ sequencer_register_callback(Sequencer * const pSeq, const eSeqCallback callback, const pfSeq_cback_t pCallback);
FctERR NONNULL__ sequencer_unregister_callback(Sequencer * const pSeq, const eSeqCallback callback);

void NONNULL__ sequencer_handler(Sequencer * const pSeq);

/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* HANDLERS_SEQUENCER_H__ */
/****************************************************************/
