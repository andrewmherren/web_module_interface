# Web Module Interface

A standardized interface library for creating web-enabled ESP32/ESP8266 modules that can be dynamically registered with web routers.

## Overview

This library provides an abstract interface (`IWebModule`) that ensures consistent implementation across different web-enabled modules. It supports both HTTP and HTTPS protocols and allows for flexible route registration.

## Features

- **Protocol Agnostic**: Support for both HTTP and HTTPS routes
- **Type Safety**: Compile-time enforcement of interface implementation
- **Flexible Routing**: Support for GET, POST, PUT, DELETE, and PATCH methods
- **Content Type Aware**: Built-in content type specification
- **Documentation Friendly**: Route descriptions for API documentation
- **Namespace Isolation**: Uses `WebModule` namespace to avoid platform conflicts

## Usage

### Implementing the Interface

```cpp
#include <web_module_interface.h>

class MyWebModule : public IWebModule {
private:
  String handleStatus() {
    return "{\"status\":\"online\"}";
  }
  
  String handleConfig(const String& requestBody, const std::map<String, String>& params) {
    // Process configuration update
    return "{\"success\":true}";
  }

public:
  std::vector<WebRoute> getHttpRoutes() override {
    return {
      WebRoute("/", WebModule::WM_GET, [this](const String& body, const std::map<String, String>& params) {
        return handleStatus();
      }, "application/json", "Get module status"),
      
      WebRoute("/config", WebModule::WM_POST, [this](const String& body, const std::map<String, String>& params) {
        return handleConfig(body, params);
      }, "application/json", "Update module configuration")
    };
  }
  
  std::vector<WebRoute> getHttpsRoutes() override {
    // For this example, HTTPS routes are identical to HTTP routes
    return getHttpRoutes();
  }
  
  String getModuleName() const override {
    return "MyWebModule";
  }
  
  String getModuleVersion() const override {
    return "1.0.0";
  }
};
```

### Route Registration with Web Router

```cpp
MyWebModule myModule;

// Register module with web router (recommended approach)
webRouter.registerModule("/mymodule", &myModule);

// Or register manually if needed:
auto routes = myModule.getHttpRoutes();
for (const auto& route : routes) {
  webRouter.addRoute(("/mymodule" + route.path).c_str(), 
                     route.method, 
                     [route](HTTPServerClass &server) {
                       // Convert server request to module format
                       std::map<String, String> params;
                       for (int i = 0; i < server.args(); i++) {
                         params[server.argName(i)] = server.arg(i);
                       }
                       String body = server.hasArg("plain") ? server.arg("plain") : "";
                       String response = route.handler(body, params);
                       server.send(200, route.contentType, response);
                     });
}
```

## Interface Methods

### Required Methods (Pure Virtual)

- `getHttpRoutes()`: Returns vector of HTTP routes
- `getHttpsRoutes()`: Returns vector of HTTPS routes  
- `getModuleName()`: Returns module name string

### Optional Methods (Default Implementation Provided)

- `getModuleVersion()`: Returns version string (default: "1.0.0")
- `getModuleDescription()`: Returns description string
- `getWebRoutes()`: Convenience method returning HTTP routes

## WebRoute Structure

```cpp
struct WebRoute {
  String path;                     // Route path (e.g., "/status", "/config")
  WebModule::Method method;        // HTTP method enum
  WebModule::RouteHandler handler; // Function pointer to handler
  String contentType;              // MIME type (e.g., "application/json")
  String description;              // Human-readable description
};
```

## HTTP Methods

Supported HTTP methods via enum:
- `WebModule::WM_GET`
- `WebModule::WM_POST` 
- `WebModule::WM_PUT`
- `WebModule::WM_DELETE`
- `WebModule::WM_PATCH`

## Installation

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps = 
  https://github.com/yourusername/web_module_interface.git
```

### Arduino IDE

1. Download or clone this repository
2. Copy to your Arduino libraries folder
3. Restart Arduino IDE

## Compatibility

- **Platforms**: ESP32, ESP8266
- **Framework**: Arduino
- **C++ Standard**: C++17 or later

## License

MIT License - see LICENSE file for details