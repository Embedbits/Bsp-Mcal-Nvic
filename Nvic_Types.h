/**
 * \defgroup Nvic Nvic
 * \brief Nvic module
 */

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

/** STM32F4 uses 4 Bits for the Priority Levels */
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
 * \ref NVIC_PRIO_BITS For STM32F4 is the range 0-15
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
 * vector positions (different peripherals on different STM32F4 devices) are
 * named according to the selected device.
 */
typedef enum
{
    NVIC_PERIPH_IRQ_WWDG                = 0u,   /**< Window WatchDog interrupt                       */
    NVIC_PERIPH_IRQ_PVD                 = 1u,   /**< PVD through EXTI Line detection interrupt       */
    NVIC_PERIPH_IRQ_TAMP_STAMP          = 2u,   /**< Tamper and TimeStamp through EXTI line          */
    NVIC_PERIPH_IRQ_RTC_WKUP            = 3u,   /**< RTC Wakeup through EXTI line                    */
    NVIC_PERIPH_IRQ_FLASH               = 4u,   /**< FLASH global interrupt                          */
    NVIC_PERIPH_IRQ_RCC                 = 5u,   /**< RCC global interrupt                            */
    NVIC_PERIPH_IRQ_EXTI0               = 6u,   /**< EXTI Line0 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI1               = 7u,   /**< EXTI Line1 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI2               = 8u,   /**< EXTI Line2 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI3               = 9u,   /**< EXTI Line3 interrupt                            */
    NVIC_PERIPH_IRQ_EXTI4               = 10u,  /**< EXTI Line4 interrupt                            */
    NVIC_PERIPH_IRQ_DMA1_STREAM0        = 11u,  /**< DMA1 Stream 0 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA1_STREAM1        = 12u,  /**< DMA1 Stream 1 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA1_STREAM2        = 13u,  /**< DMA1 Stream 2 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA1_STREAM3        = 14u,  /**< DMA1 Stream 3 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA1_STREAM4        = 15u,  /**< DMA1 Stream 4 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA1_STREAM5        = 16u,  /**< DMA1 Stream 5 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA1_STREAM6        = 17u,  /**< DMA1 Stream 6 global interrupt                  */
    NVIC_PERIPH_IRQ_ADC                 = 18u,  /**< ADC1, ADC2 and ADC3 global interrupts           */
#if defined (CAN1)
    NVIC_PERIPH_IRQ_CAN1_TX             = 19u,  /**< CAN1 TX interrupt                               */
    NVIC_PERIPH_IRQ_CAN1_RX0            = 20u,  /**< CAN1 RX0 interrupt                              */
    NVIC_PERIPH_IRQ_CAN1_RX1            = 21u,  /**< CAN1 RX1 interrupt                              */
    NVIC_PERIPH_IRQ_CAN1_SCE            = 22u,  /**< CAN1 SCE interrupt                              */
#endif
    NVIC_PERIPH_IRQ_EXTI9_5             = 23u,  /**< EXTI Line[9:5] interrupts                       */
    NVIC_PERIPH_IRQ_TIM1_BRK_TIM9       = 24u,  /**< TIM1 Break and TIM9 global interrupt            */
#if defined (TIM10)
    NVIC_PERIPH_IRQ_TIM1_UP_TIM10       = 25u,  /**< TIM1 Update and TIM10 global interrupt          */
#else
    NVIC_PERIPH_IRQ_TIM1_UP             = 25u,  /**< TIM1 Update interrupt                           */
#endif
    NVIC_PERIPH_IRQ_TIM1_TRG_COM_TIM11  = 26u,  /**< TIM1 Trigger, Commutation and TIM11 interrupt   */
    NVIC_PERIPH_IRQ_TIM1_CC             = 27u,  /**< TIM1 Capture Compare interrupt                  */
#if defined (TIM2)
    NVIC_PERIPH_IRQ_TIM2                = 28u,  /**< TIM2 global interrupt                           */
#endif
#if defined (TIM3)
    NVIC_PERIPH_IRQ_TIM3                = 29u,  /**< TIM3 global interrupt                           */
#endif
#if defined (TIM4)
    NVIC_PERIPH_IRQ_TIM4                = 30u,  /**< TIM4 global interrupt                           */
#endif
    NVIC_PERIPH_IRQ_I2C1_EV             = 31u,  /**< I2C1 Event interrupt                            */
    NVIC_PERIPH_IRQ_I2C1_ER             = 32u,  /**< I2C1 Error interrupt                            */
    NVIC_PERIPH_IRQ_I2C2_EV             = 33u,  /**< I2C2 Event interrupt                            */
    NVIC_PERIPH_IRQ_I2C2_ER             = 34u,  /**< I2C2 Error interrupt                            */
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
#if defined (USB_OTG_FS)
    NVIC_PERIPH_IRQ_OTG_FS_WKUP         = 42u,  /**< USB OTG FS Wakeup through EXTI line             */
#endif
#if defined (TIM8)
    NVIC_PERIPH_IRQ_TIM8_BRK_TIM12      = 43u,  /**< TIM8 Break and TIM12 global interrupt           */
    NVIC_PERIPH_IRQ_TIM8_UP_TIM13       = 44u,  /**< TIM8 Update and TIM13 global interrupt          */
    NVIC_PERIPH_IRQ_TIM8_TRG_COM_TIM14  = 45u,  /**< TIM8 Trigger, Commutation and TIM14 interrupt   */
    NVIC_PERIPH_IRQ_TIM8_CC             = 46u,  /**< TIM8 Capture Compare interrupt                  */
#endif
    NVIC_PERIPH_IRQ_DMA1_STREAM7        = 47u,  /**< DMA1 Stream 7 global interrupt                  */
#if defined (FMC_Bank1)
    NVIC_PERIPH_IRQ_FMC                 = 48u,  /**< FMC global interrupt                            */
#elif defined (FSMC_Bank2_3)
    NVIC_PERIPH_IRQ_FSMC                = 48u,  /**< FSMC global interrupt                           */
#endif
#if defined (SDIO)
    NVIC_PERIPH_IRQ_SDIO                = 49u,  /**< SDIO global interrupt                           */
#endif
    NVIC_PERIPH_IRQ_TIM5                = 50u,  /**< TIM5 global interrupt                           */
#if defined (SPI3)
    NVIC_PERIPH_IRQ_SPI3                = 51u,  /**< SPI3 global interrupt                           */
#endif
#if defined (UART4)
    NVIC_PERIPH_IRQ_UART4               = 52u,  /**< UART4 global interrupt                          */
#endif
#if defined (UART5)
    NVIC_PERIPH_IRQ_UART5               = 53u,  /**< UART5 global interrupt                          */
#endif
#if defined (DAC)
    NVIC_PERIPH_IRQ_TIM6_DAC            = 54u,  /**< TIM6 global and DAC underrun error interrupts   */
#elif defined (TIM6)
    NVIC_PERIPH_IRQ_TIM6                = 54u,  /**< TIM6 global interrupt                           */
#endif
#if defined (TIM7)
    NVIC_PERIPH_IRQ_TIM7                = 55u,  /**< TIM7 global interrupt                           */
#endif
    NVIC_PERIPH_IRQ_DMA2_STREAM0        = 56u,  /**< DMA2 Stream 0 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA2_STREAM1        = 57u,  /**< DMA2 Stream 1 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA2_STREAM2        = 58u,  /**< DMA2 Stream 2 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA2_STREAM3        = 59u,  /**< DMA2 Stream 3 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA2_STREAM4        = 60u,  /**< DMA2 Stream 4 global interrupt                  */
#if defined (ETH)
    NVIC_PERIPH_IRQ_ETH                 = 61u,  /**< Ethernet global interrupt                       */
    NVIC_PERIPH_IRQ_ETH_WKUP            = 62u,  /**< Ethernet Wakeup through EXTI line               */
#elif defined (DFSDM1_Filter0)
    NVIC_PERIPH_IRQ_DFSDM1_FLT0         = 61u,  /**< DFSDM1 Filter 0 global interrupt                */
    NVIC_PERIPH_IRQ_DFSDM1_FLT1         = 62u,  /**< DFSDM1 Filter 1 global interrupt                */
#endif
#if defined (CAN2)
    NVIC_PERIPH_IRQ_CAN2_TX             = 63u,  /**< CAN2 TX interrupt                               */
    NVIC_PERIPH_IRQ_CAN2_RX0            = 64u,  /**< CAN2 RX0 interrupt                              */
    NVIC_PERIPH_IRQ_CAN2_RX1            = 65u,  /**< CAN2 RX1 interrupt                              */
    NVIC_PERIPH_IRQ_CAN2_SCE            = 66u,  /**< CAN2 SCE interrupt                              */
#endif
#if defined (USB_OTG_FS)
    NVIC_PERIPH_IRQ_OTG_FS              = 67u,  /**< USB OTG FS global interrupt                     */
#endif
    NVIC_PERIPH_IRQ_DMA2_STREAM5        = 68u,  /**< DMA2 Stream 5 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA2_STREAM6        = 69u,  /**< DMA2 Stream 6 global interrupt                  */
    NVIC_PERIPH_IRQ_DMA2_STREAM7        = 70u,  /**< DMA2 Stream 7 global interrupt                  */
#if defined (USART6)
    NVIC_PERIPH_IRQ_USART6              = 71u,  /**< USART6 global interrupt                         */
#endif
#if defined (I2C3)
    NVIC_PERIPH_IRQ_I2C3_EV             = 72u,  /**< I2C3 Event interrupt                            */
    NVIC_PERIPH_IRQ_I2C3_ER             = 73u,  /**< I2C3 Error interrupt                            */
#endif
#if defined (USB_OTG_HS)
    NVIC_PERIPH_IRQ_OTG_HS_EP1_OUT      = 74u,  /**< USB OTG HS End Point 1 Out global interrupt     */
    NVIC_PERIPH_IRQ_OTG_HS_EP1_IN       = 75u,  /**< USB OTG HS End Point 1 In global interrupt      */
    NVIC_PERIPH_IRQ_OTG_HS_WKUP         = 76u,  /**< USB OTG HS Wakeup through EXTI line             */
    NVIC_PERIPH_IRQ_OTG_HS              = 77u,  /**< USB OTG HS global interrupt                     */
#elif defined (CAN3)
    NVIC_PERIPH_IRQ_CAN3_TX             = 74u,  /**< CAN3 TX interrupt                               */
    NVIC_PERIPH_IRQ_CAN3_RX0            = 75u,  /**< CAN3 RX0 interrupt                              */
    NVIC_PERIPH_IRQ_CAN3_RX1            = 76u,  /**< CAN3 RX1 interrupt                              */
    NVIC_PERIPH_IRQ_CAN3_SCE            = 77u,  /**< CAN3 SCE interrupt                              */
#endif
#if defined (DCMI)
    NVIC_PERIPH_IRQ_DCMI                = 78u,  /**< DCMI global interrupt                           */
#endif
#if defined (CRYP)
    NVIC_PERIPH_IRQ_CRYP                = 79u,  /**< CRYP crypto global interrupt                    */
#elif defined (AES)
    NVIC_PERIPH_IRQ_AES                 = 79u,  /**< AES global interrupt                            */
#endif
#if defined (STM32F415xx) || defined (STM32F417xx) || defined (STM32F427xx) || defined (STM32F429xx) || \
    defined (STM32F437xx) || defined (STM32F439xx) || defined (STM32F469xx) || defined (STM32F479xx)
    NVIC_PERIPH_IRQ_HASH_RNG            = 80u,  /**< Hash and RNG global interrupt                   */
#elif defined (RNG)
    NVIC_PERIPH_IRQ_RNG                 = 80u,  /**< RNG global interrupt                            */
#endif
    NVIC_PERIPH_IRQ_FPU                 = 81u,  /**< FPU global interrupt                            */
#if defined (UART7)
    NVIC_PERIPH_IRQ_UART7               = 82u,  /**< UART7 global interrupt                          */
#endif
#if defined (UART8)
    NVIC_PERIPH_IRQ_UART8               = 83u,  /**< UART8 global interrupt                          */
#endif
#if defined (SPI4)
    NVIC_PERIPH_IRQ_SPI4                = 84u,  /**< SPI4 global interrupt                           */
#endif
#if defined (SPI5)
    NVIC_PERIPH_IRQ_SPI5                = 85u,  /**< SPI5 global interrupt                           */
#endif
#if defined (SPI6)
    NVIC_PERIPH_IRQ_SPI6                = 86u,  /**< SPI6 global interrupt                           */
#endif
#if defined (SAI1)
    NVIC_PERIPH_IRQ_SAI1                = 87u,  /**< SAI1 global interrupt                           */
#endif
#if defined (LTDC)
    NVIC_PERIPH_IRQ_LTDC                = 88u,  /**< LTDC global interrupt                           */
    NVIC_PERIPH_IRQ_LTDC_ER             = 89u,  /**< LTDC Error global interrupt                     */
#elif defined (UART9)
    NVIC_PERIPH_IRQ_UART9               = 88u,  /**< UART9 global interrupt                          */
    NVIC_PERIPH_IRQ_UART10              = 89u,  /**< UART10 global interrupt                         */
#endif
#if defined (DMA2D)
    NVIC_PERIPH_IRQ_DMA2D               = 90u,  /**< DMA2D global interrupt                          */
#endif
#if defined (SAI2)
    NVIC_PERIPH_IRQ_SAI2                = 91u,  /**< SAI2 global interrupt                           */
#elif defined (DSI)
    NVIC_PERIPH_IRQ_QUADSPI             = 91u,  /**< QUADSPI global interrupt                        */
#endif
#if defined (DSI)
    NVIC_PERIPH_IRQ_DSI                 = 92u,  /**< DSI global interrupt                            */
#elif defined (QUADSPI)
    NVIC_PERIPH_IRQ_QUADSPI             = 92u,  /**< QUADSPI global interrupt                        */
#endif
#if defined (CEC)
    NVIC_PERIPH_IRQ_CEC                 = 93u,  /**< HDMI-CEC global interrupt                       */
#endif
#if defined (SPDIFRX)
    NVIC_PERIPH_IRQ_SPDIF_RX            = 94u,  /**< SPDIF-RX global interrupt                       */
#endif
#if defined (FMPI2C1)
    NVIC_PERIPH_IRQ_FMPI2C1_EV          = 95u,  /**< FMPI2C1 Event interrupt                         */
    NVIC_PERIPH_IRQ_FMPI2C1_ER          = 96u,  /**< FMPI2C1 Error interrupt                         */
#endif
#if defined (LPTIM1)
    NVIC_PERIPH_IRQ_LPTIM1              = 97u,  /**< LPTIM1 global interrupt                         */
#endif
#if defined (DFSDM2_Filter0)
    NVIC_PERIPH_IRQ_DFSDM2_FLT0         = 98u,  /**< DFSDM2 Filter 0 global interrupt                */
    NVIC_PERIPH_IRQ_DFSDM2_FLT1         = 99u,  /**< DFSDM2 Filter 1 global interrupt                */
    NVIC_PERIPH_IRQ_DFSDM2_FLT2         = 100u, /**< DFSDM2 Filter 2 global interrupt                */
    NVIC_PERIPH_IRQ_DFSDM2_FLT3         = 101u, /**< DFSDM2 Filter 3 global interrupt                */
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
