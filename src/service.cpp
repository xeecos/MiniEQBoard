#include "service.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
#include "log.h"
#include "stepper.h"

WebServer server(80);
void service_init()
{
    WiFi.softAP("ESP-Stepper", "");
    
    server.onNotFound([=]()
    {
        String uri = server.uri();
        LOG_UART("uri:%s\n",uri.c_str());
        server.send(200, "text/plain", "hello from ESPStepper!"); 
    });
    server.on("/stepper/set", HTTP_GET, [=]()
    { 
        if(server.hasArg("current"))
        {
            stepper_current(server.arg("current").toInt());
        }
        else if(server.hasArg("microsteps"))
        {
            stepper_microsteps(server.arg("microsteps").toInt());
        }
        else if(server.hasArg("ratio"))
        {
            stepper_ratio(server.arg("ratio").toDouble());
        }
        server.send(200, "text/json", "{\"err\":0}\0"); 
    });
    server.on("/move", HTTP_GET, [=]()
    { 
        stepper_move(server.arg("distance").toInt());
        server.send(200, "text/json", "{\"err\":0}\0"); 
    });
    server.on("/moveto", HTTP_GET, [=]()
    { 
        stepper_moveTo(server.arg("position").toInt());
        server.send(200, "text/json", "{\"err\":0}\0"); 
    });
    server.on("/speed/set", HTTP_GET, [=]()
    { 
        stepper_runSpeed(server.arg("speed").toDouble());
        server.send(200, "text/json", "{\"err\":0}\0"); 
    });
    server.on("/reset", HTTP_GET, [=]()
    { 
        stepper_reset();
        server.send(200, "text/json", "{\"err\":0}\0"); 
    });
    server.on("/version", HTTP_GET, [=]()
    { 
        server.send(200, "text/json", "{\"err\":0,\"name\":\"ESPStepper\",\"version\":\"1.0.0\"}\0"); 
    });
    server.enableCrossOrigin(true);
    server.enableCORS(true);
    server.enableDelay(true);
    SPIFFS.begin(true);
    server.serveStatic("/", SPIFFS, "/");
    server.begin();
}

void service_run()
{
    server.handleClient();
    delay(1);
}