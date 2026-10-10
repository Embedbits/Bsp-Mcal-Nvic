# NVIC MCAL Module

This repository provides the **NVIC (Nested Vectored Interrupt Controller)** MCAL module for STM32 microcontrollers.  
The module is designed to be **branch-specific**, where each branch represents one STM32 family (e.g. L4, F4, U5).  
Switching between branches allows developers to use the same interface across families.

---

## Module Overview

The NVIC MCAL module provides an abstraction layer for configuring and handling interrupts on STM32 MCUs.  
It exposes a unified API for all supported families to:

- Initialize / deinitialize NVIC configuration
- Register and manage interrupt handlers
- Configure priorities
- Control pending and active states
- Support both peripheral interrupts and core system interrupts
- Provide default handler management

---

## Public API

### Module Information
```c
nvic_ModuleVersion_t Nvic_Get_ModuleVersion(void);
```

### Initialization
```c
void Nvic_Init(void);
void Nvic_Deinit(void);
void Nvic_Task(void);
```

### Stack Pointer Access
```c
uint32_t Nvic_Get_StackPointerAddr(void);
```

### Default Handler
```c
nvic_RequestState_t Nvic_Set_DefaultHandler(nvic_IsrCallback_t defaultHandler);
```

### Peripheral IRQ Handling
```c
nvic_RequestState_t Nvic_Set_PeriphIrq_Handler (nvic_PeriphIrqList_t irqId, const nvic_IsrCallback_t irqHandler);
nvic_RequestState_t Nvic_Get_PeriphIrq_Handler (nvic_PeriphIrqList_t irqId, nvic_IsrCallback_t *irqHandler);

nvic_RequestState_t Nvic_Set_PeriphIrq_Prio    (nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t irqPrio);
nvic_RequestState_t Nvic_Get_PeriphIrq_Prio    (nvic_PeriphIrqList_t irqId, nvic_IrqPrio_t *irqPrio);

nvic_RequestState_t Nvic_Set_PeriphIrq_Active  (nvic_PeriphIrqList_t irqId);
nvic_RequestState_t Nvic_Set_PeriphIrq_Inactive(nvic_PeriphIrqList_t irqId);

nvic_RequestState_t Nvic_Set_PeriphIrq_Pending (nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t irqFlag);
nvic_RequestState_t Nvic_Get_PeriphIrq_Pending (nvic_PeriphIrqList_t irqId, nvic_IrqFlag_t *irqFlag);
```

### Core IRQ Handling
```c
nvic_RequestState_t Nvic_Set_CoreIrq_Handler(nvic_CoreIrqList_t irqId, const nvic_IsrCallback_t irqHandler);
nvic_RequestState_t Nvic_Get_CoreIrq_Handler(nvic_CoreIrqList_t irqId, nvic_IsrCallback_t *irqHandler);

nvic_RequestState_t Nvic_Set_CoreIrq_Prio   (nvic_CoreIrqList_t irqId, nvic_IrqPrio_t irqPrio);
nvic_RequestState_t Nvic_Get_CoreIrq_Prio   (nvic_CoreIrqList_t irqId, nvic_IrqPrio_t *irqPrio);
```

### Faults and System Reset
```c
nvic_RequestState_t Nvic_Get_FaultStatus(nvic_FaultStatus_t * const faultStatus); /* CFSR, HFSR, MMFAR, BFAR (read only) */
void                Nvic_Set_SystemReset(void);                                   /* SYSRESETREQ, does not return      */
```

---

## STM32H7 Specifics

- Cortex-M7 core, 4 priority bits, FPv5 double precision FPU enabled by `Nvic_Init`.
- Instruction cache (L1 I-Cache) enabled by `Nvic_Init`, data cache stays disabled (DMA buffers need no cache maintenance).
- Interrupt vector table in RAM (AXI SRAM, VTOR alignment 1024 B) - up to 163 peripheral interrupts (STM32H723 / H725 / H730 / H733 / H735).
- `nvic_PeriphIrqList_t` covers all STM32H7 devices: names are the CMSIS `IRQn_Type` names without the `_IRQn` suffix, interrupts not available on the selected device are not defined. Classic STM32H7 (`STM32H7`) and STM32H7R/S (`STM32H7RS`) have different vector layouts.
- Dual-core STM32H745 / H747 / H755 / H757: the module handles the Cortex-M7 core only.

---

## Branch Strategy

- Each **branch** corresponds to a **specific STM32 family** (e.g. `STM32L4`, `STM32F4`, `STM32U5`).
- The **public API remains consistent** across branches.
- Only the **low-level implementation differs**, ensuring portability of higher layers.

---

## Usage Example

```c
// Example: Setting up IRQ handler for a peripheral
Nvic_Set_PeriphIrq_Handler(UART1_IRQn, MyUartHandler);
Nvic_Set_PeriphIrq_Prio(UART1_IRQn, NVIC_PRIORITY_HIGH);
Nvic_Set_PeriphIrq_Active(UART1_IRQn);
```

---

## 🛠 CMake Integration

1. Include `Nvic_Lib` in your CMake library.
2. Include `Nvic_Port.h` in your project.
3. Link against the Nvic module implementation files.
4. Configure the module as needed for your hardware.

---

## License

This project is licensed under the **Creative Commons Attribution–NonCommercial 4.0 International (CC BY-NC 4.0)**.

You are free to use, modify, and share this work for **non-commercial purposes**, provided appropriate credit is given.

See [LICENSE.md](LICENSE.md) for full terms or visit [creativecommons.org/licenses/by-nc/4.0](https://creativecommons.org/licenses/by-nc/4.0/).

---

## Authors

- **Mr.Nobody** — [embedbits.com](https://embedbits.com)

Contributions are welcome! Please open a pull request.

---

## 🌐 Useful Links

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- [Azure DevOps](https://azure.microsoft.com/en-us/services/devops/)
- [Embedbits Github](https://github.com/Embedbits)
- [CC BY-NC 4.0 License](https://creativecommons.org/licenses/by-nc/4.0/)
