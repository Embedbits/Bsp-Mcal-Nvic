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

/** STM32H7 uses 4 Bits for the Priority Levels */
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
 * \ref NVIC_PRIO_BITS For STM32H7 is the range 0-15
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


/** STM32H7 lines with the same interrupt vector table layout (classic STM32H7
 *  devices define STM32H7, STM32H7R/S devices define STM32H7RS) */
#if defined (STM32H723xx) || \
    defined (STM32H725xx) || \
    defined (STM32H730xx) || \
    defined (STM32H730xxQ) || \
    defined (STM32H733xx) || \
    defined (STM32H735xx)
#define NVIC_H7_LINE_H72X               /**< STM32H723 / H725 / H730 / H733 / H735                          */
#endif

#if defined (STM32H742xx) || \
    defined (STM32H743xx) || \
    defined (STM32H745xG) || \
    defined (STM32H745xx) || \
    defined (STM32H747xG) || \
    defined (STM32H747xx) || \
    defined (STM32H750xx) || \
    defined (STM32H753xx) || \
    defined (STM32H755xx) || \
    defined (STM32H757xx)
#define NVIC_H7_LINE_H74X               /**< STM32H742 / H743 / H745 / H747 / H750 / H753 / H755 / H757     */
#endif

#if defined (STM32H7A3xx) || \
    defined (STM32H7A3xxQ) || \
    defined (STM32H7B0xx) || \
    defined (STM32H7B0xxQ) || \
    defined (STM32H7B3xx) || \
    defined (STM32H7B3xxQ)
#define NVIC_H7_LINE_H7AB               /**< STM32H7A3 / H7B0 / H7B3                                        */
#endif

#if defined (STM32H745xG) || \
    defined (STM32H745xx) || \
    defined (STM32H747xG) || \
    defined (STM32H747xx) || \
    defined (STM32H755xx) || \
    defined (STM32H757xx)
#define NVIC_H7_DUAL_CORE               /**< Dual-core STM32H745 / H747 / H755 / H757 (Cortex-M7 vectors)   */
#endif


/**
 * \enum nvic_PeriphIrqList_t
 * \brief Enumeration list of peripheral interrupt callback's
 *
 * Value of the item is position of the interrupt in the vector table (IRQn) of
 * the Cortex-M7 core. Interrupt lines not available on the selected device are
 * not defined, shared vector positions (different peripherals on different
 * STM32H7 devices) are named according to the selected device (names of CMSIS
 * IRQn_Type without "_IRQn" suffix).
 */
