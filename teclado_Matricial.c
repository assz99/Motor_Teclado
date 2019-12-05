
//#include "C:\Alberto\IFMT 2017 - I\Microcontroladores\Projetos2017 I\Teclado2017_I\teclado2017A.h"
#include <16F877A.h>
#device adc=10
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOPUT                    //No Power Up Timer
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOCPD                    //No EE protection
#FUSES NOWRT                    //Program memory not write protected
#use delay(clock=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)
#include "key_awm4x4.c"

void main()
{
   char k;
   
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   delay_ms(50);
   
   kbd_init();
   printf("Inicializando");
   delay_ms(2000);
   
   while(TRUE)
   {
      k = kbd_getc();
      
      
      if(k != 0)
      {
         printf("Tecla Clikada:");
         putc(k);
         putc('\n');
         delay_ms(500);
      }

      switch(k)
      {
         case '1':
         output_high(pin_D7);
         output_low(pin_D6);
         output_low(pin_D5);
         break;

         case '5':
         output_high(pin_D6);
         output_low(pin_D7);
         output_low(pin_D5);
         break;

         case '9':
         output_high(pin_D5);
         output_low(pin_D6);
         output_low(pin_D7);
         break;

         case 'D':
         output_low(pin_D5);
         output_low(pin_D6);
         output_low(pin_D7);
         break;
      }
   }
}

