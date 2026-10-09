/**
 * \author Mr.Nobody
 * \file Nvic_Types.h
 * \ingroup Nvic
 * \brief Nested Vector Interrupt Controller (NVIC) module global types definition
 *
 * This file contains the types definitions used across the module and are
 * available for other modules through Port file.
 *
 */

#ifndef NVIC_NVIC_TYPES_H
#define NVIC_NVIC_TYPES_H
/* ============================== INCLUDES ================================== */
#include "stdint.h"                         /* Module types definition        */
#include "Stm32.h"                          /* MCU core functionality         */
/* ========================== SYMBOLIC CONSTANTS ============================ */

/** Null pointer definition */
#define NVIC_NULL_PTR                   ( ( void* ) 0u )

/** 0 bit  for pre-emption priority, 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_0            ((uint32_t)0x00000007)

/** 1 bit  for pre-emption priority, 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_1            ((uint32_t)0x00000006)

/** 2 bits for pre-emption priority,2 bits for subpriority */
#define NVIC_PRIORITYGROUP_2            ((uint32_t)0x00000005)

/** 3 bits for pre-emption priority,1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_3            ((uint32_t)0x00000004)

/** 4 bits for pre-emption priority,0 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4            ((uint32_t)0x00000003)

/** STM32L4 uses 4 Bits for the Priority Levels */
#define NVIC_PRIO_BITS                  ( 4u )

/* ========================== EXPORTED MACROS =============================== */

/* ============================== TYPEDEFS ================================== */

/** \brief Type signaling major version of SW module */
typedef uint8_t nvic_MajorVersion_t;


/** \brief Type signaling minor version of SW module */
typedef uint8_t nvic_MinorVersion_t;


/** \brief Type signaling patch version of SW module */
typedef uint8_t nvic_PatchVersion_t;


/** \brief Type signaling actual version of SW module */
typedef struct
{
    nvic_MajorVersion_t Major; /**< Major version */
    nvic_MinorVersion_t Minor; /**< Minor version */
    nvic_PatchVersion_t Patch; /**< Patch version */
}   nvic_ModuleVersion_t;


/** Function status enumeration */
typedef enum
{
    NVIC_FUNCTION_INACTIVE = 0u, /**< Function status is inactive */
    NVIC_FUNCTION_ACTIVE         /**< Function status is active   */
}   nvic_FunctionState_t;


/** Flag states enumeration */
typedef enum
{
    NVIC_FLAG_INACTIVE = 0u, /**< Inactive flag state */
    NVIC_FLAG_ACTIVE         /**< Active flag state   */
}   nvic_FlagState_t;


/** Type defining interrupt service routines callback's */
typedef void ( *nvic_IsrCallback_t )( void );


/** Type defining interrupt priority
 * The range of priorities is defined by count of bits by symbolic constant
 * \ref NVIC_PRIO_BITS For STM32L4 is the range 0-15
 */
typedef uint32_t nvic_IrqPrio_t;


/** Enumeration list of IRQ states */
typedef enum
{
    NVIC_IRQ_INACTIVE = 0u, /**< Interrupt vector is inactive */
    NVIC_IRQ_ACTIVE         /**< Interrupt vector is active   */
}   nvic_IrqFlag_t;


/** Enumeration used to set interrupt vector state */
typedef enum
{
    NVIC_REQUEST_ERROR = 0u, /**< Processing request failed  */
    NVIC_REQUEST_OK,         /**< Processing request succeed */
}   nvic_RequestState_t;


/**
 * \enum nvic_PeriphIrqList_t
 * \brief Enumeration list of peripheral interrupt callback's
 *
 * Value of the item is position of the interrupt in the vector table (IRQn).
 * Interrupt lines not available on the selected device are not defined, shared
 * vector positions (different peripherals on different STM32L4 / STM32L4+
 * devices) are named according to the selected device (CMSIS IRQn_Type).
 * STM32L4+ devices are recognized by DMAMUX1, STM32L4P5 / L4Q5 by SDMMC2.
 */
