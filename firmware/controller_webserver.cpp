#include "controller.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
static String status_request = "";

// // Replaces placeholder with LED state value
String processor(const String &var)
{   
    if (var == "SUM")
    {
        return String(controller.sum_id);
    }
    if (var == "CHECKIN")
    {
        return String(controller.check_in);
    }
    if (var == "STATE")
    {
        return status_request;
    }
    if (var == "URL")
    {
        return controller.gg_sheet_link;
    }
    return String();
    if (var == "IP_STA")
    {   
        Serial.println(controller.ip_sta);
        return controller.ip_sta;
    }
    return String();
}

void webserver_init()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    server.on("/home", HTTP_GET, [](AsyncWebServerRequest *request) { 
        if(!request->authenticate(controller.ap_ssid.c_str(), controller.ap_pass.c_str())) {
            return request->requestAuthentication();
        }           
        request->send(SPIFFS, "/webserver_home.html", String(), false, processor); 
    });

    server.on("/add_id", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(!request->authenticate(controller.ap_ssid.c_str(), controller.ap_pass.c_str())) {
            return request->requestAuthentication();
        }
        request->send(SPIFFS, "/webserver_add_id.html", String(), false, processor); 
    });

    server.on("/setting", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(!request->authenticate(controller.ap_ssid.c_str(), controller.ap_pass.c_str())) {
            return request->requestAuthentication();
        }
        request->send(SPIFFS, "/webserver_setting.html", String(), false, processor); 
    });

    server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(401); });

    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
              { ESP.restart();
                request->send(401); });
    
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
                { Serial.println("SEND CSS");
                    request->send(SPIFFS, "/style.css", "text/css"); });

    server.on("/creat_id", HTTP_GET, [](AsyncWebServerRequest *request){
        String matricNumber, name, level, department;
        if (request->hasParam("matricNumber")) {
            matricNumber = request->getParam("matricNumber")->value();
        }
        if (request->hasParam("name")) {
            name = request->getParam("name")->value();
        }
        if (request->hasParam("level")) {
            level = request->getParam("level")->value();
        }
        if (request->hasParam("department")) {
            department = request->getParam("department")->value();
        }
        int current_id = controller.sum_id+1;
        as608_enroll();
        save_int("sum_id", current_id);
        save_status_id(String(current_id), false);
        save_string(matricNumber,String(current_id));
        send_gg_sheet(current_id,matricNumber,name,level,department);
        status_request = "Successful";
        request->send(SPIFFS, "/webserver_add_id.html", String(), false, processor);
        status_request = "";
        delay(2000);
    });

    server.on("/delete_id", HTTP_GET, [](AsyncWebServerRequest *request){
        String matric, id;
        if (request->hasParam("matric_number")) {
            matric = request->getParam("matric_number")->value();
        }
        id = load_string(matric);
        delete_NVS(matric);
        delete_NVS(id);
        as608_delete(id.toInt());
        delete_gg_sheet(id);
    request->send(SPIFFS, "/webserver_add_id.html", String(), false, processor); });

    server.on("/change_link", HTTP_GET, [](AsyncWebServerRequest *request){
        String link;
        if (request->hasParam("url")) {
            link = request->getParam("url")->value();
        }
        save_string(controller.gg_sheet_api, link);
        status_request = "Cusseccful";
        request->send(SPIFFS, "/webserver_setting.html", String(), false, processor);
        status_request = "";
    });

    server.on("/wifi_sta", HTTP_GET, [](AsyncWebServerRequest *request){
        String ssid, pass;
        if (request->hasParam("ssid")) {
            ssid = request->getParam("ssid")->value();
        }
        if (request->hasParam("pass")) {
            pass = request->getParam("pass")->value();
        }
        save_string(controller.ap_ssid, ssid);
        save_string(controller.ap_pass, pass);
        status_request = "Cusseccful";
        request->send(SPIFFS, "/webserver_setting.html", String(), false, processor);
        status_request = "";
        wifi_connect();
    });

    server.on("/wifi_ap", HTTP_GET, [](AsyncWebServerRequest *request){
        String user, apass;
        if (request->hasParam("user")) {
            user = request->getParam("user")->value();
        }
        if (request->hasParam("apass")) {
            apass = request->getParam("apass")->value();
        }
        save_string(controller.sta_ssid, user);
        save_string(controller.sta_pass, apass);
    request->send(SPIFFS, "/webserver_setting.html", String(), false, processor); });

    server.begin();
}
