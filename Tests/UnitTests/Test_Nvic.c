/**
 * \author Mr.Nobody
 * \file Test_Nvic.c
 * \ingroup Nvic
 * \brief Unit tests of Nested Vector Interrupt Controller (NVIC) module.
 *
 * Nvic.c is compiled unchanged. NVIC and SCB registers are emulated by RegMem,
 * core instructions (DSB, ISB) by CmsisHost, StartUp module is mocked.
 */

/* ============================= INCLUDES =================================== */
#include "unity.h"                          /* Unity testing framework        */
#include "RegMem.h"                         /* Register memory emulation      */
#include "CmsisHost.h"                      /* Core intrinsics emulation      */
#include "Nvic_Port.h"                      /* Module under test              */
#include "MockStartUp_Port.h"               /* StartUp module mock            */
#include "Stm32.h"                          /* NVIC / SCB registers           */
/* ============================= TYPEDEFS =================================== */

/* ======================= FORWARD DECLARATIONS ============================= */

static void Ut_Nvic_UserHandler     ( void );
static void Ut_Nvic_UserDefaultHandler( void );

/* ========================= SYMBOLIC CONSTANTS ============================= */

/** Shift of priority value in 8-bit priority field (4 implemented bits) */
#define UT_NVIC_PRIO_SHIFT                  ( 4u )

/** Highest valid priority value (4 implemented bits) */
#define UT_NVIC_PRIO_MAX                    ( 15u )

/** Count of IRQs in one 32-bit NVIC register */
#define UT_NVIC_IRQ_PER_REG                 ( 32u )

/** Priority grouping 4 (4 bits pre-emption, 0 bits sub-priority) - PRIGROUP value */
#define UT_NVIC_PRIGROUP_4                  ( 3u )

/** Index of SHPR byte of SysTick (exception 15 - 4) */
#define UT_NVIC_SHPR_IDX_SYSTICK            ( 11u )

/** Index of SHPR byte of MemManage fault (exception 4 - 4) */
#define UT_NVIC_SHPR_IDX_MEMFAULT           ( 0u )

/** Core IRQ index of reserved exception 7 (Cortex-M7, exception number - 1) */
#define UT_NVIC_CORE_IRQ_RESERVED           ( (nvic_CoreIrqList_t)6u )

/* ============================== MACROS ==================================== */

/** NVIC register index of the IRQ */
#define UT_NVIC_REG_IDX( irq )              ( (uint32_t)( irq ) / UT_NVIC_IRQ_PER_REG )

/** NVIC register bit mask of the IRQ */
#define UT_NVIC_REG_MASK( irq )             ( 1u << ( (uint32_t)( irq ) % UT_NVIC_IRQ_PER_REG ) )

/* ========================= EXPORTED VARIABLES ============================= */

/** Linker script symbols referenced by Nvic.c */
uint32_t        _estack;
const uint32_t  _ram_end;

/* ========================== LOCAL VARIABLES =============================== */

static uint32_t utNvic_UserHandlerCnt;
static uint32_t utNvic_UserDefaultHandlerCnt;

/* ============================ TEST FIXTURE ================================ */

void setUp( void )
{
    TEST_ASSERT_EQUAL( REGMEM_REQUEST_OK, RegMem_Reset() );
    CmsisHost_Reset();

    utNvic_UserHandlerCnt        = 0u;
    utNvic_UserDefaultHandlerCnt = 0u;
}


void tearDown( void )
{
    /* Mocks are verified by generated runner */
}

/* ========================== MODULE VERSION ================================ */

/**
 * \brief   Nvic_Get_ModuleVersion() returns version of the module.
 *
 * \details Reads the module version structure.
 *
 * \par Expected results
 * - Version is 1.0.0 (Major 1, Minor 0, Patch 0).
 */
