/**
 * \author Mr.Nobody
 * \file Nvic.c
 * \ingroup Nvic
 * \brief Nested Vector Interrupt Controller (NVIC) module common functionality
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


/** Shift of peripheral IRQ number to index of 32-bit NVIC register (ISER, ICER, ISPR, ICPR) */
#define NVIC_IRQ_REG_IDX_SHIFT       ( 5u )

/** Mask of peripheral IRQ bit position within 32-bit NVIC register */
#define NVIC_IRQ_REG_BIT_MASK        ( 0x1Fu )

/** Single IRQ bit in NVIC register */
#define NVIC_IRQ_REG_BIT             ( 1u )

/** Width of priority field in NVIC IP / SCB SHP registers */
#define NVIC_PRIO_REG_BITS           ( 8u )

/** Implemented priority bits are the most significant bits of priority field */
#define NVIC_PRIO_SHIFT              ( NVIC_PRIO_REG_BITS - NVIC_PRIO_BITS )

/** Highest priority value (lowest urgency) supported by implemented priority bits */
#define NVIC_PRIO_MAX                ( ( 1u << NVIC_PRIO_BITS ) - 1u )

/** Core IRQ enumeration value is exception number decremented by 1 (see \ref nvic_CoreIrqList_t) */
#define NVIC_CORE_IRQ_EXC_OFFSET     ( 1u )

/** Exception number of the first entry of SCB SHP registers (MemManage fault) */
#define NVIC_SHP_FIRST_EXC           ( 4u )

/** Full access value of coprocessor access field (2 bits per coprocessor) */
#define NVIC_CPACR_FULL_ACCESS       ( 3u )

/** Position of CP10 access field in SCB CPACR (FPU) */
#define NVIC_CPACR_CP10_POS          ( 20u )

/** Position of CP11 access field in SCB CPACR (FPU) */
#define NVIC_CPACR_CP11_POS          ( 22u )

/** SCB CPACR value enabling full access to FPU (CP10 and CP11) */
#define NVIC_CPACR_FPU_FULL_ACCESS   ( ( NVIC_CPACR_FULL_ACCESS << NVIC_CPACR_CP10_POS ) | \
                                       ( NVIC_CPACR_FULL_ACCESS << NVIC_CPACR_CP11_POS ) )

/* =============================== MACROS =================================== */

/* ========================== EXPORTED VARIABLES ============================ */

/* =========================== LOCAL VARIABLES ============================== */

/** Stack pointer value (defined in Linker file) */
extern uint32_t _estack;

/** Vector table located at the begin of FLASH memory. Used until VTOR is
 *  switched to RAM table by \ref Nvic_Init, index is exception number. */
