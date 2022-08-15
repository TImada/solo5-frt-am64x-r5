
/* This is the stack that is used by code running within main()
 * In case of NORTOS,
 * - This means all the code outside of ISR uses this stack
 * In case of FreeRTOS
 * - This means all the code until vTaskStartScheduler() is called in main()
 *   uses this stack.
 * - After vTaskStartScheduler() each task created in FreeRTOS has its own stack
 */
__TI_STACK_SIZE = 16384;
/* This is the heap size for malloc() API in NORTOS and FreeRTOS
 * This is also the heap used by pvPortMalloc in FreeRTOS
 */
__TI_HEAP_SIZE = 32768;
ENTRY(_vectors)  /* This is the entry of the application, _vector MUST be plabed starting address 0x0 */

/* This is the size of stack when R5 is in IRQ mode
 * In NORTOS,
 * - Here interrupt nesting is disabled as of now
 * - This is the stack used by ISRs registered as type IRQ
 * In FreeRTOS,
 * - Here interrupt nesting is enabled
 * - This is stack that is used initally when a IRQ is received
 * - But then the mode is switched to SVC mode and SVC stack is used for all user ISR callbacks
 * - Hence in FreeRTOS, IRQ stack size is less and SVC stack size is more
 */
__IRQ_STACK_SIZE = 256;
/* This is the size of stack when R5 is in IRQ mode
 * - In both NORTOS and FreeRTOS nesting is disabled for FIQ
 */
__FIQ_STACK_SIZE = 256;
__SVC_STACK_SIZE = 4096; /* This is the size of stack when R5 is in SVC mode */
__ABORT_STACK_SIZE = 256;  /* This is the size of stack when R5 is in ABORT mode */
__UNDEFINED_STACK_SIZE = 256;  /* This is the size of stack when R5 is in UNDEF mode */

/* DDR definitions */
DDR_START           = 0x80000000;
DDR_SIZE            = 0x10000000; /* 256MB */
DDR_MAIN_SIZE       = 0x00800000; /*   8MB */

/* MirageOS specific heap */
MIRAGE_HEAP_START   = DDR_START + DDR_MAIN_SIZE;
MIRAGE_HEAP_SIZE    = DDR_SIZE - DDR_MAIN_SIZE ;

/* ICSS firmware size (this must not be changed) */
ICSS_FW_LEN         = 0x1c800;
ICSS_FW_START       = DDR_START + DDR_SIZE; 


MEMORY
{
    R5F_VECS  : ORIGIN = 0x00000000 , LENGTH = 0x00000040
    R5F_TCMA  : ORIGIN = 0x00000040 , LENGTH = 0x00007FC0
    R5F_TCMB0 : ORIGIN = 0x41010000 , LENGTH = 0x00008000

    /* when using multi-core application's i.e more than one R5F/M4F active, make sure
     * this memory does not overlap with other R5F's
     */
    MSRAM     : ORIGIN = 0x70080000 , LENGTH = 0x140000

    /* This section can be used to put XIP section of the application in flash, make sure this does not overlap with
     * other CPUs. Also make sure to add a MPU entry for this section and mark it as cached and code executable
     */
    FLASH     : ORIGIN = 0x60200000 , LENGTH = 0x100000

    /* when using multi-core application's i.e more than one R5F/M4F active, make sure
     * this memory does not overlap with other R5F's
     */
    DDR       : ORIGIN = DDR_START , LENGTH = DDR_MAIN_SIZE
    DDR_ICSS_FW : ORIGIN = ICSS_FW_START , LENGTH = ICSS_FW_LEN
    DDR_ICSS_FW_OVERLAY_MEM : ORIGIN = ICSS_FW_START , LENGTH = ICSS_FW_LEN
    /*
    DDR_ICSS_FW : ORIGIN = (0x80000000 + 0x100000 - ICSS_FW_LEN) , LENGTH = ICSS_FW_LEN
    DDR_ICSS_FW_OVERLAY_MEM : ORIGIN = (0x80000000 + 0x100000 - ICSS_FW_LEN) , LENGTH = ICSS_FW_LEN
    */
    
    /* MirageOS heap region */
    MIRAGE_HEAP : ORIGIN = MIRAGE_HEAP_START, LENGTH = MIRAGE_HEAP_SIZE

    /* shared memory segments */
    /* On R5F,
     * - make sure there is a MPU entry which maps below regions as non-cache
     */
    USER_SHM_MEM            : ORIGIN = 0x701D0000, LENGTH = 0x00004000
    LOG_SHM_MEM             : ORIGIN = 0x701D4000, LENGTH = 0x00004000
    RTOS_NORTOS_IPC_SHM_MEM : ORIGIN = 0x701D8000, LENGTH = 0x00008000

}