void Ut_Nvic_Get_ModuleVersion_ReturnsVersion( void )
{
    nvic_ModuleVersion_t version = Nvic_Get_ModuleVersion();

    TEST_ASSERT_EQUAL_UINT8( 1u, version.Major );
    TEST_ASSERT_EQUAL_UINT8( 0u, version.Minor );
    TEST_ASSERT_EQUAL_UINT8( 0u, version.Patch );
}

/* ================================ TASK ==================================== */

/**
 * \brief   Nvic_Task() has no periodic processing.
 *
 * \details NVIC enable / priority registers and SCB VTOR / AIRCR preset, Nvic_Task() called.
 *
 * \par Expected results
 * - Registers not changed.
 */
void Ut_Nvic_Task_NoRegisterAccess( void )
{
    NVIC->ISER[ 0u ] = 0x00000101u;
    NVIC->IP[ 3u ]   = 0x50u;
    SCB->VTOR        = 0x20000000u;
    SCB->AIRCR       = 0x00000300u;

    Nvic_Task();

    TEST_ASSERT_EQUAL_HEX32( 0x00000101u, NVIC->ISER[ 0u ] );
    TEST_ASSERT_EQUAL_HEX8( 0x50u, NVIC->IP[ 3u ] );
    TEST_ASSERT_EQUAL_HEX32( 0x20000000u, SCB->VTOR );
    TEST_ASSERT_EQUAL_HEX32( 0x00000300u, SCB->AIRCR );
}

/* ============================ INITIALIZATION ============================== */

/**
 * \brief   Nvic_Init() relocates vector table and sets priority grouping.
 *
 * \details Initializes the module.
 *
 * \par Expected results
 * - SCB->VTOR = address of the vector table in RAM (Nvic_Get_StackPointerAddr()).
 * - AIRCR.PRIGROUP = 3 (4 bits pre-emption priority, 0 bits sub-priority).
 * - One DSB instruction executed (VTOR write completed, instruction cache preset as enabled -
 *   cache enabling executes no barrier).
 */
void Ut_Nvic_Init_SetsVectorTableAndPriorityGrouping( void )
{
    SCB->CCR = SCB_CCR_IC_Msk;

    Nvic_Init();

    TEST_ASSERT_EQUAL_HEX32( Nvic_Get_StackPointerAddr(), SCB->VTOR );
    TEST_ASSERT_EQUAL_HEX32( UT_NVIC_PRIGROUP_4 << SCB_AIRCR_PRIGROUP_Pos, SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk );
    TEST_ASSERT_EQUAL_UINT32( 1u, CmsisHost_Get_InstrCnt( CMSISHOST_INSTR_DSB ) );  /* VTOR write completed */
}


/**
 * \brief   Nvic_Init() enables the instruction cache, data cache stays disabled.
 *
 * \details CCR is cleared (caches disabled after reset), the module is initialized.
 *
 * \par Expected results
 * - CCR.IC = 1, CCR.DC = 0.
 * - I-Cache invalidated before enabling (barriers DSB / ISB executed).
 */
void Ut_Nvic_Init_InstructionCacheEnabled( void )
{
    SCB->CCR = 0u;

    Nvic_Init();

    TEST_ASSERT_EQUAL_HEX32( SCB_CCR_IC_Msk, SCB->CCR & ( SCB_CCR_IC_Msk | SCB_CCR_DC_Msk ) );
    TEST_ASSERT_TRUE( 3u < CmsisHost_Get_InstrCnt( CMSISHOST_INSTR_DSB ) );
    TEST_ASSERT_TRUE( 2u < CmsisHost_Get_InstrCnt( CMSISHOST_INSTR_ISB ) );
}


/**
 * \brief   Nvic_Init() fills core vectors with default handlers.
 *
 * \details Initializes the module and reads Reset and SysTick handlers.
 *
 * \par Expected results
 * - Reset handler is StartUp_Handler.
 * - SysTick handler is not NULL.
 */
