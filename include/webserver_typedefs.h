#ifndef WEB_MODULE_INTERFACE_WEBSERVER_TYPEDEFS_H
#define WEB_MODULE_INTERFACE_WEBSERVER_TYPEDEFS_H

#if defined(ESP32)
#include <WebServer.h>
typedef WebServer WebServerClass;
#elif defined(ESP8266)
#include <ESP8266WebServer.h>
typedef ESP8266WebServer WebServerClass;
#endif

#endif // WEB_MODULE_INTERFACE_WEBSERVER_TYPEDEFS_H