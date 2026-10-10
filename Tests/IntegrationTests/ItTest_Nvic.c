/**
 * \author Mr.Nobody
 * \file ItTest_Nvic.c
 * \ingroup Nvic
 * \brief Integration tests of Nested vectored interrupt controller (NVIC) module on target.
 *
 * Nvic module runs on the MCU (Cortex-M7 core). Tests verify behavior which
 * cannot be verified by unit tests (emulated registers): interrupt dispatch
 * through the RAM vector table, enable / pending state, priorities and
 * preemption of nested interrupts, global interrupt masking, default handler,
 * core exception handler (SysTick), L1 cache configuration of Nvic_Init(), fault status
 * and system reset.
 *
 * Interrupts are triggered by software (pending bit) on interrupt lines of
 * peripherals which are not used by the tests (DMA2 stream 6 / 7, present on
 * every classic STM32H7 MCU; GPDMA1 channel 6 / 7 on STM32H7R / H7S), no peripheral
 * and no external wiring is needed. The tests are independent of the board.
 */

/* ============================= INCLUDES =================================== */
#include "unity.h"                          /* Unity testing framework        */
#include "IntegrationTesting.h"             /* Integration testing on target  */
#include "Nvic_Port.h"                      /* Module under test              */
#include "Rcc_Port.h"                       /* SysTick, reset source          */
/* ============================= TYPEDEFS =================================== */

/* ======================= FORWARD DECLARATIONS ============================= */

static void It_Nvic_Wait            ( void );
static void It_Nvic_Log             ( uint32_t event );

static void It_Nvic_LowPrioHandler  ( void );
static void It_Nvic_HighPrioHandler ( void );
static void It_Nvic_CountHandler    ( void );
static void It_Nvic_DefaultHandler  ( void );
static void It_Nvic_SysTickHandler  ( void );

/* ========================= SYMBOLIC CONSTANTS ============================= */

/** Interrupt lines used as low / high priority interrupt (not used by tests, present on every MCU of the family) */
#if defined(STM32H7RS)
    #define IT_NVIC_IRQ_LOW                 ( NVIC_PERIPH_IRQ_GPDMA1_CHANNEL7 )
    #define IT_NVIC_IRQ_HIGH                ( NVIC_PERIPH_IRQ_GPDMA1_CHANNEL6 )
#else
    #define IT_NVIC_IRQ_LOW                 ( NVIC_PERIPH_IRQ_DMA2_STREAM7 )
    #define IT_NVIC_IRQ_HIGH                ( NVIC_PERIPH_IRQ_DMA2_STREAM6 )
#endif /* STM32H7RS */

/** Maximal interrupt priority value (lowest urgency, 4 priority bits on STM32H7) */
#define IT_NVIC_PRIO_MAX                    ( ( 1u << __NVIC_PRIO_BITS ) - 1u )

/** Count of wait loop iterations (interrupt entry latency, far below) */
#define IT_NVIC_WAIT_LOOPS                  ( 1000u )

/** Size of the event log */
#define IT_NVIC_LOG_SIZE                    ( 8u )

/** Events of the event log */
#define IT_NVIC_EV_LOW_START                ( 1u )      /**< Low priority handler entered  */
#define IT_NVIC_EV_LOW_END                  ( 2u )      /**< Low priority handler finished */
#define IT_NVIC_EV_HIGH                     ( 3u )      /**< High priority handler         */

/** Count of SysTick interrupts the test waits for */
#define IT_NVIC_SYSTICK_CNT                 ( 10u )

/** Maximal count of wait loop iterations for SysTick interrupts (timeout) */
#define IT_NVIC_SYSTICK_WAIT_LOOPS          ( 2000000u )

/* ============================== MACROS ==================================== */

/* ========================== LOCAL VARIABLES =============================== */

/** Count of calls of \ref It_Nvic_CountHandler */
static volatile uint32_t    itNvic_CountCnt;

/** Count of calls of \ref It_Nvic_DefaultHandler */
static volatile uint32_t    itNvic_DefaultCnt;

/** Count of calls of \ref It_Nvic_SysTickHandler */
static volatile uint32_t    itNvic_SysTickCnt;

/** Event log of nested handlers */
static volatile uint32_t    itNvic_EventLog[ IT_NVIC_LOG_SIZE ];

/** Count of logged events */
static volatile uint32_t    itNvic_EventCnt;

/* ============================= TEST SETUP ================================= */