void Ut_Nvic_Init_CoreVectorsPointToDefaultHandlers( void )
{
    nvic_IsrCallback_t resetHandler = NULL;
    nvic_IsrCallback_t sysTickHandler = NULL;

    Nvic_Init();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Handler( NVIC_CORE_IRQ_RESET,   &resetHandler   ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Handler( NVIC_CORE_IRQ_SYSTICK, &sysTickHandler ) );

    TEST_ASSERT_EQUAL_PTR( StartUp_Handler, resetHandler );
    TEST_ASSERT_NOT_NULL( sysTickHandler );
}


/**
 * \brief   Nvic_Init() fills all peripheral vectors with the default handler.
 *
 * \details Initializes the module and reads handlers of all peripheral IRQs.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK for every IRQ, all handlers are equal and not NULL.
 */
void Ut_Nvic_Init_AllPeriphVectorsHaveDefaultHandler( void )
{
    nvic_IsrCallback_t firstHandler = NULL;

    Nvic_Init();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Handler( (nvic_PeriphIrqList_t)0u, &firstHandler ) );
    TEST_ASSERT_NOT_NULL( firstHandler );

    for( uint32_t irqId = 0u; NVIC_PERIPH_IRQ_SIZE > irqId; irqId++ )
    {
        nvic_IsrCallback_t handler = NULL;

        TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Handler( (nvic_PeriphIrqList_t)irqId, &handler ) );
        TEST_ASSERT_EQUAL_PTR( firstHandler, handler );
    }
}


/**
 * \brief   Nvic_Deinit() restores default peripheral handlers.
 *
 * \details Initializes the module, sets user handler of USART1, deinitializes the
 *          module and reads the USART1 handler.
 *
 * \par Expected results
 * - USART1 handler is not the user handler any more.
 */
void Ut_Nvic_Deinit_RestoresDefaultHandlers( void )
{
    nvic_IsrCallback_t handler = NULL;

    Nvic_Init();
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( NVIC_PERIPH_IRQ_USART1, Ut_Nvic_UserHandler ) );

    Nvic_Deinit();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Handler( NVIC_PERIPH_IRQ_USART1, &handler ) );
    TEST_ASSERT_NOT_EQUAL( (uintptr_t)Ut_Nvic_UserHandler, (uintptr_t)handler );
}

/* ========================= PERIPHERAL HANDLERS ============================ */

/**
 * \brief   Nvic_Set_PeriphIrq_Handler() stores the handler in the vector table.
 *
 * \details Sets user handler of TIM2 and reads it back.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, handler reads back the user handler.
 */
void Ut_Nvic_Set_PeriphIrq_Handler_StoresHandler( void )
{
    nvic_IsrCallback_t handler = NULL;

    Nvic_Init();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Handler( NVIC_PERIPH_IRQ_TIM2, Ut_Nvic_UserHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Handler( NVIC_PERIPH_IRQ_TIM2, &handler ) );

    TEST_ASSERT_EQUAL_PTR( Ut_Nvic_UserHandler, handler );
}


/**
 * \brief   Nvic_Set_PeriphIrq_Handler() rejects invalid arguments.
 *
 * \details Calls the function with IRQ out of range and with NULL handler.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in both cases.
 */
void Ut_Nvic_Set_PeriphIrq_Handler_InvalidArgs_ReturnsError( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Handler( NVIC_PERIPH_IRQ_SIZE, Ut_Nvic_UserHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Handler( NVIC_PERIPH_IRQ_TIM2, NULL ) );
}


/**
 * \brief   Nvic_Get_PeriphIrq_Handler() rejects invalid arguments.
 *
 * \details Calls the function with IRQ out of range and with NULL output pointer.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in both cases.
 */
