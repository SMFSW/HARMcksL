/*!\file handlers_sequencer.h
** \author SMFSW
** \copyright MIT (c) 2017-2025, SMFSW
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
/*!\enum _eSeqCallback
** \brief Callback id
**/
typedef enum _eSeqCallback {
	cback_preSeq = 0,
	cback_postSeq,
	cback_preHandle,
	cback_postHandle,
	cback_max,
} eSeqCallback;


typedef struct _StruSequencer	StruSequencer;			//!< Typedef for StruSequencer used by function pointers included in struct

typedef void (*pfHandler_t)(void);						//!< Basic handler function pointer typedef
typedef void (*pfSeq_cback_t)(StruSequencer const *);


typedef struct _StruSequencer_cfg {
	const pfHandler_t *	aHandlers;
	uintCPU_t			nb_handlers;
	pfSeq_cback_t		pf_callback[cback_max];
	uint32_t			handler_en;
} StruSequencer_cfg;


/*!\struct _StruSequencer
** \brief Sequencer structure
**/
struct _StruSequencer {
	StruSequencer_cfg	cfg;
	uintCPU_t			current_handler;
	bool				sequencer_en;
};


// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
__INLINE void NONNULL_INLINE__ handler_disable(StruSequencer * const pSeq, const uint8_t idx) {
	CLR_BITS(pSeq->cfg.handler_en, LSHIFT32(1UL, idx)); }

__INLINE void NONNULL_INLINE__ handler_enable(StruSequencer * const pSeq, const uint8_t idx) {
	SET_BITS(pSeq->cfg.handler_en, LSHIFT32(1UL, idx)); }

__INLINE void NONNULL_INLINE__ handler_set_mask(StruSequencer * const pSeq, const uint32_t mask) {
	pSeq->cfg.handler_en = mask; }

__INLINE void NONNULL_INLINE__ sequencer_stop(StruSequencer * const pSeq) {
	pSeq->sequencer_en = false; }

__INLINE void NONNULL_INLINE__ sequencer_start(StruSequencer * const pSeq) {
	pSeq->sequencer_en = true; }

FctERR NONNULL__ sequencer_init(StruSequencer * const pSeq, const pfHandler_t * const aHandlers, const uintCPU_t nb_handlers, const bool start);
FctERR NONNULL__ sequencer_init_all(StruSequencer * const pSeq, const StruSequencer_cfg * const pCfg, const bool start);
FctERR NONNULL__ sequencer_deinit(StruSequencer * const pSeq);

// Note: sequencer_init has to be called prior registering any callback
FctERR NONNULL__ sequencer_register_callback(StruSequencer * const pSeq, const eSeqCallback callback, const pfSeq_cback_t pCallback);
FctERR NONNULL__ sequencer_unregister_callback(StruSequencer * const pSeq, const eSeqCallback callback);

void NONNULL__ sequencer_handler(StruSequencer * const pSeq);

/****************************************************************/
#ifdef __cplusplus
	}
#endif

#endif	/* HANDLERS_SEQUENCER_H__ */
/****************************************************************/
