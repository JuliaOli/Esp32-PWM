// Change the values of the freq variable 
// to find the quickest response to valve operation

// Frequency	        Resolution (maximum)	    Duty Cycle
// 1 Hz até 1 kHz	        20 bits	             0 a 100%
// 1 kHz a 100 kHz	      13 bits	             0 a 100%
// 100 kHz a 1 MHz	       7 bits	             0 a 100%
// 1 MHz a 20 MHz	         3 bits	             25%, 50%,75% ou 100%
// 20 MHz a 40MHz	         1 bit	             50%

#include <Arduino.h>

const int ledPino = 16;  // 16 corresponds to GPIO16

//configuration of PWM parameters
const int freq = 78000; // PWM frequency: 78kHz
const int canal = 0; // Channel that will generate the signal, varies from 0 to 15
const int resolucao = 13; // values from 0 to 8192 (2 ** 13)

void setup() {

  // configure PWM through parameters
  ledcSetup(canal, freq, resolucao);
  
  // Attach the channel to the GPIO
  ledcAttachPin(ledPino, canal);

}

void loop() {
  
  // Code to change the dutyCycle starting at 100%
  for(int dutyCycle = 8192; dutyCycle >= 0; dutyCycle--){
    ledcWrite(canal, dutyCycle);   
    delay(15);
  }
}

// Caso a resolução ou o duty cycle esteja errado ele irá retornar o seguinte erro:
// E (196) ledc: requested frequency and duty resolution can not be achieved
//  try reducing freq_hz or duty_resolution. div_param=128

// DAC (conversor digital analógico) com resolução de 8 bits: GPIO25/26

// Note that in the ledcWrite() 
// function we use the channel that is generating the signal, and not the GPIO.

// You can get the same signal from the same channel in different GPIOs. 
// To achieve that,  you just need to attach those GPIOs to the same channel on the setup().

// https://randomnerdtutorials.com/esp32-pwm-arduino-ide/

// Teste no Osciloscopio https://randomnerdtutorials.com/esp8266-pwm-arduino-ide/

