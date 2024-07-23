#include "controller.h"

void set_led (uint8_t r,uint8_t g, uint8_t b) {
    digitalWrite(PIN_LED_RED, r);
    digitalWrite(PIN_LED_GREEN, g);
    digitalWrite(PIN_LED_BLUE, b);
}

void set_led_color(color_led color) {
    switch (color)
    {
    case BLUE:
        set_led(0,0,1);
        break;
    case GREEN:
        set_led(0,1,0);
        break;
    case RED:
        set_led(1,0,0);
        break;
    case YELLOW:
        set_led(1,1,0);
        break;
    case PURPLE:
        set_led(1,0,1);
        break;
    }
}

void update_status_led(status_t status) {
    switch (status) 
    {
    case AVAILBLE:
        set_led_color(BLUE);
        digitalWrite(PIN_BUZZER,LOW);
        break;
    case CUSSECCFUL:
        set_led_color(GREEN);
        break;
    case ERROR:
        set_led_color(RED);
        digitalWrite(PIN_BUZZER,HIGH);
        break;
    case NO_INTERNET:
        set_led_color(YELLOW);
        break;
    case CREAT_ID:
        set_led_color(PURPLE);
        break;
    }
}