void setUp( void )
{
    itNvic_CountCnt   = 0u;
    itNvic_DefaultCnt = 0u;
    itNvic_SysTickCnt = 0u;
    itNvic_EventCnt   = 0u;

    for( uint32_t logIdx = 0u; IT_NVIC_LOG_SIZE > logIdx; logIdx++ )
    {
        itNvic_EventLog[ logIdx ] = 0u;
    }
}


void tearDown( void )
{
    /* Every test case runs after system reset */
}

/* =============================== TESTS ==================================== */

/*------------------------- Handler and dispatch -----------------------------*/

/**
 * \brief   Peripheral handler on target is stored in the RAM vector table.
 *
 * \details Sets handler of DMA2 stream 7 interrupt line, reads it back and tries to set NULL
 *          handler.
 *
 * \par Expected results
 * - Handler reads back the set handler.
 * - NULL handler: NVIC_REQUEST_ERROR.
 */
void It_Nvic_Set_PeriphIrq_Handler_ReadBack( void )
{
    nvic_IsrCallback_t handler = NVIC_NULL_PTR;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, It_Nvic_CountHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, &handler ) );
    TEST_ASSERT_EQUAL_PTR( It_Nvic_CountHandler, handler );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, NVIC_NULL_PTR ) );
}


/**
 * \brief   Pending enabled interrupt calls its handler.
 *
 * \details Sets handler of DMA2 stream 7 line, enables the line and sets it pending by software.
 *
 * \par Expected results
 * - Handler is called once.
 * - Pending state is cleared by interrupt entry (NVIC_IRQ_INACTIVE).
 */
void It_Nvic_Set_PeriphIrq_Pending_ActiveIrq_HandlerCalledOnce( void )
{
    nvic_IrqFlag_t pending = NVIC_IRQ_ACTIVE;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, It_Nvic_CountHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW ) );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 1u, itNvic_CountCnt );

    /* Pending state is cleared by interrupt entry */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, &pending ) );
    TEST_ASSERT_EQUAL( NVIC_IRQ_INACTIVE, pending );
}


/**
 * \brief   Pending disabled interrupt is executed after its enabling.
 *
 * \details
 * 1. Sets handler of DMA2 stream 7 line, disables the line and sets it pending.
 * 2. Enables the line.
 *
 * \par Expected results
 * 1. Handler is not called, line stays pending.
 * 2. Handler is called once, pending state is cleared.
 */
void It_Nvic_Set_PeriphIrq_Active_PendingInactiveIrq_HandlerCalledAfterActivation( void )
{
    nvic_IrqFlag_t pending = NVIC_IRQ_INACTIVE;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, It_Nvic_CountHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Inactive( IT_NVIC_IRQ_LOW ) );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();

    /* Disabled interrupt stays pending */
    TEST_ASSERT_EQUAL_UINT32( 0u, itNvic_CountCnt );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, &pending ) );
    TEST_ASSERT_EQUAL( NVIC_IRQ_ACTIVE, pending );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 1u, itNvic_CountCnt );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, &pending ) );
    TEST_ASSERT_EQUAL( NVIC_IRQ_INACTIVE, pending );
}


/**
 * \brief   Pending state cleared before enabling does not call the handler.
 *
 * \details Sets handler of disabled DMA2 stream 7 line, sets the line pending, clears pending
 *          state and enables the line.
 *
 * \par Expected results
 * - Pending state reads back NVIC_IRQ_INACTIVE after clearing.
 * - Handler is not called.
 */
void It_Nvic_Set_PeriphIrq_Pending_ClearedBeforeActivation_HandlerNotCalled( void )
{
    nvic_IrqFlag_t pending = NVIC_IRQ_ACTIVE;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, It_Nvic_CountHandler ) );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE   ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_INACTIVE ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, &pending ) );
    TEST_ASSERT_EQUAL( NVIC_IRQ_INACTIVE, pending );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 0u, itNvic_CountCnt );
}


/**
 * \brief   Disabled interrupt does not call the handler.
 *
 * \details Sets handler of DMA2 stream 7 line, enables and disables the line and sets it
 *          pending.
 *
 * \par Expected results
 * - Handler is not called.
 */
void It_Nvic_Set_PeriphIrq_Inactive_ActiveIrq_HandlerNotCalled( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, It_Nvic_CountHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active  ( IT_NVIC_IRQ_LOW ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Inactive( IT_NVIC_IRQ_LOW ) );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 0u, itNvic_CountCnt );
}


/**
 * \brief   Interrupt without registered handler calls the user default handler.
 *
 * \details Sets user default handler (disables DMA2 stream 7 line when called), enables DMA2 stream 7
 *          line without registered handler and sets it pending. Then tries to set
 *          NULL default handler.
 *
 * \par Expected results
 * - User default handler is called once.
 * - NULL default handler: NVIC_REQUEST_ERROR.
 */
