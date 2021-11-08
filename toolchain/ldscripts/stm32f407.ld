 
/**
 * Linker script meant to work with any of the STM32F4XX MCUs
 * Currently not doing anything with the CCM SRAM
 */

MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 1M
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
	ccm (rwx) : ORIGIN = 0x10000000, LENGTH = 64K
}

INCLUDE ./cortex-m-generic.ld
