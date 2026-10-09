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

/** STM32U5 uses 4 Bits for the Priority Levels */
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
 * \ref NVIC_PRIO_BITS For STM32U5 is the range 0-15
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
    NVIC_PERIPH_IRQ_WWDG             = 0u,   /**< Window WatchDog interrupt                     */
    NVIC_PERIPH_IRQ_PVD_PVM          = 1u,   /**< PVD/PVM through EXTI Line detection Interrupt */
    NVIC_PERIPH_IRQ_RTC              = 2u,   /**< RTC non-secure interrupt                      */
    NVIC_PERIPH_IRQ_RTC_S            = 3u,   /**< RTC secure interrupt                          */
    NVIC_PERIPH_IRQ_TAMP             = 4u,   /**< Tamper global interrupt                       */
    NVIC_PERIPH_IRQ_RAMCFG           = 5u,   /**< RAMCFG global interrupt                       */
    NVIC_PERIPH_IRQ_FLASH            = 6u,   /**< FLASH non-secure global interrupt             */
    NVIC_PERIPH_IRQ_FLASH_S          = 7u,   /**< FLASH secure global interrupt                 */
    NVIC_PERIPH_IRQ_GTZC             = 8u,   /**< Global TrustZone Controller interrupt         */
    NVIC_PERIPH_IRQ_RCC              = 9u,   /**< RCC non secure global interrupt               */
    NVIC_PERIPH_IRQ_RCC_S            = 10u,  /**< RCC secure global interrupt                   */
    NVIC_PERIPH_IRQ_EXTI0            = 11u,  /**< EXTI Line0 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI1            = 12u,  /**< EXTI Line1 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI2            = 13u,  /**< EXTI Line2 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI3            = 14u,  /**< EXTI Line3 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI4            = 15u,  /**< EXTI Line4 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI5            = 16u,  /**< EXTI Line5 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI6            = 17u,  /**< EXTI Line6 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI7            = 18u,  /**< EXTI Line7 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI8            = 19u,  /**< EXTI Line8 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI9            = 20u,  /**< EXTI Line9 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI10           = 21u,  /**< EXTI Line10 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI11           = 22u,  /**< EXTI Line11 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI12           = 23u,  /**< EXTI Line12 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI13           = 24u,  /**< EXTI Line13 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI14           = 25u,  /**< EXTI Line14 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI15           = 26u,  /**< EXTI Line15 interrupt                         */
    NVIC_PERIPH_IRQ_IWDG             = 27u,  /**< IWDG global interrupt                         */
#if defined (SAES)
    NVIC_PERIPH_IRQ_SAES             = 28u,  /**< Secure AES global interrupt                   */
#endif
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL0  = 29u,  /**< GPDMA1 Channel 0 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL1  = 30u,  /**< GPDMA1 Channel 1 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL2  = 31u,  /**< GPDMA1 Channel 2 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL3  = 32u,  /**< GPDMA1 Channel 3 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL4  = 33u,  /**< GPDMA1 Channel 4 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL5  = 34u,  /**< GPDMA1 Channel 5 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL6  = 35u,  /**< GPDMA1 Channel 6 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL7  = 36u,  /**< GPDMA1 Channel 7 global interrupt             */
    NVIC_PERIPH_IRQ_ADC1             = 37u,  /**< ADC1 global interrupt (shared with ADC2)     */
#if defined (ADC2)
    NVIC_PERIPH_IRQ_ADC1_2           = 37u,  /**< ADC1 and ADC2 global interrupt               */