void Ut_Nvic_Get_PeriphIrq_Handler_InvalidArgs_ReturnsError( void )
{
    nvic_IsrCallback_t handler = NULL;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_PeriphIrq_Handler( NVIC_PERIPH_IRQ_SIZE, &handler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_PeriphIrq_Handler( NVIC_PERIPH_IRQ_TIM2, NULL ) );
}


/**
 * \brief   Default peripheral handler calls user default handler.
 *
 * \details Initializes the module, sets user default handler and calls the handler
 *          of unconfigured WWDG interrupt.
 *
 * \par Expected results
 * - User default handler is called once.
 */
void Ut_Nvic_DefaultPeriphHandler_CallsUserDefaultHandler( void )
{
    nvic_IsrCallback_t handler = NULL;

    Nvic_Init();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_DefaultHandler( Ut_Nvic_UserDefaultHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Handler( NVIC_PERIPH_IRQ_WWDG, &handler ) );

    handler();  /* Unconfigured interrupt */

    TEST_ASSERT_EQUAL_UINT32( 1u, utNvic_UserDefaultHandlerCnt );
}


/**
 * \brief   Nvic_Set_DefaultHandler() rejects NULL handler.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR.
 */
void Ut_Nvic_Set_DefaultHandler_Null_ReturnsError( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_DefaultHandler( NULL ) );
}

/* ========================= PERIPHERAL PRIORITY ============================ */

/**
 * \brief   Nvic_Set_PeriphIrq_Prio() writes priority to IP register.
 *
 * \details Sets priority 5 of USART1.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, IP byte of USART1 = 5 << 4 (4 implemented bits).
 * - IP bytes of neighbour IRQs stay 0.
 */
void Ut_Nvic_Set_PeriphIrq_Prio_WritesIp( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Prio( NVIC_PERIPH_IRQ_USART1, 5u ) );

    TEST_ASSERT_EQUAL_HEX8( 5u << UT_NVIC_PRIO_SHIFT, NVIC->IP[ NVIC_PERIPH_IRQ_USART1 ] );
    TEST_ASSERT_EQUAL_HEX8( 0u, NVIC->IP[ NVIC_PERIPH_IRQ_USART1 - 1u ] );
    TEST_ASSERT_EQUAL_HEX8( 0u, NVIC->IP[ NVIC_PERIPH_IRQ_USART1 + 1u ] );
}


/**
 * \brief   Nvic_Set_PeriphIrq_Prio() handles boundary IRQ and priority.
 *
 * \details Sets maximal priority value (15) of the last peripheral IRQ.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, IP byte of the last IRQ = 15 << 4.
 */
void Ut_Nvic_Set_PeriphIrq_Prio_MaxPrio_LastIrq_WritesIp( void )
{
    const nvic_PeriphIrqList_t lastIrq = (nvic_PeriphIrqList_t)( NVIC_PERIPH_IRQ_SIZE - 1u );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Prio( lastIrq, UT_NVIC_PRIO_MAX ) );

    TEST_ASSERT_EQUAL_HEX8( UT_NVIC_PRIO_MAX << UT_NVIC_PRIO_SHIFT, NVIC->IP[ lastIrq ] );
}


/**
 * \brief   Nvic_Set_PeriphIrq_Prio() rejects invalid arguments.
 *
 * \details Calls the function with priority 16 (above maximum) and with IRQ out of
 *          range.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in both cases, IP byte of USART1 stays 0.
 */
void Ut_Nvic_Set_PeriphIrq_Prio_InvalidArgs_ReturnsErrorWithoutWrite( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Prio( NVIC_PERIPH_IRQ_USART1, UT_NVIC_PRIO_MAX + 1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Prio( NVIC_PERIPH_IRQ_SIZE,   1u ) );

    TEST_ASSERT_EQUAL_HEX8( 0u, NVIC->IP[ NVIC_PERIPH_IRQ_USART1 ] );
}


/**
 * \brief   Nvic_Get_PeriphIrq_Prio() reads priority from IP register.
 *
 * \details Presets IP byte of TIM2 to 9 << 4 and reads the priority.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, priority 9.
 */
