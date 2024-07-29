//Include required libraries
#include "controller.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

void task_update_status(void *parameter) {
  while(1) {
    delay(100);
    if(WiFi.status() != WL_CONNECTED) {
      controller.status = NO_INTERNET;
      lcd_show_ip(controller.sta_ssid, "Connecting...");
      wifi_reconnect();
    } else if(controller.available) {
      if(controller.status != AVAILBLE) {
        lcd_show_ip(controller.sta_ssid, WiFi.localIP().toString());
      }
      controller.status = AVAILBLE;
    }
    update_status_led(controller.status);
  }
}

void task_as608_controller(void *parameter)
{
  while(1) {
    if(controller.status == AVAILBLE)
    {
      int id = as608_getID();
      if(id>0)
      { 
        digitalWrite(PIN_BUZZER,HIGH);
        delay(100);
        digitalWrite(PIN_BUZZER,LOW);

        controller.status = CUSSECCFUL;
        controller.available = false;
        bool current_status = !load_status_id(String(id));
        String name = load_string(String(id));
        if(current_status) {
          lcd_show_ip(controller.name, "Check In");
          controller.check_in++;
        }else{
          lcd_show_ip(controller.name, "Check Out");
          controller.check_in--;
        }
        save_int("check_in",controller.check_in);
        save_status_id(String(id),current_status);
        update_gg_sheet(id, current_status);
      }else if(id < 0)
      { 
        lcd_show_ip("Not Found :(", "");
        controller.status = ERROR;
        controller.available = false;
      }
    }
    if(controller.status != CUSSECCFUL) {
      delay(500);
    }
    if(controller.status == CUSSECCFUL || controller.status == ERROR)
    {
      controller.available = true;
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  controller_setup();
  Serial.println("init...");
  gpio_init();
  lcd_init();
  wifi_init();
  webserver_init();
  delay(500);
  lcd_show_ip(controller.sta_ssid, WiFi.localIP().toString());
  fingerPrint_init();
//  controller.sum_id = as608_getCount();
  Serial.println(controller.sum_id);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Task update status");
  xTaskCreatePinnedToCore(task_update_status,"Task update status",10000,NULL,1,NULL,0);
  Serial.println("Task as608 controller");
  xTaskCreatePinnedToCore(task_as608_controller,"Task as608 controller",10000,NULL,1,NULL,0);
}
void loop() {} 
