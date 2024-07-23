#include "controller.h"

global controller;
Preferences prefs;

void gpio_init() {
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
}

void controller_setup()
{
    controller.available = true;
    prefs.begin("data_nvs", false);
    controller.ap_ssid = prefs.getString("ap_ssid", "admin");
    controller.ap_pass = prefs.getString("ap_pass", "1234");
    controller.sta_ssid = prefs.getString("sta_ssid", STA_SSID);
    controller.sta_pass = prefs.getString("sta_pass", STA_PASS);
    controller.gg_sheet_link = prefs.getString("gg_sheet_link", GG_SHEET_LINK);
    controller.gg_sheet_api = prefs.getString("gg_sheet_api", GG_SHEET_API);
    controller.check_in = prefs.getInt("check_in", 0);
    controller.sum_id = prefs.getInt("sum_id", 0);
    controller.status = AVAILBLE;
}

bool load_status_id(String id) {
  return prefs.getBool(id.c_str(), 0);
}

void save_status_id(String id, bool status) {
  prefs.putBool(id.c_str(), status);
}

void save_string(String var, String value) {
  prefs.putString(var.c_str(), value);
}

String load_string(String var) {
  return prefs.getString(var.c_str(), "");
}

void delete_NVS(String var) {
  prefs.remove(var.c_str());
}

void save_int(String var, int value) {
  prefs.putInt(var.c_str(),value);
}

String get_time() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }
  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  String asString(timeStringBuff);
  asString.replace(" ", "-");
  Serial.print("Time:");
  Serial.println(asString);
  return asString;
}

String get_moth() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }
  char monthBuffer[20]; // Buffer size should be enough for the month string
  strftime(monthBuffer, sizeof(monthBuffer), "%B", &timeinfo); // Get full month name
  String monthString(monthBuffer);
  Serial.print("Month:");
  Serial.println(monthString);
  return monthString;
}

// https://script.google.com/macros/s/AKfycbwJ08rgLj5EudAbs-3qUDu0OhsU6nuuYSCzaNb2BFfn2qMEpLJEU_7BamAsqX8OazIu/exec
void send_gg_sheet(uint8_t id, String matricNumber, String name, String level, String department, bool state, String sheetName) {
  char urlFinal[256];
  snprintf(urlFinal,sizeof(urlFinal),"%s?time=%s&id=%s&matricNumber=%s&name=%s&level=%s&department=%s&state=%s&sheetName=%s",
  controller.gg_sheet_api.c_str(),get_time().c_str(),String(id).c_str(),matricNumber.c_str(),name.c_str(),level.c_str(),department.c_str(),(state?"ON":"OFF"),sheetName.c_str());
  HTTPClient http;
  http.begin(urlFinal);
  Serial.println(urlFinal);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  Serial.println(http.GET());
  http.end();
}

void update_gg_sheet(uint8_t id, bool state) {
  char urlFinal[256];
  snprintf(urlFinal,sizeof(urlFinal),"%s?time=%s&id=%s&state=%s&sheetName=%s",
  controller.gg_sheet_api.c_str(),get_time().c_str(),String(id).c_str(),(state?"ON":"OFF"),get_moth().c_str());
  HTTPClient http;
  http.begin(urlFinal);
  Serial.println(urlFinal);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);

  if (http.GET() > 0) {
      controller.name = http.getString();
    } else {
      controller.name = "";
    }
  http.end();
}

void delete_gg_sheet(String id) {
  char urlFinal[256];
  snprintf(urlFinal,sizeof(urlFinal),"%s?id=%s&delete=true",
  controller.gg_sheet_api.c_str(),id.c_str());
  Serial.println(urlFinal);
  HTTPClient http;
  http.begin(urlFinal);
  Serial.println(urlFinal);
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  Serial.println(http.GET());
  http.end();
}

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_init() {
    lcd.init();
    lcd.init();
    lcd.backlight();
    // lcd.clear();
}

void lcd_show_ip(String line1, String line2) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(line1);
    lcd.setCursor(0,1);
    lcd.print(line2);
}