void Ut_Nvic_Get_PeriphIrq_Prio_ReadsIp( void )
{
    nvic_IrqPrio_t prio = 0u;

    NVIC->IP[ NVIC_PERIPH_IRQ_TIM2 ] = (uint8_t)( 9u << UT_NVIC_PRIO_SHIFT );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Prio( NVIC_PERIPH_IRQ_TIM2, &prio ) );
    TEST_ASSERT_EQUAL_UINT32( 9u, prio );
}


/**
 * \brief   Nvic_Get_PeriphIrq_Prio() rejects NULL output pointer.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR.
 */
void Ut_Nvic_Get_PeriphIrq_Prio_NullPtr_ReturnsError( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_PeriphIrq_Prio( NVIC_PERIPH_IRQ_TIM2, NULL ) );
}

/* ====================== PERIPHERAL ENABLE / PENDING ======================= */

/**
 * \brief   Nvic_Set_PeriphIrq_Active() enables the IRQ in ISER register.
 *
 * \details Enables USART1 IRQ.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, ISER contains only USART1 bit, ICER is not written.
 */
void Ut_Nvic_Set_PeriphIrq_Active_WritesIserBit( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( NVIC_PERIPH_IRQ_USART1 ) );

    TEST_ASSERT_EQUAL_HEX32( UT_NVIC_REG_MASK( NVIC_PERIPH_IRQ_USART1 ), NVIC->ISER[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_USART1 ) ] );
    TEST_ASSERT_EQUAL_HEX32( 0u, NVIC->ICER[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_USART1 ) ] );
}


/**
 * \brief   Nvic_Set_PeriphIrq_Active() writes the correct ISER bit for every IRQ.
 *
 * \details Enables all peripheral IRQs one by one (including IRQs above 31).
 *
 * \par Expected results
 * - NVIC_REQUEST_OK for every IRQ, only the bit of the IRQ is written to its ISER
 *   register (write-1-to-set register).
 */
void Ut_Nvic_Set_PeriphIrq_Active_AllIrqs_WritesOwnBit( void )
{
    for( uint32_t irqId = 0u; NVIC_PERIPH_IRQ_SIZE > irqId; irqId++ )
    {
        TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Active( (nvic_PeriphIrqList_t)irqId ) );

        /* ISER is write-1-to-set: only the IRQ bit is written */
        TEST_ASSERT_EQUAL_HEX32( UT_NVIC_REG_MASK( irqId ), NVIC->ISER[ UT_NVIC_REG_IDX( irqId ) ] );
    }
}


/**
 * \brief   Nvic_Set_PeriphIrq_Inactive() disables the IRQ in ICER register.
 *
 * \details Disables TIM2 IRQ.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, ICER contains only TIM2 bit, ISER is not written.
 * - One DSB and one ISB instruction executed (IRQ disabled before return).
 */
void Ut_Nvic_Set_PeriphIrq_Inactive_WritesIcerBitWithBarriers( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Inactive( NVIC_PERIPH_IRQ_TIM2 ) );

    TEST_ASSERT_EQUAL_HEX32( UT_NVIC_REG_MASK( NVIC_PERIPH_IRQ_TIM2 ), NVIC->ICER[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_TIM2 ) ] );
    TEST_ASSERT_EQUAL_HEX32( 0u, NVIC->ISER[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_TIM2 ) ] );

    /* Interrupt is disabled before return (ARM recommended sequence) */
    TEST_ASSERT_EQUAL_UINT32( 1u, CmsisHost_Get_InstrCnt( CMSISHOST_INSTR_DSB ) );
    TEST_ASSERT_EQUAL_UINT32( 1u, CmsisHost_Get_InstrCnt( CMSISHOST_INSTR_ISB ) );
}