typedef enum
{
    NVIC_PERIPH_IRQ_WWDG                = 0u,   /**< Window WatchDog interrupt                       */
    NVIC_PERIPH_IRQ_PVD_PVM             = 1u,   /**< PVD / PVM1 / PVM2 / PVM3 / PVM4 through EXTI    */
    NVIC_PERIPH_IRQ_TAMP_STAMP          = 2u,   /**< Tamper and TimeStamp through EXTI line          */
    NVIC_PERIPH_IRQ_RTC_WKUP            = 3u,   /**< RTC Wakeup through EXTI line                    */
    NVIC_PERIPH_IRQ_FLASH               = 4u,   /**< FLASH global interrupt                          */
    NVIC_PERIPH_IRQ_RCC                 = 5u,   /**< RCC global interrupt                            */
    NVIC_PERIPH_IRQ_EXTI0               = 6u,   /**< EXTI Line0 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI1               = 7u,   /**< EXTI Line1 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI2               = 8u,   /**< EXTI Line2 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI3               = 9u,   /**< EXTI Line3 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI4               = 10u,  /**< EXTI Line4 interrupt                            */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL1       = 11u,  /**< DMA1 Channel 1 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL2       = 12u,  /**< DMA1 Channel 2 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL3       = 13u,  /**< DMA1 Channel 3 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL4       = 14u,  /**< DMA1 Channel 4 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL5       = 15u,  /**< DMA1 Channel 5 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL6       = 16u,  /**< DMA1 Channel 6 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL7       = 17u,  /**< DMA1 Channel 7 global interrupt                 */
#if defined (ADC2)
    NVIC_PERIPH_IRQ_ADC1_2              = 18u,  /**< ADC1 and ADC2 global interrupts                 */
#else
    NVIC_PERIPH_IRQ_ADC1                = 18u,  /**< ADC1 global interrupt                           */
#endif
#if defined (CAN1)
    NVIC_PERIPH_IRQ_CAN1_TX             = 19u,  /**< CAN1 TX interrupt                               */
    NVIC_PERIPH_IRQ_CAN1_RX0            = 20u,  /**< CAN1 RX0 interrupt                              */
    NVIC_PERIPH_IRQ_CAN1_RX1            = 21u,  /**< CAN1 RX1 interrupt                              */
    NVIC_PERIPH_IRQ_CAN1_SCE            = 22u,  /**< CAN1 SCE interrupt                              */
#endif
    NVIC_PERIPH_IRQ_EXTI9_5             = 23u,  /**< EXTI Line[9:5] interrupts                       */
    NVIC_PERIPH_IRQ_TIM1_BRK_TIM15      = 24u,  /**< TIM1 Break and TIM15 global interrupt           */
    NVIC_PERIPH_IRQ_TIM1_UP_TIM16       = 25u,  /**< TIM1 Update and TIM16 global interrupt          */
#if defined (TIM17)
    NVIC_PERIPH_IRQ_TIM1_TRG_COM_TIM17  = 26u,  /**< TIM1 Trigger, Commutation and TIM17 interrupt   */
#else
    NVIC_PERIPH_IRQ_TIM1_TRG_COM        = 26u,  /**< TIM1 Trigger and Commutation interrupt          */
#endif
    NVIC_PERIPH_IRQ_TIM1_CC             = 27u,  /**< TIM1 Capture Compare interrupt                  */
    NVIC_PERIPH_IRQ_TIM2                = 28u,  /**< TIM2 global interrupt                           */
#if defined (TIM3)
    NVIC_PERIPH_IRQ_TIM3                = 29u,  /**< TIM3 global interrupt                           */
#endif
#if defined (TIM4)
    NVIC_PERIPH_IRQ_TIM4                = 30u,  /**< TIM4 global interrupt                           */
#endif
    NVIC_PERIPH_IRQ_I2C1_EV             = 31u,  /**< I2C1 Event interrupt                            */
    NVIC_PERIPH_IRQ_I2C1_ER             = 32u,  /**< I2C1 Error interrupt                            */
#if defined (I2C2)
    NVIC_PERIPH_IRQ_I2C2_EV             = 33u,  /**< I2C2 Event interrupt                            */
    NVIC_PERIPH_IRQ_I2C2_ER             = 34u,  /**< I2C2 Error interrupt                            */
#endif
    NVIC_PERIPH_IRQ_SPI1                = 35u,  /**< SPI1 global interrupt                           */
#if defined (SPI2)
    NVIC_PERIPH_IRQ_SPI2                = 36u,  /**< SPI2 global interrupt                           */
#endif
    NVIC_PERIPH_IRQ_USART1              = 37u,  /**< USART1 global interrupt                         */
    NVIC_PERIPH_IRQ_USART2              = 38u,  /**< USART2 global interrupt                         */
#if defined (USART3)
    NVIC_PERIPH_IRQ_USART3              = 39u,  /**< USART3 global interrupt                         */
#endif
    NVIC_PERIPH_IRQ_EXTI15_10           = 40u,  /**< EXTI Line[15:10] interrupts                     */
    NVIC_PERIPH_IRQ_RTC_ALARM           = 41u,  /**< RTC Alarm (A and B) through EXTI line           */
#if defined (DFSDM1_Filter3) && \
    !defined (SDMMC2)
    NVIC_PERIPH_IRQ_DFSDM1_FLT3         = 42u,  /**< DFSDM1 Filter 3 global interrupt (not L4P5/Q5)  */
#endif
#if defined (TIM8)
    NVIC_PERIPH_IRQ_TIM8_BRK            = 43u,  /**< TIM8 Break interrupt                            */
    NVIC_PERIPH_IRQ_TIM8_UP             = 44u,  /**< TIM8 Update interrupt                           */
    NVIC_PERIPH_IRQ_TIM8_TRG_COM        = 45u,  /**< TIM8 Trigger and Commutation interrupt          */
    NVIC_PERIPH_IRQ_TIM8_CC             = 46u,  /**< TIM8 Capture Compare interrupt                  */
#endif
#if defined (ADC3)
    NVIC_PERIPH_IRQ_ADC3                = 47u,  /**< ADC3 global interrupt                           */
#elif defined (SDMMC2)
    NVIC_PERIPH_IRQ_SDMMC2              = 47u,  /**< SDMMC2 global interrupt                         */
#endif
#if defined (FMC_Bank1_R)
    NVIC_PERIPH_IRQ_FMC                 = 48u,  /**< FMC global interrupt                            */
#endif
#if defined (SDMMC1)
    NVIC_PERIPH_IRQ_SDMMC1              = 49u,  /**< SDMMC1 global interrupt                         */
#endif
#if defined (TIM5)
    NVIC_PERIPH_IRQ_TIM5                = 50u,  /**< TIM5 global interrupt                           */
#endif
#if defined (SPI3)
    NVIC_PERIPH_IRQ_SPI3                = 51u,  /**< SPI3 global interrupt                           */
#endif
#if defined (UART4)
    NVIC_PERIPH_IRQ_UART4               = 52u,  /**< UART4 global interrupt                          */
#endif
#if defined (UART5)
    NVIC_PERIPH_IRQ_UART5               = 53u,  /**< UART5 global interrupt                          */
#endif
#if defined (DAC1)
    NVIC_PERIPH_IRQ_TIM6_DAC            = 54u,  /**< TIM6 global and DAC1 underrun error interrupts  */
#else
    NVIC_PERIPH_IRQ_TIM6                = 54u,  /**< TIM6 global interrupt                           */
#endif
#if defined (TIM7)
    NVIC_PERIPH_IRQ_TIM7                = 55u,  /**< TIM7 global interrupt                           */
#endif
    NVIC_PERIPH_IRQ_DMA2_CHANNEL1       = 56u,  /**< DMA2 Channel 1 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL2       = 57u,  /**< DMA2 Channel 2 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL3       = 58u,  /**< DMA2 Channel 3 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL4       = 59u,  /**< DMA2 Channel 4 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL5       = 60u,  /**< DMA2 Channel 5 global interrupt                 */
#if defined (DFSDM1_Filter0)
    NVIC_PERIPH_IRQ_DFSDM1_FLT0         = 61u,  /**< DFSDM1 Filter 0 global interrupt                */
    NVIC_PERIPH_IRQ_DFSDM1_FLT1         = 62u,  /**< DFSDM1 Filter 1 global interrupt                */
#endif
#if defined (DFSDM1_Filter2) && \
    !defined (SDMMC2)
    NVIC_PERIPH_IRQ_DFSDM1_FLT2         = 63u,  /**< DFSDM1 Filter 2 global interrupt (not L4P5/Q5)  */
#endif
    NVIC_PERIPH_IRQ_COMP                = 64u,  /**< COMP1 / COMP2 interrupts through EXTI lines     */
    NVIC_PERIPH_IRQ_LPTIM1              = 65u,  /**< LPTIM1 global interrupt                         */
    NVIC_PERIPH_IRQ_LPTIM2              = 66u,  /**< LPTIM2 global interrupt                         */
#if defined (USB_OTG_FS)
    NVIC_PERIPH_IRQ_OTG_FS              = 67u,  /**< USB OTG FS global interrupt                     */
#elif defined (USB)
    NVIC_PERIPH_IRQ_USB                 = 67u,  /**< USB event interrupt through EXTI line           */
#endif
    NVIC_PERIPH_IRQ_DMA2_CHANNEL6       = 68u,  /**< DMA2 Channel 6 global interrupt                 */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL7       = 69u,  /**< DMA2 Channel 7 global interrupt                 */
    NVIC_PERIPH_IRQ_LPUART1             = 70u,  /**< LPUART1 global interrupt                        */
#if defined (OCTOSPI1)
    NVIC_PERIPH_IRQ_OCTOSPI1            = 71u,  /**< OctoSPI1 global interrupt                       */
#else
    NVIC_PERIPH_IRQ_QUADSPI             = 71u,  /**< Quad SPI global interrupt                       */
#endif
    NVIC_PERIPH_IRQ_I2C3_EV             = 72u,  /**< I2C3 Event interrupt                            */
    NVIC_PERIPH_IRQ_I2C3_ER             = 73u,  /**< I2C3 Error interrupt                            */
#if defined (SAI1)
    NVIC_PERIPH_IRQ_SAI1                = 74u,  /**< SAI1 global interrupt                           */
#endif
#if defined (SAI2)
    NVIC_PERIPH_IRQ_SAI2                = 75u,  /**< SAI2 global interrupt                           */
#endif
#if defined (OCTOSPI2)
    NVIC_PERIPH_IRQ_OCTOSPI2            = 76u,  /**< OctoSPI2 global interrupt                       */
#elif defined (SWPMI1)
    NVIC_PERIPH_IRQ_SWPMI1              = 76u,  /**< Serial Wire Interface 1 global interrupt        */
#endif
    NVIC_PERIPH_IRQ_TSC                 = 77u,  /**< Touch Sense Controller global interrupt         */
#if defined (DSI)
    NVIC_PERIPH_IRQ_DSI                 = 78u,  /**< DSI global interrupt                            */
#elif defined (LCD)
    NVIC_PERIPH_IRQ_LCD                 = 78u,  /**< LCD global interrupt                            */
#endif
#if defined (AES)
    NVIC_PERIPH_IRQ_AES                 = 79u,  /**< AES global interrupt                            */
#endif
#if defined (HASH) && \
    !defined (DMAMUX1)
    NVIC_PERIPH_IRQ_HASH_RNG            = 80u,  /**< HASH and RNG global interrupt                   */
#else
    NVIC_PERIPH_IRQ_RNG                 = 80u,  /**< RNG global interrupt                            */
#endif
    NVIC_PERIPH_IRQ_FPU                 = 81u,  /**< FPU global interrupt                            */
#if defined (HASH) && \
    defined (DMAMUX1)
    NVIC_PERIPH_IRQ_HASH_CRS            = 82u,  /**< HASH and CRS global interrupt                   */
#elif defined (CRS)
    NVIC_PERIPH_IRQ_CRS                 = 82u,  /**< CRS global interrupt                            */
#endif
#if defined (I2C4) && \
    defined (DMAMUX1)
    NVIC_PERIPH_IRQ_I2C4_ER             = 83u,  /**< I2C4 Error interrupt (STM32L4+ order)           */
    NVIC_PERIPH_IRQ_I2C4_EV             = 84u,  /**< I2C4 Event interrupt (STM32L4+ order)           */
#elif defined (I2C4)
    NVIC_PERIPH_IRQ_I2C4_EV             = 83u,  /**< I2C4 Event interrupt                            */
    NVIC_PERIPH_IRQ_I2C4_ER             = 84u,  /**< I2C4 Error interrupt                            */
#endif
#if defined (PSSI)
    NVIC_PERIPH_IRQ_DCMI_PSSI           = 85u,  /**< DCMI / PSSI global interrupt                    */
#elif defined (DCMI)
    NVIC_PERIPH_IRQ_DCMI                = 85u,  /**< DCMI global interrupt                           */
#endif
#if defined (CAN2)
    NVIC_PERIPH_IRQ_CAN2_TX             = 86u,  /**< CAN2 TX interrupt                               */
    NVIC_PERIPH_IRQ_CAN2_RX0            = 87u,  /**< CAN2 RX0 interrupt                              */
    NVIC_PERIPH_IRQ_CAN2_RX1            = 88u,  /**< CAN2 RX1 interrupt                              */
    NVIC_PERIPH_IRQ_CAN2_SCE            = 89u,  /**< CAN2 SCE interrupt                              */
#elif defined (PKA)
    NVIC_PERIPH_IRQ_PKA                 = 86u,  /**< PKA global interrupt                            */
#endif
#if defined (DMA2D)
    NVIC_PERIPH_IRQ_DMA2D               = 90u,  /**< DMA2D global interrupt                          */
#endif
#if defined (LTDC)
    NVIC_PERIPH_IRQ_LTDC                = 91u,  /**< LTDC global interrupt                           */
    NVIC_PERIPH_IRQ_LTDC_ER             = 92u,  /**< LTDC Error global interrupt                     */
#endif
#if defined (GFXMMU)
    NVIC_PERIPH_IRQ_GFXMMU              = 93u,  /**< GFXMMU global error interrupt                   */
#endif
#if defined (DMAMUX1)
    NVIC_PERIPH_IRQ_DMAMUX1_OVR         = 94u,  /**< DMAMUX1 overrun global interrupt                */
#endif
    NVIC_PERIPH_IRQ_SIZE                        /**< Count of peripheral Interrupts                  */
}   nvic_PeriphIrqList_t;


