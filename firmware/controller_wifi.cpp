#include "controller.h"

TaskHandle_t wifi_connect_handle = NULL;
TaskHandle_t wifi_reconnect_handle = NULL;

void task_wifi_connect(void *parameter)
{
    // wifi_connect_handle = xTaskGetCurrentTaskHandle();
    WiFi.begin(controller.sta_ssid.c_str(), controller.sta_pass.c_str());
    Serial.print("Connecting to Wifi: ");
    Serial.println(controller.sta_ssid.c_str());
    int wait = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        wait++;
        if (wait == 10) {
            Serial.println("Connect Wifi Faile");
            wifi_connect_handle = NULL;
            vTaskDelete(NULL);
            return;
        }
    }
    Serial.print("ESP32 IP on the WiFi network: ");
    Serial.println(WiFi.localIP());
    controller.ip_sta = WiFi.localIP().toString();
    wifi_connect_handle = NULL;
    vTaskDelete(NULL);
}

void wifi_connect() { 
    if(wifi_connect_handle == NULL) {
        xTaskCreatePinnedToCore(task_wifi_connect,"Task wifi connect",10000,NULL,1,&wifi_connect_handle,0);
    }
}

void task_wifi_reconnect(void *parameter) {
    while(controller.status == NO_INTERNET) {
        Serial.println("Wifi Reconnect");
        wifi_connect();
        delay(10000);
    }
    vTaskDelete(wifi_reconnect_handle);
    wifi_reconnect_handle = NULL;
}

void wifi_reconnect() {
    if(wifi_connect_handle == NULL && wifi_reconnect_handle == NULL) {
        xTaskCreatePinnedToCore(task_wifi_reconnect,"Task wifi reconnect",10000,NULL,1,&wifi_reconnect_handle,0);
    }
}

void wifi_init()
{
    WiFi.mode(WIFI_MODE_APSTA);
    // WiFi.softAP(controller.ap_ssid.c_str(), controller.ap_pass.c_str());
    // Serial.print("ESP32 IP as soft AP: ");
    // Serial.println(WiFi.softAPIP());
    wifi_connect();
}