/**
 * \brief   Enable / disable functions reject IRQ out of range.
 *
 * \details Calls Nvic_Set_PeriphIrq_Active() and Nvic_Set_PeriphIrq_Inactive() with
 *          NVIC_PERIPH_IRQ_SIZE.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in both cases.
 */
void Ut_Nvic_Set_PeriphIrq_ActiveInactive_InvalidIrq_ReturnsError( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Active( NVIC_PERIPH_IRQ_SIZE ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_PeriphIrq_Inactive( NVIC_PERIPH_IRQ_SIZE ) );
}


/**
 * \brief   Nvic_Set_PeriphIrq_Pending() with active flag sets pending bit.
 *
 * \details Sets EXTI0 IRQ pending.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, ISPR contains only EXTI0 bit, ICPR is not written.
 */
void Ut_Nvic_Set_PeriphIrq_Pending_Active_WritesIspr( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( NVIC_PERIPH_IRQ_EXTI0, NVIC_IRQ_ACTIVE ) );

    TEST_ASSERT_EQUAL_HEX32( UT_NVIC_REG_MASK( NVIC_PERIPH_IRQ_EXTI0 ), NVIC->ISPR[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_EXTI0 ) ] );
    TEST_ASSERT_EQUAL_HEX32( 0u, NVIC->ICPR[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_EXTI0 ) ] );
}


/**
 * \brief   Nvic_Set_PeriphIrq_Pending() with inactive flag clears pending bit.
 *
 * \details Clears EXTI0 pending IRQ.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, ICPR contains only EXTI0 bit, ISPR is not written.
 */
void Ut_Nvic_Set_PeriphIrq_Pending_Inactive_WritesIcpr( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_PeriphIrq_Pending( NVIC_PERIPH_IRQ_EXTI0, NVIC_IRQ_INACTIVE ) );

    TEST_ASSERT_EQUAL_HEX32( UT_NVIC_REG_MASK( NVIC_PERIPH_IRQ_EXTI0 ), NVIC->ICPR[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_EXTI0 ) ] );
    TEST_ASSERT_EQUAL_HEX32( 0u, NVIC->ISPR[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_EXTI0 ) ] );
}


/**
 * \brief   Nvic_Get_PeriphIrq_Pending() reads pending bit of the IRQ.
 *
 * \details Presets ISPR bit of USART1 and reads pending state of USART1 and of the
 *          next IRQ in the same register.
 *
 * \par Expected results
 * - USART1: NVIC_IRQ_ACTIVE.
 * - Next IRQ: NVIC_IRQ_INACTIVE.
 */
void Ut_Nvic_Get_PeriphIrq_Pending_ReadsIsprBit( void )
{
    nvic_IrqFlag_t irqFlag = NVIC_IRQ_INACTIVE;

    NVIC->ISPR[ UT_NVIC_REG_IDX( NVIC_PERIPH_IRQ_USART1 ) ] = UT_NVIC_REG_MASK( NVIC_PERIPH_IRQ_USART1 );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Pending( NVIC_PERIPH_IRQ_USART1, &irqFlag ) );
    TEST_ASSERT_EQUAL( NVIC_IRQ_ACTIVE, irqFlag );

    /* Neighbour IRQ in the same register is not pending */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_PeriphIrq_Pending( (nvic_PeriphIrqList_t)( NVIC_PERIPH_IRQ_USART1 + 1u ), &irqFlag ) );
    TEST_ASSERT_EQUAL( NVIC_IRQ_INACTIVE, irqFlag );
}


/**
 * \brief   Nvic_Get_PeriphIrq_Pending() rejects invalid arguments.
 *
 * \details Calls the function with IRQ out of range and with NULL output pointer.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in both cases.
 */
void Ut_Nvic_Get_PeriphIrq_Pending_InvalidArgs_ReturnsError( void )
{
    nvic_IrqFlag_t irqFlag = NVIC_IRQ_INACTIVE;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_PeriphIrq_Pending( NVIC_PERIPH_IRQ_SIZE,   &irqFlag ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_PeriphIrq_Pending( NVIC_PERIPH_IRQ_USART1, NULL ) );
}