void It_Nvic_Set_DefaultHandler_IrqWithoutHandler_DefaultHandlerCalled( void )
{
    /* Line without registered handler is routed to the default handler */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_DefaultHandler( It_Nvic_DefaultHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW ) );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 1u, itNvic_DefaultCnt );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_DefaultHandler( NVIC_NULL_PTR ) );
}


/**
 * \brief   Nvic_Deinit() on target replaces registered handlers by default handler.
 *
 * \details Sets handler of DMA2 stream 7 line and user default handler, deinitializes the
 *          module, reads the DMA2 stream 7 handler, then enables the line and sets it pending.
 *
 * \par Expected results
 * - DMA2 stream 7 handler is not the registered handler any more.
 * - Pending line calls the user default handler (vector table in RAM is still used),
 *   registered handler is not called.
 */
void It_Nvic_Deinit_RegisteredHandler_ReplacedByDefaultHandler( void )
{
    nvic_IsrCallback_t handler = NVIC_NULL_PTR;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, It_Nvic_CountHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_DefaultHandler( It_Nvic_DefaultHandler ) );

    Nvic_Deinit();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, &handler ) );
    TEST_ASSERT_NOT_EQUAL( It_Nvic_CountHandler, handler );

    /* Vector table is still used (VTOR) - pending line calls the default handler */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 0u, itNvic_CountCnt );
    TEST_ASSERT_EQUAL_UINT32( 1u, itNvic_DefaultCnt );
}

/*------------------------- Priority and preemption --------------------------*/

/**
 * \brief   All priority values of peripheral interrupt are read back from hardware.
 *
 * \details Sets priorities 0 - 15 (4 priority bits) of DMA2 stream 7 line and reads them back,
 *          then sets priority 16.
 *
 * \par Expected results
 * - Every priority 0 - 15 reads back.
 * - Priority 16: NVIC_REQUEST_ERROR, priority 15 is kept.
 */
void It_Nvic_Set_PeriphIrq_Prio_AllValues_ReadBack( void )
{
    nvic_IrqPrio_t irqPrio = 0u;

    for( nvic_IrqPrio_t prioIdx = 0u; IT_NVIC_PRIO_MAX >= prioIdx; prioIdx++ )
    {
        TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Prio( IT_NVIC_IRQ_LOW, prioIdx ) );
        TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Prio( IT_NVIC_IRQ_LOW, &irqPrio ) );
        TEST_ASSERT_EQUAL_UINT32( prioIdx, irqPrio );
    }

    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Prio( IT_NVIC_IRQ_LOW, IT_NVIC_PRIO_MAX + 1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Prio( IT_NVIC_IRQ_LOW, &irqPrio ) );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_PRIO_MAX, irqPrio );
}


/**
 * \brief   Interrupt with higher priority preempts running handler.
 *
 * \details DMA2 stream 7 line (priority 5) handler sets DMA2 stream 6 line (priority 2) pending during
 *          its execution. Both handlers log their events, DMA2 stream 7 line is set pending.
 *
 * \par Expected results
 * - 3 events in order: low priority start, high priority, low priority end.
 */
void It_Nvic_Set_PeriphIrq_Prio_HigherPriority_PreemptsRunningHandler( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW,  It_Nvic_LowPrioHandler  ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_HIGH, It_Nvic_HighPrioHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Prio( IT_NVIC_IRQ_LOW,  5u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Prio( IT_NVIC_IRQ_HIGH, 2u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW  ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_HIGH ) );

    /* Low priority handler sets the high priority interrupt pending */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 3u, itNvic_EventCnt );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_EV_LOW_START, itNvic_EventLog[ 0u ] );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_EV_HIGH,      itNvic_EventLog[ 1u ] );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_EV_LOW_END,   itNvic_EventLog[ 2u ] );
}


/**
 * \brief   Interrupt with lower priority waits for the running handler.
 *
 * \details DMA2 stream 7 line (priority 2) handler sets DMA2 stream 6 line (priority 5) pending during
 *          its execution. Both handlers log their events, DMA2 stream 7 line is set pending.
 *
 * \par Expected results
 * - 3 events in order: low priority start, low priority end, high priority (tail
 *   chained).
 */
void It_Nvic_Set_PeriphIrq_Prio_LowerPriority_ExecutedAfterRunningHandler( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW,  It_Nvic_LowPrioHandler  ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_HIGH, It_Nvic_HighPrioHandler ) );

    /* "High" interrupt has lower urgency than the running "low" handler */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Prio( IT_NVIC_IRQ_LOW,  2u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Prio( IT_NVIC_IRQ_HIGH, 5u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW  ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_HIGH ) );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 3u, itNvic_EventCnt );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_EV_LOW_START, itNvic_EventLog[ 0u ] );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_EV_LOW_END,   itNvic_EventLog[ 1u ] );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_EV_HIGH,      itNvic_EventLog[ 2u ] );
}


