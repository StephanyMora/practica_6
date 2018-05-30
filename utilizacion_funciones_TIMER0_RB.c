#define uC18f4620
#include <18F4620.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)


//se declaran variables globales
unsigned int enteroTemperatura = 0, decimalTemperatura = 0, enteroRH = 0, decimalRH = 0, checksum = 0, lectura = 0;
unsigned int i = 1;
bool mostrarTemperatura = false, mostrarHumedad = false;


//esta función es para saber qué variable se elijio para mostrar --- TERMINALRLA
#INT_RB
void variableSeleccionada(){
   if(input()){
      mostrarTemperatura = true;
   }
   else if(input()){
      mostrarHumedad = true;    
   }
}

//especificaciones para el Tmr0
setup_timer_0(RTCC_INTERNAL | RTCC_DIV_128)
//se configura la precarga
set_timer0(65224);

//ISR para mostrar leds
#INT_TIMER0
void isr_mostrarDisplay(){
   int numerosDisplay[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
   int mostrarSensado[9] = {0};
   numerosDisplay[mostrarSensado[i]];
   output_
   i *= 2;
   if(i > 8){
      i = 1;
   }
   //Falta seleccionar el transistor a encender
   set_timer0(65224);   
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


//funcion para leer datos del sensor -- CREO QUE ESTA FUNCIÓN ESTÁ MAL
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
   unsigned int variableSeleccionada =  ;  
   setup_oscillator(OSC_16MHZ | OSC_NORMAL);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_RB);
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
         //reviso que los datos recaudados sean corrector
         if(checksum == (enteroRH + decimalRH + enteroTemperatura + decimalTemperatura)){
          //si sí lo son, pregunto qué variable se quiere mostrar, temperatura o humedad
          //guardo los números de manera adecuada corregir esto
            if(mostrarTemperatura == true){
               mostrarSensado[1] = enteroTemperaturan / 10;
               mostrarSensado[2] = enteroTemperatura % 10;
               mostrarSensado[4] = decimalTemperatura / 10;
               mostrarSensado[8] = decimalTemperatura % 10;
            }
            else if(mostrarHumedad == true){
               mostrarSensado[1] = enteroRH / 10;
               mostrarSensado[2] = enteroRH % 10;
               mostrarSensado[4] = decimalRH / 10;
               mostrarSensado[8] = decimalRH % 10;
            }          
         }
      }
   }	
}   