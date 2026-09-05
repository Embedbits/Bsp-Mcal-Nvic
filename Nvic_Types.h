/*
 *    Mr.Nobody, COPYRIGHT (c) 2021
 *    ALL RIGHTS RESERVED
 *
 */

/**
 * \file Nvic_Types.h
 * \ingroup Nvic
 * \brief Nvic module global types definition
 *
 * This file contains the types definitions used across the module and are 
 * available for other modules through Port file.
 *
 */

#ifndef NVIC_NVIC_TYPES_H
#define NVIC_NVIC_TYPES_H
/* ============================== INCLUDES ================================== */
#include "stdint.h"                         /* Module types definition        */
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
    NVIC_PERIPH_IRQ_WWDG             = 0,   /**< Window WatchDog interrupt                     */
    NVIC_PERIPH_IRQ_PVD_PVM          = 1,   /**< PVD/PVM through EXTI Line detection Interrupt */
    NVIC_PERIPH_IRQ_RTC              = 2,   /**< RTC non-secure interrupt                      */
    NVIC_PERIPH_IRQ_RTC_S            = 3,   /**< RTC secure interrupt                          */
    NVIC_PERIPH_IRQ_TAMP             = 4,   /**< Tamper global interrupt                       */
    NVIC_PERIPH_IRQ_RAMCFG           = 5,   /**< RAMCFG global interrupt                       */
    NVIC_PERIPH_IRQ_FLASH            = 6,   /**< FLASH non-secure global interrupt             */
    NVIC_PERIPH_IRQ_FLASH_S          = 7,   /**< FLASH secure global interrupt                 */
    NVIC_PERIPH_IRQ_GTZC             = 8,   /**< Global TrustZone Controller interrupt         */
    NVIC_PERIPH_IRQ_RCC              = 9,   /**< RCC non secure global interrupt               */
    NVIC_PERIPH_IRQ_RCC_S            = 10,  /**< RCC secure global interrupt                   */
    NVIC_PERIPH_IRQ_EXTI0            = 11,  /**< EXTI Line0 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI1            = 12,  /**< EXTI Line1 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI2            = 13,  /**< EXTI Line2 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI3            = 14,  /**< EXTI Line3 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI4            = 15,  /**< EXTI Line4 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI5            = 16,  /**< EXTI Line5 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI6            = 17,  /**< EXTI Line6 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI7            = 18,  /**< EXTI Line7 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI8            = 19,  /**< EXTI Line8 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI9            = 20,  /**< EXTI Line9 interrupt                          */
    NVIC_PERIPH_IRQ_EXTI10           = 21,  /**< EXTI Line10 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI11           = 22,  /**< EXTI Line11 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI12           = 23,  /**< EXTI Line12 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI13           = 24,  /**< EXTI Line13 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI14           = 25,  /**< EXTI Line14 interrupt                         */
    NVIC_PERIPH_IRQ_EXTI15           = 26,  /**< EXTI Line15 interrupt                         */
    NVIC_PERIPH_IRQ_IWDG             = 27,  /**< IWDG global interrupt                         */
    NVIC_PERIPH_IRQ_SAES             = 28,  /**< Secure AES global interrupt                   */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL0  = 29,  /**< GPDMA1 Channel 0 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL1  = 30,  /**< GPDMA1 Channel 1 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL2  = 31,  /**< GPDMA1 Channel 2 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL3  = 32,  /**< GPDMA1 Channel 3 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL4  = 33,  /**< GPDMA1 Channel 4 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL5  = 34,  /**< GPDMA1 Channel 5 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL6  = 35,  /**< GPDMA1 Channel 6 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL7  = 36,  /**< GPDMA1 Channel 7 global interrupt             */
    NVIC_PERIPH_IRQ_ADC1_2           = 37,  /**< ADC1_2 global interrupt                       */
    NVIC_PERIPH_IRQ_DAC1             = 38,  /**< DAC1 global interrupt                         */
    NVIC_PERIPH_IRQ_FDCAN1_IT0       = 39,  /**< FDCAN1 interrupt 0                            */
    NVIC_PERIPH_IRQ_FDCAN1_IT1       = 40,  /**< FDCAN1 interrupt 1                            */
    NVIC_PERIPH_IRQ_TIM1_BRK         = 41,  /**< TIM1 Break interrupt                          */
    NVIC_PERIPH_IRQ_TIM1_UP          = 42,  /**< TIM1 Update interrupt                         */
    NVIC_PERIPH_IRQ_TIM1_TRG_COM     = 43,  /**< TIM1 Trigger and Commutation interrupt        */
    NVIC_PERIPH_IRQ_TIM1_CC          = 44,  /**< TIM1 Capture Compare interrupt                */
    NVIC_PERIPH_IRQ_TIM2             = 45,  /**< TIM2 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM3             = 46,  /**< TIM3 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM4             = 47,  /**< TIM4 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM5             = 48,  /**< TIM5 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM6             = 49,  /**< TIM6 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM7             = 50,  /**< TIM7 global interrupt                         */
    NVIC_PERIPH_IRQ_TIM8_BRK         = 51,  /**< TIM8 Break interrupt                          */
    NVIC_PERIPH_IRQ_TIM8_UP          = 52,  /**< TIM8 Update interrupt                         */
    NVIC_PERIPH_IRQ_TIM8_TRG_COM     = 53,  /**< TIM8 Trigger and Commutation interrupt        */
    NVIC_PERIPH_IRQ_TIM8_CC          = 54,  /**< TIM8 Capture Compare interrupt                */
    NVIC_PERIPH_IRQ_I2C1_EV          = 55,  /**< I2C1 Event interrupt                          */
    NVIC_PERIPH_IRQ_I2C1_ER          = 56,  /**< I2C1 Error interrupt                          */
    NVIC_PERIPH_IRQ_I2C2_EV          = 57,  /**< I2C2 Event interrupt                          */
    NVIC_PERIPH_IRQ_I2C2_ER          = 58,  /**< I2C2 Error interrupt                          */
    NVIC_PERIPH_IRQ_SPI1             = 59,  /**< SPI1 global interrupt                         */
    NVIC_PERIPH_IRQ_SPI2             = 60,  /**< SPI2 global interrupt                         */
    NVIC_PERIPH_IRQ_USART1           = 61,  /**< USART1 global interrupt                       */
    NVIC_PERIPH_IRQ_USART2           = 62,  /**< USART2 global interrupt                       */
    NVIC_PERIPH_IRQ_USART3           = 63,  /**< USART3 global interrupt                       */
    NVIC_PERIPH_IRQ_UART4            = 64,  /**< UART4 global interrupt                        */
    NVIC_PERIPH_IRQ_UART5            = 65,  /**< UART5 global interrupt                        */
    NVIC_PERIPH_IRQ_LPUART1          = 66,  /**< LPUART1 global interrupt                      */
    NVIC_PERIPH_IRQ_LPTIM1           = 67,  /**< LPTIM1 global interrupt                       */
    NVIC_PERIPH_IRQ_LPTIM2           = 68,  /**< LPTIM2 global interrupt                       */
    NVIC_PERIPH_IRQ_TIM15            = 69,  /**< TIM15 global interrupt                        */
    NVIC_PERIPH_IRQ_TIM16            = 70,  /**< TIM16 global interrupt                        */
    NVIC_PERIPH_IRQ_TIM17            = 71,  /**< TIM17 global interrupt                        */
    NVIC_PERIPH_IRQ_COMP             = 72,  /**< COMP1 and COMP2 through EXTI Lines interrupts */
    NVIC_PERIPH_IRQ_OTG_HS           = 73,  /**< USB OTG HS global interrupt                   */
    NVIC_PERIPH_IRQ_CRS              = 74,  /**< CRS global interrupt                          */
    NVIC_PERIPH_IRQ_FMC              = 75,  /**< FSMC global interrupt                         */
    NVIC_PERIPH_IRQ_OCTOSPI1         = 76,  /**< OctoSPI1 global interrupt                     */
    NVIC_PERIPH_IRQ_PWR_S3WU         = 77,  /**< PWR wake up from Stop3 interrupt              */
    NVIC_PERIPH_IRQ_SDMMC1           = 78,  /**< SDMMC1 global interrupt                       */
    NVIC_PERIPH_IRQ_SDMMC2           = 79,  /**< SDMMC2 global interrupt                       */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL8  = 80,  /**< GPDMA1 Channel 8 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL9  = 81,  /**< GPDMA1 Channel 9 global interrupt             */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL10 = 82,  /**< GPDMA1 Channel 10 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL11 = 83,  /**< GPDMA1 Channel 11 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL12 = 84,  /**< GPDMA1 Channel 12 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL13 = 85,  /**< GPDMA1 Channel 13 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL14 = 86,  /**< GPDMA1 Channel 14 global interrupt            */
    NVIC_PERIPH_IRQ_GPDMA1_CHANNEL15 = 87,  /**< GPDMA1 Channel 15 global interrupt            */
    NVIC_PERIPH_IRQ_I2C3_EV          = 88,  /**< I2C3 event interrupt                          */
    NVIC_PERIPH_IRQ_I2C3_ER          = 89,  /**< I2C3 error interrupt                          */
    NVIC_PERIPH_IRQ_SAI1             = 90,  /**< Serial Audio Interface 1 global interrupt     */
    NVIC_PERIPH_IRQ_SAI2             = 91,  /**< Serial Audio Interface 2 global interrupt     */
    NVIC_PERIPH_IRQ_TSC              = 92,  /**< Touch Sense Controller global interrupt       */
    NVIC_PERIPH_IRQ_AES              = 93,  /**< AES global interrupt                          */
    NVIC_PERIPH_IRQ_RNG              = 94,  /**< RNG global interrupt                          */
    NVIC_PERIPH_IRQ_FPU              = 95,  /**< FPU global interrupt                          */
    NVIC_PERIPH_IRQ_HASH             = 96,  /**< HASH global interrupt                         */
    NVIC_PERIPH_IRQ_PKA              = 97,  /**< PKA global interrupt                          */
    NVIC_PERIPH_IRQ_LPTIM3           = 98,  /**< LPTIM3 global interrupt                       */
    NVIC_PERIPH_IRQ_SPI3             = 99,  /**< SPI3 global interrupt                         */
    NVIC_PERIPH_IRQ_I2C4_ER          = 100, /**< I2C4 Error interrupt                          */
    NVIC_PERIPH_IRQ_I2C4_EV          = 101, /**< I2C4 Event interrupt                          */
    NVIC_PERIPH_IRQ_MDF1_FLT0        = 102, /**< MDF1 Filter 0 global interrupt                */
    NVIC_PERIPH_IRQ_MDF1_FLT1        = 103, /**< MDF1 Filter 1 global interrupt                */
    NVIC_PERIPH_IRQ_MDF1_FLT2        = 104, /**< MDF1 Filter 2 global interrupt                */
    NVIC_PERIPH_IRQ_MDF1_FLT3        = 105, /**< MDF1 Filter 3 global interrupt                */
    NVIC_PERIPH_IRQ_UCPD1            = 106, /**< UCPD1 global interrupt                        */
    NVIC_PERIPH_IRQ_ICACHE           = 107, /**< Instruction cache global interrupt            */
    NVIC_PERIPH_IRQ_OTFDEC1          = 108, /**< OTFDEC1 global interrupt                      */
    NVIC_PERIPH_IRQ_OTFDEC2          = 109, /**< OTFDEC2 global interrupt                      */
    NVIC_PERIPH_IRQ_LPTIM4           = 110, /**< LPTIM4 global interrupt                       */
    NVIC_PERIPH_IRQ_DCACHE1          = 111, /**< Data cache global interrupt                   */
    NVIC_PERIPH_IRQ_ADF1             = 112, /**< ADF interrupt                                 */
    NVIC_PERIPH_IRQ_ADC4             = 113, /**< ADC4 (12bits) global interrupt                */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL0  = 114, /**< LPDMA1 SmartRun Channel 0 global interrupt    */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL1  = 115, /**< LPDMA1 SmartRun Channel 1 global interrupt    */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL2  = 116, /**< LPDMA1 SmartRun Channel 2 global interrupt    */
    NVIC_PERIPH_IRQ_LPDMA1_CHANNEL3  = 117, /**< LPDMA1 SmartRun Channel 3 global interrupt    */
    NVIC_PERIPH_IRQ_DMA2D            = 118, /**< DMA2D global interrupt                        */
    NVIC_PERIPH_IRQ_DCMI_PSSI        = 119, /**< DCMI/PSSI global interrupt                    */
    NVIC_PERIPH_IRQ_OCTOSPI2         = 120, /**< OCTOSPI2 global interrupt                     */
    NVIC_PERIPH_IRQ_MDF1_FLT4        = 121, /**< MDF1 Filter 4 global interrupt                */
    NVIC_PERIPH_IRQ_MDF1_FLT5        = 122, /**< MDF1 Filter 5 global interrupt                */
    NVIC_PERIPH_IRQ_CORDIC           = 123, /**< CORDIC global interrupt                       */
    NVIC_PERIPH_IRQ_FMAC             = 124, /**< FMAC global interrupt                         */
    NVIC_PERIPH_IRQ_LSECSSD          = 125, /**< LSECSSD and MSI_PLL_UNLOCK global interrupts  */
    NVIC_PERIPH_IRQ_USART6           = 126, /**< USART6 global interrupt                       */
    NVIC_PERIPH_IRQ_I2C5_ER          = 127, /**< I2C5 Error interrupt                          */
    NVIC_PERIPH_IRQ_I2C5_EV          = 128, /**< I2C5 Event interrupt                          */
    NVIC_PERIPH_IRQ_I2C6_ER          = 129, /**< I2C6 Error interrupt                          */
    NVIC_PERIPH_IRQ_I2C6_EV          = 130, /**< I2C6 Error interrupt                          */
    NVIC_PERIPH_IRQ_HSPI1            = 131, /**< HSPI1 global interrupt                        */
    NVIC_PERIPH_IRQ_GPU2D            = 132, /**< GPU2D global interrupt                        */
    NVIC_PERIPH_IRQ_GPU2D_ER         = 133, /**< GPU2D Error interrupt                         */
    NVIC_PERIPH_IRQ_GFXMMU           = 134, /**< GFXMMU global interrupt                       */
    NVIC_PERIPH_IRQ_LTDC             = 135, /**< LCD-TFT global interrupt                      */
    NVIC_PERIPH_IRQ_LTDC_ER          = 136, /**< LCD-TFT Error interrupt                       */
    NVIC_PERIPH_IRQ_DSI              = 137, /**< DSIHOST global interrupt                      */
    NVIC_PERIPH_IRQ_DCACHE2          = 138, /**< DCACHE2 Data cache global interrupt           */
    NVIC_PERIPH_IRQ_GFXTIM           = 139, /**< GFXTIM global interrupt                       */
    NVIC_PERIPH_IRQ_JPEG             = 140, /**< JPEG sync interrupt                           */
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
    NVIC_CORE_IRQ_SEC_HARDFAULT      = 2u,  /**< 3 Cortex-M4 Hard Fault Interrupt        */
    NVIC_CORE_IRQ_NONSEC_HARDFAULT   = 3u,  /**< 4 Cortex-M4 Hard Fault Interrupt        */
    NVIC_CORE_IRQ_MEMFAULT           = 4u,  /**< 5 Cortex-M4 Memory Management Interrupt */
    NVIC_CORE_IRQ_BUSFAULT           = 5u,  /**< 6 Cortex-M4 Bus Fault Interrupt         */
    NVIC_CORE_IRQ_USAGEFAULT         = 6u,  /**< 7 Cortex-M4 Usage Fault Interrupt       */
    NVIC_CORE_IRQ_SECUREFAULT        = 7u,  /*!< 8  Secure Fault                         */
    NVIC_CORE_IRQ_SVCALL             = 10u, /**< 11 Cortex-M4 SV Call Interrupt          */
    NVIC_CORE_IRQ_DEBUGMONITOR       = 11u, /**< 12 Cortex-M4 Debug Monitor Interrupt    */
    NVIC_CORE_IRQ_PENDSV             = 13u, /**< 14 Cortex-M4 Pend SV Interrupt          */
    NVIC_CORE_IRQ_SYSTICK            = 14u, /**< 15 Cortex-M4 System Tick Interrupt      */
    NVIC_CORE_IRQ_SIZE                      /**< 16 Count of core Interrupts             */
}   nvic_CoreIrqList_t;

/* ========================== EXPORTED VARIABLES ============================ */

/* ========================= EXPORTED FUNCTIONS ============================= */


#endif /* NVIC_NVIC_TYPES_H */
