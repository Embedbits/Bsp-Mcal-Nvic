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

/** STM32L4XX uses 4 Bits for the Priority Levels */
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
 */
typedef enum
{
    NVIC_PERIPH_IRQ_WWDG            = 0u,   /**< Window WatchDog interrupt                     */
    NVIC_PERIPH_IRQ_PVD_AVD         = 1u,   /**< PVD/AVD through EXTI Line detection Interrupt */
    NVIC_PERIPH_IRQ_RTC             = 2u,   /**< RTC non-secure interrupt                      */
    NVIC_PERIPH_IRQ_RTC_S           = 3u,   /**< RTC secure interrupt                          */
    NVIC_PERIPH_IRQ_TAMP            = 4u,   /**< Tamper global interrupt                       */
    NVIC_PERIPH_IRQ_RAMCFG          = 5u,   /**< RAMCFG global interrupt                       */
    NVIC_PERIPH_IRQ_FLASH           = 6u,   /**< FLASH non-secure global interrupt             */
    NVIC_PERIPH_IRQ_FLASH_S         = 7u,   /**< FLASH secure global interrupt                 */
    NVIC_PERIPH_IRQ_GTZC            = 8u,   /**< Global TrustZone Controller interrupt         */
    NVIC_PERIPH_IRQ_RCC             = 9u,   /**< RCC non secure global interrupt               */
    NVIC_PERIPH_IRQ_RCC_S           = 10u,  /**< RCC secure global interrupt                   */
    NVIC_PERIPH_IRQ_EXTI0           = 11u,  /**< EXTI Line0 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI1           = 12u,  /**< EXTI Line1 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI2           = 13u,  /**< EXTI Line2 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI3           = 14u,  /**< EXTI Line3 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI4           = 15u,  /**< EXTI Line4 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI5           = 16u,  /**< EXTI Line5 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI6           = 17u,  /**< EXTI Line6 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI7           = 18u,  /**< EXTI Line7 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI8           = 19u,  /**< EXTI Line8 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI9           = 20u,  /**< EXTI Line9 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI10          = 21u,  /**< EXTI Line10 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI11          = 22u,  /**< EXTI Line11 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI12          = 23u,  /**< EXTI Line12 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI13          = 24u,  /**< EXTI Line13 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI14          = 25u,  /**< EXTI Line14 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI15          = 26u,  /**< EXTI Line15 interrupt                         */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL0 = 27u,  /**< GPDMA1 Channel 0 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL1 = 28u,  /**< GPDMA1 Channel 1 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL2 = 29u,  /**< GPDMA1 Channel 2 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL3 = 30u,  /**< GPDMA1 Channel 3 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL4 = 31u,  /**< GPDMA1 Channel 4 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL5 = 32u,  /**< GPDMA1 Channel 5 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL6 = 33u,  /**< GPDMA1 Channel 6 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL7 = 34u,  /**< GPDMA1 Channel 7 global interrupt             */
    NVIC_PERIPH_IRQ_IWDG            = 35u,  /**< IWDG global interrupt                         */
#if defined (SAES)
    NVIC_PERIPH_IRQ_SAES            = 36u,  /**< Secure AES global interrupt                   */
#endif
    NVIC_PERIPH_IRQ_ADC1            = 37u,  /**< ADC1 global interrupt                         */
    NVIC_PERIPH_IRQ_DAC1            = 38u,  /**< DAC1 global interrupt                         */
    NVIC_PERIPH_IRQ_FDCAN1_IT0      = 39u,  /**< FDCAN1 interrupt 0                            */
    NVIC_PERIPH_IRQ_FDCAN1_IT1      = 40u,  /**< FDCAN1 interrupt 1                            */
    NVIC_PERIPH_IRQ_TIM1_BRK        = 41u,  /**< TIM1 Break interrupt                          */
    NVIC_PERIPH_IRQ_TIM1_UP         = 42u,  /**< TIM1 Update interrupt                         */
    NVIC_PERIPH_IRQ_TIM1_TRG_COM    = 43u,  /**< TIM1 Trigger and Commutation interrupt        */
    NVIC_PERIPH_IRQ_TIM1_CC         = 44u,  /**< TIM1 Capture Compare interrupt                */
    NVIC_PERIPH_IRQ_TIM2            = 45u,  /**< TIM2 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM3            = 46u,  /**< TIM3 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM4            = 47u,  /**< TIM4 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM5            = 48u,  /**< TIM5 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM6            = 49u,  /**< TIM6 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM7            = 50u,  /**< TIM7 global interrupt                         */
    NVIC_PERIPH_IRQ_I2C1_EV         = 51u,  /**< I2C1 Event interrupt                          */
    NVIC_PERIPH_IRQ_I2C1_ER         = 52u,  /**< I2C1 Error interrupt                          */
    NVIC_PERIPH_IRQ_I2C2_EV         = 53u,  /**< I2C2 Event interrupt                          */
    NVIC_PERIPH_IRQ_I2C2_ER         = 54u,  /**< I2C2 Error interrupt                          */
    NVIC_PERIPH_IRQ_SPI1            = 55u,  /**< SPI1 global interrupt                         */
    NVIC_PERIPH_IRQ_SPI2            = 56u,  /**< SPI2 global interrupt                         */
    NVIC_PERIPH_IRQ_SPI3            = 57u,  /**< SPI3 global interrupt                         */
    NVIC_PERIPH_IRQ_USART1          = 58u,  /**< USART1 global interrupt                       */
    NVIC_PERIPH_IRQ_USART2          = 59u,  /**< USART2 global interrupt                       */
    NVIC_PERIPH_IRQ_USART3          = 60u,  /**< USART3 global interrupt                       */
    NVIC_PERIPH_IRQ_UART4           = 61u,  /**< UART4 global interrupt                        */
    NVIC_PERIPH_IRQ_UART5           = 62u,  /**< UART5 global interrupt                        */
    NVIC_PERIPH_IRQ_LPUART1         = 63u,  /**< LPUART1 global interrupt                      */
    NVIC_PERIPH_IRQ_LPTIM1          = 64u,  /**< LPTIM1 global interrupt                       */
    NVIC_PERIPH_IRQ_TIM8_BRK        = 65u,  /**< TIM8 Break interrupt                          */
    NVIC_PERIPH_IRQ_TIM8_UP         = 66u,  /**< TIM8 Update interrupt                         */
    NVIC_PERIPH_IRQ_TIM8_TRG_COM    = 67u,  /**< TIM8 Trigger and Commutation interrupt        */
    NVIC_PERIPH_IRQ_TIM8_CC         = 68u,  /**< TIM8 Capture Compare interrupt                */
    NVIC_PERIPH_IRQ_ADC2            = 69u,  /**< ADC2 global interrupt                         */
    NVIC_PERIPH_IRQ_LPTIM2          = 70u,  /**< LPTIM2 global interrupt                       */
    NVIC_PERIPH_IRQ_TIM15           = 71u,  /**< TIM15 global interrupt                        */
#if defined (TIM16)
    NVIC_PERIPH_IRQ_TIM16           = 72u,  /**< TIM16 global interrupt                        */
#endif
#if defined (TIM17)
    NVIC_PERIPH_IRQ_TIM17           = 73u,  /**< TIM17 global interrupt                        */
#endif
    NVIC_PERIPH_IRQ_USB_DRD_FS      = 74u,  /**< USB FS global interrupt                       */
    NVIC_PERIPH_IRQ_CRS             = 75u,  /**< CRS global interrupt                          */
    NVIC_PERIPH_IRQ_UCPD1           = 76u,  /**< UCPD1 global interrupt                        */
    NVIC_PERIPH_IRQ_FMC             = 77u,  /**< FMC global interrupt                          */
    NVIC_PERIPH_IRQ_OCTOSPI1        = 78u,  /**< OctoSPI1 global interrupt                     */
    NVIC_PERIPH_IRQ_SDMMC1          = 79u,  /**< SDMMC1 global interrupt                       */
    NVIC_PERIPH_IRQ_I2C3_EV         = 80u,  /**< I2C3 event interrupt                          */
    NVIC_PERIPH_IRQ_I2C3_ER         = 81u,  /**< I2C3 error interrupt                          */
    NVIC_PERIPH_IRQ_SPI4            = 82u,  /**< SPI4 global interrupt                         */
#if defined (SPI5)
    NVIC_PERIPH_IRQ_SPI5            = 83u,  /**< SPI5 global interrupt                         */
#endif
#if defined (SPI6)
    NVIC_PERIPH_IRQ_SPI6            = 84u,  /**< SPI6 global interrupt                         */
#endif
    NVIC_PERIPH_IRQ_USART6          = 85u,  /**< USART6 global interrupt                       */
#if defined (USART10)
    NVIC_PERIPH_IRQ_USART10         = 86u,  /**< USART10 global interrupt                      */
#endif
#if defined (USART11)
    NVIC_PERIPH_IRQ_USART11         = 87u,  /**< USART11 global interrupt                      */
#endif
#if defined (SAI1)
    NVIC_PERIPH_IRQ_SAI1            = 88u,  /**< Serial Audio Interface 1 global interrupt     */
#endif
#if defined (SAI2)
    NVIC_PERIPH_IRQ_SAI2            = 89u,  /**< Serial Audio Interface 2 global interrupt     */
#endif
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL0 = 90u,  /**< GPDMA2 Channel 0 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL1 = 91u,  /**< GPDMA2 Channel 1 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL2 = 92u,  /**< GPDMA2 Channel 2 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL3 = 93u,  /**< GPDMA2 Channel 3 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL4 = 94u,  /**< GPDMA2 Channel 4 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL5 = 95u,  /**< GPDMA2 Channel 5 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL6 = 96u,  /**< GPDMA2 Channel 6 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA2_CHANNEL7 = 97u,  /**< GPDMA2 Channel 7 global interrupt             */
#if defined (UART7)
    NVIC_PERIPH_IRQ_UART7           = 98u,  /**< UART7 global interrupt                        */
#endif
#if defined (UART8)
    NVIC_PERIPH_IRQ_UART8           = 99u,  /**< UART8 global interrupt                        */
#endif
#if defined (UART9)
    NVIC_PERIPH_IRQ_UART9           = 100u, /**< UART9 global interrupt                        */
#endif
#if defined (UART12)
    NVIC_PERIPH_IRQ_UART12          = 101u, /**< UART12 global interrupt                       */
#endif
#if defined (SDMMC2)
    NVIC_PERIPH_IRQ_SDMMC2          = 102u, /**< SDMMC2 global interrupt                       */
#endif
    NVIC_PERIPH_IRQ_FPU             = 103u, /**< FPU global interrupt                          */
    NVIC_PERIPH_IRQ_ICACHE          = 104u, /**< Instruction cache global interrupt            */
    NVIC_PERIPH_IRQ_DCACHE1         = 105u, /**< Data cache global interrupt                   */
#if defined (ETH)
    NVIC_PERIPH_IRQ_ETH             = 106u, /**< Ethernet global interrupt                     */
    NVIC_PERIPH_IRQ_ETH_WKUP        = 107u, /**< Ethernet Wakeup global interrupt              */
#endif
    NVIC_PERIPH_IRQ_DCMI_PSSI       = 108u, /**< DCMI/PSSI global interrupt                    */
    NVIC_PERIPH_IRQ_FDCAN2_IT0      = 109u, /**< FDCAN2 interrupt 0                            */
    NVIC_PERIPH_IRQ_FDCAN2_IT1      = 110u, /**< FDCAN2 interrupt 1                            */
#if defined (CORDIC)
    NVIC_PERIPH_IRQ_CORDIC          = 111u, /**< CORDIC global interrupt                       */
#endif
#if defined (FMAC)
    NVIC_PERIPH_IRQ_FMAC            = 112u, /**< FMAC global interrupt                         */
#endif
    NVIC_PERIPH_IRQ_DTS             = 113u, /**< DTS global interrupt                          */
    NVIC_PERIPH_IRQ_RNG             = 114u, /**< RNG global interrupt                          */
#if defined (OTFDEC1)
    NVIC_PERIPH_IRQ_OTFDEC1         = 115u, /**< OTFDEC1 global interrupt                      */
#endif
#if defined (AES)
    NVIC_PERIPH_IRQ_AES             = 116u, /**< AES global interrupt                          */
#endif
    NVIC_PERIPH_IRQ_HASH            = 117u, /**< HASH global interrupt                         */
    NVIC_PERIPH_IRQ_PKA             = 118u, /**< PKA global interrupt                          */
    NVIC_PERIPH_IRQ_CEC             = 119u, /**< CEC-HDMI global interrupt                     */
    NVIC_PERIPH_IRQ_TIM12           = 120u, /**< TIM12 global interrupt                        */
#if defined (TIM13)
    NVIC_PERIPH_IRQ_TIM13           = 121u, /**< TIM13 global interrupt                        */
#endif
#if defined (TIM14)
    NVIC_PERIPH_IRQ_TIM14           = 122u, /**< TIM14 global interrupt                        */
#endif
    NVIC_PERIPH_IRQ_I3C1_EV         = 123u, /**< I3C1 event interrupt                          */
    NVIC_PERIPH_IRQ_I3C1_ER         = 124u, /**< I3C1 error interrupt                          */
#if defined (I2C4)
    NVIC_PERIPH_IRQ_I2C4_EV         = 125u, /**< I2C4 event interrupt                          */
    NVIC_PERIPH_IRQ_I2C4_ER         = 126u, /**< I2C4 error interrupt                          */
#endif
#if defined (LPTIM3)
    NVIC_PERIPH_IRQ_LPTIM3          = 127u, /**< LPTIM3 global interrupt                       */
#endif
#if defined (LPTIM4)
    NVIC_PERIPH_IRQ_LPTIM4          = 128u, /**< LPTIM4 global interrupt                       */
#endif
#if defined (LPTIM5)
    NVIC_PERIPH_IRQ_LPTIM5          = 129u, /**< LPTIM5 global interrupt                       */
#endif
#if defined (LPTIM6)
    NVIC_PERIPH_IRQ_LPTIM6          = 130u, /**< LPTIM6 global interrupt                       */
#endif
#if defined (I3C2)
    NVIC_PERIPH_IRQ_I3C2_EV         = 131u, /**< I3C2 Event interrupt                          */
    NVIC_PERIPH_IRQ_I3C2_ER         = 132u, /**< I3C2 Error interrupt                          */
#endif
#if defined (COMP1)
    NVIC_PERIPR_IRQ_COMP1          = 133u,  /**< Comparator 1 global interrupt                 */
#endif
    NVIC_PERIPH_IRQ_SIZE                    /**< Count of peripheral Interrupts                */
}   nvic_PeriphIrqList_t;


