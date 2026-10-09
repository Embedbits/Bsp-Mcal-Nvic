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

/** STM32G4 uses 4 Bits for the Priority Levels */
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
 * \ref NVIC_PRIO_BITS For STM32G4 is the range 0-15
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
 * vector positions (TIM7 / TIM7_DAC, COMP4 / COMP4_5_6 on different STM32G4
 * devices) are named according to the selected device.
 */
typedef enum
{
    NVIC_PERIPH_IRQ_WWDG               =   0u, /**< Window WatchDog Interrupt                                                              */
    NVIC_PERIPH_IRQ_PVD_PVM            =   1u, /**< PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection Interrupts                         */
    NVIC_PERIPH_IRQ_RTC_TAMP_LSECSS    =   2u, /**< RTC Tamper and TimeStamp and RCC LSE CSS interrupts through the EXTI                   */
    NVIC_PERIPH_IRQ_RTC_WKUP           =   3u, /**< RTC Wakeup interrupt through the EXTI line                                             */
    NVIC_PERIPH_IRQ_FLASH              =   4u, /**< FLASH global Interrupt                                                                 */
    NVIC_PERIPH_IRQ_RCC                =   5u, /**< RCC global Interrupt                                                                   */
    NVIC_PERIPH_IRQ_EXTI0              =   6u, /**< EXTI Line0 Interrupt                                                                   */
    NVIC_PERIPH_IRQ_EXTI1              =   7u, /**< EXTI Line1 Interrupt                                                                   */
    NVIC_PERIPH_IRQ_EXTI2              =   8u, /**< EXTI Line2 Interrupt                                                                   */
    NVIC_PERIPH_IRQ_EXTI3              =   9u, /**< EXTI Line3 Interrupt                                                                   */
    NVIC_PERIPH_IRQ_EXTI4              =  10u, /**< EXTI Line4 Interrupt                                                                   */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL1      =  11u, /**< DMA1 Channel 1 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL2      =  12u, /**< DMA1 Channel 2 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL3      =  13u, /**< DMA1 Channel 3 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL4      =  14u, /**< DMA1 Channel 4 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL5      =  15u, /**< DMA1 Channel 5 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA1_CHANNEL6      =  16u, /**< DMA1 Channel 6 global Interrupt                                                        */
#if defined(DMA1_Channel7)
    NVIC_PERIPH_IRQ_DMA1_CHANNEL7      =  17u, /**< DMA1 Channel 7 global Interrupt                                                        */
#endif
    NVIC_PERIPH_IRQ_ADC1_2             =  18u, /**< ADC1 and ADC2 global Interrupt                                                         */
#if defined(USB)
    NVIC_PERIPH_IRQ_USB_HP             =  19u, /**< USB HP Interrupt                                                                       */
    NVIC_PERIPH_IRQ_USB_LP             =  20u, /**< USB LP  Interrupt                                                                      */
#endif
    NVIC_PERIPH_IRQ_FDCAN1_IT0         =  21u, /**< FDCAN1 IT0 Interrupt                                                                   */
    NVIC_PERIPH_IRQ_FDCAN1_IT1         =  22u, /**< FDCAN1 IT1 Interrupt                                                                   */
    NVIC_PERIPH_IRQ_EXTI9_5            =  23u, /**< External Line[9:5] Interrupts                                                          */
    NVIC_PERIPH_IRQ_TIM1_BRK_TIM15     =  24u, /**< TIM1 Break, Transition error, Index error and TIM15 global interrupt                   */
    NVIC_PERIPH_IRQ_TIM1_UP_TIM16      =  25u, /**< TIM1 Update Interrupt and TIM16 global interrupt                                       */
    NVIC_PERIPH_IRQ_TIM1_TRG_COM_TIM17 =  26u, /**< TIM1 TIM1 Trigger, Commutation, Direction change, Index and TIM17 global interrupt     */
    NVIC_PERIPH_IRQ_TIM1_CC            =  27u, /**< TIM1 Capture Compare Interrupt                                                         */
    NVIC_PERIPH_IRQ_TIM2               =  28u, /**< TIM2 global Interrupt                                                                  */
    NVIC_PERIPH_IRQ_TIM3               =  29u, /**< TIM3 global Interrupt                                                                  */
    NVIC_PERIPH_IRQ_TIM4               =  30u, /**< TIM4 global Interrupt                                                                  */
    NVIC_PERIPH_IRQ_I2C1_EV            =  31u, /**< I2C1 Event Interrupt                                                                   */
    NVIC_PERIPH_IRQ_I2C1_ER            =  32u, /**< I2C1 Error Interrupt                                                                   */
    NVIC_PERIPH_IRQ_I2C2_EV            =  33u, /**< I2C2 Event Interrupt                                                                   */
    NVIC_PERIPH_IRQ_I2C2_ER            =  34u, /**< I2C2 Error Interrupt                                                                   */
    NVIC_PERIPH_IRQ_SPI1               =  35u, /**< SPI1 global Interrupt                                                                  */
    NVIC_PERIPH_IRQ_SPI2               =  36u, /**< SPI2 global Interrupt                                                                  */
    NVIC_PERIPH_IRQ_USART1             =  37u, /**< USART1 global Interrupt                                                                */
    NVIC_PERIPH_IRQ_USART2             =  38u, /**< USART2 global Interrupt                                                                */
#if defined(USART3)
    NVIC_PERIPH_IRQ_USART3             =  39u, /**< USART3 global Interrupt                                                                */
#endif
    NVIC_PERIPH_IRQ_EXTI15_10          =  40u, /**< External Line[15:10] Interrupts                                                        */
    NVIC_PERIPH_IRQ_RTC_ALARM          =  41u, /**< RTC Alarm (A and B) through EXTI Line Interrupt                                        */
#if defined(USB)
    NVIC_PERIPH_IRQ_USBWAKEUP          =  42u, /**< USB Wakeup through EXTI line Interrupt                                                 */
#endif
    NVIC_PERIPH_IRQ_TIM8_BRK           =  43u, /**< TIM8 Break, Transition error and Index error Interrupt                                 */
    NVIC_PERIPH_IRQ_TIM8_UP            =  44u, /**< TIM8 Update Interrupt                                                                  */
    NVIC_PERIPH_IRQ_TIM8_TRG_COM       =  45u, /**< TIM8 Trigger, Commutation, Direction change and Index Interrupt                        */
    NVIC_PERIPH_IRQ_TIM8_CC            =  46u, /**< TIM8 Capture Compare Interrupt                                                         */
#if defined(ADC3)
    NVIC_PERIPH_IRQ_ADC3               =  47u, /**< ADC3 global  Interrupt                                                                 */
#endif
#if defined(RCC_AHB3ENR_FMCEN)
    NVIC_PERIPH_IRQ_FMC                =  48u, /**< FMC global Interrupt                                                                   */
#endif
    NVIC_PERIPH_IRQ_LPTIM1             =  49u, /**< LP TIM1 Interrupt                                                                      */
#if defined(TIM5)
    NVIC_PERIPH_IRQ_TIM5               =  50u, /**< TIM5 global Interrupt                                                                  */
#endif
#if defined(SPI3)
    NVIC_PERIPH_IRQ_SPI3               =  51u, /**< SPI3 global Interrupt                                                                  */
#endif
#if defined(UART4)
    NVIC_PERIPH_IRQ_UART4              =  52u, /**< UART4 global Interrupt                                                                 */
#endif
#if defined(UART5)
    NVIC_PERIPH_IRQ_UART5              =  53u, /**< UART5 global Interrupt                                                                 */
#endif
    NVIC_PERIPH_IRQ_TIM6_DAC           =  54u, /**< TIM6 global and DAC1&3 underrun error  interrupts                                      */
#if defined(STM32GBK1CB) || \
    defined(STM32G411xB) || \
    defined(STM32G411xC) || \
    defined(STM32G431xx) || \
    defined(STM32G441xx) || \
    defined(STM32G471xx) || \
    defined(STM32G491xx) || \
    defined(STM32G4A1xx)
    NVIC_PERIPH_IRQ_TIM7               =  55u, /**< TIM7 global interrupts                                                                 */
#endif
#if defined(STM32G414xx) || \
    defined(STM32G473xx) || \
    defined(STM32G474xx) || \
    defined(STM32G483xx) || \
    defined(STM32G484xx)
    NVIC_PERIPH_IRQ_TIM7_DAC           =  55u, /**< TIM7 global and DAC2&4 underrun error  interrupts                                      */
#endif
    NVIC_PERIPH_IRQ_DMA2_CHANNEL1      =  56u, /**< DMA2 Channel 1 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL2      =  57u, /**< DMA2 Channel 2 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL3      =  58u, /**< DMA2 Channel 3 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL4      =  59u, /**< DMA2 Channel 4 global Interrupt                                                        */
    NVIC_PERIPH_IRQ_DMA2_CHANNEL5      =  60u, /**< DMA2 Channel 5 global Interrupt                                                        */
#if defined(ADC4)
    NVIC_PERIPH_IRQ_ADC4               =  61u, /**< ADC4 global Interrupt                                                                  */
#endif
#if defined(ADC5)
    NVIC_PERIPH_IRQ_ADC5               =  62u, /**< ADC5 global Interrupt                                                                  */
#endif
#if defined(UCPD1)
    NVIC_PERIPH_IRQ_UCPD1              =  63u, /**< UCPD global Interrupt                                                                  */
#endif
    NVIC_PERIPH_IRQ_COMP1_2_3          =  64u, /**< COMP1, COMP2 and COMP3 Interrupts                                                      */
#if defined(STM32GBK1CB) || \
    defined(STM32G411xB) || \
    defined(STM32G411xC) || \
    defined(STM32G431xx) || \
    defined(STM32G441xx) || \
    defined(STM32G471xx) || \
    defined(STM32G491xx) || \
    defined(STM32G4A1xx)
    NVIC_PERIPH_IRQ_COMP4              =  65u, /**< COMP4                                                                                  */
#endif
#if defined(STM32G414xx) || \
    defined(STM32G473xx) || \
    defined(STM32G474xx) || \
    defined(STM32G483xx) || \
    defined(STM32G484xx)
    NVIC_PERIPH_IRQ_COMP4_5_6          =  65u, /**< COMP4, COMP5 and COMP6                                                                 */
#endif
#if defined(COMP7)
    NVIC_PERIPH_IRQ_COMP7              =  66u, /**< COMP7 Interrupt                                                                        */
#endif
#if defined(HRTIM1)
    NVIC_PERIPH_IRQ_HRTIM1_MASTER      =  67u, /**< HRTIM Master Timer global Interrupt                                                    */
#endif
#if defined(HRTIM1_TIMA)
    NVIC_PERIPH_IRQ_HRTIM1_TIMA        =  68u, /**< HRTIM Timer A global Interrupt                                                         */
#endif
#if defined(HRTIM1_TIMB)
    NVIC_PERIPH_IRQ_HRTIM1_TIMB        =  69u, /**< HRTIM Timer B global Interrupt                                                         */
#endif
#if defined(HRTIM1_TIMC)
    NVIC_PERIPH_IRQ_HRTIM1_TIMC        =  70u, /**< HRTIM Timer C global Interrupt                                                         */
#endif
#if defined(HRTIM1_TIMD)
    NVIC_PERIPH_IRQ_HRTIM1_TIMD        =  71u, /**< HRTIM Timer D global Interrupt                                                         */
#endif
#if defined(HRTIM1_TIME)
    NVIC_PERIPH_IRQ_HRTIM1_TIME        =  72u, /**< HRTIM Timer E global Interrupt                                                         */
#endif
#if defined(HRTIM1)
    NVIC_PERIPH_IRQ_HRTIM1_FLT         =  73u, /**< HRTIM Fault global Interrupt                                                           */
#endif
#if defined(HRTIM1_TIMF)
    NVIC_PERIPH_IRQ_HRTIM1_TIMF        =  74u, /**< HRTIM Timer F global Interrupt                                                         */
#endif
    NVIC_PERIPH_IRQ_CRS                =  75u, /**< CRS global interrupt                                                                   */
#if defined(SAI1)
    NVIC_PERIPH_IRQ_SAI1               =  76u, /**< Serial Audio Interface global interrupt                                                */
#endif
#if defined(TIM20)
    NVIC_PERIPH_IRQ_TIM20_BRK          =  77u, /**< TIM20 Break, Transition error and Index error Interrupt                                */
    NVIC_PERIPH_IRQ_TIM20_UP           =  78u, /**< TIM20 Update interrupt                                                                 */
    NVIC_PERIPH_IRQ_TIM20_TRG_COM      =  79u, /**< TIM20 Trigger, Commutation, Direction change and Index Interrupt                       */
    NVIC_PERIPH_IRQ_TIM20_CC           =  80u, /**< TIM20 Capture Compare interrupt                                                        */
#endif
    NVIC_PERIPH_IRQ_FPU                =  81u, /**< FPU global interrupt                                                                   */
#if defined(I2C4)
    NVIC_PERIPH_IRQ_I2C4_EV            =  82u, /**< I2C4 Event interrupt                                                                   */
    NVIC_PERIPH_IRQ_I2C4_ER            =  83u, /**< I2C4 Error interrupt                                                                   */
#endif
#if defined(SPI4)
    NVIC_PERIPH_IRQ_SPI4               =  84u, /**< SPI4 Event interrupt                                                                   */
#endif
#if defined(AES)
    NVIC_PERIPH_IRQ_AES                =  85u, /**< AES global interrupt                                                                   */
#endif
#if defined(FDCAN2)
    NVIC_PERIPH_IRQ_FDCAN2_IT0         =  86u, /**< FDCAN2 interrupt line 0 interrupt                                                      */
    NVIC_PERIPH_IRQ_FDCAN2_IT1         =  87u, /**< FDCAN2 interrupt line 1 interrupt                                                      */
#endif
#if defined(FDCAN3)
    NVIC_PERIPH_IRQ_FDCAN3_IT0         =  88u, /**< FDCAN3 interrupt line 0 interrupt                                                      */
    NVIC_PERIPH_IRQ_FDCAN3_IT1         =  89u, /**< FDCAN3 interrupt line 1 interrupt                                                      */
#endif
    NVIC_PERIPH_IRQ_RNG                =  90u, /**< RNG global interrupt                                                                   */
    NVIC_PERIPH_IRQ_LPUART1            =  91u, /**< LP UART 1 Interrupt                                                                    */
#if defined(I2C3)
    NVIC_PERIPH_IRQ_I2C3_EV            =  92u, /**< I2C3 Event Interrupt                                                                   */
    NVIC_PERIPH_IRQ_I2C3_ER            =  93u, /**< I2C3 Error interrupt                                                                   */
#endif
    NVIC_PERIPH_IRQ_DMAMUX_OVR         =  94u, /**< DMAMUX overrun global interrupt                                                        */
#if defined(QUADSPI)
    NVIC_PERIPH_IRQ_QUADSPI            =  95u, /**< QUADSPI interrupt                                                                      */
#endif
#if defined(DMA1_Channel8)
    NVIC_PERIPH_IRQ_DMA1_CHANNEL8      =  96u, /**< DMA1 Channel 8 interrupt                                                               */
#endif
    NVIC_PERIPH_IRQ_DMA2_CHANNEL6      =  97u, /**< DMA2 Channel 6 interrupt                                                               */
#if defined(DMA2_Channel7)
    NVIC_PERIPH_IRQ_DMA2_CHANNEL7      =  98u, /**< DMA2 Channel 7 interrupt                                                               */
#endif
#if defined(DMA2_Channel8)
    NVIC_PERIPH_IRQ_DMA2_CHANNEL8      =  99u, /**< DMA2 Channel 8 interrupt                                                               */
#endif
    NVIC_PERIPH_IRQ_CORDIC             = 100u, /**< CORDIC global Interrupt                                                                */
    NVIC_PERIPH_IRQ_FMAC               = 101u, /**< FMAC global Interrupt                                                                  */
    NVIC_PERIPH_IRQ_SIZE                          /**< Count of peripheral Interrupts (FMAC_IRQn + 1)              */
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