SECTIONS
{
    /* This has the R5F entry point and vector table, this MUST be at 0x0 */
    .vectors : ALIGN (8) {}  > R5F_VECS

    /* This has the R5F boot code until MPU is enabled,  this MUST be at a address < 0x80000000
     * i.e this cannot be placed in DDR
     */
    .text.hwi   : ALIGN (8) {} > MSRAM
    .text.cache : ALIGN (8) {} > MSRAM
    .text.mpu   : ALIGN (8) {} > MSRAM
    .text.boot  : ALIGN (8) {} > MSRAM
    .text:abort : ALIGN (8) {} > MSRAM  /* this helps in loading symbols when using XIP mode */

    /* Sections needed for C++ projects */
    .ARM.exidx  : ALIGN (8) {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
     } > MSRAM  /* Needed for C++ exception handling */
    .init_array : ALIGN (8) {} > MSRAM  /* Contains function pointers called before main */
    .fini_array : ALIGN (8) {} > MSRAM  /* Contains function pointers called after main */

    /* This is rest of code. This can be placed in DDR if DDR is available and needed */
    _mem_start = DDR_START;
    _mem_size = DDR_SIZE;
    _stext = _mem_start;
    .interp     : ALIGN (8) {} > DDR
    .text       : ALIGN (8) {} > DDR
    . = ALIGN (8);
    _etext = .;
    .rodata     : ALIGN (8) {} > DDR
    .eh_frame   : ALIGN (8) {} > DDR
    . = ALIGN (8);
    _erodata = .;

    /* This is rest of initialized data. This can be placed in DDR if DDR is available and needed */
    .got : ALIGN (8) {
        *(.got.plt)
        *(.got)
    } > DDR
    .data : ALIGN (8) {
        *(.data)
        *(.data.*)
    } > DDR
    .tdata : ALIGN (8) { } > DDR /* TLS */
    . = ALIGN (8);
    _edata = .;

    /* This is rest of uninitialized data. This can be placed in DDR if DDR is available and needed */
    .tbss : { /* TLS */
        *(.tbss)
        . = ALIGN (8);
    } > DDR
    .bss : {
        __bss_start__ = .;
        __BSS_START = .;
        *(.bss)
        *(.bss.*)
        . = ALIGN (8);
        __BSS_END = .;
        __bss_end__ = .;
        . = ALIGN (8);
    } > DDR
    . = ALIGN (8);
    _ebss = .;

    /* This is where the stacks for different R5F modes go */
    .irqstack : ALIGN(16) {
        __IRQ_STACK_START = .;
        . = . + __IRQ_STACK_SIZE;
        __IRQ_STACK_END = .;
        . = ALIGN (8);
    } > DDR
    .fiqstack : ALIGN(16) {
        __FIQ_STACK_START = .;
        . = . + __FIQ_STACK_SIZE;
        __FIQ_STACK_END = .;
    } > DDR
    .svcstack : ALIGN(16) {
        __SVC_STACK_START = .;
        . = . + __SVC_STACK_SIZE;
        __SVC_STACK_END = .;
    } > DDR
    .abortstack : ALIGN(16) {
        __ABORT_STACK_START = .;
        . = . + __ABORT_STACK_SIZE;
        __ABORT_STACK_END = .;
    } > DDR
    .undefinedstack : ALIGN(16) {
        __UNDEFINED_STACK_START = .;
        . = . + __UNDEFINED_STACK_SIZE;
        __UNDEFINED_STACK_END = .;
    } > DDR

    .stack (NOLOAD) : ALIGN(16) {
        __TI_STACK_BASE = .;
        KEEP(*(.stack))
        . = . + __TI_STACK_SIZE;
        __STACK_END = .;
    } > DDR
    . = ALIGN (8);
    _estack = .;

    .heap (NOLOAD) : {
        end = .;
        KEEP(*(.heap))
        . = . + __TI_HEAP_SIZE;
    } > DDR
    . = ALIGN (8);
    _efheap = .;

    /* General purpose user shared memory, used in some examples */
    .bss.user_shared_mem (NOLOAD) : { KEEP(*(.bss.user_shared_mem)) } > USER_SHM_MEM
    /* this is used when Debug log's to shared memory are enabled, else this is not used */
    .bss.log_shared_mem  (NOLOAD) : { KEEP(*(.bss.log_shared_mem)) } > LOG_SHM_MEM
    /* this is used only when IPC RPMessage is enabled, else this is not used */
    .bss.ipc_vring_mem   (NOLOAD) : { KEEP(*(.bss.ipc_vring_mem)) } > RTOS_NORTOS_IPC_SHM_MEM

    /* Kernel end */
    . = ALIGN (8);
    _ekernel = .;

    .mirage_heap (NOLOAD) : {
        _mirage_heap_start = .;
        KEEP(*(.mirage_heap))
        . = . + MIRAGE_HEAP_SIZE;
        _mirage_heap_end = .;
    } > MIRAGE_HEAP
    . = ALIGN (8);
    _emheap = .;

    .icssfw : {
        *(*RX_PRU_SLICE1_b00_Swt)
        *(*RX_PRU_SLICE0_b00_Swt)
        *(*TX_PRU_SLICE0_b00_DMac)
        *(*RX_PRU_SLICE1_b00_DMac)
        *(*RX_PRU_SLICE0_b00_DMac)
        *(*TX_PRU_SLICE0_b00_Swt)
        *(*TX_PRU_SLICE1_b00_DMac)
        *(*TX_PRU_SLICE1_b00_Swt)
        *(*RTU0_SLICE0_b00_Swt)
        *(*RTU0_SLICE1_b00_Swt)
        *(*RTU0_SLICE0_b00_DMac)
        *(*RTU0_SLICE1_b00_DMac)
    } > DDR_ICSS_FW

    .icss_mem (NOLOAD) : {
        *(*gEnetSoc_icssg1HostPoolMem_1)
        *(*gEnetSoc_icssg1HostQueueMem_1)
        *(*gEnetSoc_icssg1ScratchMem_1)
    } > DDR_ICSS_FW_OVERLAY_MEM

}

/*
NOTE: Below memory is reserved for DMSC usage
 - During Boot till security handoff is complete
   0x701E0000 - 0x701FFFFF (128KB)
 - After "Security Handoff" is complete (i.e at run time)
   0x701FC000 - 0x701FFFFF (16KB)

 Security handoff is complete when this message is sent to the DMSC,
   TISCI_MSG_SEC_HANDOVER

 This should be sent once all cores are loaded and all application
 specific firewall calls are setup.
*/


