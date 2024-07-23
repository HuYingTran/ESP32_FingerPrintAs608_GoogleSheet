#include <iostream>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>
#include "time.h"

#define STA_SSID "HuynhTran"
#define STA_PASS "Huynh123"
#define GG_SHEET_LINK "https://docs.google.com/spreadsheets/d/1u8VTG0Kuv-sl8rhMm7leTuXQJc8ndmMLlcBCc6uB350/edit?gid=1932027102#gid=1932027102"
#define GG_SHEET_API "https://script.google.com/macros/s/AKfycbxGgDNv9DaTQUsXiuAahhZrGEssGPE0zGABLDoatCN8fYwL8krxIdOWZzbZKEOHFbJz/exec"

// Pin for led RGB
#define PIN_LED_RED 19
#define PIN_LED_GREEN 18
#define PIN_LED_BLUE 5

// Pin for Buzzer
#define PIN_BUZZER 15

enum status_t {
    AVAILBLE = 0,
    CUSSECCFUL = 1,
    ERROR = 2,
    NO_INTERNET = 3,
    CREAT_ID = 4
};

enum color_led {
    BLUE = 0,
    GREEN = 1,
    RED = 2,
    YELLOW = 3,
    PURPLE = 4
};

typedef struct {
  status_t status;
  bool available;
  uint8_t sum_id;
  String ap_ssid;
  String ap_pass;
  String sta_ssid;
  String sta_pass;
  String gg_sheet_link;
  String gg_sheet_api;
  String ip_sta;
  String name;
  int check_in;
} global;

extern global controller;

// Function controller_led
void update_status_led(status_t status);

// Function controller_wifi
void wifi_init();
void wifi_connect();
void wifi_reconnect();

// Function controller_fingerprint
void fingerPrint_init();
void as608_enroll();
int as608_getID();
uint8_t as608_delete(uint8_t id);

void webserver_init();

// Function controller
void gpio_init();
void controller_setup();
bool load_status_id(String id);
void save_status_id(String id, bool status);
void save_string(String var, String value);
void save_int(String var, int value);
String load_string(String var);
void delete_NVS(String var);
uint8_t as608_getCount();
void lcd_init();
void lcd_show_ip(String line1, String line2);

void send_gg_sheet(uint8_t id, String matricNumber, String name, String level, String department, bool state=false, String sheetName="Home");
void update_gg_sheet(uint8_t id, bool state=false);
void delete_gg_sheet(String id);