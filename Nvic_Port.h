/**
 * \author Mr.Nobody
 * \file Nvic_Port.h
 * \ingroup Nvic
 * \brief Nvic module public functionality
 *
 * This file contains all available public functionality, any other files shall 
 * not used outside of the module.
 *
 */

#ifndef NVIC_NVIC_PORT_H
#define NVIC_NVIC_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================== INCLUDES ================================== */
#include "Nvic_Types.h"                     /* Module types definition        */
/* ============================== TYPEDEFS ================================== */

/* ========================== SYMBOLIC CONSTANTS ============================ */

/* ========================== EXPORTED MACROS =============================== */

/* ========================== EXPORTED VARIABLES ============================ */

/* ========================= EXPORTED FUNCTIONS ============================= */

nvic_ModuleVersion_t        Nvic_Get_ModuleVersion      ( void );

void                        Nvic_Init                   ( void );
void                        Nvic_Deinit                 ( void );
void                        Nvic_Task                   ( void );

uint32_t                    Nvic_Get_StackPointerAddr   ( void );

nvic_RequestState_t         Nvic_Set_DefaultHandler     ( nvic_IsrCallback_t defaultHandler );

nvic_RequestState_t         Nvic_Set_PeriphIrq_Handler  ( nvic_PeriphIrqList_t irqId, const nvic_IsrCallback_t irqHandler );
nvic_RequestState_t         Nvic_Get_PeriphIrq_Handler  ( nvic_PeriphIrqList_t irqId, nvic_IsrCallback_t *irqHandler );

nvic_RequestState_t         Nvic_Set_PeriphIrq_Prio     ( nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t irqPrio );
nvic_RequestState_t         Nvic_Get_PeriphIrq_Prio     ( nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t *irqPrio );

nvic_RequestState_t         Nvic_Set_PeriphIrq_Active   ( nvic_PeriphIrqList_t irqId );
nvic_RequestState_t         Nvic_Set_PeriphIrq_Inactive ( nvic_PeriphIrqList_t irqId );

nvic_RequestState_t         Nvic_Set_PeriphIrq_Pending  ( nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t irqFlag );
nvic_RequestState_t         Nvic_Get_PeriphIrq_Pending  ( nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t *irqFlag );


nvic_RequestState_t         Nvic_Set_CoreIrq_Handler    ( nvic_CoreIrqList_t irqId, const nvic_IsrCallback_t irqHandler );
nvic_RequestState_t         Nvic_Get_CoreIrq_Handler    ( nvic_CoreIrqList_t irqId, nvic_IsrCallback_t *irqHandler );

nvic_RequestState_t         Nvic_Set_CoreIrq_Prio       ( nvic_CoreIrqList_t irqId, nvic_IrqPrio_t irqPrio );
nvic_RequestState_t         Nvic_Get_CoreIrq_Prio       ( nvic_CoreIrqList_t irqId, nvic_IrqPrio_t *irqPrio );


/**
 * \brief Activates interrupts.
 *
 */
__attribute__((always_inline)) static inline void Nvic_Set_InterruptsActive( void )
{
    __asm volatile ("cpsie i" : : : "memory");
}


/**
 * \brief De-activates interrupts.
 *
 */
__attribute__((always_inline)) static inline void Nvic_Set_InterruptsInactive( void )
{
    __asm volatile ("cpsid i" : : : "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* NVIC_NVIC_PORT_H */

