#define uC18f4620 
#include <18F4620.h>
#fuses HS, NOFCMEN, NOIESO, PUT, NOBROWNOUT, NOWDT
#fuses NOPBADEN, NOMCLR, STVREN, NOLVP, NODEBUG
#use delay(clock=16000000)
#define ledTemperatura PIN_E0
#define ledHumedad PIN_E1
#define seleccion PIN_B0
#define datosDHT PIN_C4

#BIT setTrisC = 0xF94.4

//se declaran variables globales
int8 enteroTemperatura = 0, decimalTemperatura = 0, enteroRH = 0, decimalRH = 0, checksum = 0, lectura = 0;
unsigned int variableMostrada = 0;
unsigned int numerosDisplay[10] = {126, 48, 109, 121, 51, 91, 95, 112, 127, 123};
unsigned int numerosDisplayPunto[10] = {254, 176, 237, 249, 179, 219, 223, 240, 255, 251};
unsigned int mostrarSensado[9] = {0};
unsigned int indiceDisplay = 1, contadorPushButtn = 1;

#INT_TIMER0
void isr_mostrarDisplay(){
   indiceDisplay *= 2;
   if(indiceDisplay > 8){
        indiceDisplay = 1;
   }
   if(indiceDisplay == 2){
      output_a(indiceDisplay);
      output_d(numerosDisplayPunto[mostrarSensado[indiceDisplay]]);
   }else if(indiceDisplay != 2){
      output_a(indiceDisplay);
      output_d(numerosDisplay[mostrarSensado[indiceDisplay]]);
   }
}

#INT_EXT
void variableSeleccionada_RB(){
   if(seleccion){
      contadorPushButtn++;   
   }
}

int establecerConexion(){
   setTrisC = 0;
   output_low(datosDHT);
   delay_ms(18);
   output_high(datosDHT);
   delay_us(30);
   setTrisC = 1;
   lectura = 0;
   delay_us(40);
   if(input(datosDHT) == 0){
      delay_us(80);
      if(input(datosDHT) == 1){
         delay_us(40);
         lectura = 1;
         
      }
   }
   return lectura;
}

unsigned int8 leerDHT(){
   unsigned int8 i, k, datosLeidos = 0;
   disable_interrupts(GLOBAL);   
   for(i = 0; i < 8; i++){
      k = 0;
      while(!input(datosDHT)){
         k++;
         if(k > 100){
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
               break;
            }
            delay_us(1);
         }
      }
   }
   enable_interrupts(GLOBAL);
   return datosLeidos;
}
void main (void){
   setup_oscillator(OSC_16MHZ);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64|RTCC_8_BIT);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_EXT);
   enable_interrupts(GLOBAL);
   set_tris_a(0x00);
   set_tris_e(0x00);
   set_tris_b(0xFF);
   set_tris_d(0x00);   
   while(1){
      if(contadorPushButtn > 2){
         contadorPushButtn = 1;
      }
      if(contadorPushButtn % 2 != 0){
         variableMostrada = 1;
         output_low(ledHumedad);
         output_high(ledTemperatura);
      }
      else{
         variableMostrada = 0;  
         output_low(ledTemperatura);
         output_high(ledHumedad); 
      }      
      if(establecerConexion()){
         enteroRH = leerDHT();
         decimalRH = leerDHT();
         enteroTemperatura = leerDHT();
         decimalTemperatura = leerDHT();
         checksum = leerDHT();
         if(checksum == (enteroRH + decimalRH + enteroTemperatura + decimalTemperatura)){
            if(variableMostrada == 1){
               mostrarSensado[1] = enteroTemperatura / 10;
               mostrarSensado[2] = enteroTemperatura % 10;
               mostrarSensado[4] = decimalTemperatura / 10;
               mostrarSensado[8] = decimalTemperatura % 10;               
            }
            else if(variableMostrada == 0){ 
               mostrarSensado[1] = enteroRH / 10;
               mostrarSensado[2] = enteroRH % 10;
               mostrarSensado[4] = decimalRH / 10;
               mostrarSensado[8] = decimalRH % 10;
            }
         }
      }    
   }  
}  

