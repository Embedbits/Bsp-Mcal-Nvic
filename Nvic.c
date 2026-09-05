/**
 * \author Mr.Nobody
 * \file Nvic.h
 * \ingroup Nvic
 * \brief Nvic module common functionality
 *
 */
/* ============================== INCLUDES ================================== */
#include "Nvic.h"                           /* Self include                   */
#include "Nvic_Port.h"                      /* Own port file include          */
#include "Nvic_Types.h"                     /* Module types definitions       */
#include "StartUp_Port.h"                   /* Startup module functionality   */
#include "Stm32.h"                          /* MCU Registers definitions      */
/* ============================== TYPEDEFS ================================== */

/** Structure type used by interrupt vector configuration */
typedef struct
{
    uint32_t StackPointer;
    nvic_IsrCallback_t CoreIrq[ NVIC_CORE_IRQ_SIZE ];     /**< System core interrupts */
    nvic_IsrCallback_t PeriphIrq[ NVIC_PERIPH_IRQ_SIZE ]; /**< Peripherals interrupts */
}   nvic_IrqVectorTable_t;

/* ======================== FORWARD DECLARATIONS ============================ */

static void Nvic_Config( void );
static void Nvic_PeriphVectTableInit( void );
static void Nvic_CoreVectTableInit( void );
static void Nvic_DefaultPeriphIsr( void );
static void Nvic_DefaultCoreIsr( void );
static void Nvic_NMI_DefaultHandler( void );
static void Nvic_HardFault_DefaultHandler( void );
static void Nvic_MemFault_DefaultHandler( void );
static void Nvic_BusFault_DefaultHandler( void );
static void Nvic_UsageFault_DefaultHandler( void );
static void Nvic_SvCall_DefaultHandler( void );
static void Nvic_DebugMonitor_DefaultHandler( void );
static void Nvic_PendSv_DefaultHandler( void );
static void Nvic_SysTick_DefaultHandler( void );

/* ========================== SYMBOLIC CONSTANTS ============================ */

/** Value of major version of SW module */
#define NVIC_MAJOR_VERSION           ( 1u )

/** Value of minor version of SW module */
#define NVIC_MINOR_VERSION           ( 0u )

/** Value of patch version of SW module */
#define NVIC_PATCH_VERSION           ( 0u )

/* =============================== MACROS =================================== */

/* ========================== EXPORTED VARIABLES ============================ */

/* =========================== LOCAL VARIABLES ============================== */

/** Stack pointer value (defined in Linker file) */
extern uint32_t _estack;

/** Vector table located at the begin of FLASH memory. */
__attribute__ ((section(".isr_vector"))) const void* vector_table[] =
{
    &_estack,              // Initial Stack Pointer
    StartUp_Handler,
    Nvic_NMI_DefaultHandler,
    Nvic_HardFault_DefaultHandler,
    Nvic_MemFault_DefaultHandler,
    Nvic_BusFault_DefaultHandler,
    Nvic_UsageFault_DefaultHandler,
    Nvic_SvCall_DefaultHandler,
    Nvic_DebugMonitor_DefaultHandler,
    Nvic_PendSv_DefaultHandler,
    Nvic_SysTick_DefaultHandler,
};

/** Custom vector table */
__attribute__ ((section ("._irqVectorTable_RAM"), used))
static volatile nvic_IrqVectorTable_t   nvic_IrqVectTable
                                            = { 0u };

static volatile nvic_IsrCallback_t      nvic_DefaultHandler
                                            = NVIC_NULL_PTR;

/* ========================= EXPORTED FUNCTIONS ============================= */

/**
 * \brief Returns module SW version
 *
 * \return Module SW version
 */
nvic_ModuleVersion_t Nvic_Get_ModuleVersion( void )
{
    nvic_ModuleVersion_t retVersion;

    retVersion.Major = NVIC_MAJOR_VERSION;
    retVersion.Minor = NVIC_MINOR_VERSION;
    retVersion.Patch = NVIC_PATCH_VERSION;

    return (retVersion);
}


/**
 * \brief Initializes module Nvic
 *
 * This function shall call every necessary sub-module initialization function 
 * and set up all the necessary resources for the module to work. In case of
 * failure, the function shall handle it by itself and shall not be transferred
 * to AppMain layer.
 */
void Nvic_Init( void )
{
    Nvic_CoreVectTableInit();
    Nvic_PeriphVectTableInit();

    Nvic_Config();
}


