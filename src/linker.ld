ENTRY(_start_)
OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 1M;             /*kernel will be loaded in 1MB adress*/
    .text : 
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}
