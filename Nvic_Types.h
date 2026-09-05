/**
 * \author Mr.Nobody
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
#define NVIC_NULL_PTR                        ( ( void* ) 0u )

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
    NVIC_PERIPH_IRQ_WWDG               = 0,      /**< Window WatchDog Interrupt                                                          */
    NVIC_PERIPH_IRQ_PVD_PVM            = 1,      /**< PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection Interrupts                     */
    NVIC_PERIPH_IRQ_RTC_TAMP_LSECSS    = 2,      /**< RTC Tamper and TimeStamp and RCC LSE CSS interrupts through the EXTI               */
    NVIC_PERIPH_IRQ_RTC_WKUP           = 3,      /**< RTC Wakeup interrupt through the EXTI line                                         */
    NVIC_PERIPH_IRQ_FLASH              = 4,      /**< FLASH global Interrupt                                                             */
    NVIC_PERIPH_IRQ_RCC                = 5,      /**< RCC global Interrupt                                                               */
    NVIC_PERIPH_IRQ_EXTI0              = 6,      /**< EXTI Line0 Interrupt                                                               */
    NVIC_PERIPH_IRQ_EXTI1              = 7,      /**< EXTI Line1 Interrupt                                                               */
    NVIC_PERIPH_IRQ_EXTI2              = 8,      /**< EXTI Line2 Interrupt                                                               */
    NVIC_PERIPH_IRQ_EXTI3              = 9,      /**< EXTI Line3 Interrupt                                                               */
    NVIC_PERIPH_IRQ_EXTI4              = 10,     /**< EXTI Line4 Interrupt                                                               */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL1      = 11,     /**< DMA1 Channel 1 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL2      = 12,     /**< DMA1 Channel 2 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL3      = 13,     /**< DMA1 Channel 3 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL4      = 14,     /**< DMA1 Channel 4 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL5      = 15,     /**< DMA1 Channel 5 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL6      = 16,     /**< DMA1 Channel 6 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL7      = 17,     /**< DMA1 Channel 7 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_ADC1_2             = 18,     /**< ADC1 and ADC2 global Interrupt                                                     */
    NVIC_PERIPH_IRQ_USB_HP             = 19,     /**< USB HP Interrupt                                                                   */
    NVIC_PERIPH_IRQ_USB_LP             = 20,     /**< USB LP  Interrupt                                                                  */
    NVIC_PERIPH_IRQ_FDCAN1_IT0         = 21,     /**< FDCAN1 IT0 Interrupt                                                               */
    NVIC_PERIPH_IRQ_FDCAN1_IT1         = 22,     /**< FDCAN1 IT1 Interrupt                                                               */
    NVIC_PERIPH_IRQ_EXTI9_5            = 23,     /**< External Line[9:5] Interrupts                                                      */
    NVIC_PERIPH_IRQ_TIM1_BRK_TIM15     = 24,     /**< TIM1 Break, Transition error, Index error and TIM15 global interrupt               */
    NVIC_PERIPH_IRQ_TIM1_UP_TIM16      = 25,     /**< TIM1 Update Interrupt and TIM16 global interrupt                                   */
    NVIC_PERIPH_IRQ_TIM1_TRG_COM_TIM17 = 26,     /**< TIM1 TIM1 Trigger, Commutation, Direction change, Index and TIM17 global interrupt */
    NVIC_PERIPH_IRQ_TIM1_CC            = 27,     /**< TIM1 Capture Compare Interrupt                                                     */
    NVIC_PERIPH_IRQ_TIM2               = 28,     /**< TIM2 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_TIM3               = 29,     /**< TIM3 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_TIM4               = 30,     /**< TIM4 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_I2C1_EV            = 31,     /**< I2C1 Event Interrupt                                                               */
    NVIC_PERIPH_IRQ_I2C1_ER            = 32,     /**< I2C1 Error Interrupt                                                               */
    NVIC_PERIPH_IRQ_I2C2_EV            = 33,     /**< I2C2 Event Interrupt                                                               */
    NVIC_PERIPH_IRQ_I2C2_ER            = 34,     /**< I2C2 Error Interrupt                                                               */
    NVIC_PERIPH_IRQ_SPI1               = 35,     /**< SPI1 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_SPI2               = 36,     /**< SPI2 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_USART1             = 37,     /**< USART1 global Interrupt                                                            */
    NVIC_PERIPH_IRQ_USART2             = 38,     /**< USART2 global Interrupt                                                            */
    NVIC_PERIPH_IRQ_USART3             = 39,     /**< USART3 global Interrupt                                                            */
    NVIC_PERIPH_IRQ_EXTI15_10          = 40,     /**< External Line[15:10] Interrupts                                                    */
    NVIC_PERIPH_IRQ_RTC_Alarm          = 41,     /**< RTC Alarm (A and B) through EXTI Line Interrupt                                    */
    NVIC_PERIPH_IRQ_USBWAKEUP          = 42,     /**< USB Wakeup through EXTI line Interrupt                                             */
    NVIC_PERIPH_IRQ_TIM8_BRK           = 43,     /**< TIM8 Break, Transition error and Index error Interrupt                             */
    NVIC_PERIPH_IRQ_TIM8_UP            = 44,     /**< TIM8 Update Interrupt                                                              */
    NVIC_PERIPH_IRQ_TIM8_TRG_COM       = 45,     /**< TIM8 Trigger, Commutation, Direction change and Index Interrupt                    */
    NVIC_PERIPH_IRQ_TIM8_CC            = 46,     /**< TIM8 Capture Compare Interrupt                                                     */
    NVIC_PERIPH_IRQ_ADC3               = 47,     /**< ADC3 global  Interrupt                                                             */
    NVIC_PERIPH_IRQ_FMC                = 48,     /**< FMC global Interrupt                                                               */
    NVIC_PERIPH_IRQ_LPTIM1             = 49,     /**< LP TIM1 Interrupt                                                                  */
    NVIC_PERIPH_IRQ_TIM5               = 50,     /**< TIM5 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_SPI3               = 51,     /**< SPI3 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_UART4              = 52,     /**< UART4 global Interrupt                                                             */
    NVIC_PERIPH_IRQ_UART5              = 53,     /**< UART5 global Interrupt                                                             */
    NVIC_PERIPH_IRQ_TIM6_DAC           = 54,     /**< TIM6 global and DAC1&3 underrun error  interrupts                                  */
    NVIC_PERIPH_IRQ_TIM7_DAC           = 55,     /**< TIM7 global and DAC2&4 underrun error  interrupts                                  */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL1      = 56,     /**< DMA2 Channel 1 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL2      = 57,     /**< DMA2 Channel 2 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL3      = 58,     /**< DMA2 Channel 3 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL4      = 59,     /**< DMA2 Channel 4 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL5      = 60,     /**< DMA2 Channel 5 global Interrupt                                                    */
    NVIC_PERIPH_IRQ_ADC4               = 61,     /**< ADC4 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_ADC5               = 62,     /**< ADC5 global Interrupt                                                              */
    NVIC_PERIPH_IRQ_UCPD1              = 63,     /**< UCPD global Interrupt                                                              */
    NVIC_PERIPH_IRQ_COMP1_2_3          = 64,     /**< COMP1, COMP2 and COMP3 Interrupts                                                  */
    NVIC_PERIPH_IRQ_COMP4_5_6          = 65,     /**< COMP4, COMP5 and COMP6                                                             */
    NVIC_PERIPH_IRQ_COMP7              = 66,     /**< COMP7 Interrupt                                                                    */
    NVIC_PERIPH_IRQ_HRTIM1_MASTER      = 67,     /**< HRTIM Master Timer global Interrupt                                                */
    NVIC_PERIPH_IRQ_HRTIM1_TIMA        = 68,     /**< HRTIM Timer A global Interrupt                                                     */
    NVIC_PERIPH_IRQ_HRTIM1_TIMB        = 69,     /**< HRTIM Timer B global Interrupt                                                     */
    NVIC_PERIPH_IRQ_HRTIM1_TIMC        = 70,     /**< HRTIM Timer C global Interrupt                                                     */
    NVIC_PERIPH_IRQ_HRTIM1_TIMD        = 71,     /**< HRTIM Timer D global Interrupt                                                     */
    NVIC_PERIPH_IRQ_HRTIM1_TIME        = 72,     /**< HRTIM Timer E global Interrupt                                                     */
    NVIC_PERIPH_IRQ_HRTIM1_FLT         = 73,     /**< HRTIM Fault global Interrupt                                                       */
    NVIC_PERIPH_IRQ_HRTIM1_TIMF        = 74,     /**< HRTIM Timer F global Interrupt                                                     */
    NVIC_PERIPH_IRQ_CRS                = 75,     /**< CRS global interrupt                                                               */
    NVIC_PERIPH_IRQ_SAI1               = 76,     /**< Serial Audio Interface global interrupt                                            */
    NVIC_PERIPH_IRQ_TIM20_BRK          = 77,     /**< TIM20 Break, Transition error and Index error Interrupt                            */
    NVIC_PERIPH_IRQ_TIM20_UP           = 78,     /**< TIM20 Update interrupt                                                             */
    NVIC_PERIPH_IRQ_TIM20_TRG_COM      = 79,     /**< TIM20 Trigger, Commutation, Direction change and Index Interrupt                   */
    NVIC_PERIPH_IRQ_TIM20_CC           = 80,     /**< TIM20 Capture Compare interrupt                                                    */
    NVIC_PERIPH_IRQ_FPU                = 81,     /**< FPU global interrupt                                                               */
    NVIC_PERIPH_IRQ_I2C4_EV            = 82,     /**< I2C4 Event interrupt                                                               */
    NVIC_PERIPH_IRQ_I2C4_ER            = 83,     /**< I2C4 Error interrupt                                                               */
    NVIC_PERIPH_IRQ_SPI4               = 84,     /**< SPI4 Event interrupt                                                               */
    NVIC_PERIPH_IRQ_FDCAN2_IT0         = 86,     /**< FDCAN2 interrupt line 0 interrupt                                                  */
    NVIC_PERIPH_IRQ_FDCAN2_IT1         = 87,     /**< FDCAN2 interrupt line 1 interrupt                                                  */
    NVIC_PERIPH_IRQ_FDCAN3_IT0         = 88,     /**< FDCAN3 interrupt line 0 interrupt                                                  */
    NVIC_PERIPH_IRQ_FDCAN3_IT1         = 89,     /**< FDCAN3 interrupt line 1 interrupt                                                  */
    NVIC_PERIPH_IRQ_RNG                = 90,     /**< RNG global interrupt                                                               */
    NVIC_PERIPH_IRQ_LPUART1            = 91,     /**< LP UART 1 Interrupt                                                                */
    NVIC_PERIPH_IRQ_I2C3_EV            = 92,     /**< I2C3 Event Interrupt                                                               */
    NVIC_PERIPH_IRQ_I2C3_ER            = 93,     /**< I2C3 Error interrupt                                                               */
    NVIC_PERIPH_IRQ_DMAMUX_OVR         = 94,     /**< DMAMUX overrun global interrupt                                                    */
    NVIC_PERIPH_IRQ_QUADSPI            = 95,     /**< QUADSPI interrupt                                                                  */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL8      = 96,     /**< DMA1 Channel 8 interrupt                                                           */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL6      = 97,     /**< DMA2 Channel 6 interrupt                                                           */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL7      = 98,     /**< DMA2 Channel 7 interrupt                                                           */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL8      = 99,     /**< DMA2 Channel 8 interrupt                                                           */
    NVIC_PERIPH_IRQ_CORDIC             = 100,    /**< CORDIC global Interrupt                                                            */
    NVIC_PERIPH_IRQ_FMAC               = 101,    /**< FMAC global Interrupt                                                              */
    NVIC_PERIPH_IRQ_SIZE                         /**< Count of peripheral Interrupts                                                     */
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
    NVIC_CORE_IRQ_SVCALL             = 11u, /**< 11 Cortex-M4 SV Call Interrupt          */
    NVIC_CORE_IRQ_DEBUGMONITOR       = 12u, /**< 12 Cortex-M4 Debug Monitor Interrupt    */
    NVIC_CORE_IRQ_PENDSV             = 13u, /**< 14 Cortex-M4 Pend SV Interrupt          */
    NVIC_CORE_IRQ_SYSTICK            = 14u, /**< 15 Cortex-M4 System Tick Interrupt      */
    NVIC_CORE_IRQ_SIZE                      /**< 16 Count of core Interrupts             */
}   nvic_CoreIrqList_t;

/* ========================== EXPORTED VARIABLES ============================ */

/* ========================= EXPORTED FUNCTIONS ============================= */


#endif /* NVIC_NVIC_TYPES_H */