/**
 * \brief   Globally masked interrupt is executed after unmasking.
 *
 * \details
 * 1. Sets handler of DMA2 stream 7 line, enables the line, masks interrupts globally
 *    (Nvic_Set_InterruptsInactive) and sets the line pending.
 * 2. Unmasks interrupts globally (Nvic_Set_InterruptsActive).
 *
 * \par Expected results
 * 1. Handler is not called.
 * 2. Handler is called once.
 */
void It_Nvic_Set_InterruptsInactive_PendingIrq_ExecutedAfterActivation( void )
{
    uint32_t countMasked = 0xFFFFFFFFu;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( IT_NVIC_IRQ_LOW, It_Nvic_CountHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( IT_NVIC_IRQ_LOW ) );

    Nvic_Set_InterruptsInactive();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_LOW, NVIC_IRQ_ACTIVE ) );
    It_Nvic_Wait();
    countMasked = itNvic_CountCnt;

    Nvic_Set_InterruptsActive();
    It_Nvic_Wait();

    TEST_ASSERT_EQUAL_UINT32( 0u, countMasked );
    TEST_ASSERT_EQUAL_UINT32( 1u, itNvic_CountCnt );
}

/*------------------------------ Core exceptions -----------------------------*/

/**
 * \brief   SysTick core exception calls the registered handler periodically.
 *
 * \details Sets SysTick handler, reads it back, starts SysTick with 1 ms interval
 *          (RCC module) and waits for 10 interrupts (loop timeout).
 *
 * \par Expected results
 * - Handler reads back the set handler.
 * - Handler is called at least 10x before timeout.
 */
void It_Nvic_Set_CoreIrq_Handler_SysTick_HandlerCalledPeriodically( void )
{
    nvic_IsrCallback_t handler = NVIC_NULL_PTR;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Handler( NVIC_CORE_IRQ_SYSTICK, It_Nvic_SysTickHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Handler( NVIC_CORE_IRQ_SYSTICK, &handler ) );
    TEST_ASSERT_EQUAL_PTR( It_Nvic_SysTickHandler, handler );

    TEST_ASSERT_EQUAL( RCC_REQUEST_OK, Rcc_Set_SysTickInterval( 1u ) );

    for( volatile uint32_t loopIdx = 0u;
         ( IT_NVIC_SYSTICK_WAIT_LOOPS > loopIdx ) &&
         ( IT_NVIC_SYSTICK_CNT > itNvic_SysTickCnt );
         loopIdx++ )
    {
        /* Wait for SysTick interrupts */
    }

    TEST_ASSERT_GREATER_OR_EQUAL_UINT32( IT_NVIC_SYSTICK_CNT, itNvic_SysTickCnt );
}


/**
 * \brief   Priorities of configurable core exceptions are read back from hardware.
 *
 * \details Sets priority 7 of SysTick, 15 of PendSV and 3 of SVCall and reads them
 *          back, then sets priority of NMI and HardFault.
 *
 * \par Expected results
 * - SysTick 7, PendSV 15, SVCall 3 read back.
 * - NMI, HardFault (fixed priority): NVIC_REQUEST_ERROR.
 */
void It_Nvic_Set_CoreIrq_Prio_ConfigurableExceptions_ReadBack( void )
{
    nvic_IrqPrio_t irqPrio = 0u;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK, 7u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK, &irqPrio ) );
    TEST_ASSERT_EQUAL_UINT32( 7u, irqPrio );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_PENDSV, IT_NVIC_PRIO_MAX ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Prio( NVIC_CORE_IRQ_PENDSV, &irqPrio ) );
    TEST_ASSERT_EQUAL_UINT32( IT_NVIC_PRIO_MAX, irqPrio );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SVCALL, 3u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Prio( NVIC_CORE_IRQ_SVCALL, &irqPrio ) );
    TEST_ASSERT_EQUAL_UINT32( 3u, irqPrio );

    /* Fixed priority exceptions */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_NMI,       1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_HARDFAULT, 1u ) );
}

/*--------------------------------- L1 caches --------------------------------*/

/**
 * \brief   Nvic_Init() enables the instruction cache, the data cache stays disabled.
 *
 * \details StartUp executed Nvic_Init() before the test - SCB->CCR of the running firmware is
 *          read.
 *
 * \par Expected results
 * - CCR.IC = 1 (L1 instruction cache enabled), CCR.DC = 0 (data cache disabled, DMA buffers
 *   need no cache maintenance).
 */
