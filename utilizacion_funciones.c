#define uC18f4620
#include <18F4620.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)


//se declaran variables globales
unsigned int enteroTemperatura = 0, decimalTemperatura = 0, enteroRH = 0, decimalRH = 0, checksum = 0, lectura = 0;


//especificaciones para el Tmr0
setup_timer_0(RTCC_INTERNAL | RTCC_DIV_128)
//se configura la precarga
set_timer0(65224);

//ISR para mostrar leds
#int_tmr0
void isr_mostrarDisplay(){
   
}  

//se guarda lo que envie el sensor por el puerto RC4
datosDHT = 0xF82.4; 

//función para establecer conexión con el sensor
void establecerConexion(){
   set_tris_a(0x00);
   output_a(0x00);
   delay_ms(18);
   output_a(0xFF);
   delay_ms(30);
   set_tris_a(0xFF);
}

//función para revisar si hay conexion con el sensor
void estadoConexion(){
   lectura = 0;
   delay_ms(40);
   if(datosDHT == 0){
      delay_ms(80);
      if(datosDHT == 1){
         lectura = 1;
         delay_ms(40);
      }
   }
}


//funcion para leer datos del sensor
unsigned int8 leerDHT11(){
   unsigned int8 i, k, datosLeidos = 0;   
   if(Time_out){
      break;
   }
   for(i = 0; i < 8; i++){
      k = 0;
      while(!input(datosDHT)){
         k++;
         if(k > 100){
            Time_out = 1;
            break;
         }
         delay_us(1);
      }
      delay_us(30);
      if(!input(datosDHT)){
         bit_clear(datosLeidos, (7 - i));
      }
      else{
         bit_set(datosLeidos, (7 - i));
         while(input(datosDHT)){
            k++;
            if(k > 100){
               Time_out = 1;
               break;
            }
            delay_us(1);
         }
      }
   }
   return datosLeidos;
}




void main (void){
   setup_oscillator(OSC_16MHZ | OSC_NORMAL);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   while(1){    
      Time_out = 0;
      Start_signal();
      if(estadoConexion()){
         enteroRH = Read_Data();
         decimalRH = Read_Data();
         enteroTemperatura = Read_Data();
         decimalTemperatura = Read_Data();
         checksum = Read_Data();
         if(checksum == (enteroRH + decimalRH + enteroTemperatura + decimalTemperatura)){
                   
         }
      }
   }	
}   