#endif
    NVIC_PERIPH_IRQ_DAC1             = 38u,  /**< DAC1 global interrupt                         */
    NVIC_PERIPH_IRQ_FDCAN1_IT0       = 39u,  /**< FDCAN1 interrupt 0                            */
    NVIC_PERIPH_IRQ_FDCAN1_IT1       = 40u,  /**< FDCAN1 interrupt 1                            */
    NVIC_PERIPH_IRQ_TIM1_BRK         = 41u,  /**< TIM1 Break interrupt                          */
    NVIC_PERIPH_IRQ_TIM1_UP          = 42u,  /**< TIM1 Update interrupt                         */
    NVIC_PERIPH_IRQ_TIM1_TRG_COM     = 43u,  /**< TIM1 Trigger and Commutation interrupt        */
    NVIC_PERIPH_IRQ_TIM1_CC          = 44u,  /**< TIM1 Capture Compare interrupt                */
    NVIC_PERIPH_IRQ_TIM2             = 45u,  /**< TIM2 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM3             = 46u,  /**< TIM3 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM4             = 47u,  /**< TIM4 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM5             = 48u,  /**< TIM5 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM6             = 49u,  /**< TIM6 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM7             = 50u,  /**< TIM7 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM8_BRK         = 51u,  /**< TIM8 Break interrupt                          */
    NVIC_PERIPH_IRQ_TIM8_UP          = 52u,  /**< TIM8 Update interrupt                         */
    NVIC_PERIPH_IRQ_TIM8_TRG_COM     = 53u,  /**< TIM8 Trigger and Commutation interrupt        */
    NVIC_PERIPH_IRQ_TIM8_CC          = 54u,  /**< TIM8 Capture Compare interrupt                */
    NVIC_PERIPH_IRQ_I2C1_EV          = 55u,  /**< I2C1 Event interrupt                          */
    NVIC_PERIPH_IRQ_I2C1_ER          = 56u,  /**< I2C1 Error interrupt                          */
    NVIC_PERIPH_IRQ_I2C2_EV          = 57u,  /**< I2C2 Event interrupt                          */
    NVIC_PERIPH_IRQ_I2C2_ER          = 58u,  /**< I2C2 Error interrupt                          */
    NVIC_PERIPH_IRQ_SPI1             = 59u,  /**< SPI1 global interrupt                         */
    NVIC_PERIPH_IRQ_SPI2             = 60u,  /**< SPI2 global interrupt                         */
    NVIC_PERIPH_IRQ_USART1           = 61u,  /**< USART1 global interrupt                       */
#if defined (USART2)
    NVIC_PERIPH_IRQ_USART2           = 62u,  /**< USART2 global interrupt                       */
#endif
    NVIC_PERIPH_IRQ_USART3           = 63u,  /**< USART3 global interrupt                       */
    NVIC_PERIPH_IRQ_UART4            = 64u,  /**< UART4 global interrupt                        */
    NVIC_PERIPH_IRQ_UART5            = 65u,  /**< UART5 global interrupt                        */
    NVIC_PERIPH_IRQ_LPUART1          = 66u,  /**< LPUART1 global interrupt                      */
    NVIC_PERIPH_IRQ_LPTIM1           = 67u,  /**< LPTIM1 global interrupt                       */
    NVIC_PERIPH_IRQ_LPTIM2           = 68u,  /**< LPTIM2 global interrupt                       */
    NVIC_PERIPH_IRQ_TIM15            = 69u,  /**< TIM15 global interrupt                        */
    NVIC_PERIPH_IRQ_TIM16            = 70u,  /**< TIM16 global interrupt                        */
    NVIC_PERIPH_IRQ_TIM17            = 71u,  /**< TIM17 global interrupt                        */
    NVIC_PERIPH_IRQ_COMP             = 72u,  /**< COMP1 and COMP2 through EXTI Lines interrupts */
#if defined (USB_DRD_FS)
    NVIC_PERIPH_IRQ_USB              = 73u,  /**< USB DRD FS global interrupt                   */
#endif
#if defined (USB_OTG_FS)
    NVIC_PERIPH_IRQ_OTG_FS           = 73u,  /**< USB OTG FS global interrupt                   */
#endif
#if defined (USB_OTG_HS)
    NVIC_PERIPH_IRQ_OTG_HS           = 73u,  /**< USB OTG HS global interrupt                   */
#endif
    NVIC_PERIPH_IRQ_CRS              = 74u,  /**< CRS global interrupt                          */
#if defined (FMC_Bank1_R)
    NVIC_PERIPH_IRQ_FMC              = 75u,  /**< FSMC global interrupt                         */
#endif
    NVIC_PERIPH_IRQ_OCTOSPI1         = 76u,  /**< OctoSPI1 global interrupt                     */
    NVIC_PERIPH_IRQ_PWR_S3WU         = 77u,  /**< PWR wake up from Stop3 interrupt              */
    NVIC_PERIPH_IRQ_SDMMC1           = 78u,  /**< SDMMC1 global interrupt                       */
#if defined (SDMMC2)
    NVIC_PERIPH_IRQ_SDMMC2           = 79u,  /**< SDMMC2 global interrupt                       */
#endif
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL8  = 80u,  /**< GPDMA1 Channel 8 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL9  = 81u,  /**< GPDMA1 Channel 9 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL10 = 82u,  /**< GPDMA1 Channel 10 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL11 = 83u,  /**< GPDMA1 Channel 11 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL12 = 84u,  /**< GPDMA1 Channel 12 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL13 = 85u,  /**< GPDMA1 Channel 13 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL14 = 86u,  /**< GPDMA1 Channel 14 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL15 = 87u,  /**< GPDMA1 Channel 15 global interrupt            */
    NVIC_PERIPH_IRQ_I2C3_EV          = 88u,  /**< I2C3 event interrupt                          */
    NVIC_PERIPH_IRQ_I2C3_ER          = 89u,  /**< I2C3 error interrupt                          */
    NVIC_PERIPH_IRQ_SAI1             = 90u,  /**< Serial Audio Interface 1 global interrupt     */
