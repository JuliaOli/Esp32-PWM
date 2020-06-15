// USB COMMUNICATION

#include <Arduino.h>
#include <SimpleCLI.h>

#define SENSOR  GPIO_NUM_23

int sensorValue;
const int portpin = 15;
const int LED1 = 19;

// Create CLI Object
SimpleCLI cli;

// Commands
Command param_set;
Command param_get;

int led_status = 0;

void SetCommandCallback(cmd* c);
void GetCommandCallback(cmd* c);
void CliErrorCallback(cmd_error* e);

// Callback in case of an error
void CliErrorCallback(cmd_error* e)
{
    CommandError cmdError(e); // Create wrapper object
    Serial.print("valore=ERROR: ");
    Serial.println(cmdError.toString());
}

void SetCommandCallback(cmd* c)
{
    Command cmd(c); // Create wrapper object

    // Get arguments from serial
    Argument param = cmd.getArgument("param");
    Argument value = cmd.getArgument("value");

    String strParam = param.getValue();
    // Values will be presented in this way on serial:
    // Serial.println("CMD: " +  param.getValue() + " " +  value.getValue());

    if (strParam == "luz") {
        int numberValue = value.getValue().toInt();

        if (numberValue < 1){
          digitalWrite(LED1, HIGH);
          led_status = 1;
        }
        else{
          digitalWrite(LED1, LOW);
          led_status = 0;
        }
        Serial.println("valore=OK");
    }
}

void GetCommandCallback(cmd* c)
{
    Command cmd(c); // Create wrapper object

    // Get arguments
    Argument param = cmd.getArgument("param");

    String strPatam = param.getValue();

    if (strPatam == "led") {
        Serial.println("LED_value=" + String(led_status));
    }

    if (strPatam == "sensor") {
        Serial.println("LDR_value=" + String(analogRead(portpin)));
    }

    if (strPatam == "all") {
        Serial.println("valore=" 
            +  String(analogRead(portpin))
            + "," + String(led_status));
    }
}
// Exemplo de comandos aceitos
// set luz 0 (liga)
// set luz 1 (apaga)
// get led
// get sensor
// get all

void setup()
{
  /*/ [Optional] Check if our command was successfully added
  if (!ping) {
    Serial.println("Something went wrong :(");
  } else {
    Serial.println("Ping was added to the CLI!");
  }*/

  // set parameters to setCallback
  param_set = cli.addCommand("set", SetCommandCallback);
  param_set.addPositionalArgument("param", "null"); //parammeter name
  param_set.addPositionalArgument("value", "0"); //parammeter value

  // set parameters to getCallback
  param_get = cli.addCommand("get", GetCommandCallback);
  param_get.addPositionalArgument("param", "null"); //parammeter name

  // Set error Callback
  cli.setOnError(CliErrorCallback);

  //Mode sensores e led
  pinMode(LED1, OUTPUT);
  pinMode(portpin, INPUT);

  Serial.begin(9600); // starts the serial port at 9600
}

void loop()
{
  if (Serial.available()) {
        // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');

        // Echo the user input
        Serial.print("# ");
        Serial.println(input);

        // Parse the user input into the CLI
        cli.parse(input);
    }
  delay(700); // wait 100ms for next reading
}