typedef enum
{
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_WWDG                   = 0u,   /**< Window WatchDog Interrupt ( wwdg1_it, wwdg2_it)              */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_PVD_PVM                = 0u,   /**< PVD/PVM through EXTI Line detection Interrupt                */
#endif
#if defined (NVIC_H7_LINE_H74X) || \
    defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_PVD_AVD                = 1u,   /**< PVD/AVD through EXTI Line detection Interrupt                */
#elif defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_PVD_PVM                = 1u,   /**< PVD/PVM through EXTI Line detection Interrupt                */
#endif
#if defined (NVIC_H7_LINE_H74X) || \
    defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_TAMP_STAMP             = 2u,   /**< Tamper and TimeStamp interrupts through the EXTI line        */
#elif defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_RTC_TAMP_STAMP_CSS_LSE = 2u,   /**< Tamper, TimeStamp, CSS and LSE interrupts through the EXTI line */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_DTS                    = 2u,   /**< Digital Temperature Sensor Global Interrupt                  */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_RTC_WKUP               = 3u,   /**< RTC Wakeup interrupt through the EXTI line                   */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_IWDG                   = 3u,   /**< Internal Watchdog interrupt                                  */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FLASH                  = 4u,   /**< FLASH global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_WWDG                   = 4u,   /**< Window WatchDog Interrupt ( wwdg1_it, wwdg2_it)              */
#endif
    NVIC_PERIPH_IRQ_RCC                    = 5u,   /**< RCC global Interrupt                                         */
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_EXTI0                  = 6u,   /**< EXTI Line0 Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_EXTI1                  = 7u,   /**< EXTI Line1 Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_EXTI2                  = 8u,   /**< EXTI Line2 Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_FLASH                  = 8u,   /**< FLASH global Interrupt                                       */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_EXTI3                  = 9u,   /**< EXTI Line3 Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_RAMECC                 = 9u,   /**< RAMECC interrupts                                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_EXTI4                  = 10u,  /**< EXTI Line4 Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_FPU                    = 10u,  /**< FPU global interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM0           = 11u,  /**< DMA1 Stream 0 global Interrupt                               */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM1           = 12u,  /**< DMA1 Stream 1 global Interrupt                               */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM2           = 13u,  /**< DMA1 Stream 2 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TAMP                   = 13u,  /**< Tamper and TimeStamp interrupts through EXTI Line detection  */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM3           = 14u,  /**< DMA1 Stream 3 global Interrupt                               */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM4           = 15u,  /**< DMA1 Stream 4 global Interrupt                               */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM5           = 16u,  /**< DMA1 Stream 5 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI0                  = 16u,  /**< EXTI Line0 Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM6           = 17u,  /**< DMA1 Stream 6 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI1                  = 17u,  /**< EXTI Line1 Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_ADC                    = 18u,  /**< ADC1 and  ADC2 global Interrupts                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI2                  = 18u,  /**< EXTI Line2 Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FDCAN1_IT0             = 19u,  /**< FDCAN1 Interrupt line 0                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI3                  = 19u,  /**< EXTI Line3 Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FDCAN2_IT0             = 20u,  /**< FDCAN2 Interrupt line 0                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI4                  = 20u,  /**< EXTI Line4 Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FDCAN1_IT1             = 21u,  /**< FDCAN1 Interrupt line 1                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI5                  = 21u,  /**< EXTI Line5 interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FDCAN2_IT1             = 22u,  /**< FDCAN2 Interrupt line 1                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI6                  = 22u,  /**< EXTI Line6 interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_EXTI9_5                = 23u,  /**< External Line[9:5] Interrupts                                */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI7                  = 23u,  /**< EXTI Line7 interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM1_BRK               = 24u,  /**< TIM1 Break Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI8                  = 24u,  /**< EXTI Line8 interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM1_UP                = 25u,  /**< TIM1 Update Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI9                  = 25u,  /**< EXTI Line9 interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM1_TRG_COM           = 26u,  /**< TIM1 Trigger and Commutation Interrupt                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI10                 = 26u,  /**< EXTI Line10 interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM1_CC                = 27u,  /**< TIM1 Capture Compare Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI11                 = 27u,  /**< EXTI Line11 interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM2                   = 28u,  /**< TIM2 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI12                 = 28u,  /**< EXTI Line12 interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM3                   = 29u,  /**< TIM3 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI13                 = 29u,  /**< EXTI Line13 interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM4                   = 30u,  /**< TIM4 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI14                 = 30u,  /**< EXTI Line14 interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C1_EV                = 31u,  /**< I2C1 Event Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_EXTI15                 = 31u,  /**< EXTI Line15 interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C1_ER                = 32u,  /**< I2C1 Error Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_RTC                    = 32u,  /**< RTC Wakeup and Alarm interrupts through EXTI Line detection  */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C2_EV                = 33u,  /**< I2C2 Event Interrupt                                         */
#elif defined (SAES)
    NVIC_PERIPH_IRQ_SAES                   = 33u,  /**< SAES interrupt                                               */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C2_ER                = 34u,  /**< I2C2 Error Interrupt                                         */
#elif defined (STM32H7S3xx) || \
      defined (STM32H7S7xx)
    NVIC_PERIPH_IRQ_CRYP                   = 34u,  /**< CRYP crypto global interrupt                                 */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SPI1                   = 35u,  /**< SPI1 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_PKA                    = 35u,  /**< PKA interrupt                                                */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SPI2                   = 36u,  /**< SPI2 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HASH                   = 36u,  /**< HASH interrupt                                               */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_USART1                 = 37u,  /**< USART1 global Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_RNG                    = 37u,  /**< RNG global interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_USART2                 = 38u,  /**< USART2 global Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_ADC1_2                 = 38u,  /**< ADC1 & ADC2 interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_USART3                 = 39u,  /**< USART3 global Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL0        = 39u,  /**< GPDMA1 Channel 0 interrupt                                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_EXTI15_10              = 40u,  /**< External Line[15:10] Interrupts                              */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL1        = 40u,  /**< GPDMA1 Channel 1 interrupt                                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_RTC_ALARM              = 41u,  /**< RTC Alarm (A and B) through EXTI Line Interrupt              */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL2        = 41u,  /**< GPDMA1 Channel 2 interrupt                                   */
#endif
#if defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_DFSDM2                 = 42u,  /**< DFSDM2 global Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL3        = 42u,  /**< GPDMA1 Channel 3 interrupt                                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM8_BRK_TIM12         = 43u,  /**< TIM8 Break Interrupt and TIM12 global interrupt              */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL4        = 43u,  /**< GPDMA1 Channel 4 interrupt                                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM8_UP_TIM13          = 44u,  /**< TIM8 Update Interrupt and TIM13 global interrupt             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL5        = 44u,  /**< GPDMA1 Channel 5 interrupt                                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM8_TRG_COM_TIM14     = 45u,  /**< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL6        = 45u,  /**< GPDMA1 Channel 6 interrupt                                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM8_CC                = 46u,  /**< TIM8 Capture Compare Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL7        = 46u,  /**< GPDMA1 Channel 7 interrupt                                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA1_STREAM7           = 47u,  /**< DMA1 Stream7 Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM1_BRK               = 47u,  /**< TIM1 Break Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FMC                    = 48u,  /**< FMC global Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM1_UP                = 48u,  /**< TIM1 Update Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SDMMC1                 = 49u,  /**< SDMMC1 global Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM1_TRG_COM           = 49u,  /**< TIM1 Trigger and Commutation Interrupt                       */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM5                   = 50u,  /**< TIM5 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM1_CC                = 50u,  /**< TIM1 Capture Compare Interrupt                               */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SPI3                   = 51u,  /**< SPI3 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM2                   = 51u,  /**< TIM2 global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_UART4                  = 52u,  /**< UART4 global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM3                   = 52u,  /**< TIM3 global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_UART5                  = 53u,  /**< UART5 global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM4                   = 53u,  /**< TIM4 global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM6_DAC               = 54u,  /**< TIM6 global and DAC1&2 underrun error  interrupts            */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM5                   = 54u,  /**< TIM5 global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_TIM7                   = 55u,  /**< TIM7 global interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM6                   = 55u,  /**< TIM6 global interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM0           = 56u,  /**< DMA2 Stream 0 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM7                   = 56u,  /**< TIM7 global interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM1           = 57u,  /**< DMA2 Stream 1 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM9                   = 57u,  /**< TIM9 global interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM2           = 58u,  /**< DMA2 Stream 2 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SPI1                   = 58u,  /**< SPI1 global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM3           = 59u,  /**< DMA2 Stream 3 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SPI2                   = 59u,  /**< SPI2 global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM4           = 60u,  /**< DMA2 Stream 4 global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SPI3                   = 60u,  /**< SPI3 global Interrupt                                        */
#endif
#if defined (NVIC_H7_LINE_H74X) || \
    defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_ETH                    = 61u,  /**< Ethernet global Interrupt                                    */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SPI4                   = 61u,  /**< SPI4 global Interrupt                                        */
#endif
#if defined (NVIC_H7_LINE_H74X) || \
    defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_ETH_WKUP               = 62u,  /**< Ethernet Wakeup through EXTI line Interrupt                  */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SPI5                   = 62u,  /**< SPI5 global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FDCAN_CAL              = 63u,  /**< FDCAN Calibration unit Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SPI6                   = 63u,  /**< SPI6 global Interrupt                                        */
#endif
#if defined (NVIC_H7_DUAL_CORE)
    NVIC_PERIPH_IRQ_CM7_SEV                = 64u,  /**< CM7 Send event interrupt for CM4                             */
#elif defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_DFSDM1_FLT4            = 64u,  /**< DFSDM Filter4 Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL0        = 64u,  /**< HPDMA1 Channel 0 global interrupt                            */
#endif
#if defined (NVIC_H7_DUAL_CORE)
    NVIC_PERIPH_IRQ_CM4_SEV                = 65u,  /**< CM4 Send event interrupt for CM7                             */
#elif defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_DFSDM1_FLT5            = 65u,  /**< DFSDM Filter5 Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL1        = 65u,  /**< HPDMA1 Channel 1 global interrupt                            */
#endif
#if defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_DFSDM1_FLT6            = 66u,  /**< DFSDM Filter6 Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL2        = 66u,  /**< HPDMA1 Channel 2 global interrupt                            */
#endif
#if defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_DFSDM1_FLT7            = 67u,  /**< DFSDM Filter7 Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL3        = 67u,  /**< HPDMA1 Channel 3 global interrupt                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM5           = 68u,  /**< DMA2 Stream 5 global interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL4        = 68u,  /**< HPDMA1 Channel 4 global interrupt                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM6           = 69u,  /**< DMA2 Stream 6 global interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL5        = 69u,  /**< HPDMA1 Channel 5 global interrupt                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2_STREAM7           = 70u,  /**< DMA2 Stream 7 global interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL6        = 70u,  /**< HPDMA1 Channel 6 global interrupt                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_USART6                 = 71u,  /**< USART6 global interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL7        = 71u,  /**< HPDMA1 Channel 7 global interrupt                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C3_EV                = 72u,  /**< I2C3 event interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SAI1_A                 = 72u,  /**< Serial Audio Interface 1 block A interrupt                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C3_ER                = 73u,  /**< I2C3 error interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SAI1_B                 = 73u,  /**< Serial Audio Interface 1 block B interrupt                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_OTG_HS_EP1_OUT         = 74u,  /**< USB OTG HS End Point 1 Out global interrupt                  */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SAI2_A                 = 74u,  /**< Serial Audio Interface 2 block A interrupt                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_OTG_HS_EP1_IN          = 75u,  /**< USB OTG HS End Point 1 In global interrupt                   */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SAI2_B                 = 75u,  /**< Serial Audio Interface 2 block B interrupt                   */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_OTG_HS_WKUP            = 76u,  /**< USB OTG HS Wakeup through EXTI interrupt                     */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I2C1_EV                = 76u,  /**< I2C1 Event Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_OTG_HS                 = 77u,  /**< USB OTG HS global interrupt                                  */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I2C1_ER                = 77u,  /**< I2C1 Error Interrupt                                         */
#endif
#if defined (NVIC_H7_LINE_H72X) || \
    defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_DCMI_PSSI              = 78u,  /**< DCMI and PSSI global interrupt                               */
#elif defined (NVIC_H7_LINE_H74X)
    NVIC_PERIPH_IRQ_DCMI                   = 78u,  /**< DCMI global interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I2C2_EV                = 78u,  /**< I2C2 Event Interrupt                                         */
#endif
#if defined (STM32H730xx) || \
    defined (STM32H730xxQ) || \
    defined (STM32H733xx) || \
    defined (STM32H735xx) || \
    defined (STM32H750xx) || \
    defined (STM32H753xx) || \
    defined (STM32H755xx) || \
    defined (STM32H757xx) || \
    defined (STM32H7B0xx) || \
    defined (STM32H7B0xxQ) || \
    defined (STM32H7B3xx) || \
    defined (STM32H7B3xxQ)
    NVIC_PERIPH_IRQ_CRYP                   = 79u,  /**< CRYP crypto global interrupt                                 */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I2C2_ER                = 79u,  /**< I2C2 Error Interrupt                                         */
#endif
#if defined (STM32H730xx) || \
    defined (STM32H730xxQ) || \
    defined (STM32H733xx) || \
    defined (STM32H735xx) || \
    defined (STM32H750xx) || \
    defined (STM32H753xx) || \
    defined (STM32H755xx) || \
    defined (STM32H757xx) || \
    defined (STM32H7B0xx) || \
    defined (STM32H7B0xxQ) || \
    defined (STM32H7B3xx) || \
    defined (STM32H7B3xxQ)
    NVIC_PERIPH_IRQ_HASH_RNG               = 80u,  /**< HASH and RNG global interrupt                                */
#elif defined (STM32H723xx) || \
      defined (STM32H725xx) || \
      defined (STM32H742xx) || \
      defined (STM32H743xx) || \
      defined (STM32H745xG) || \
      defined (STM32H745xx) || \
      defined (STM32H747xG) || \
      defined (STM32H747xx) || \
      defined (STM32H7A3xx) || \
      defined (STM32H7A3xxQ)
    NVIC_PERIPH_IRQ_RNG                    = 80u,  /**< RNG global interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I2C3_EV                = 80u,  /**< I2C3 event interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_FPU                    = 81u,  /**< FPU global interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I2C3_ER                = 81u,  /**< I2C3 error interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_UART7                  = 82u,  /**< UART7 global interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_USART1                 = 82u,  /**< USART1 global Interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_UART8                  = 83u,  /**< UART8 global interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_USART2                 = 83u,  /**< USART2 global Interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SPI4                   = 84u,  /**< SPI4 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_USART3                 = 84u,  /**< USART3 global Interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SPI5                   = 85u,  /**< SPI5 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_UART4                  = 85u,  /**< UART4 global Interrupt                                       */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SPI6                   = 86u,  /**< SPI6 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_UART5                  = 86u,  /**< UART5 global Interrupt                                       */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SAI1                   = 87u,  /**< SAI1 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_UART7                  = 87u,  /**< UART7 global interrupt                                       */
#endif
#if defined (NVIC_H7_LINE_H72X) || \
    defined (NVIC_H7_LINE_H7AB) || \
    defined (NVIC_H7_DUAL_CORE) || \
    defined (STM32H743xx) || \
    defined (STM32H750xx) || \
    defined (STM32H753xx)
    NVIC_PERIPH_IRQ_LTDC                   = 88u,  /**< LTDC global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_UART8                  = 88u,  /**< UART8 global interrupt                                       */
#endif
#if defined (NVIC_H7_LINE_H72X) || \
    defined (NVIC_H7_LINE_H7AB) || \
    defined (NVIC_H7_DUAL_CORE) || \
    defined (STM32H743xx) || \
    defined (STM32H750xx) || \
    defined (STM32H753xx)
    NVIC_PERIPH_IRQ_LTDC_ER                = 89u,  /**< LTDC Error global Interrupt                                  */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I3C1_EV                = 89u,  /**< I3C1 event interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMA2D                  = 90u,  /**< DMA2D global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I3C1_ER                = 90u,  /**< I3C1 error interrupt                                         */
#endif
#if defined (NVIC_H7_LINE_H74X) || \
    defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_SAI2                   = 91u,  /**< SAI2 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_OTG_HS                 = 91u,  /**< USB OTG HS global interrupt                                  */
#endif
#if defined (OCTOSPI1)
    NVIC_PERIPH_IRQ_OCTOSPI1               = 92u,  /**< OCTOSPI1 global interrupt                                    */
#elif defined (QUADSPI)
    NVIC_PERIPH_IRQ_QUADSPI                = 92u,  /**< Quad SPI global interrupt                                    */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_ETH                    = 92u,  /**< Ethernet global Interrupt                                    */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_LPTIM1                 = 93u,  /**< LP TIM1 interrupt                                            */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_CORDIC                 = 93u,  /**< CORDIC global interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_CEC                    = 94u,  /**< HDMI-CEC global Interrupt                                    */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GFXTIM                 = 94u,  /**< GFXTIM global interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C4_EV                = 95u,  /**< I2C4 Event Interrupt                                         */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_DCMIPP                 = 95u,  /**< DCMIPP global interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_I2C4_ER                = 96u,  /**< I2C4 Error Interrupt                                         */
#elif defined (STM32H7R7xx) || \
      defined (STM32H7S7xx)
    NVIC_PERIPH_IRQ_LTDC                   = 96u,  /**< LTDC global Interrupt                                        */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SPDIF_RX               = 97u,  /**< SPDIF-RX global Interrupt                                    */
#elif defined (STM32H7R7xx) || \
      defined (STM32H7S7xx)
    NVIC_PERIPH_IRQ_LTDC_ER                = 97u,  /**< LTDC Error global Interrupt                                  */
#endif
#if defined (NVIC_H7_LINE_H74X)
    NVIC_PERIPH_IRQ_OTG_FS_EP1_OUT         = 98u,  /**< USB OTG HS2 global interrupt                                 */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_DMA2D                  = 98u,  /**< DMA2D global Interrupt                                       */
#endif
#if defined (NVIC_H7_LINE_H74X)
    NVIC_PERIPH_IRQ_OTG_FS_EP1_IN          = 99u,  /**< USB OTG HS2 End Point 1 Out global interrupt                 */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_JPEG                   = 99u,  /**< JPEG global Interrupt                                        */
#endif
#if defined (NVIC_H7_LINE_H74X)
    NVIC_PERIPH_IRQ_OTG_FS_WKUP            = 100u, /**< USB OTG HS2 End Point 1 In global interrupt                  */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GFXMMU                 = 100u, /**< GFXMMU global interrupt                                      */
#endif
#if defined (NVIC_H7_LINE_H74X)
    NVIC_PERIPH_IRQ_OTG_FS                 = 101u, /**< USB OTG HS2 Wakeup through EXTI interrupt                    */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_I3C1_WKUP              = 101u, /**< I3C1 wakeup global interrupt                                 */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMAMUX1_OVR            = 102u, /**< DMAMUX1 Overrun interrupt                                    */
#elif defined (MCE1)
    NVIC_PERIPH_IRQ_MCE1                   = 102u, /**< MCE1 global interrupt                                        */
#endif
#if defined (HRTIM1)
    NVIC_PERIPH_IRQ_HRTIM1_MASTER          = 103u, /**< HRTIM Master Timer global Interrupts                         */
#elif defined (MCE2)
    NVIC_PERIPH_IRQ_MCE2                   = 103u, /**< MCE2 global interrupt                                        */
#endif
#if defined (HRTIM1_TIMA)
    NVIC_PERIPH_IRQ_HRTIM1_TIMA            = 104u, /**< HRTIM Timer A global Interrupt                               */
#elif defined (MCE3)
    NVIC_PERIPH_IRQ_MCE3                   = 104u, /**< MCE3 global interrupt                                        */
#endif
#if defined (HRTIM1_TIMB)
    NVIC_PERIPH_IRQ_HRTIM1_TIMB            = 105u, /**< HRTIM Timer B global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_XSPI1                  = 105u, /**< XSPI1 global interrupt                                       */
#endif
#if defined (HRTIM1_TIMC)
    NVIC_PERIPH_IRQ_HRTIM1_TIMC            = 106u, /**< HRTIM Timer C global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_XSPI2                  = 106u, /**< XSPI2 global interrupt                                       */
#endif
#if defined (HRTIM1_TIMD)
    NVIC_PERIPH_IRQ_HRTIM1_TIMD            = 107u, /**< HRTIM Timer D global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_FMC                    = 107u, /**< FMC global Interrupt                                         */
#endif
#if defined (HRTIM1_TIME)
    NVIC_PERIPH_IRQ_HRTIM1_TIME            = 108u, /**< HRTIM Timer E global Interrupt                               */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SDMMC1                 = 108u, /**< SDMMC1 global Interrupt                                      */
#endif
#if defined (HRTIM1)
    NVIC_PERIPH_IRQ_HRTIM1_FLT             = 109u, /**< HRTIM Fault global Interrupt                                 */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SDMMC2                 = 109u, /**< SDMMC2 global Interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DFSDM1_FLT0            = 110u, /**< DFSDM Filter1 Interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DFSDM1_FLT1            = 111u, /**< DFSDM Filter2 Interrupt                                      */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DFSDM1_FLT2            = 112u, /**< DFSDM Filter3 Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_OTG_FS                 = 112u, /**< USB OTG HS2 Wakeup through EXTI interrupt                    */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DFSDM1_FLT3            = 113u, /**< DFSDM Filter4 Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM12                  = 113u, /**< TIM12 global interrupt                                       */
#endif
#if defined (SAI3)
    NVIC_PERIPH_IRQ_SAI3                   = 114u, /**< SAI3 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM13                  = 114u, /**< TIM13 global interrupt                                       */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SWPMI1                 = 115u, /**< Serial Wire Interface 1 global interrupt                     */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_TIM14                  = 115u, /**< TIM14 global interrupt                                       */
#endif
    NVIC_PERIPH_IRQ_TIM15                  = 116u, /**< TIM15 global Interrupt                                       */
    NVIC_PERIPH_IRQ_TIM16                  = 117u, /**< TIM16 global Interrupt                                       */
    NVIC_PERIPH_IRQ_TIM17                  = 118u, /**< TIM17 global Interrupt                                       */
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_MDIOS_WKUP             = 119u, /**< MDIOS Wakeup  Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_LPTIM1                 = 119u, /**< LP TIM1 interrupt                                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_MDIOS                  = 120u, /**< MDIOS global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_LPTIM2                 = 120u, /**< LP TIM2 global interrupt                                     */
#endif
#if defined (NVIC_H7_LINE_H7AB) || \
    defined (NVIC_H7_DUAL_CORE) || \
    defined (STM32H743xx) || \
    defined (STM32H750xx) || \
    defined (STM32H753xx)
    NVIC_PERIPH_IRQ_JPEG                   = 121u, /**< JPEG global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_LPTIM3                 = 121u, /**< LP TIM3 global interrupt                                     */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_MDMA                   = 122u, /**< MDMA global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_LPTIM4                 = 122u, /**< LP TIM4 global interrupt                                     */
#endif
#if defined (STM32H7RS)
    NVIC_PERIPH_IRQ_LPTIM5                 = 123u, /**< LP TIM5 global interrupt                                     */
#elif defined (DSI)
    NVIC_PERIPH_IRQ_DSI                    = 123u, /**< DSI global Interrupt                                         */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_SDMMC2                 = 124u, /**< SDMMC2 global Interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_SPDIF_RX               = 124u, /**< SPDIF-RX global Interrupt                                    */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_HSEM1                  = 125u, /**< HSEM1 global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_MDIOS                  = 125u, /**< MDIOS global Interrupt                                       */
#endif
#if defined (NVIC_H7_DUAL_CORE)
    NVIC_PERIPH_IRQ_HSEM2                  = 126u, /**< HSEM2 global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_ADF1_FLT0              = 126u, /**< ADF1 Filter 0 global Interrupt                               */
#endif
#if defined (ADC3)
    NVIC_PERIPH_IRQ_ADC3                   = 127u, /**< ADC3 global Interrupt                                        */
#elif defined (DAC2)
    NVIC_PERIPH_IRQ_DAC2                   = 127u, /**< DAC2 global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_CRS                    = 127u, /**< Clock Recovery Global Interrupt                              */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_DMAMUX2_OVR            = 128u, /**< DMAMUX2 Overrun interrupt                                    */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_UCPD1                  = 128u, /**< UCPD1 global interrupt                                       */
#endif
#if defined (BDMA_Channel0)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL0          = 129u, /**< BDMA Channel 0 global Interrupt                              */
#elif defined (BDMA2_Channel0)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL0         = 129u, /**< BDMA2 Channel 0 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_CEC                    = 129u, /**< HDMI-CEC global Interrupt                                    */
#endif
#if defined (BDMA_Channel1)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL1          = 130u, /**< BDMA Channel 1 global Interrupt                              */
#elif defined (BDMA2_Channel1)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL1         = 130u, /**< BDMA2 Channel 1 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_PSSI                   = 130u, /**< PSSI global interrupt                                        */
#endif
#if defined (BDMA_Channel2)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL2          = 131u, /**< BDMA Channel 2 global Interrupt                              */
#elif defined (BDMA2_Channel2)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL2         = 131u, /**< BDMA2 Channel 2 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_LPUART1                = 131u, /**< LP UART1 interrupt                                           */
#endif
#if defined (BDMA_Channel3)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL3          = 132u, /**< BDMA Channel 3 global Interrupt                              */
#elif defined (BDMA2_Channel3)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL3         = 132u, /**< BDMA2 Channel 3 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_WAKEUP_PIN             = 132u, /**< Interrupt for all 6 wake-up pins                             */
#endif
#if defined (BDMA_Channel4)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL4          = 133u, /**< BDMA Channel 4 global Interrupt                              */
#elif defined (BDMA2_Channel4)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL4         = 133u, /**< BDMA2 Channel 4 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL8        = 133u, /**< GPDMA1 Channel 8 global interrupt                            */
#endif
#if defined (BDMA_Channel5)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL5          = 134u, /**< BDMA Channel 5 global Interrupt                              */
#elif defined (BDMA2_Channel5)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL5         = 134u, /**< BDMA2 Channel 5 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL9        = 134u, /**< GPDMA1 Channel 9 global interrupt                            */
#endif
#if defined (BDMA_Channel6)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL6          = 135u, /**< BDMA Channel 6 global Interrupt                              */
#elif defined (BDMA2_Channel6)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL6         = 135u, /**< BDMA2 Channel 6 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL10       = 135u, /**< GPDMA1 Channel 10 global interrupt                           */
#endif
#if defined (BDMA_Channel7)
    NVIC_PERIPH_IRQ_BDMA_CHANNEL7          = 136u, /**< BDMA Channel 7 global Interrupt                              */
#elif defined (BDMA2_Channel7)
    NVIC_PERIPH_IRQ_BDMA2_CHANNEL7         = 136u, /**< BDMA2 Channel 7 global Interrupt                             */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL11       = 136u, /**< GPDMA1 Channel 11 global interrupt                           */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_COMP                   = 137u, /**< COMP global Interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL12       = 137u, /**< GPDMA1 Channel 12 global interrupt                           */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_LPTIM2                 = 138u, /**< LP TIM2 global interrupt                                     */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL13       = 138u, /**< GPDMA1 Channel 13 global interrupt                           */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_LPTIM3                 = 139u, /**< LP TIM3 global interrupt                                     */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL14       = 139u, /**< GPDMA1 Channel 14 global interrupt                           */
#endif
#if defined (NVIC_H7_LINE_H74X) || \
    defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_LPTIM4                 = 140u, /**< LP TIM4 global interrupt                                     */
#elif defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_UART9                  = 140u, /**< UART9 global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL15       = 140u, /**< GPDMA1 Channel 15 global interrupt                           */
#endif
#if defined (NVIC_H7_LINE_H74X) || \
    defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_LPTIM5                 = 141u, /**< LP TIM5 global interrupt                                     */
#elif defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_USART10                = 141u, /**< USART10 global interrupt                                     */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL8        = 141u, /**< HPDMA1 Channel 8 global interrupt                            */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_LPUART1                = 142u, /**< LP UART1 interrupt                                           */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL9        = 142u, /**< HPDMA1 Channel 9 global interrupt                            */
#endif
#if defined (NVIC_H7_LINE_H7AB) || \
    defined (NVIC_H7_DUAL_CORE)
    NVIC_PERIPH_IRQ_WWDG_RST               = 143u, /**< Window Watchdog reset interrupt (exti_d2_wwdg_it, exti_d1_wwdg_it) */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL10       = 143u, /**< HPDMA1 Channel 10 global interrupt                           */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_CRS                    = 144u, /**< Clock Recovery Global Interrupt                              */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL11       = 144u, /**< HPDMA1 Channel 11 global interrupt                           */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_ECC                    = 145u, /**< ECC diagnostic Global Interrupt                              */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL12       = 145u, /**< HPDMA1 Channel 12 global interrupt                           */
#endif
#if defined (SAI4)
    NVIC_PERIPH_IRQ_SAI4                   = 146u, /**< SAI4 global interrupt                                        */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL13       = 146u, /**< HPDMA1 Channel 13 global interrupt                           */
#endif
#if defined (NVIC_H7_LINE_H72X) || \
    defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_DTS                    = 147u, /**< Digital Temperature Sensor Global Interrupt                  */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL14       = 147u, /**< HPDMA1 Channel 14 global interrupt                           */
#endif
#if defined (NVIC_H7_DUAL_CORE)
    NVIC_PERIPH_IRQ_HOLD_CORE              = 148u, /**< Hold core interrupt                                          */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_HPDMA1_CHANNEL15       = 148u, /**< HPDMA1 Channel 15 global interrupt                           */
#endif
#if defined (STM32H7)
    NVIC_PERIPH_IRQ_WAKEUP_PIN             = 149u, /**< Interrupt for all 6 wake-up pins                             */
#elif defined (STM32H7R7xx) || \
      defined (STM32H7S7xx)
    NVIC_PERIPH_IRQ_GPU2D                  = 149u, /**< GPU2D interrupt                                              */
#endif
#if defined (OCTOSPI2)
    NVIC_PERIPH_IRQ_OCTOSPI2               = 150u, /**< OctoSPI2 global interrupt                                    */
#elif defined (STM32H7R7xx) || \
      defined (STM32H7S7xx)
    NVIC_PERIPH_IRQ_GPU2D_ER               = 150u, /**< GPU2D error interrupt                                        */
#endif
#if defined (OTFDEC1)
    NVIC_PERIPH_IRQ_OTFDEC1                = 151u, /**< OTFDEC1 global interrupt                                     */
#elif defined (ICACHE)
    NVIC_PERIPH_IRQ_ICACHE                 = 151u, /**< ICACHE interrupt                                             */
#endif
#if defined (OTFDEC2)
    NVIC_PERIPH_IRQ_OTFDEC2                = 152u, /**< OTFDEC2 global interrupt                                     */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_FDCAN1_IT0             = 152u, /**< FDCAN1 Interrupt line 0                                      */
#endif
#if defined (FMAC)
    NVIC_PERIPH_IRQ_FMAC                   = 153u, /**< FMAC global interrupt                                        */
#elif defined (NVIC_H7_LINE_H7AB)
    NVIC_PERIPH_IRQ_GFXMMU                 = 153u, /**< GFXMMU global interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_FDCAN1_IT1             = 153u, /**< FDCAN1 Interrupt line 1                                      */
#endif
#if defined (BDMA1)
    NVIC_PERIPH_IRQ_BDMA1                  = 154u, /**< BDMA1 for DFSM global interrupt                              */
#elif defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_CORDIC                 = 154u, /**< CORDIC global interrupt                                      */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_FDCAN2_IT0             = 154u, /**< FDCAN2 Interrupt line 0                                      */
#endif
#if defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_UART9                  = 155u, /**< UART9 global Interrupt                                       */
#elif defined (STM32H7RS)
    NVIC_PERIPH_IRQ_FDCAN2_IT1             = 155u, /**< FDCAN2 Interrupt line 1                                      */
#endif
#if defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_USART10                = 156u, /**< USART10 global interrupt                                     */
#endif
#if defined (I2C5)
    NVIC_PERIPH_IRQ_I2C5_EV                = 157u, /**< I2C5 event interrupt                                         */
#endif
#if defined (I2C5)
    NVIC_PERIPH_IRQ_I2C5_ER                = 158u, /**< I2C5 error interrupt                                         */
#endif
#if defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_FDCAN3_IT0             = 159u, /**< FDCAN3 Interrupt line 0                                      */
#endif
#if defined (NVIC_H7_LINE_H72X)
    NVIC_PERIPH_IRQ_FDCAN3_IT1             = 160u, /**< FDCAN3 Interrupt line 1                                      */
#endif
#if defined (TIM23)
    NVIC_PERIPH_IRQ_TIM23                  = 161u, /**< TIM23 global interrupt                                       */
#endif
#if defined (TIM24)
    NVIC_PERIPH_IRQ_TIM24                  = 162u, /**< TIM24 global interrupt                                       */
#endif
    NVIC_PERIPH_IRQ_SIZE                            /**< Count of peripheral Interrupts                               */
}   nvic_PeriphIrqList_t;


