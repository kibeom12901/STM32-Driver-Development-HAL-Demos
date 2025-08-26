/* File: STM32F407VGTx_FLASH.ld */
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 192K
}

SECTIONS
{
  .isr_vector : { KEEP(*(.isr_vector)) } > FLASH

  .text :
  {
    *(.text*) *(.rodata*) *(.glue_7) *(.glue_7t)
    KEEP(*(.init)) KEEP(*(.fini))
  } > FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > FLASH
  .ARM.exidx   : { *(.ARM.exidx*) } > FLASH

  .eh_frame : { *(.eh_frame*) } > FLASH

  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    _sdata = .; *(.data*)  _edata = .;
  } > SRAM

  .bss :
  {
    _sbss = .; *(.bss*) *(COMMON) _ebss = .;
  } > SRAM

  _estack = ORIGIN(SRAM) + LENGTH(SRAM);
}
