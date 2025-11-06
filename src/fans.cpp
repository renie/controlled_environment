#include "../secrets.h"
#include <Arduino.h>
#include "driver/ledc.h"
#include "./fans.h"

namespace Fans {
  static const int pwmFreq = FAN_PWM_FREQ;
  static const int pwmResolution = FAN_PWM_RESOLUTION;
  static const int pwmChannel = FAN_PWM_CHANNEL;
  static int pwmValue = 0;
  static int pwmInPercentage = 0;

  static int mapPercentageToPWM(int percentage) {
    percentage = constrain(percentage, 0, 100);

    return map(percentage, 0, 100, FAN_PWM_DEADZONE_LOW, FAN_PWM_DEADZONE_HIGH);
  }

  static void initializeESP32PWM() {
    ledc_timer_config_t ledc_timer;
    ledc_timer.duty_resolution = (ledc_timer_bit_t)pwmResolution;
    ledc_timer.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_timer.timer_num = LEDC_TIMER_0;
    ledc_timer.freq_hz = pwmFreq;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    // Configure channel
    ledc_channel_config_t ledc_channel;
    ledc_channel.channel    = (ledc_channel_t)pwmChannel;
    ledc_channel.timer_sel  = LEDC_TIMER_0;
    ledc_channel.gpio_num   = FAN_PIN;
    ledc_channel.speed_mode = LEDC_LOW_SPEED_MODE;
    ledc_channel.intr_type  = LEDC_INTR_DISABLE;
    ledc_channel.duty       = 0;
    ledc_channel.hpoint     = 0;
    ledc_channel.flags.output_invert = 0;

    ledc_channel_config(&ledc_channel);
  }

  static void setFanPwm() {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel, pwmValue);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)pwmChannel);
  }

  void initializePins() {
    // this plocess can be as simple as `analogWriting` on pins
    // but ESP32 is a bit more complicated, so its initialization
    // it in a function apart
    initializeESP32PWM();  
  }

  void setSpeed(int percentage) {
    pwmInPercentage = percentage;
    pwmValue = mapPercentageToPWM(pwmInPercentage);
    setFanPwm();

    Serial.println("Fan set to (" + String(pwmInPercentage) + ")");
    Serial.println("");
    Serial.println("------------------");
    Serial.println("");
  }

  int getSpeed() {
    return pwmInPercentage;
  }
}