/* ============================ CORE INTERRUPTS ============================= */

/**
 * \brief   Nvic_Set_CoreIrq_Handler() stores the core exception handler.
 *
 * \details Initializes the module, sets user SysTick handler, reads it back and calls
 *          it.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, handler reads back the user handler.
 * - User handler is called once.
 */
void Ut_Nvic_Set_CoreIrq_Handler_StoresHandler( void )
{
    nvic_IsrCallback_t handler = NULL;

    Nvic_Init();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Handler( NVIC_CORE_IRQ_SYSTICK, Ut_Nvic_UserHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Handler( NVIC_CORE_IRQ_SYSTICK, &handler ) );

    TEST_ASSERT_EQUAL_PTR( Ut_Nvic_UserHandler, handler );

    handler();
    TEST_ASSERT_EQUAL_UINT32( 1u, utNvic_UserHandlerCnt );
}


/**
 * \brief   Core handler setter / getter reject invalid arguments.
 *
 * \details Calls Nvic_Set_CoreIrq_Handler() and Nvic_Get_CoreIrq_Handler() with
 *          exception out of range and with NULL handler / output pointer.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in all cases.
 */
void Ut_Nvic_Set_CoreIrq_Handler_InvalidArgs_ReturnsError( void )
{
    nvic_IsrCallback_t handler = NULL;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Handler( NVIC_CORE_IRQ_SIZE,    Ut_Nvic_UserHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Handler( NVIC_CORE_IRQ_SYSTICK, NULL ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_CoreIrq_Handler( NVIC_CORE_IRQ_SIZE,    &handler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_CoreIrq_Handler( NVIC_CORE_IRQ_SYSTICK, NULL ) );
}


/**
 * \brief   Default core handler calls user default handler.
 *
 * \details Initializes the module, sets user default handler and calls the handler of
 *          reserved core exception 7 (no dedicated default handler).
 *
 * \par Expected results
 * - User default handler is called once.
 */
void Ut_Nvic_DefaultCoreHandler_CallsUserDefaultHandler( void )
{
    nvic_IsrCallback_t handler = NULL;

    Nvic_Init();

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_DefaultHandler( Ut_Nvic_UserDefaultHandler ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Handler( UT_NVIC_CORE_IRQ_RESERVED, &handler ) );

    handler();  /* Core exception without dedicated default handler */

    TEST_ASSERT_EQUAL_UINT32( 1u, utNvic_UserDefaultHandlerCnt );
}


/**
 * \brief   Nvic_Set_CoreIrq_Prio() writes SysTick priority to SHPR register.
 *
 * \details Sets priority 3 of SysTick.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, SHPR byte 11 = 3 << 4.
 */
void Ut_Nvic_Set_CoreIrq_Prio_SysTick_WritesShpr( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK, 3u ) );

    TEST_ASSERT_EQUAL_HEX8( 3u << UT_NVIC_PRIO_SHIFT, SCB->SHPR[ UT_NVIC_SHPR_IDX_SYSTICK ] );
}


/**
 * \brief   Nvic_Set_CoreIrq_Prio() writes MemManage priority to the first SHPR byte.
 *
 * \details Sets maximal priority value (15) of MemManage fault.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, SHPR byte 0 = 15 << 4.
 */
void Ut_Nvic_Set_CoreIrq_Prio_MemFault_WritesFirstShpr( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_MEMFAULT, UT_NVIC_PRIO_MAX ) );

    TEST_ASSERT_EQUAL_HEX8( UT_NVIC_PRIO_MAX << UT_NVIC_PRIO_SHIFT, SCB->SHPR[ UT_NVIC_SHPR_IDX_MEMFAULT ] );
}


