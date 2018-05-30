#define uC18f4620 
#include <18F4620.h>
#fuses XT, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, MCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#define datosDHT PIN_C4
#define ledTemperatura PIN_E0
#define ledHumedad PIN_E1
#define seleccion PIN_B0


//se declaran variables globales
unsigned int enteroTemperatura = 0, decimalTemperatura = 0, enteroRH = 0, decimalRH = 0, checksum = 0, lectura = 0;
unsigned int i = 1, Time_out = 0, variableMostrada = 0;
unsigned int numerosDisplay[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
unsigned int mostrarSensado[9] = {0};

//esta función es para saber qué variable se elijio para mostrar --- 
#INT_EXT
void variableSeleccionada_RB(){
   if(INTCON.INTF){
      variableMostrada = 1;
      output_high(ledTemperatura);
   }
   else{
      variableMostrada = 0; 
      output_high(ledHumedad);   
   }
   clear_interrupt(INT_EXT);
}

//especificaciones para el Tmr0

//se configura la precarga


//ISR para mostrar leds
#INT_TIMER0
void isr_mostrarDisplay(){
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_128);
   set_timer0(65224);
   int transistorEncendido = 0, i = 0;
   numerosDisplay[mostrarSensado[i]];
   i *= 2;
   if(i > 8){
      i = 1;
   }
   if(transistorEncendido > 3){
      transistorEncendido = 0;
   }
   output_a(0x01) >> transistorEncendido;
   transistorEncendido++;
   set_timer0(65224);  
}


//función para establecer conexión con el sensor
void establecerConexion(){
   set_tris_c(0x00);
   output_c(0x00);
   delay_ms(18);
   output_c(0xFF);
   delay_ms(30);
   set_tris_c(0xFF);
}

//función para revisar si hay conexion con el sensor
int estadoConexion(){
   lectura = 0;
   delay_ms(40);
   if(input(datosDHT == 0)){
      delay_ms(80);
      if(input(datosDHT == 1)){
         lectura = 1;
         delay_ms(40);
      }
   }
   return lectura;
}


//funcion para leer datos del sensor 
unsigned int8 leerDHT(){
   unsigned int8 i, k, datosLeidos = 0;   
   if(Time_out){
      break;
   }
   for(i = 0; i < 8; i++){
      k = 0;
      while(input(!datosDHT)){
         k++;
         if(k > 100){
            Time_out = 1;
            break;
         }
         delay_us(1);
      }
      delay_us(30);
      if(input(!datosDHT)){
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
   ext_int_edge(H_TO_L);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
   set_tris_a(0x00);
   set_tris_e(0x00);
   set_tris_b(0xFF);
   set_tris_c(0xFF);
   set_tris_d(0x00);
   while(1){    
      Time_out = 0;
      establecerConexion();
      if(estadoConexion()){
         enteroRH = leerDHT();
         decimalRH = leerDHT();
         enteroTemperatura = leerDHT();
         decimalTemperatura = leerDHT();
         checksum = leerDHT();
         //reviso que los datos recaudados sean corrector
         if(checksum == (enteroRH + decimalRH + enteroTemperatura + decimalTemperatura)){
          //si sí lo son, pregunto qué variable se quiere mostrar, temperatura o humedad
          //guardo los números de manera adecuada corregir esto
            if(variableMostrada == 1){
               mostrarSensado[1] = enteroTemperatura / 10;
               mostrarSensado[2] = enteroTemperatura % 10;
               mostrarSensado[4] = decimalTemperatura / 10;
               mostrarSensado[8] = decimalTemperatura % 10;
            }
            else{
               mostrarSensado[1] = enteroRH / 10;
               mostrarSensado[2] = enteroRH % 10;
               mostrarSensado[4] = decimalRH / 10;
               mostrarSensado[8] = decimalRH % 10;
            }          
         }
      }
   }  
}   

