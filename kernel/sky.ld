ENTRY(_start)
/*OUTPUT_FORMAT("binary")*/
OUTPUT_ARCH(i386)
SECTIONS{
 . =0x8000;

 .text :{
      *(.text)
       }
 .data :{
      *(.*data)
       }
 .bss :{
     *(.bss)
      }
} 