#if defined (SAI2)
    NVIC_PERIPH_IRQ_SAI2             = 91u,  /**< Serial Audio Interface 2 global interrupt     */
#endif
    NVIC_PERIPH_IRQ_TSC              = 92u,  /**< Touch Sense Controller global interrupt       */
#if defined (AES)
    NVIC_PERIPH_IRQ_AES              = 93u,  /**< AES global interrupt                          */
#endif
    NVIC_PERIPH_IRQ_RNG              = 94u,  /**< RNG global interrupt                          */
    NVIC_PERIPH_IRQ_FPU              = 95u,  /**< FPU global interrupt                          */
    NVIC_PERIPH_IRQ_HASH             = 96u,  /**< HASH global interrupt                         */
#if defined (PKA)
    NVIC_PERIPH_IRQ_PKA              = 97u,  /**< PKA global interrupt                          */
#endif
    NVIC_PERIPH_IRQ_LPTIM3           = 98u,  /**< LPTIM3 global interrupt                       */
    NVIC_PERIPH_IRQ_SPI3             = 99u,  /**< SPI3 global interrupt                         */
    NVIC_PERIPH_IRQ_I2C4_ER          = 100u, /**< I2C4 Error interrupt                          */
    NVIC_PERIPH_IRQ_I2C4_EV          = 101u, /**< I2C4 Event interrupt                          */
    NVIC_PERIPH_IRQ_MDF1_FLT0        = 102u, /**< MDF1 Filter 0 global interrupt                */
    NVIC_PERIPH_IRQ_MDF1_FLT1        = 103u, /**< MDF1 Filter 1 global interrupt                */
#if !defined (STM32U535xx) && \
    !defined (STM32U545xx)
    NVIC_PERIPH_IRQ_MDF1_FLT2        = 104u, /**< MDF1 Filter 2 global interrupt                */
#endif
#if !defined (STM32U535xx) && \
    !defined (STM32U545xx)
    NVIC_PERIPH_IRQ_MDF1_FLT3        = 105u, /**< MDF1 Filter 3 global interrupt                */
#endif
#if defined (UCPD1)
    NVIC_PERIPH_IRQ_UCPD1            = 106u, /**< UCPD1 global interrupt                        */
#endif
    NVIC_PERIPH_IRQ_ICACHE           = 107u, /**< Instruction cache global interrupt            */
#if defined (OTFDEC1)
    NVIC_PERIPH_IRQ_OTFDEC1          = 108u, /**< OTFDEC1 global interrupt                      */
#endif
#if defined (OTFDEC2)
    NVIC_PERIPH_IRQ_OTFDEC2          = 109u, /**< OTFDEC2 global interrupt                      */
#endif
    NVIC_PERIPH_IRQ_LPTIM4           = 110u, /**< LPTIM4 global interrupt                       */
    NVIC_PERIPH_IRQ_DCACHE1          = 111u, /**< Data cache global interrupt                   */
    NVIC_PERIPH_IRQ_ADF1             = 112u, /**< ADF interrupt                                 */
    NVIC_PERIPH_IRQ_ADC4             = 113u, /**< ADC4 (12bits) global interrupt                */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL0  = 114u, /**< LPDMA1 SmartRun Channel 0 global interrupt    */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL1  = 115u, /**< LPDMA1 SmartRun Channel 1 global interrupt    */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL2  = 116u, /**< LPDMA1 SmartRun Channel 2 global interrupt    */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL3  = 117u, /**< LPDMA1 SmartRun Channel 3 global interrupt    */
#if defined (DMA2D)
    NVIC_PERIPH_IRQ_DMA2D            = 118u, /**< DMA2D global interrupt                        */
#endif
    NVIC_PERIPH_IRQ_DCMI_PSSI        = 119u, /**< DCMI/PSSI global interrupt                    */
#if defined (OCTOSPI2)
    NVIC_PERIPH_IRQ_OCTOSPI2         = 120u, /**< OCTOSPI2 global interrupt                     */
#endif
#if !defined (STM32U535xx) && \
    !defined (STM32U545xx)
    NVIC_PERIPH_IRQ_MDF1_FLT4        = 121u, /**< MDF1 Filter 4 global interrupt                */
#endif
#if !defined (STM32U535xx) && \
    !defined (STM32U545xx)
    NVIC_PERIPH_IRQ_MDF1_FLT5        = 122u, /**< MDF1 Filter 5 global interrupt                */