__attribute__ ((section(".isr_vector"))) const void* vector_table[] = {
    &_estack,                           /*  0 Initial Stack Pointer */
    StartUp_Handler,                    /*  1 Reset                 */
    Nvic_NMI_DefaultHandler,            /*  2 NMI                   */
    Nvic_HardFault_DefaultHandler,      /*  3 HardFault             */
    Nvic_MemFault_DefaultHandler,       /*  4 MemManage             */
    Nvic_BusFault_DefaultHandler,       /*  5 BusFault              */
    Nvic_UsageFault_DefaultHandler,     /*  6 UsageFault            */
    NVIC_NULL_PTR,                      /*  7 Reserved              */
    NVIC_NULL_PTR,                      /*  8 Reserved              */
    NVIC_NULL_PTR,                      /*  9 Reserved              */
    NVIC_NULL_PTR,                      /* 10 Reserved              */
    Nvic_SvCall_DefaultHandler,         /* 11 SVCall                */
    Nvic_DebugMonitor_DefaultHandler,   /* 12 DebugMonitor          */
    NVIC_NULL_PTR,                      /* 13 Reserved              */
    Nvic_PendSv_DefaultHandler,         /* 14 PendSV                */
    Nvic_SysTick_DefaultHandler,        /* 15 SysTick               */
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
 *
 * Initializes core and peripheral vector tables, switches VTOR to the RAM
 * vector table, configures priority grouping and enables FPU access.
 *
 * \note Cortex-M4 erratum 776924 (device errata ES0430 / ES0431 / ES0523 2.1.2
 *       "VDIV or VSQRT instructions might not complete correctly when very short
 *       ISRs are used"): lazy context save
 *       of the floating point state is disabled (FPCCR.LSPEN = 0, automatic
 *       saving ASPEN kept). FP context of an interrupted FP code is always stacked
 *       - interrupt latency is longer by the FP context stacking.
 */
void Nvic_Init( void )
{
    Nvic_CoreVectTableInit();
    Nvic_PeriphVectTableInit();

    Nvic_Config();

/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && \
    (__FPU_USED == 1)
    SCB->CPACR |= NVIC_CPACR_FPU_FULL_ACCESS;  /* set CP10 and CP11 Full Access */
#endif

#if (__FPU_PRESENT == 1)
    /* Cortex-M4 erratum 776924: VDIV / VSQRT result may be lost with lazy stacking and
       a very short ISR - lazy context save is disabled (harmless if FPU is not used) */
    CLEAR_BIT( FPU->FPCCR, FPU_FPCCR_LSPEN_Msk );
#endif
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
    return;
}


/**
 * \brief Returns address of the interrupt vector table in RAM.
 *
 * The first entry of the table holds the initial stack pointer.
 *
 * \return Address of the RAM vector table (value loaded into SCB VTOR).
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
 * \note The handler is installed directly in the vector table. Cortex-M4 erratum
 *       838869 (ES0430 / ES0431 / ES0523 2.1.3 "Store immediate overlapping exception
 *       return operation might vector to incorrect interrupt"): the handler has to end
 *       with __DSB() - stores are completed before the exception return (all MCAL handlers
 *       do so).
 *
 * \param irqId      [in]: Interrupt vector identification
 * \param irqHandler [in]: Interrupt vector callback routine pointer
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
 * \param irqId       [in]: Interrupt vector identification
 * \param irqHandler [out]: Pointer to store interrupt vector callback routine address. Must not be NULL.
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_PeriphIrq_Handler( nvic_PeriphIrqList_t irqId, nvic_IsrCallback_t *irqHandler )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( ( NVIC_PERIPH_IRQ_SIZE > irqId      ) &&
        ( NVIC_NULL_PTR       != irqHandler )    )
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
 * \param irqId   [in]: Interrupt vector identification
 * \param irqPrio [in]: Interrupt vector priority value (0 - \ref NVIC_PRIO_MAX)
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Prio( nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( ( NVIC_PERIPH_IRQ_SIZE > irqId   ) &&
        ( NVIC_PRIO_MAX       >= irqPrio )    )
    {
        NVIC->IP[ (uint32_t)irqId ] = (uint8_t)( irqPrio << NVIC_PRIO_SHIFT );

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
 * \param irqId    [in]: Interrupt vector identification
 * \param irqPrio [out]: Pointer to store interrupt vector priority. Must not be NULL.
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_PeriphIrq_Prio( nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t *irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( ( NVIC_PERIPH_IRQ_SIZE > irqId   ) &&
        ( NVIC_NULL_PTR       != irqPrio )    )
    {
        *irqPrio = (nvic_IrqPrio_t)( (uint32_t)NVIC->IP[ (uint32_t)irqId ] >> NVIC_PRIO_SHIFT );

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
 * \param irqId [in]: Interrupt vector identification
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Active( nvic_PeriphIrqList_t irqId )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        /* Index of interrupt enable register and bit of the interrupt within it */
        const uint32_t irqEnableRegOffset = ( (uint32_t)irqId ) >> NVIC_IRQ_REG_IDX_SHIFT;
        const uint32_t irqActivationMask  = NVIC_IRQ_REG_BIT << ( ( (uint32_t)irqId ) & NVIC_IRQ_REG_BIT_MASK );

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
 * \param irqId [in]: Interrupt vector identification
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Inactive( nvic_PeriphIrqList_t irqId )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        /* Index of interrupt clear-enable register and bit of the interrupt within it */
        const uint32_t irqDisableRegOffset = ( (uint32_t)irqId ) >> NVIC_IRQ_REG_IDX_SHIFT;
        const uint32_t irqDeactivationMask = NVIC_IRQ_REG_BIT << ( ( (uint32_t)irqId ) & NVIC_IRQ_REG_BIT_MASK );

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
 * \param irqId   [in]: Interrupt vector identification
 * \param irqFlag [in]: Interrupt required pending status
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_PeriphIrq_Pending( nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t irqFlag )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_PERIPH_IRQ_SIZE > irqId )
    {
        const uint32_t irqRegOffset = ( (uint32_t)irqId ) >> NVIC_IRQ_REG_IDX_SHIFT;
        const uint32_t irqMask      = NVIC_IRQ_REG_BIT << ( ( (uint32_t)irqId ) & NVIC_IRQ_REG_BIT_MASK );

        if( NVIC_IRQ_INACTIVE != irqFlag )
        {
            /* Set interrupt vector pending active */
            NVIC->ISPR[ irqRegOffset ] = irqMask;
        }
        else
        {
            /* Clear pending interrupt vector */
            NVIC->ICPR[ irqRegOffset ] = irqMask;
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
 * \param irqId    [in]: Interrupt vector identification
 * \param irqFlag [out]: Pointer to store interrupt pending status. Must not be NULL.
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_PeriphIrq_Pending( nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t *irqFlag )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( ( NVIC_PERIPH_IRQ_SIZE > irqId   ) &&
        ( NVIC_NULL_PTR       != irqFlag )    )
    {
        const uint32_t irqRegOffset = ( (uint32_t)irqId ) >> NVIC_IRQ_REG_IDX_SHIFT;
        const uint32_t irqMask      = NVIC_IRQ_REG_BIT << ( ( (uint32_t)irqId ) & NVIC_IRQ_REG_BIT_MASK );
        const uint32_t regValue     = NVIC->ISPR[ irqRegOffset ] & irqMask;

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
 * \note The handler is installed directly in the vector table. Cortex-M4 erratum
 *       838869: a handler returning from the exception has to end with __DSB().
 *
 * \param irqId      [in]: Interrupt vector identification
 * \param irqHandler [in]: Interrupt vector callback routine pointer
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
 * \param irqId       [in]: Interrupt vector identification
 * \param irqHandler [out]: Pointer to store interrupt vector callback routine address. Must not be NULL.
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_CoreIrq_Handler( nvic_CoreIrqList_t irqId, nvic_IsrCallback_t *irqHandler )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( ( NVIC_CORE_IRQ_SIZE > irqId      ) &&
        ( NVIC_NULL_PTR     != irqHandler )    )
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
 * \note  Only exceptions with configurable priority are accepted
 *        (\ref NVIC_CORE_IRQ_MEMFAULT - \ref NVIC_CORE_IRQ_SYSTICK). Reset, NMI
 *        and HardFault have fixed priority and return error.
 *
 * \param irqId   [in]: Interrupt vector identification
 * \param irqPrio [in]: Interrupt vector priority value (0 - \ref NVIC_PRIO_MAX)
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Set_CoreIrq_Prio( nvic_CoreIrqList_t irqId, nvic_IrqPrio_t irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    /* Reset, NMI and HardFault have fixed priority - not present in SHP registers */
    if( ( NVIC_CORE_IRQ_SIZE     > irqId   ) &&
        ( NVIC_CORE_IRQ_MEMFAULT <= irqId  ) &&
        ( NVIC_PRIO_MAX         >= irqPrio )    )
    {
        const uint32_t shpIdx = ( (uint32_t)irqId + NVIC_CORE_IRQ_EXC_OFFSET ) - NVIC_SHP_FIRST_EXC;

        SCB->SHP[ shpIdx ] = (uint8_t)( irqPrio << NVIC_PRIO_SHIFT );

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
 * \note  Only exceptions with configurable priority are accepted
 *        (\ref NVIC_CORE_IRQ_MEMFAULT - \ref NVIC_CORE_IRQ_SYSTICK).
 *
 * \param irqId    [in]: Interrupt vector identification
 * \param irqPrio [out]: Pointer to store interrupt vector priority. Must not be NULL.
 *
 * \return Processing request state. If request executed successfully returns "OK",
 *         otherwise returns error.
 */
nvic_RequestState_t Nvic_Get_CoreIrq_Prio( nvic_CoreIrqList_t irqId, nvic_IrqPrio_t *irqPrio )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    /* Reset, NMI and HardFault have fixed priority - not present in SHP registers */
    if( ( NVIC_CORE_IRQ_SIZE     > irqId   ) &&
        ( NVIC_CORE_IRQ_MEMFAULT <= irqId  ) &&
        ( NVIC_NULL_PTR         != irqPrio )    )
    {
        const uint32_t shpIdx = ( (uint32_t)irqId + NVIC_CORE_IRQ_EXC_OFFSET ) - NVIC_SHP_FIRST_EXC;

        *irqPrio = (nvic_IrqPrio_t)( (uint32_t)SCB->SHP[ shpIdx ] >> NVIC_PRIO_SHIFT );

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
 * \note The user default handler is called by the default interrupt handlers which
 *       end with __DSB() after it (Cortex-M4 erratum 838869).
 *
 * \param defaultHandler [in]: Pointer to user default handler
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


/**
 * \brief Reads fault status registers of the core.
 *
 * \note  Registers are only read, fault flags are not cleared. Fault address
 *        registers are valid only if the related valid flag is set in CFSR.
 *
 * \param faultStatus [out]: Pointer to store fault status. Must not be NULL.
 *
 * \return Processing request state. Returns \ref NVIC_REQUEST_OK if request
 *         was processed without problems. Otherwise returns \ref NVIC_REQUEST_ERROR.
 */
nvic_RequestState_t Nvic_Get_FaultStatus( nvic_FaultStatus_t * const faultStatus )
{
    nvic_RequestState_t returnState = NVIC_REQUEST_ERROR;

    if( NVIC_NULL_PTR != faultStatus )
    {
        faultStatus->Cfsr  = SCB->CFSR;
        faultStatus->Hfsr  = SCB->HFSR;
        faultStatus->Mmfar = SCB->MMFAR;
        faultStatus->Bfar  = SCB->BFAR;

        returnState = NVIC_REQUEST_OK;
    }
    else
    {
        /* Null pointer assigned */
        returnState = NVIC_REQUEST_ERROR;
    }

    return ( returnState );
}


/**
 * \brief Requests system reset of the MCU (SCB AIRCR SYSRESETREQ).
 *
 * \note  The function does not return. Core and peripherals are reset, content
 *        of SRAM is kept (unless SRAM erase on reset is set by option bytes).
 */
void Nvic_Set_SystemReset( void )
{
    NVIC_SystemReset();
}


/* =========================== LOCAL FUNCTIONS ============================== */

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
    nvic_IrqVectTable.StackPointer                          = stackAddr;
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

        __DSB();    /* Cortex-M4 erratum 838869 (ES0430 / ES0431 / ES0523 2.1.3): stores completed before the exception return */
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

        __DSB();    /* Cortex-M4 erratum 838869 (ES0430 / ES0431 / ES0523 2.1.3): stores completed before the exception return */
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
    __DSB();    /* Cortex-M4 erratum 838869 (ES0430 / ES0431 / ES0523 2.1.3): stores completed before the exception return */
}

/* =========================== INTERRUPT HANDLERS =========================== */

/* ================================ TASKS =================================== */
