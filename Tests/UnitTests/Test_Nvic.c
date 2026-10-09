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

/** Index of SHP byte of SysTick (exception 15 - 4) */
#define UT_NVIC_SHP_IDX_SYSTICK             ( 11u )

/** Index of SHP byte of MemManage fault (exception 4 - 4) */
#define UT_NVIC_SHP_IDX_MEMFAULT            ( 0u )

/** Core IRQ index of reserved exception 7 (Cortex-M4, exception number - 1) */
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

/* ========================= INTERRUPT IDENTIFICATION ======================= */

/**
 * \brief   Peripheral interrupt identifications match the CMSIS IRQ numbers of the device.
 *
 * \details Compares every item of nvic_PeriphIrqList_t defined for the selected device with
 *          the IRQn of the CMSIS device header (same preprocessor conditions as Nvic_Types.h).
 *          Executed for every unit test preset - covers all STM32L4 / STM32L4+ variants.
 *
 * \par Expected results
 * - Every interrupt identification equals the CMSIS IRQ number of the device.
 */
void Ut_Nvic_PeriphIrqList_MatchesCmsisIrqNumbers( void )
{
    TEST_ASSERT_EQUAL_INT( WWDG_IRQn, NVIC_PERIPH_IRQ_WWDG );
    TEST_ASSERT_EQUAL_INT( PVD_PVM_IRQn, NVIC_PERIPH_IRQ_PVD_PVM );
    TEST_ASSERT_EQUAL_INT( TAMP_STAMP_IRQn, NVIC_PERIPH_IRQ_TAMP_STAMP );
    TEST_ASSERT_EQUAL_INT( RTC_WKUP_IRQn, NVIC_PERIPH_IRQ_RTC_WKUP );
    TEST_ASSERT_EQUAL_INT( FLASH_IRQn, NVIC_PERIPH_IRQ_FLASH );
    TEST_ASSERT_EQUAL_INT( RCC_IRQn, NVIC_PERIPH_IRQ_RCC );
    TEST_ASSERT_EQUAL_INT( EXTI0_IRQn, NVIC_PERIPH_IRQ_EXTI0 );
    TEST_ASSERT_EQUAL_INT( EXTI1_IRQn, NVIC_PERIPH_IRQ_EXTI1 );
    TEST_ASSERT_EQUAL_INT( EXTI2_IRQn, NVIC_PERIPH_IRQ_EXTI2 );
    TEST_ASSERT_EQUAL_INT( EXTI3_IRQn, NVIC_PERIPH_IRQ_EXTI3 );
    TEST_ASSERT_EQUAL_INT( EXTI4_IRQn, NVIC_PERIPH_IRQ_EXTI4 );
    TEST_ASSERT_EQUAL_INT( DMA1_Channel1_IRQn, NVIC_PERIPH_IRQ_DMA1_CHANNEL1 );
    TEST_ASSERT_EQUAL_INT( DMA1_Channel2_IRQn, NVIC_PERIPH_IRQ_DMA1_CHANNEL2 );
    TEST_ASSERT_EQUAL_INT( DMA1_Channel3_IRQn, NVIC_PERIPH_IRQ_DMA1_CHANNEL3 );
    TEST_ASSERT_EQUAL_INT( DMA1_Channel4_IRQn, NVIC_PERIPH_IRQ_DMA1_CHANNEL4 );
    TEST_ASSERT_EQUAL_INT( DMA1_Channel5_IRQn, NVIC_PERIPH_IRQ_DMA1_CHANNEL5 );
    TEST_ASSERT_EQUAL_INT( DMA1_Channel6_IRQn, NVIC_PERIPH_IRQ_DMA1_CHANNEL6 );
    TEST_ASSERT_EQUAL_INT( DMA1_Channel7_IRQn, NVIC_PERIPH_IRQ_DMA1_CHANNEL7 );
#if defined (ADC2)
    TEST_ASSERT_EQUAL_INT( ADC1_2_IRQn, NVIC_PERIPH_IRQ_ADC1_2 );
#else
    TEST_ASSERT_EQUAL_INT( ADC1_IRQn, NVIC_PERIPH_IRQ_ADC1 );
#endif
#if defined (CAN1)
    TEST_ASSERT_EQUAL_INT( CAN1_TX_IRQn, NVIC_PERIPH_IRQ_CAN1_TX );
    TEST_ASSERT_EQUAL_INT( CAN1_RX0_IRQn, NVIC_PERIPH_IRQ_CAN1_RX0 );
    TEST_ASSERT_EQUAL_INT( CAN1_RX1_IRQn, NVIC_PERIPH_IRQ_CAN1_RX1 );
    TEST_ASSERT_EQUAL_INT( CAN1_SCE_IRQn, NVIC_PERIPH_IRQ_CAN1_SCE );
#endif
    TEST_ASSERT_EQUAL_INT( EXTI9_5_IRQn, NVIC_PERIPH_IRQ_EXTI9_5 );
    TEST_ASSERT_EQUAL_INT( TIM1_BRK_TIM15_IRQn, NVIC_PERIPH_IRQ_TIM1_BRK_TIM15 );
    TEST_ASSERT_EQUAL_INT( TIM1_UP_TIM16_IRQn, NVIC_PERIPH_IRQ_TIM1_UP_TIM16 );
#if defined (TIM17)
    TEST_ASSERT_EQUAL_INT( TIM1_TRG_COM_TIM17_IRQn, NVIC_PERIPH_IRQ_TIM1_TRG_COM_TIM17 );
#else
    TEST_ASSERT_EQUAL_INT( TIM1_TRG_COM_IRQn, NVIC_PERIPH_IRQ_TIM1_TRG_COM );
#endif
    TEST_ASSERT_EQUAL_INT( TIM1_CC_IRQn, NVIC_PERIPH_IRQ_TIM1_CC );
    TEST_ASSERT_EQUAL_INT( TIM2_IRQn, NVIC_PERIPH_IRQ_TIM2 );
#if defined (TIM3)
    TEST_ASSERT_EQUAL_INT( TIM3_IRQn, NVIC_PERIPH_IRQ_TIM3 );
#endif
#if defined (TIM4)
    TEST_ASSERT_EQUAL_INT( TIM4_IRQn, NVIC_PERIPH_IRQ_TIM4 );
#endif
    TEST_ASSERT_EQUAL_INT( I2C1_EV_IRQn, NVIC_PERIPH_IRQ_I2C1_EV );
    TEST_ASSERT_EQUAL_INT( I2C1_ER_IRQn, NVIC_PERIPH_IRQ_I2C1_ER );
#if defined (I2C2)
    TEST_ASSERT_EQUAL_INT( I2C2_EV_IRQn, NVIC_PERIPH_IRQ_I2C2_EV );
    TEST_ASSERT_EQUAL_INT( I2C2_ER_IRQn, NVIC_PERIPH_IRQ_I2C2_ER );
#endif
    TEST_ASSERT_EQUAL_INT( SPI1_IRQn, NVIC_PERIPH_IRQ_SPI1 );
#if defined (SPI2)
    TEST_ASSERT_EQUAL_INT( SPI2_IRQn, NVIC_PERIPH_IRQ_SPI2 );
#endif
    TEST_ASSERT_EQUAL_INT( USART1_IRQn, NVIC_PERIPH_IRQ_USART1 );
    TEST_ASSERT_EQUAL_INT( USART2_IRQn, NVIC_PERIPH_IRQ_USART2 );
#if defined (USART3)
    TEST_ASSERT_EQUAL_INT( USART3_IRQn, NVIC_PERIPH_IRQ_USART3 );
#endif
    TEST_ASSERT_EQUAL_INT( EXTI15_10_IRQn, NVIC_PERIPH_IRQ_EXTI15_10 );
    TEST_ASSERT_EQUAL_INT( RTC_Alarm_IRQn, NVIC_PERIPH_IRQ_RTC_ALARM );
#if defined (DFSDM1_Filter3) && \
    !defined (SDMMC2)
    TEST_ASSERT_EQUAL_INT( DFSDM1_FLT3_IRQn, NVIC_PERIPH_IRQ_DFSDM1_FLT3 );
#endif
#if defined (TIM8)
    TEST_ASSERT_EQUAL_INT( TIM8_BRK_IRQn, NVIC_PERIPH_IRQ_TIM8_BRK );
    TEST_ASSERT_EQUAL_INT( TIM8_UP_IRQn, NVIC_PERIPH_IRQ_TIM8_UP );
    TEST_ASSERT_EQUAL_INT( TIM8_TRG_COM_IRQn, NVIC_PERIPH_IRQ_TIM8_TRG_COM );
    TEST_ASSERT_EQUAL_INT( TIM8_CC_IRQn, NVIC_PERIPH_IRQ_TIM8_CC );
#endif
#if defined (ADC3)
    TEST_ASSERT_EQUAL_INT( ADC3_IRQn, NVIC_PERIPH_IRQ_ADC3 );
#elif defined (SDMMC2)
    TEST_ASSERT_EQUAL_INT( SDMMC2_IRQn, NVIC_PERIPH_IRQ_SDMMC2 );
#endif
#if defined (FMC_Bank1_R)
    TEST_ASSERT_EQUAL_INT( FMC_IRQn, NVIC_PERIPH_IRQ_FMC );
#endif
#if defined (SDMMC1)
    TEST_ASSERT_EQUAL_INT( SDMMC1_IRQn, NVIC_PERIPH_IRQ_SDMMC1 );
#endif
#if defined (TIM5)
    TEST_ASSERT_EQUAL_INT( TIM5_IRQn, NVIC_PERIPH_IRQ_TIM5 );
#endif
#if defined (SPI3)
    TEST_ASSERT_EQUAL_INT( SPI3_IRQn, NVIC_PERIPH_IRQ_SPI3 );
#endif
#if defined (UART4)
    TEST_ASSERT_EQUAL_INT( UART4_IRQn, NVIC_PERIPH_IRQ_UART4 );
#endif
#if defined (UART5)
    TEST_ASSERT_EQUAL_INT( UART5_IRQn, NVIC_PERIPH_IRQ_UART5 );
#endif
#if defined (DAC1)
    TEST_ASSERT_EQUAL_INT( TIM6_DAC_IRQn, NVIC_PERIPH_IRQ_TIM6_DAC );
#else
    TEST_ASSERT_EQUAL_INT( TIM6_IRQn, NVIC_PERIPH_IRQ_TIM6 );
#endif
#if defined (TIM7)
    TEST_ASSERT_EQUAL_INT( TIM7_IRQn, NVIC_PERIPH_IRQ_TIM7 );
#endif
    TEST_ASSERT_EQUAL_INT( DMA2_Channel1_IRQn, NVIC_PERIPH_IRQ_DMA2_CHANNEL1 );
    TEST_ASSERT_EQUAL_INT( DMA2_Channel2_IRQn, NVIC_PERIPH_IRQ_DMA2_CHANNEL2 );
    TEST_ASSERT_EQUAL_INT( DMA2_Channel3_IRQn, NVIC_PERIPH_IRQ_DMA2_CHANNEL3 );
    TEST_ASSERT_EQUAL_INT( DMA2_Channel4_IRQn, NVIC_PERIPH_IRQ_DMA2_CHANNEL4 );
    TEST_ASSERT_EQUAL_INT( DMA2_Channel5_IRQn, NVIC_PERIPH_IRQ_DMA2_CHANNEL5 );
#if defined (DFSDM1_Filter0)
    TEST_ASSERT_EQUAL_INT( DFSDM1_FLT0_IRQn, NVIC_PERIPH_IRQ_DFSDM1_FLT0 );
    TEST_ASSERT_EQUAL_INT( DFSDM1_FLT1_IRQn, NVIC_PERIPH_IRQ_DFSDM1_FLT1 );
#endif
#if defined (DFSDM1_Filter2) && \
    !defined (SDMMC2)
    TEST_ASSERT_EQUAL_INT( DFSDM1_FLT2_IRQn, NVIC_PERIPH_IRQ_DFSDM1_FLT2 );
#endif
    TEST_ASSERT_EQUAL_INT( COMP_IRQn, NVIC_PERIPH_IRQ_COMP );
    TEST_ASSERT_EQUAL_INT( LPTIM1_IRQn, NVIC_PERIPH_IRQ_LPTIM1 );
    TEST_ASSERT_EQUAL_INT( LPTIM2_IRQn, NVIC_PERIPH_IRQ_LPTIM2 );
#if defined (USB_OTG_FS)
    TEST_ASSERT_EQUAL_INT( OTG_FS_IRQn, NVIC_PERIPH_IRQ_OTG_FS );
#elif defined (USB)
    TEST_ASSERT_EQUAL_INT( USB_IRQn, NVIC_PERIPH_IRQ_USB );
#endif
    TEST_ASSERT_EQUAL_INT( DMA2_Channel6_IRQn, NVIC_PERIPH_IRQ_DMA2_CHANNEL6 );
    TEST_ASSERT_EQUAL_INT( DMA2_Channel7_IRQn, NVIC_PERIPH_IRQ_DMA2_CHANNEL7 );
    TEST_ASSERT_EQUAL_INT( LPUART1_IRQn, NVIC_PERIPH_IRQ_LPUART1 );
#if defined (OCTOSPI1)
    TEST_ASSERT_EQUAL_INT( OCTOSPI1_IRQn, NVIC_PERIPH_IRQ_OCTOSPI1 );
#else
    TEST_ASSERT_EQUAL_INT( QUADSPI_IRQn, NVIC_PERIPH_IRQ_QUADSPI );
#endif
    TEST_ASSERT_EQUAL_INT( I2C3_EV_IRQn, NVIC_PERIPH_IRQ_I2C3_EV );
    TEST_ASSERT_EQUAL_INT( I2C3_ER_IRQn, NVIC_PERIPH_IRQ_I2C3_ER );
#if defined (SAI1)
    TEST_ASSERT_EQUAL_INT( SAI1_IRQn, NVIC_PERIPH_IRQ_SAI1 );
#endif
#if defined (SAI2)
    TEST_ASSERT_EQUAL_INT( SAI2_IRQn, NVIC_PERIPH_IRQ_SAI2 );
#endif
#if defined (OCTOSPI2)
    TEST_ASSERT_EQUAL_INT( OCTOSPI2_IRQn, NVIC_PERIPH_IRQ_OCTOSPI2 );
#elif defined (SWPMI1)
    TEST_ASSERT_EQUAL_INT( SWPMI1_IRQn, NVIC_PERIPH_IRQ_SWPMI1 );
#endif
    TEST_ASSERT_EQUAL_INT( TSC_IRQn, NVIC_PERIPH_IRQ_TSC );
#if defined (DSI)
    TEST_ASSERT_EQUAL_INT( DSI_IRQn, NVIC_PERIPH_IRQ_DSI );
#elif defined (LCD)
    TEST_ASSERT_EQUAL_INT( LCD_IRQn, NVIC_PERIPH_IRQ_LCD );
#endif
#if defined (AES)
    TEST_ASSERT_EQUAL_INT( AES_IRQn, NVIC_PERIPH_IRQ_AES );
#endif
#if defined (HASH) && \
    !defined (DMAMUX1)
    TEST_ASSERT_EQUAL_INT( HASH_RNG_IRQn, NVIC_PERIPH_IRQ_HASH_RNG );
#else
    TEST_ASSERT_EQUAL_INT( RNG_IRQn, NVIC_PERIPH_IRQ_RNG );
#endif
    TEST_ASSERT_EQUAL_INT( FPU_IRQn, NVIC_PERIPH_IRQ_FPU );
#if defined (HASH) && \
    defined (DMAMUX1)
    TEST_ASSERT_EQUAL_INT( HASH_CRS_IRQn, NVIC_PERIPH_IRQ_HASH_CRS );
#elif defined (CRS)
    TEST_ASSERT_EQUAL_INT( CRS_IRQn, NVIC_PERIPH_IRQ_CRS );
#endif
#if defined (I2C4) && \
    defined (DMAMUX1)
    TEST_ASSERT_EQUAL_INT( I2C4_ER_IRQn, NVIC_PERIPH_IRQ_I2C4_ER );
    TEST_ASSERT_EQUAL_INT( I2C4_EV_IRQn, NVIC_PERIPH_IRQ_I2C4_EV );
#elif defined (I2C4)
    TEST_ASSERT_EQUAL_INT( I2C4_EV_IRQn, NVIC_PERIPH_IRQ_I2C4_EV );
    TEST_ASSERT_EQUAL_INT( I2C4_ER_IRQn, NVIC_PERIPH_IRQ_I2C4_ER );
#endif
#if defined (PSSI)
    TEST_ASSERT_EQUAL_INT( DCMI_PSSI_IRQn, NVIC_PERIPH_IRQ_DCMI_PSSI );
#elif defined (DCMI)
    TEST_ASSERT_EQUAL_INT( DCMI_IRQn, NVIC_PERIPH_IRQ_DCMI );
#endif
#if defined (CAN2)
    TEST_ASSERT_EQUAL_INT( CAN2_TX_IRQn, NVIC_PERIPH_IRQ_CAN2_TX );
    TEST_ASSERT_EQUAL_INT( CAN2_RX0_IRQn, NVIC_PERIPH_IRQ_CAN2_RX0 );
    TEST_ASSERT_EQUAL_INT( CAN2_RX1_IRQn, NVIC_PERIPH_IRQ_CAN2_RX1 );
    TEST_ASSERT_EQUAL_INT( CAN2_SCE_IRQn, NVIC_PERIPH_IRQ_CAN2_SCE );
#elif defined (PKA)
    TEST_ASSERT_EQUAL_INT( PKA_IRQn, NVIC_PERIPH_IRQ_PKA );
#endif
#if defined (DMA2D)
    TEST_ASSERT_EQUAL_INT( DMA2D_IRQn, NVIC_PERIPH_IRQ_DMA2D );
#endif
#if defined (LTDC)
    TEST_ASSERT_EQUAL_INT( LTDC_IRQn, NVIC_PERIPH_IRQ_LTDC );
    TEST_ASSERT_EQUAL_INT( LTDC_ER_IRQn, NVIC_PERIPH_IRQ_LTDC_ER );
#endif
#if defined (GFXMMU)
    TEST_ASSERT_EQUAL_INT( GFXMMU_IRQn, NVIC_PERIPH_IRQ_GFXMMU );
#endif
#if defined (DMAMUX1)
    TEST_ASSERT_EQUAL_INT( DMAMUX1_OVR_IRQn, NVIC_PERIPH_IRQ_DMAMUX1_OVR );
#endif
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
 * - One DSB instruction executed (VTOR write completed).
 */
void Ut_Nvic_Init_SetsVectorTableAndPriorityGrouping( void )
{
    Nvic_Init();

    TEST_ASSERT_EQUAL_HEX32( Nvic_Get_StackPointerAddr(), SCB->VTOR );
    TEST_ASSERT_EQUAL_HEX32( UT_NVIC_PRIGROUP_4 << SCB_AIRCR_PRIGROUP_Pos, SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk );
    TEST_ASSERT_EQUAL_UINT32( 1u, CmsisHost_Get_InstrCnt( CMSISHOST_INSTR_DSB ) );  /* VTOR write completed */
}


/**
 * \brief   Nvic_Init() disables lazy context save of the floating point state.
 *
 * \details FPCCR preset with ASPEN and LSPEN (reset value), the module is initialized.
 *
 * \note    Cortex-M4 r0p1 core erratum (Arm ID 776924 "VDIV or VSQRT instructions might not
 *          complete correctly when very short ISRs are used", protection taken over from
 *          STM32F4 device errata bug AB#669): with the FPU
 *          enabled and lazy context save, a VDIV / VSQRT interrupted by an ISR without FP
 *          instruction returning within 14 cycles (e.g. empty default SysTick handler) may not
 *          write its result. Workaround - FPCCR.LSPEN cleared.
 *
 * \par Expected results
 * - FPCCR.LSPEN = 0, FPCCR.ASPEN kept (automatic FP context save).
 */
void Ut_Nvic_Init_FpuLazyStackingDisabled( void )
{
    FPU->FPCCR = FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;

    Nvic_Init();

    TEST_ASSERT_EQUAL_HEX32( FPU_FPCCR_ASPEN_Msk, FPU->FPCCR & ( FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk ) );
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
 * \brief   Nvic_Set_CoreIrq_Prio() writes SysTick priority to SHP register.
 *
 * \details Sets priority 3 of SysTick.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, SHP byte 11 = 3 << 4.
 */
void Ut_Nvic_Set_CoreIrq_Prio_SysTick_WritesShp( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK, 3u ) );

    TEST_ASSERT_EQUAL_HEX8( 3u << UT_NVIC_PRIO_SHIFT, SCB->SHP[ UT_NVIC_SHP_IDX_SYSTICK ] );
}