/**
 * \brief Cortex-M IRQ list. The index is decremented by 1 to have StackPointer separated.
 *
 */
typedef enum
{
    NVIC_CORE_IRQ_RESET              = 0u,  /**< 1 Reset vector                          */
    NVIC_CORE_IRQ_NMI                = 1u,  /**< 2 Cortex-M4 Non Maskable Interrupt      */
    NVIC_CORE_IRQ_HARDFAULT          = 2u,  /**< 3 Cortex-M4 Hard Fault Interrupt        */
    NVIC_CORE_IRQ_MEMFAULT           = 3u,  /**< 5 Cortex-M4 Memory Management Interrupt */
    NVIC_CORE_IRQ_BUSFAULT           = 4u,  /**< 6 Cortex-M4 Bus Fault Interrupt         */
    NVIC_CORE_IRQ_USAGEFAULT         = 5u,  /**< 7 Cortex-M4 Usage Fault Interrupt       */
    NVIC_CORE_IRQ_SECUREFAULT        = 6u,  /*!< 8  Secure Fault                         */
    NVIC_CORE_IRQ_SVCALL             = 10u, /**< 11 Cortex-M4 SV Call Interrupt          */
    NVIC_CORE_IRQ_DEBUGMONITOR       = 11u, /**< 12 Cortex-M4 Debug Monitor Interrupt    */
    NVIC_CORE_IRQ_PENDSV             = 13u, /**< 14 Cortex-M4 Pend SV Interrupt          */
    NVIC_CORE_IRQ_SYSTICK            = 14u, /**< 15 Cortex-M4 System Tick Interrupt      */
    NVIC_CORE_IRQ_SIZE                      /**< 16 Count of core Interrupts             */
}   nvic_CoreIrqList_t;

/* ========================== EXPORTED VARIABLES ============================ */

/* ========================= EXPORTED FUNCTIONS ============================= */


#endif /* NVIC_NVIC_TYPES_H */
