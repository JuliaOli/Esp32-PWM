#include <SimpleCLI.h>

// Create CLI Object
SimpleCLI cli;
// Commands
Command param_set;
Command param_get;

void SetCommandCallback(cmd* c);
void GetCommandCallback(cmd* c);
void CliErrorCallback(cmd_error* e);

// Set error Callback
cli.setOnError(CliErrorCallback); 

// set parameters to set callback
param_set = cli.addCommand("set", SetCommandCallback);
param_set.addPositionalArgument("param", "null"); //parammeter name
param_set.addPositionalArgument("value", "0"); //parammeter value

// set parameters to get callback
param_get = cli.addCommand("get", GetCommandCallback);
param_get.addPositionalArgument("param", "null"); //parammeter name

void CliErrorCallback(cmd_error* e)
{
    CommandError cmdError(e); // Create wrapper object
    Serial.print("valore=ERROR: ");
    Serial.println(cmdError.toString());
}

// Set vai pegar commandos impressos no serial pela rasp aqui
// tem que tomar cuidado pra não coletar valores que a própria ESP mandou?
void SetCommandCallback(cmd* c)
{
    Command cmd(c); // Create wrapper object

    // Get arguments from serial
    Argument param = cmd.getArgument("param");
    Argument value = cmd.getArgument("value");

    String strParam = param.getValue();
    // Values will be presented in this way on serial:
    // Serial.println("CMD: " +  param.getValue() + " " +  value.getValue());

    if (strPatam == "run") {
        int numberValue = value.getValue().toInt();

        if (numberValue < 1)
            core_config.run = false;
        else
            core_config.run = true;

        Serial.println("valore=OK");
    }

    if (strParam == "mode") {
        int numberValue = value.getValue().toInt();
        if (numberValue == 0) {
            //Forced Mode
            core_config.constant_rate_mode = true;
            core_config.pressure_alarm = 100;
            core_config.pressure_alarm_off = 50;
            core_config.inhale_critical_alarm_ms = 16000;
            core_config.exhale_critical_alarm_ms = 16000;
            core_config.BreathMode = M_BREATH_FORCED;
        }
        else {
            //Assisted Mode
            core_config.constant_rate_mode = false;
            core_config.pressure_alarm = 100;
            core_config.pressure_alarm_off = 50;
            core_config.inhale_critical_alarm_ms = 16000;
            core_config.exhale_critical_alarm_ms = 16000;
            core_config.BreathMode = M_BREATH_ASSISTED;
        }
        Serial.println("valore=OK");
    }

    void GetCommandCallback(cmd* c)
{
    Command cmd(c); // Create wrapper object

    // Get arguments
    Argument param = cmd.getArgument("param");

    String strPatam = param.getValue();

    //Serial.println("CMD: " +  param.getValue() + " " +  value.getValue());

    if (strPatam == "pressure") {
        Serial.println("valore=" + String(pressure[0].last_pressure));
    }

    if (strPatam == "flow") {
        Serial.println("valore=" + String(gasflux[0].last_flux));
    }

    if (strPatam == "all") {
        Serial.println("valore=" + String(pressure[1].last_pressure) + "," + String(tidal_volume_c.FLUX) + "," + String(last_O2) + "," + String(last_bpm)
            + "," + String(tidal_volume_c.TidalVolume) + "," + String(last_peep)
            + "," + String(temperature) + "," + String(batteryPowered ? 1 : 0) + "," + String(currentBatteryCharge)
            + "," + String(currentP_Peak)
            + "," + String(currentTvIsnp)
            + "," + String(currentTvEsp)
            + "," + String(currentVM));
    }
}


void loop(){
    if (Serial.available()) {
    // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');

        // Echo the user input
        // Esse echo não parece necessário melhor não botar
          Serial.print("# ");
          Serial.println(input);

    // Parse the user input into the CLI
    //Aqui a mágica acontece e ele chama os callbacks tudo?
        cli.parse(input);
        
    }
}