void It_Nvic_Init_InstructionCacheEnabled_DataCacheDisabled( void )
{
    TEST_ASSERT_EQUAL_HEX32( SCB_CCR_IC_Msk, SCB->CCR & SCB_CCR_IC_Msk );
    TEST_ASSERT_EQUAL_HEX32( 0u, SCB->CCR & SCB_CCR_DC_Msk );
}

/*----------------------------- Fault status / reset -------------------------*/

/**
 * \brief   Fault status is clear on target without fault.
 *
 * \details Reads the fault status after system reset, then calls the function with
 *          NULL pointer.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, CFSR = 0, HFSR = 0.
 * - NULL pointer: NVIC_REQUEST_ERROR.
 */
void It_Nvic_Get_FaultStatus_NoFault_StatusRegistersCleared( void )
{
    nvic_FaultStatus_t faultStatus = { 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0u };

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_FaultStatus( &faultStatus ) );

    TEST_ASSERT_EQUAL_HEX32( 0u, faultStatus.Cfsr );
    TEST_ASSERT_EQUAL_HEX32( 0u, faultStatus.Hfsr );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_FaultStatus( NVIC_NULL_PTR ) );
}


/**
 * \brief   Nvic_Set_SystemReset() resets the MCU.
 *
 * \details
 * 1. Stage 0: clears reset source flags, expects reset and requests system reset.
 * 2. Stage 1 (after reset): checks the reset source.
 *
 * \par Expected results
 * 1. MCU is reset, code after the request is not executed.
 * 2. Software reset flag is set.
 */
void It_Nvic_Set_SystemReset_ResetsMcu( void )
{
    if( 0u == IntegrationTesting_Get_Stage() )
    {
        TEST_ASSERT_EQUAL( RCC_REQUEST_OK, Rcc_Set_ResetSourceClear() );

        IntegrationTesting_Set_ResetExpected();

        Nvic_Set_SystemReset();

        TEST_FAIL_MESSAGE( "System reset did not occur" );
    }
    else
    {
        rcc_FlagState_t swReset = RCC_FLAG_INACTIVE;

        TEST_ASSERT_EQUAL( RCC_REQUEST_OK, Rcc_Get_ResetSource( RCC_RESET_SRC_SW, &swReset ) );
        TEST_ASSERT_EQUAL( RCC_FLAG_ACTIVE, swReset );
    }
}

/* ========================== LOCAL FUNCTIONS =============================== */

/**
 * \brief Short wait - interrupt entry and handler execution.
 */
static void It_Nvic_Wait( void )
{
    for( volatile uint32_t loopIdx = 0u; IT_NVIC_WAIT_LOOPS > loopIdx; loopIdx++ )
    {
        /* Busy wait */
    }
}


/**
 * \brief Stores event into the event log.
 *
 * \param event [in]: Event identification
 */
static void It_Nvic_Log( uint32_t event )
{
    if( IT_NVIC_LOG_SIZE > itNvic_EventCnt )
    {
        itNvic_EventLog[ itNvic_EventCnt ] = event;
    }
    else
    {
        /* Log is full */
    }

    itNvic_EventCnt++;
}


/**
 * \brief Low priority handler - sets high priority interrupt pending during its execution.
 */
static void It_Nvic_LowPrioHandler( void )
{
    It_Nvic_Log( IT_NVIC_EV_LOW_START );

    (void)Nvic_Set_PeriphIrq_Pending( IT_NVIC_IRQ_HIGH, NVIC_IRQ_ACTIVE );
    It_Nvic_Wait();

    It_Nvic_Log( IT_NVIC_EV_LOW_END );
}


/**
 * \brief High priority handler - logs its execution.
 */
static void It_Nvic_HighPrioHandler( void )
{
    It_Nvic_Log( IT_NVIC_EV_HIGH );
}


/**
 * \brief Handler counting its calls.
 */
static void It_Nvic_CountHandler( void )
{
    itNvic_CountCnt++;
}


/**
 * \brief User default handler counting its calls.
 */
static void It_Nvic_DefaultHandler( void )
{
    itNvic_DefaultCnt++;

    /* Pending line without handler would be entered again - disable it */
    (void)Nvic_Set_PeriphIrq_Inactive( IT_NVIC_IRQ_LOW );
}


/**
 * \brief SysTick handler counting its calls.
 */
static void It_Nvic_SysTickHandler( void )
{
    itNvic_SysTickCnt++;
}
