
//#include <Teclado_motor.h>
#include <18F452.h>
#device ADC=10
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#use delay(crystal=20000000)


// Include I2C_LCD driver source code


#use I2C(master, I2C1, FAST = 100000, STREAM = I2C_LCD)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)
#include <I2C_LCD.c>
#include <Teclado4x4.h>
#INCLUDE <stdlib.h>
static int m[8];   
static int n[8];
int flag = 1,count = 0;
int vel_dc =5,i=0,vel_step= 5, p;
char k;
boolean step_ON =false;
boolean dc_ON = false;
boolean step_horario = true;

void lcd(){
if(step_ON == true){
 LCD_Goto(11, 1);
printf(lcd_out,"1");     // Print i with 3 numbers max
}else{
LCD_Goto(11, 1);
printf(lcd_out,"0");  
}
if(dc_ON == true){
LCD_Goto(10, 2);
printf(lcd_out,"1");
}else{
LCD_Goto(10, 2);
printf(lcd_out,"0");
}

LCD_Goto(16, 1);
printf(lcd_out,"%d",vel_step);
LCD_Goto(16, 2);
printf(lcd_out,"%d",vel_dc-1);

}

#INT_RTCC
void  RTCC_isr(void) 
{  

   if (count < flag){
      count++;
         
      if(step_horario ==true){
   
         if(i<4){
            output_d(m[i]);
            i++;
         }else{
            i=0;
         }
      }else if(step_horario == false){
         if(i<4){
            output_d(n[i]);
            i++;
      }else{
         i = 0;
         }
      }
   }else{
   count = 0;
   }
   
   
}

void motor_step(){

if(step_on == false){
   output_d(0b00000000);
   fprintf(PORT1,"Desligou motor step\n");
   lcd();
   return;  
   }
   //fprintf(PORT1,"Entrou motor step\n");
   switch(vel_step)
   {
      case 1:
      flag = 1;
      
      break;
      
      case 2:
      flag = 3;
      ;
      break;

      case 3:
      flag = 5;
      
      break;

      case 4:
      flag = 25;
      
      break;

      case 5:
      flag = 50;
      p=1;
      break;
      lcd();
           
   }
   //fprintf(PORT1,"valor da flag=%u\n",flag);   
lcd();
}


void motor_dc()
{
   if(dc_on == false){
   set_pwm1_duty(0);
   set_pwm2_duty(0);
   lcd();
   return;
   }
   switch(vel_dc)
   {
      case 1:
      set_pwm1_duty(255);
      set_pwm2_duty(0);
      break;

      case 2:
      set_pwm1_duty(240);
      set_pwm2_duty(0);
      break;

      case 3:
      set_pwm1_duty(230);
      set_pwm2_duty(0);
      break;

      case 4:
      set_pwm1_duty(220);
      set_pwm2_duty(0);
      break;

      case 5:
      set_pwm1_duty(210);
      set_pwm2_duty(0);
      break;

      case 6:
      set_pwm1_duty(0);
      set_pwm2_duty(210);
      break;

      case 7:
      set_pwm1_duty(0);
      set_pwm2_duty(220);
      break;

      case 8:
      set_pwm1_duty(0);
      set_pwm2_duty(230);
      break;

      case 9:
      set_pwm1_duty(0);
      set_pwm2_duty(240);
      break;

      case 10:
      set_pwm1_duty(0);
      set_pwm2_duty(255);
      break;
   }
lcd();
}


void main()
{
   k="0";
   n[0] = 0b00000100;
   n[1] = 0b00001000;
   n[2] = 0b00000010;                                                                 
   n[3] = 0b00000001; 
   
   m[0] = 0b00000001;                                                                                                                      
   m[1] = 0b00000010;      
   m[2] = 0b00001000;                                                                                                                      
   m[3] = 0b00000100;
   
   LCD_Begin(0x4E);                    // Initialize LCD module with I2C address = 0x4E
 
 // LCD_Goto(2, 1);                     // Go to column 2 row 1
  //LCD_Out("Hello, world!");
   
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   //setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2);
   set_timer0( 63036 );
   //setup_comparator(NC_NC_NC_NC);
   //setup_vref(FALSE);
   setup_timer_2(T2_DIV_BY_16,255,1);//819 us overflow,819 us interrupt
   
   setup_ccp1(CCP_PWM);
   setup_ccp2(CCP_PWM);
   delay_ms(50);
   
   
   LCD_Goto(1, 1);
   LCD_Out("Motor-STEP VEL:");     
   LCD_Goto(1, 2);
   LCD_Out("Motor-DC   VEL:");
   
   lcd();
   
       
   
   
   kbd_init();
   printf("Inicializando\n");
   delay_ms(2000);
   set_pwm1_duty(0);
   set_pwm2_duty(0);
   output_low(pin_d0);
   output_low(pin_d1);
   output_low(pin_d2);
   output_low(pin_d3);
   //enable_interrupts(INT_RTCC);
   enable_interrupts(GLOBAL);
   disable_interrupts(INT_RTCC);
   
   while(TRUE)
   {
      

       k=kbd_getc();
      
      
      if(k!=0)
      {
         printf("Tecla Pressionada:");
         putc(k);
         putc('\n');
         delay_ms(100);
         
         switch(k)
         {
         
            case '1':
            step_horario = false;
            fprintf(PORT1, "Movimento anti-horario");
            break;
            
             case '2':
            if(step_on == true){
            step_on = false;
            disable_interrupts(INT_RTCC);
            fprintf(PORT1,"Desligando motor step\n");
            }else if(step_on == false){
            
            step_on = true;
            i=0;
            enable_interrupts(INT_RTCC);
            fprintf(PORT1,"Ligando motor step\n");}
            motor_step();
            break;
            
            case '3':
            step_horario = true;
            fprintf(PORT1, "Movimento horário");
            break;
            
            case '4':
            if(vel_step==1)
            {
               fprintf(PORT1,"Velocidade step Minima alcancada\n");
               break;
            }

            fprintf(PORT1,"Diminuindo step velocidade\n");
            vel_step--;
            motor_step();
            
            break;
            
            case '6':
            if(vel_step==5)
            {
               fprintf(PORT1,"Velocidade step Maxima alcancada\n");
               break;
            }

            fprintf(PORT1,"Aumentando step velocidade\n");
            vel_step++;
            motor_step();
            break;        
            
            case '7':
            if(vel_dc==1)
            {
               fprintf(PORT1,"Velocidade maxima anti-horaria alcancada\n");
               break;
            }

            fprintf(PORT1,"Diminuindo velocidade\n");
            vel_dc--;
            motor_dc();
            break;
            
            case '8':
            if(dc_on == true){
            dc_on = false;
            fprintf(PORT1,"Desligando motor\n");
            }else if(dc_on == false){
            dc_on = true;
            fprintf(PORT1,"Ligando motor\n");
            }
            motor_dc();
            break;
            
            case '9':
            
            if(vel_dc==10)
            {
               fprintf(PORT1,"Velocidade maxima horaria alcancada\n");
               break;
            }

            fprintf(PORT1,"Aumentando velocidade\n");
            vel_dc++;
            motor_dc();
            break;

            default:
            fprintf(PORT1,"ERRO!\n");
            break;
         }

      }
      
      
      
   }
}

