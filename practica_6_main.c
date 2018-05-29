#define uC18f4620
#include <18F4620.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)


#int_tmr0
void isr_mostrarDisplay(){
   
}  

void main (void){
   setup_oscillator(OSC_16MHZ | OSC_NORMAL);
   clear_interrupt(INT_RDA);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   while(1){

   }
}	