/**
 * \brief Deinitializes module Nvic
 *
 * This function shall call every necessary sub-module deinitialization function 
 * and free all the resources allocated by the module. In case of failure, the 
 * function shall handle it by itself and shall not be transferred to AppMain 
 * layer.
 */
void Nvic_Deinit( void )
{
    Nvic_PeriphVectTableInit();
    Nvic_CoreVectTableInit();

    Nvic_Config();
}


/**
 * \brief Main task of module Nvic
 *
 * This function shall be called in the main loop of the application or the task
 * scheduler. It shall be called periodically, depending on the module's 
 * requirements.
 */
void Nvic_Task( void )
{

}


/**
 * \brief Returns address of StackPointer.
 *
 * \return Address of stack pointer as uint32_t
 */
uint32_t Nvic_Get_StackPointerAddr( void )
{
    return ( (uint32_t)&nvic_IrqVectTable );
}


/**
 * \brief Peripheral interrupt vector setter routine
 *
 * User can configure custom handler for required interrupt vector.
 *
 * \param[in] irqId      : Interrupt vector identification
 * \param[in] irqHandler : Interrupt vector callback routine pointer
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Handler( nvic_PeriphIrqList_t irqId, const nvic_IsrCallback_t irqHandler )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( ( NVIC_PERIPH_IRQ_SIZE > irqId      ) &&
        ( NVIC_NULL_PTR       != irqHandler )    )
    {
        nvic_IrqVectTable.PeriphIrq[ irqId ] = irqHandler;

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Peripheral interrupt vector getter routine
 *
 * User can read configured handler address for required interrupt vector.
 *
 * \param[in]  irqId      : Interrupt vector identification
 * \param[out] irqHandler : Interrupt vector callback routine address
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_PeriphIrq_Handler( nvic_PeriphIrqList_t irqId, nvic_IsrCallback_t *irqHandler )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        *irqHandler = nvic_IrqVectTable.PeriphIrq[ irqId ];

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Peripheral interrupt priority setter routine
 *
 * User can configure priority for required interrupt vector.
 *
 * \param[in] irqId   : Interrupt vector identification
 * \param[in] irqPrio : Interrupt vector priority value
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Prio( nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        NVIC->IP[((uint32_t)irqId)] = (uint8_t)((irqPrio << (8u - NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Peripheral interrupt priority getter routine
 *
 * User can read configured priority for required interrupt vector.
 *
 * \param[in]  irqId   : Interrupt vector identification
 * \param[out] irqPrio : Interrupt vector priority
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_PeriphIrq_Prio( nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t *irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        *irqPrio = (((uint32_t)NVIC->IP[((uint32_t)irqId)] >> (8u - NVIC_PRIO_BITS)));

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Peripheral interrupt vector activation routine
 *
 * User can activate interrupt vector through this routine.
 *
 * \param[in] irqId : Interrupt vector identification
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Active( nvic_PeriphIrqList_t irqId )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        /* Down-shift bits of 31 to get interrupt enable register offset */
        uint32_t irqEnableRegOffset = ( (uint32_t) irqId ) >> 5UL;

        uint32_t irqActivationMask = ( uint32_t )( 1u << ( ( ( uint32_t ) irqId ) & 0x1FUL ) );

        __COMPILER_BARRIER();

        NVIC->ISER[ irqEnableRegOffset ] = irqActivationMask;

        __COMPILER_BARRIER();

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Peripheral interrupt vector de-activation routine
 *
 * User can de-activate interrupt vector through this routine.
 *
 * \param[in] irqId : Interrupt vector identification
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Inactive( nvic_PeriphIrqList_t irqId )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        /* Down-shift bits of 31 to get interrupt enable register offset */
        uint32_t irqDisableRegOffset = ( (uint32_t) irqId ) >> 5u;

        uint32_t irqDeactivationMask = ( uint32_t )( 1u << ( ( ( uint32_t ) irqId ) & 0x1 ) );

        NVIC->ICER[ irqDisableRegOffset ] = irqDeactivationMask;

        __DSB();
        __ISB();

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Peripheral interrupt pending bit setter
 *
 * User can activate or deactivate pending state of interrupt vector.
 *
 * \param[in] irqId   : Interrupt vector identification
 * \param[in] irqFlag : Interrupt required pending status
 *
 * \return State of storing interrupt handler pointer into interrupt vector table
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Pending( nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t irqFlag )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        if( NVIC_IRQ_INACTIVE != irqFlag )
        {
            /* Set interrupt vector pending active */
            NVIC->ISPR[(((uint32_t)irqId) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)irqId) & 0x1FUL));
        }
        else
        {
            /* Clear pending interrupt vector */
            NVIC->ICPR[(((uint32_t)irqId) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)irqId) & 0x1FUL));
        }

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Peripheral interrupt pending state getter routine
 *
 * User can read pending state of interrupt vector.
 *
 * \param[in]  irqId   : Interrupt vector identification
 * \param[out] irqFlag : Interrupt pending status
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_PeriphIrq_Pending( nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t *irqFlag )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        uint32_t regValue = NVIC->ISPR[(((uint32_t)irqId) >> 5UL)] & (1UL << (((uint32_t)irqId) & 0x1FUL));

        if( 0u != regValue )
        {
            *irqFlag = NVIC_IRQ_ACTIVE;
        }
        else
        {
            *irqFlag = NVIC_IRQ_INACTIVE;
        }

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Core interrupt vector setter routine
 *
 * User can configure custom handler for required core interrupt vector.
 *
 * \param[in] irqId      : Interrupt vector identification
 * \param[in] irqHandler : Interrupt vector callback routine pointer
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_CoreIrq_Handler( nvic_CoreIrqList_t irqId, const nvic_IsrCallback_t irqHandler )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( ( NVIC_CORE_IRQ_SIZE > irqId        ) &&
        ( NVIC_NULL_PTR       != irqHandler )    )
    {
        nvic_IrqVectTable.CoreIrq[ irqId ] = irqHandler;

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Core interrupt vector getter routine
 *
 * User can read configured handler address for required core interrupt vector.
 *
 * \param[in]  irqId      : Interrupt vector identification
 * \param[out] irqHandler : Interrupt vector callback routine address
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_CoreIrq_Handler( nvic_CoreIrqList_t irqId, nvic_IsrCallback_t *irqHandler )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_CORE_IRQ_SIZE > irqId )
    {
        *irqHandler = nvic_IrqVectTable.CoreIrq[ irqId ];

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Core interrupt priority setter routine
 *
 * User can configure priority for required core interrupt vector.
 *
 * \param[in] irqId   : Interrupt vector identification
 * \param[in] irqPrio : Interrupt vector priority value
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_CoreIrq_Prio( nvic_CoreIrqList_t irqId, nvic_IrqPrio_t irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_CORE_IRQ_SIZE > irqId )
    {
        irqId += 1u;

        SCB->SHP[(((uint32_t)irqId) & 0xFUL)-4UL] = (uint8_t)((irqPrio << (8U - NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Core interrupt priority getter routine
 *
 * User can read configured priority for required core interrupt vector.
 *
 * \param[in]  irqId   : Interrupt vector identification
 * \param[out] irqPrio : Interrupt vector priority
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_CoreIrq_Prio( nvic_CoreIrqList_t irqId, nvic_IrqPrio_t *irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_CORE_IRQ_SIZE > irqId )
    {
        *irqPrio = (uint32_t)SCB->SHP[(((uint32_t)irqId) & 0xFUL)-4UL] >> (8U - NVIC_PRIO_BITS);

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Default handler setter function
 *
 * All interrupt vectors are routed to default handler which is by default
 * represented by endless loop. User can assign custom default handler.
 *
 * \param[in] defaultHandler : Pointer to user default handler
 *
 * \return Assigning state of setter.
 */
nvic_RequestState_t Nvic_Set_DefaultHandler( nvic_IsrCallback_t defaultHandler )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_NULL_PTR != defaultHandler )
    {
        nvic_DefaultHandler = defaultHandler;

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        /* Null pointer assigned */
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}

/* =========================== LOCAL FUNCTIONS ============================== */

/**
  * \brief  Setup the microcontroller system.
  * \retval None
  */
void SystemInit(void)
{
    Nvic_PeriphVectTableInit();
    Nvic_CoreVectTableInit();
    Nvic_Config();

/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 20U)|(3UL << 22U));  /* set CP10 and CP11 Full Access */
#endif
}


/**
 * \brief Interrupt system core configuration routine.
 *
 * Interrupt table is stored within internal memory range and its start address
 * must be stored in VTOR. If interrupt is activated, the offset is calculated
 * and functionality on stored address is executed. Every IRQ take 32 bit space
 * as an address in memory space.
 * Priority grouping shall be configured to customer needs. For our application
 * is basic 4 bits priority used.
 *
 */
static void Nvic_Config( void )
{
    /* Set address of interrupt vector into VTOR */
    SCB->VTOR = (uint32_t)&nvic_IrqVectTable;

    __DSB();

    /* Configure priority grouping. */
    NVIC_SetPriorityGrouping( NVIC_PRIORITYGROUP_4 );
}


/**
 * \brief Initialization of peripheral vector table to default handlers
 *
 * After MCU start-up, vector table shall contain addresses to default (empty)
 * handlers. This addresses are then updated by user configuration.
 *
 */
static void Nvic_PeriphVectTableInit( void )
{
    for( nvic_PeriphIrqList_t intId = 0u; NVIC_PERIPH_IRQ_SIZE > intId; intId ++ )
    {
        nvic_IrqVectTable.PeriphIrq[ intId ] = Nvic_DefaultPeriphIsr;
    }
}


/**
 * \brief Initialization of MCU core vector table to default handlers
 *
 */
static void Nvic_CoreVectTableInit( void )
{
    extern const uint32_t _ram_end;

    uint32_t stackAddr = 0u;

    for( nvic_CoreIrqList_t intId = 0u; NVIC_CORE_IRQ_SIZE > intId; intId ++ )
    {
        nvic_IrqVectTable.CoreIrq[ intId ] = Nvic_DefaultCoreIsr;
    }

    stackAddr = (uint32_t)&_ram_end;

    /* Load initial stack pointer address */
    nvic_IrqVectTable.StackPointer                              = stackAddr;
    /* Store reset handler vector */
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_RESET        ] = StartUp_Handler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_NMI          ] = Nvic_NMI_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_HARDFAULT    ] = Nvic_HardFault_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_MEMFAULT     ] = Nvic_MemFault_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_BUSFAULT     ] = Nvic_BusFault_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_USAGEFAULT   ] = Nvic_UsageFault_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_SVCALL       ] = Nvic_SvCall_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_DEBUGMONITOR ] = Nvic_DebugMonitor_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_PENDSV       ] = Nvic_PendSv_DefaultHandler;
    nvic_IrqVectTable.CoreIrq[ NVIC_CORE_IRQ_SYSTICK      ] = Nvic_SysTick_DefaultHandler;
}


