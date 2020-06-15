#include <Ticker.h> //Ticker Library
#include <Wire.h>
#include <math.h>
#include <SimpleCLI.h>
#include "sfm3019_all.h"

// Create CLI Object
SimpleCLI cli;
// Commands
Command param_set;
Command param_get;


void SetCommandCallback(cmd* c);
void GetCommandCallback(cmd* c);
void CliErrorCallback(cmd_error* e);


cli.setOnError(CliErrorCallback); // Set error Callback
param_set = cli.addCommand("set", SetCommandCallback);
param_set.addPositionalArgument("param", "null");
param_set.addPositionalArgument("value", "0");

param_get = cli.addCommand("get", GetCommandCallback);
param_get.addPositionalArgument("param", "null");


void CliErrorCallback(cmd_error* e)
{
    CommandError cmdError(e); // Create wrapper object
    Serial.print("valore=ERROR: ");
    Serial.println(cmdError.toString());
}

void SetCommandCallback(cmd* c)
{
    Command cmd(c); // Create wrapper object

    // Get arguments
    Argument param = cmd.getArgument("param");
    Argument value = cmd.getArgument("value");

    String strPatam = param.getValue();

    //Serial.println("CMD: " +  param.getValue() + " " +  value.getValue());

    if (strPatam == "run") {
        int numberValue = value.getValue().toInt();

        if (numberValue < 1)
            core_config.run = false;
        else
            core_config.run = true;

        Serial.println("valore=OK");
    }

    if (strPatam == "mode") {
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

    if (strPatam == "rate") {
        float numberValue = value.getValue().toFloat();
        core_config.respiratory_rate = numberValue;
        core_config.inhale_ms = 60000.0 / core_config.respiratory_rate * (1 - core_config.respiratory_ratio);
        core_config.exhale_ms = 60000.0 / core_config.respiratory_rate * (core_config.respiratory_ratio);
        Serial.println("valore=OK");
    }

    if (strPatam == "ratio") {
        float numberValue = value.getValue().toFloat();
        core_config.respiratory_ratio = numberValue;
        core_config.inhale_ms = 60000.0 / core_config.respiratory_rate * (1 - core_config.respiratory_ratio);
        core_config.exhale_ms = 60000.0 / core_config.respiratory_rate * (core_config.respiratory_ratio);
        Serial.println("valore=OK");
    }

    if (strPatam == "assist_ptrigger") {
        float numberValue = value.getValue().toFloat();
        core_config.assist_pressure_delta_trigger = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "assist_flow_min") {
        float numberValue = value.getValue().toFloat();
        core_config.flux_close = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "ptarget") {
        float numberValue = value.getValue().toFloat();
        core_config.target_pressure_auto = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pressure_support") {
        float numberValue = value.getValue().toFloat();
        core_config.target_pressure_assist = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "peep") {
        float numberValue = value.getValue().toFloat();
        core_config.pressure_forced_exhale_min = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pid_p") {
        float numberValue = value.getValue().toFloat();
        core_config.P = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pid_i") {
        float numberValue = value.getValue().toFloat();
        core_config.I = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pid_d") {
        float numberValue = value.getValue().toFloat();
        core_config.D = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pid_p2") {
        float numberValue = value.getValue().toFloat();
        core_config.P2 = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pid_i2") {
        float numberValue = value.getValue().toFloat();
        core_config.I2 = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pid_d2") {
        float numberValue = value.getValue().toFloat();
        core_config.D2 = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pause_inhale") {
        int numberValue = value.getValue().toInt();
        core_config.pause_inhale = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pause_lg") {
        int numberValue = value.getValue().toInt();
        core_config.pause_lg = numberValue ? true : false;
        Serial.println("valore=OK");
    }

    if (strPatam == "pause_lg_time") {
        int numberValue = value.getValue().toFloat();
        core_config.pause_lg_timer = numberValue * 1000.0;
        Serial.println("valore=OK");
    }

    if (strPatam == "pause_lg_p") {
        int numberValue = value.getValue().toFloat();
        core_config.pause_lg_p = numberValue ;
        Serial.println("valore=OK");
    }    

    if (strPatam == "pause_exhale") {
        int numberValue = value.getValue().toInt();
        core_config.pause_exhale = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "pid_limit") {
        float numberValue = value.getValue().toFloat();
        core_config.pid_limit = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "alarm_snooze") {
        int numberValue = value.getValue().toInt();
        ResetAlarm(numberValue);
        Serial.println("valore=OK");
    }

    if (strPatam == "alarm") {
        int numberValue = value.getValue().toInt();
        TriggerAlarm(ALARM_GUI_ALARM);
        Serial.println("valore=OK");
    }

    if (strPatam == "watchdog_reset") {
        int numberValue = value.getValue().toInt();
        watchdog_time = millis();
        ALARM_FLAG = ALARM_FLAG & (~GenerateFlag(__ERROR_WDOG_PI));
        Serial.println("valore=OK");
    }

    if (strPatam == "console") {
        int numberValue = value.getValue().toInt();
        __CONSOLE_MODE = numberValue != 0 ? true : false;
        Serial.println("valore=OK");
    }

    if (strPatam == "timestamp") {
        int numberValue = value.getValue().toInt();
        __ADDTimeStamp = numberValue != 0 ? true : false;
        Serial.println("valore=OK");
    }

    if (strPatam == "wdenable") {
        int numberValue = value.getValue().toInt();
        __WDENABLE = numberValue != 0 ? true : false;
        ALARM_FLAG = ALARM_FLAG & (~GenerateFlag(__ERROR_ALARM_PI));
        Serial.println("valore=OK");
    }

    if (strPatam == "backup_enable") {
        int numberValue = value.getValue().toInt();
        core_config.backup_enable = numberValue ? true : false;
        Serial.println("valore=OK");
    }

    if (strPatam == "backup_min_rate") {
        float numberValue = value.getValue().toFloat();
        numberValue = numberValue<1?1:numberValue;
        core_config.backup_min_rate = numberValue;
        Serial.println("valore=OK");
    }

    if (strPatam == "stats_clear") {
        ResetStatsBegin();
    }
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

    if (strPatam == "o2") {
        Serial.println("valore=" + String(last_O2));
    }

    if (strPatam == "bpm") {
        Serial.println("valore=" + String(last_bpm));
    }

    if (strPatam == "backup") {
        Serial.println("valore=" + String(0));
    }

    if (strPatam == "tidal") {
        Serial.println("valore=" + String(tidal_volume_c.TidalVolume));
    }

    if (strPatam == "peep") {
        Serial.println("valore=" + String(last_peep));
    }

    if (strPatam == "temperature") {
        Serial.println("valore=" + String(temperature));
    }

    if (strPatam == "power_mode") {
        Serial.println("valore=" + String(batteryPowered ? 1 : 0));
    }

    if (strPatam == "battery") {

        Serial.println("valore=" + String(currentBatteryCharge));
    }

    if (strPatam == "version") {
        Serial.println("valore=" + String(_FIRMWARE_VERSION_));
    }

    if (strPatam == "alarm") {
        Serial.println("valore=" + String(ALARM_FLAG));
    }

    if (strPatam == "warning") {
        Serial.println("valore=" + String(0));
    }

    if (strPatam == "run") {
        Serial.println("valore=" + String(core_config.run ? 1 : 0));
    }

    if (strPatam == "mode") {
        Serial.println("valore=" + String(core_config.BreathMode == M_BREATH_ASSISTED ? 1 : 0));
    }
    if (strPatam == "rate") {
        Serial.println("valore=" + String(core_config.respiratory_rate));
    }
    if (strPatam == "ratio") {
        Serial.println("valore=" + String(core_config.respiratory_ratio));
    }
    if (strPatam == "assist_ptrigger") {
        Serial.println("valore=" + String(core_config.assist_pressure_delta_trigger));
    }
    if (strPatam == "assist_flow_min") {
        Serial.println("valore=" + String(core_config.flux_close));
    }
    if (strPatam == "ptarget") {
        Serial.println("valore=" + String(core_config.target_pressure_auto));
    }
    if (strPatam == "pressure_support") {
        Serial.println("valore=" + String(core_config.target_pressure_assist));
    }
    if (strPatam == "backup_enable") {
        Serial.println("valore=" + String(core_config.backup_enable ? 1 : 0));
    }
    if (strPatam == "backup_min_rate") {
        Serial.println("valore=" + String(core_config.backup_min_rate));
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

    if (strPatam == "calib") {
        Serial.print("Valore=");
        for (int j = 0; j < N_PRESSURE_SENSORS; j++) {
            i2c_MuxSelect(pressure_sensor_i2c_mux[j]);
            float mean = 0;
            PRES_SENS_CT[j].ZERO = 0;
            for (int q = 0; q < 100; q++) {
                read_pressure_sensor(j);
                mean += pressure[j].last_pressure;
            }
            PRES_SENS_CT[j].ZERO = mean / 100;

            Serial.print(String(PRES_SENS_CT[j].ZERO) + ",");
        }
        Serial.println(" ");
    }

    if (strPatam == "calibv") {
        if (fabs(tidal_volume_c.ExpVolumeVenturi) > 0)
            tidal_volume_c.AutoZero = fabs(tidal_volume_c.InspVolumeVenturi) / fabs(tidal_volume_c.ExpVolumeVenturi);

        Serial.println("valore=" + String(tidal_volume_c.InspVolumeVenturi) + "," + String(tidal_volume_c.ExpVolumeVenturi) + "," + String(tidal_volume_c.AutoZero));
    }

    if (strPatam == "stats") {
        if (__stat_param.mean_cnt > 0) {
            float overshoot_avg = __stat_param.overshoot_avg / __stat_param.mean_cnt;
            float overshoot_length_avg = __stat_param.overshoot_length_avg / __stat_param.mean_cnt;
            float final_error_avg = __stat_param.final_error_avg / __stat_param.mean_cnt;
            float t1050_avg = __stat_param.t1050_avg / __stat_param.mean_cnt;
            float t1090_avg = __stat_param.t1090_avg / __stat_param.mean_cnt;
            float tpeak_avg = __stat_param.tpeak_avg / __stat_param.mean_cnt;
            float t9010_avg = __stat_param.t9010_avg / __stat_param.mean_cnt;
            float t9050_avg = __stat_param.t9050_avg / __stat_param.mean_cnt;
            float peep_avg = __stat_param.peep_avg / __stat_param.mean_cnt;
            float t10_avg = __stat_param.t10_avg / __stat_param.mean_cnt;
            float time_to_peak_avg = __stat_param.time_to_peak_avg / __stat_param.mean_cnt;
            float flux_peak_avg = __stat_param.flux_peak_avg / __stat_param.mean_cnt;
            float flux_t1090_avg = __stat_param.flux_t1090_avg / __stat_param.mean_cnt;
            float flux_t9010_avg = __stat_param.flux_t9010_avg / __stat_param.mean_cnt;

            Serial.println("valore=overshoot_avg:" + String(overshoot_avg)
                + ",overshoot_length_avg:" + String(overshoot_length_avg)
                + ",final_error:" + String(final_error_avg)
                + ",t1050_avg:" + String(t1050_avg)
                + ",t1090_avg:" + String(t1090_avg)
                + ",tpeak_avg:" + String(tpeak_avg)
                + ",t9010_avg:" + String(t9010_avg)
                + ",t9050_avg:" + String(t9050_avg)
                + ",peep_avg:" + String(peep_avg)
                + ",t10_avg:" + String(t10_avg)
                + ",time_to_peak_avg:" + String(time_to_peak_avg)
                + ",flux_peak_avg:" + String(flux_peak_avg)
                + ",flux_t1090_avg:" + String(flux_t1090_avg)
                + ",flux_t9010_avg:" + String(flux_t9010_avg));
        }
        else {
            Serial.println("valore=no_data");
        }
    }


     if (strPatam == "pause_lg") {
        Serial.println("valore=" + String(core_config.pause_lg));
    }

    if (strPatam == "pause_lg_time") {
        Serial.println("valore=" + String(core_config.pause_lg_timer/1000.0));
    }

    if (strPatam == "pause_lg_p") {
      Serial.println("valore=" + String(core_config.pause_lg_p));
    }    

}


void loop(){
    if (Serial.available()) {
    // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');

    // Echo the user input
    // Serial.print("# ");
    //  Serial.println(input);

    // Parse the user input into the CLI
        cli.parse(input);
    }
}