/**
 * \brief   Nvic_Set_CoreIrq_Prio() writes MemManage priority to the first SHP byte.
 *
 * \details Sets maximal priority value (15) of MemManage fault.
 *
 * \par Expected results
 * - NVIC_REQUEST_OK, SHP byte 0 = 15 << 4.
 */
void Ut_Nvic_Set_CoreIrq_Prio_MemFault_WritesFirstShp( void )
{
    TEST_ASSERT_EQUAL( NVIC_REQUEST_OK, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_MEMFAULT, UT_NVIC_PRIO_MAX ) );

    TEST_ASSERT_EQUAL_HEX8( UT_NVIC_PRIO_MAX << UT_NVIC_PRIO_SHIFT, SCB->SHP[ UT_NVIC_SHP_IDX_MEMFAULT ] );
}


/**
 * \brief   Nvic_Set_CoreIrq_Prio() rejects exceptions with fixed priority and invalid
 *          arguments.
 *
 * \details Calls the function for Reset, NMI and HardFault, for SysTick with priority
 *          16 and for exception out of range.
 *
 * \par Expected results
 * - NVIC_REQUEST_ERROR in all cases, no SHP byte is written.
 */
void Ut_Nvic_Set_CoreIrq_Prio_FixedPrioException_ReturnsError( void )
{
    /* Reset, NMI and HardFault have fixed priority */
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_RESET,     1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_NMI,       1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_HARDFAULT, 1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SYSTICK,   UT_NVIC_PRIO_MAX + 1u ) );
    TEST_ASSERT_EQUAL( NVIC_REQUEST_ERROR, Nvic_Set_CoreIrq_Prio( NVIC_CORE_IRQ_SIZE,      1u ) );

    for( uint32_t shpIdx = 0u; UT_NVIC_SHP_IDX_SYSTICK >= shpIdx; shpIdx++ )
    {
        TEST_ASSERT_EQUAL_HEX8( 0u, SCB->SHP[ shpIdx ] );
    }
}


/**
 * \brief   Nvic_Get_CoreIrq_Prio() reads priority from SHP register.
 *
 * \details Presets SysTick SHP byte to 7 << 4 and reads the priority, then calls the
 *          function for NMI (fixed priority) and with NULL output pointer.
 *
 * \par Expected results
 * - SysTick: NVIC_REQUEST_OK, priority 7.
 * - NMI, NULL pointer: NVIC_REQUEST_ERROR.
 */
void Ut_Nvic_Get_CoreIrq_Prio_ReadsShp( void )
{
    nvic_IrqPrio_t prio = 0u;

    SCB->SHP[ UT_NVIC_SHP_IDX_SYSTICK ] = (uint8_t)( 7u << UT_NVIC_PRIO_SHIFT );

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
