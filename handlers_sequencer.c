/*!\file handlers_sequencer.c
** \author SMFSW
** \copyright MIT (c) 2017-2026, SMFSW
** \brief Simple task sequencer handling
**/
/****************************************************************/
#include "sarmfsw.h"
#include "handlers_sequencer.h"
/****************************************************************/


FctERR NONNULL__ sequencer_init(StruSequencer * const pSeq, const pfHandler_t * const aHandlers, const uintCPU_t nb_handlers, const bool start)
{
	if (nb_handlers > 32)	{ return ERROR_VALUE; }

	memset(pSeq, 0, sizeof(StruSequencer));

	pSeq->cfg.aHandlers = aHandlers;
	pSeq->cfg.nb_handlers = nb_handlers;

	pSeq->cfg.handler_en = (uint32_t) -1;

	pSeq->sequencer_en = start;

	return ERROR_OK;
}

FctERR NONNULL__ sequencer_init_all(StruSequencer * const pSeq, const StruSequencer_cfg * const pCfg, const bool start)
{
	if (pCfg->nb_handlers > 32)		{ return ERROR_VALUE; }
	if (pCfg->aHandlers == NULL)	{ return ERROR_VALUE; }

	memcpy(&pSeq->cfg, pCfg, sizeof(StruSequencer_cfg));

	pSeq->current_handler = 0U;
	pSeq->sequencer_en = start;

	return ERROR_OK;
}


FctERR NONNULL__ sequencer_deinit(StruSequencer * const pSeq)
{
	memset(pSeq, 0, sizeof(StruSequencer));

	return ERROR_OK;
}


FctERR NONNULL__ sequencer_register_callback(StruSequencer * const pSeq, const eSeqCallback callback, const pfSeq_cback_t pCallback)
{
	if (callback >= cback_max)	{ return ERROR_VALUE; }

	pSeq->cfg.pf_callback[callback] = pCallback;

	return ERROR_OK;
}

FctERR NONNULL__ sequencer_unregister_callback(StruSequencer * const pSeq, const eSeqCallback callback)
{
	if (callback >= cback_max)	{ return ERROR_VALUE; }

	pSeq->cfg.pf_callback[callback] = NULL;

	return ERROR_OK;
}


void NONNULL__ sequencer_handler(StruSequencer * const pSeq)
{
	if (pSeq->sequencer_en)
	{
		if (pSeq->cfg.pf_callback[cback_preSeq] != NULL)	{ pSeq->cfg.pf_callback[cback_preSeq](pSeq); }

		for (pSeq->current_handler = 0U ; pSeq->current_handler < pSeq->cfg.nb_handlers ; pSeq->current_handler++)
		{
			if (!pSeq->sequencer_en)	{ break; }	// If sequencer is disabled during loop

			const uint32_t handler_bit = LSHIFT32(1UL, pSeq->current_handler);

			if (TEST_BITS_SET(pSeq->cfg.handler_en, handler_bit))
			{
				if (pSeq->cfg.pf_callback[cback_preHandle] != NULL)		{ pSeq->cfg.pf_callback[cback_preHandle](pSeq); }
				if (pSeq->cfg.aHandlers[pSeq->current_handler] != NULL)	{ pSeq->cfg.aHandlers[pSeq->current_handler](); }
				if (pSeq->cfg.pf_callback[cback_postHandle] != NULL)	{ pSeq->cfg.pf_callback[cback_postHandle](pSeq); }
			}
		}

		if (pSeq->cfg.pf_callback[cback_postSeq] != NULL)	{ pSeq->cfg.pf_callback[cback_postSeq](pSeq); }
	}
}