/**
 * \brief Cortex-M IRQ list. The index is exception number decremented by 1 to
 *        have StackPointer separated (comments contain exception number).
 *
 * \note  Exceptions 7 - 10 and 13 are reserved on Cortex-M4.
 */
typedef enum
{
    NVIC_CORE_IRQ_RESET              = 0u,  /**< 1 Reset vector                          */
    NVIC_CORE_IRQ_NMI                = 1u,  /**< 2 Cortex-M4 Non Maskable Interrupt      */
    NVIC_CORE_IRQ_HARDFAULT          = 2u,  /**< 3 Cortex-M4 Hard Fault Interrupt        */
    NVIC_CORE_IRQ_MEMFAULT           = 3u,  /**< 4 Cortex-M4 Memory Management Interrupt */
    NVIC_CORE_IRQ_BUSFAULT           = 4u,  /**< 5 Cortex-M4 Bus Fault Interrupt         */
    NVIC_CORE_IRQ_USAGEFAULT         = 5u,  /**< 6 Cortex-M4 Usage Fault Interrupt       */
    NVIC_CORE_IRQ_SVCALL             = 10u, /**< 11 Cortex-M4 SV Call Interrupt          */
    NVIC_CORE_IRQ_DEBUGMONITOR       = 11u, /**< 12 Cortex-M4 Debug Monitor Interrupt    */
    NVIC_CORE_IRQ_PENDSV             = 13u, /**< 14 Cortex-M4 Pend SV Interrupt          */
    NVIC_CORE_IRQ_SYSTICK            = 14u, /**< 15 Cortex-M4 System Tick Interrupt      */
    NVIC_CORE_IRQ_SIZE                      /**< Count of core Interrupts (15 entries)   */
}   nvic_CoreIrqList_t;


/**
 * \brief Fault status of the core (raw values of SCB fault registers).
 *
 * \note  Fault address registers are valid only if the related valid flag
 *        (MMARVALID, BFARVALID) is set in CFSR.
 */
typedef struct
{
    uint32_t Cfsr;  /**< Configurable Fault Status Register (MMFSR, BFSR, UFSR) */
    uint32_t Hfsr;  /**< HardFault Status Register                              */
    uint32_t Mmfar; /**< MemManage Fault Address Register                       */
    uint32_t Bfar;  /**< BusFault Address Register                              */
}   nvic_FaultStatus_t;

/* ========================== EXPORTED VARIABLES ============================ */

/* ========================= EXPORTED FUNCTIONS ============================= */


#endif /* NVIC_NVIC_TYPES_H */
