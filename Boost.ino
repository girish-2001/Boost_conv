/* 

 ----------------- ATtiny85 embedded program -----------------------------
 ------------------- Bucky : Buck convertor ------------------------------
 ---------- Designed and Developed by Girish K --------------------
  * Output PWM at IC PIN 5 : PB0
  * Pot Input at IC PIN 2 : PB3
  * Feedback Input at IC PIN 3 : PB4


*/

// Header file for AVR programming
#include<avr/io.h>

// Global variable for data input
int pot_input=0,feedback=0,output_pwm=0;        // To store potentiometer input ADC value

// Function to initialise registers
void initialise()
{
  DDRB=0x01;                                // Setting input-output ports

  TCCR0A|=(1<<WGM00)|(1<<WGM02);            // Set Waveform as Phase correct PWM
  TCCR0A|=(1<<COM0A0)|(1<<COM0A1);          // Set inverted compare match mode
  TCCR0B|=(1<<CS00)|(1<<CS01);              // Set frequency of PWM as 125 kHz using prescaler of 64
  
  ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);  // Enable ADC and set sampling rate at 125kHz
}

void input_ADC()
{
  ADMUX|=(1<<MUX1)|(1<<MUX0);               // Set ADC input channel as 3
  ADCSRA|=(1<<ADSC);            // Start ADC conversion
  while(!(ADCSRA&(1<<ADIF)));   // Wait for ADIF to go high, indicating completion
  pot_input=int(ADCL);          // Extracting lower 8 bits of data
  pot_input+=(int(ADCH)*256);   // Extracting higher 8 bits of data
}

void feedback_ADC()
{
  ADMUX|=(1<<MUX1);               // Set ADC input channel as 2
  ADMUX&=~(1<<MUX0);
  ADCSRA|=(1<<ADSC);            // Start ADC conversion
  while(!(ADCSRA&(1<<ADIF)));   // Wait for ADIF to go high, indicating completion
  feedback=int(ADCL);          // Extracting lower 8 bits of data
  feedback+=(int(ADCH)*256);   // Extracting higher 8 bits of data
}

void control()
{
  if(input_pot>feedback)        // if feedback is higher than input voltage set
  output_pwm--;                 // increase duty cycle
  else if(input_pot<feedback)   // if feedback is lower than input voltage set
  output_pwm++;                 // decrease duty cycle

  if(output_pwm>253)            // limiting duty cycle within 1 to 254
  output_pwm=254;               
  if(output_pwm<1)              // limiting duty cycle within 1 to 254
  output_pwm=1;
}

int main()
{
  initialise();           // Initialising registers
  while(1)                // Infinite loop
  {
    input_ADC();          // Read input from potentiometer using ADC
    feedback_ADC();       // Read feedback voltage
    control();            // Compute output pwm duty cycle
    OCR0A= output_pwm;   // Assign input value to output compare match
  }
}

// END OF CODE