/**
 * \brief   Nvic_Set_CoreIrq_Prio() rejects exceptions with fixed priority and invalid
 *          arguments.
 *
 * \details Calls the function for Reset, NMI and HardFault, for SysTick with priority
 *          16 and for exception out of range.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in all cases, no SHPR byte is written.
 */
void Ut_Nvic_Set_CoreIrq_Prio_FixedPrioException_ReturnsError( void )
{
    /* Reset, NMI and HardFault have fixed priority */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_RESET,     1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_NMI,       1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_HARDFAULT, 1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK,   UT_NVIC_PRIO_MAX + 1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SIZE,      1u ) );

    for( uint32_t shprIdx = 0u; UT_NVIC_SHPR_IDX_SYSTICK >= shprIdx; shprIdx++ )
    {
        TEST_ASSERT_EQUAL_HEX8( 0u, SCB->SHPR[ shprIdx ] );
    }
}


/**
 * \brief   Nvic_Get_CoreIrq_Prio() reads priority from SHPR register.
 *
 * \details Presets SysTick SHPR byte to 7 << 4 and reads the priority, then calls the
 *          function for NMI (fixed priority) and with NULL output pointer.
 *
 * \par Expected results
 * - SysTick: NVIC_REQUEST_OK, priority 7.
 * - NMI, NULL pointer: NVIC_REQUEST_ERROR.
 */
void Ut_Nvic_Get_CoreIrq_Prio_ReadsShpr( void )
{
    nvic_IrqPrio_t prio = 0u;

    SCB->SHPR[ UT_NVIC_SHPR_IDX_SYSTICK ] = (uint8_t)( 7u << UT_NVIC_PRIO_SHIFT );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK, &prio ) );
    TEST_ASSERT_EQUAL_UINT32( 7u, prio );

    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_CoreIrq_Prio( NVIC_CORE_IRQ_NMI,     &prio ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK, NULL  ) );
}


/**
 * \brief   Nvic_Get_FaultStatus() reads SCB fault registers.
 *
 * \details Presets CFSR, HFSR, MMFAR and BFAR and reads the fault status.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, all four values read back.
 * - CFSR and HFSR are not cleared (only read).
 */
void Ut_Nvic_Get_FaultStatus_ReadsScbFaultRegisters( void )
{
    nvic_FaultStatus_t faultStatus = { 0u };

    SCB->CFSR  = 0x00000400u;
    SCB->HFSR  = 0x40000000u;
    SCB->MMFAR = 0x20001000u;
    SCB->BFAR  = 0x40021000u;

    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Get_FaultStatus( &faultStatus ) );
    TEST_ASSERT_EQUAL_HEX32( 0x00000400u, faultStatus.Cfsr  );
    TEST_ASSERT_EQUAL_HEX32( 0x40000000u, faultStatus.Hfsr  );
    TEST_ASSERT_EQUAL_HEX32( 0x20001000u, faultStatus.Mmfar );
    TEST_ASSERT_EQUAL_HEX32( 0x40021000u, faultStatus.Bfar  );

    /* Fault flags are only read, never cleared */
    TEST_ASSERT_EQUAL_HEX32( 0x00000400u, SCB->CFSR );
    TEST_ASSERT_EQUAL_HEX32( 0x40000000u, SCB->HFSR );
}


/**
 * \brief   Nvic_Get_FaultStatus() rejects NULL output pointer.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR.
 */
void Ut_Nvic_Get_FaultStatus_NullPointer_ReturnsError( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Get_FaultStatus( NULL ) );
}

/* =========================== LOCAL FUNCTIONS ============================== */

/** User interrupt handler - counts calls */
static void Ut_Nvic_UserHandler( void )
{
    utNvic_UserHandlerCnt++;
}


/** User default handler - counts calls */
static void Ut_Nvic_UserDefaultHandler( void )
{
    utNvic_UserDefaultHandlerCnt++;
}