/**
 * \brief Cortex-M IRQ list. The index is exception number decremented by 1 to
 *        have StackPointer separated (comments contain exception number).
 *
 * \note  Exceptions 7 - 10 and 13 are reserved on Cortex-M7.
 */
typedef enum
{
    NVIC_CORE_IRQ_RESET              = 0u,  /**< 1 Reset vector                          */
    NVIC_CORE_IRQ_NMI                = 1u,  /**< 2 Cortex-M7 Non Maskable Interrupt      */
    NVIC_CORE_IRQ_HARDFAULT          = 2u,  /**< 3 Cortex-M7 Hard Fault Interrupt        */
    NVIC_CORE_IRQ_MEMFAULT           = 3u,  /**< 4 Cortex-M7 Memory Management Interrupt */
    NVIC_CORE_IRQ_BUSFAULT           = 4u,  /**< 5 Cortex-M7 Bus Fault Interrupt         */
    NVIC_CORE_IRQ_USAGEFAULT         = 5u,  /**< 6 Cortex-M7 Usage Fault Interrupt       */
    NVIC_CORE_IRQ_SVCALL             = 10u, /**< 11 Cortex-M7 SV Call Interrupt          */
    NVIC_CORE_IRQ_DEBUGMONITOR       = 11u, /**< 12 Cortex-M7 Debug Monitor Interrupt    */
    NVIC_CORE_IRQ_PENDSV             = 13u, /**< 14 Cortex-M7 Pend SV Interrupt          */
    NVIC_CORE_IRQ_SYSTICK            = 14u, /**< 15 Cortex-M7 System Tick Interrupt      */
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