/**
 * \brief Default peripheral interrupt handler.
 *
 * In case the user configured custom default handler, this is executed if
 * un-configured interrupt is called. If the user default handler is not set,
 * infinity loop is called.
 *
 */
static void Nvic_DefaultPeriphIsr( void )
{
    if( NVIC_NULL_PTR != nvic_DefaultHandler )
    {
        nvic_DefaultHandler();
    }
    else
    {
        while( 1u )
        {

        }
    }
}


/**
 * \brief Default core interrupt handler.
 *
 * In case the user configured custom default handler, this is executed if
 * un-configured interrupt is called. If the user default handler is not set,
 * infinity loop is called.
 *
 */
static void Nvic_DefaultCoreIsr( void )
{
    if( NVIC_NULL_PTR != nvic_DefaultHandler )
    {
        nvic_DefaultHandler();
    }
    else
    {
        while( 1u )
        {

        }
    }
}


/**
 * \brief Non-Maskable Interrupt (NMI) default handler
 *
 */
static void Nvic_NMI_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief Hard-Fault Interrupt default handler
 *
 */
static void Nvic_HardFault_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief Memory Fault Interrupt default handler
 *
 */
static void Nvic_MemFault_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief Bus Fault Interrupt default handler
 *
 */
static void Nvic_BusFault_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief Usage Fault Interrupt default handler
 *
 */
static void Nvic_UsageFault_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief Supervisor call interrupt default handler
 *
 */
static void Nvic_SvCall_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief Debugging handler interrupt default handler
 *
 */
static void Nvic_DebugMonitor_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief Pending Supervisor Call (PendSv) interrupt default handler
 *
 */
static void Nvic_PendSv_DefaultHandler( void )
{
    while( 1u )
    {

    }
}


/**
 * \brief SysTick interrupt default handler
 *
 */
static void Nvic_SysTick_DefaultHandler( void )
{

}

/* =========================== INTERRUPT HANDLERS =========================== */

/* ================================ TASKS =================================== */