#endif
    NVIC_PERIPH_IRQ_CORDIC           = 123u, /**< CORDIC global interrupt                       */
    NVIC_PERIPH_IRQ_FMAC             = 124u, /**< FMAC global interrupt                         */
    NVIC_PERIPH_IRQ_LSECSSD          = 125u, /**< LSECSSD and MSI_PLL_UNLOCK global interrupts  */
#if defined (USART6)
    NVIC_PERIPH_IRQ_USART6           = 126u, /**< USART6 global interrupt                       */
#endif
#if defined (I2C5)
    NVIC_PERIPH_IRQ_I2C5_ER          = 127u, /**< I2C5 Error interrupt                          */
#endif
#if defined (I2C5)
    NVIC_PERIPH_IRQ_I2C5_EV          = 128u, /**< I2C5 Event interrupt                          */
#endif
#if defined (I2C6)
    NVIC_PERIPH_IRQ_I2C6_ER          = 129u, /**< I2C6 Error interrupt                          */
#endif
#if defined (I2C6)
    NVIC_PERIPH_IRQ_I2C6_EV          = 130u, /**< I2C6 Event interrupt                          */
#endif
#if defined (HSPI1)
    NVIC_PERIPH_IRQ_HSPI1            = 131u, /**< HSPI1 global interrupt                        */
#endif
#if defined (GPU2D)
    NVIC_PERIPH_IRQ_GPU2D            = 132u, /**< GPU2D global interrupt                        */
#endif
#if defined (GPU2D)
    NVIC_PERIPH_IRQ_GPU2D_ER         = 133u, /**< GPU2D Error interrupt                         */
#endif
#if defined (GFXMMU)
    NVIC_PERIPH_IRQ_GFXMMU           = 134u, /**< GFXMMU global interrupt                       */
#endif
#if defined (LTDC)
    NVIC_PERIPH_IRQ_LTDC             = 135u, /**< LCD-TFT global interrupt                      */
#endif
#if defined (LTDC)
    NVIC_PERIPH_IRQ_LTDC_ER          = 136u, /**< LCD-TFT Error interrupt                       */
#endif
#if defined (DSI)
    NVIC_PERIPH_IRQ_DSI              = 137u, /**< DSIHOST global interrupt                      */
#endif
#if defined (DCACHE2)
    NVIC_PERIPH_IRQ_DCACHE2          = 138u, /**< DCACHE2 Data cache global interrupt           */
#endif
#if defined (GFXTIM)
    NVIC_PERIPH_IRQ_GFXTIM           = 139u, /**< GFXTIM global interrupt                       */
#endif
#if defined (JPEG)
    NVIC_PERIPH_IRQ_JPEG             = 140u, /**< JPEG sync interrupt                           */
#endif
    NVIC_PERIPH_IRQ_SIZE                    /**< Count of peripheral Interrupts                */
}   nvic_PeriphIrqList_t;


/**
 * \brief Cortex-M IRQ list. The index is exception number decremented by 1 to
 *        have StackPointer separated (comments contain exception number).
 *
 * \note  Secure and Non-secure HardFault share single vector (exception 3,
 *        banked by security state). Exceptions 8 - 10 and 13 are reserved on
 *        Cortex-M33.
 */
typedef enum
{
    NVIC_CORE_IRQ_RESET              = 0u,  /**< 1 Reset vector                           */
    NVIC_CORE_IRQ_NMI                = 1u,  /**< 2 Cortex-M33 Non Maskable Interrupt      */
    NVIC_CORE_IRQ_HARDFAULT          = 2u,  /**< 3 Cortex-M33 Hard Fault Interrupt        */
    NVIC_CORE_IRQ_MEMFAULT           = 3u,  /**< 4 Cortex-M33 Memory Management Interrupt */
    NVIC_CORE_IRQ_BUSFAULT           = 4u,  /**< 5 Cortex-M33 Bus Fault Interrupt         */
    NVIC_CORE_IRQ_USAGEFAULT         = 5u,  /**< 6 Cortex-M33 Usage Fault Interrupt       */
    NVIC_CORE_IRQ_SECUREFAULT        = 6u,  /**< 7 Cortex-M33 Secure Fault Interrupt      */
    NVIC_CORE_IRQ_SVCALL             = 10u, /**< 11 Cortex-M33 SV Call Interrupt          */
    NVIC_CORE_IRQ_DEBUGMONITOR       = 11u, /**< 12 Cortex-M33 Debug Monitor Interrupt    */
    NVIC_CORE_IRQ_PENDSV             = 13u, /**< 14 Cortex-M33 Pend SV Interrupt          */
    NVIC_CORE_IRQ_SYSTICK            = 14u, /**< 15 Cortex-M33 System Tick Interrupt      */
    NVIC_CORE_IRQ_SIZE                      /**< Count of core Interrupts (15 entries)    */
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
