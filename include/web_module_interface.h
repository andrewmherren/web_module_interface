#ifndef WEB_MODULE_INTERFACE_H
#define WEB_MODULE_INTERFACE_H

#include <Arduino.h>
#include <functional>
#include <map>
#include <vector>

// Use a completely different namespace to avoid conflicts with ESP32/ESP8266
// built-in HTTP enums
namespace WebModule {

// HTTP Methods enum - prefixed to avoid conflicts
enum Method {
  WM_GET = 0,
  WM_POST = 1,
  WM_PUT = 2,
  WM_DELETE = 3,
  WM_PATCH = 4
};

// Route handler function signature
typedef std::function<String(const String &requestBody,
                             const std::map<String, String> &params)>
    RouteHandler;

} // namespace WebModule

// Web route structure
struct WebRoute {
  String path;                     // Route path (e.g., "/status", "/config")
  WebModule::Method method;        // HTTP method
  WebModule::RouteHandler handler; // Function pointer to handler
  String contentType;              // Optional: "text/html", "application/json"
  String description;              // Optional: Human-readable description

  // Constructors for convenience
  WebRoute() : method(WebModule::WM_GET), contentType("text/html") {}

  WebRoute(const String &p, WebModule::Method m, WebModule::RouteHandler h)
      : path(p), method(m), handler(h), contentType("text/html") {}

  WebRoute(const String &p, WebModule::Method m, WebModule::RouteHandler h,
           const String &ct)
      : path(p), method(m), handler(h), contentType(ct) {}

  WebRoute(const String &p, WebModule::Method m, WebModule::RouteHandler h,
           const String &ct, const String &desc)
      : path(p), method(m), handler(h), contentType(ct), description(desc) {}
};

// Abstract interface that all web modules must implement
class IWebModule {
public:
  virtual ~IWebModule() = default;

  // Required methods - pure virtual to enforce implementation
  virtual std::vector<WebRoute> getHttpRoutes() = 0;
  virtual std::vector<WebRoute> getHttpsRoutes() = 0;
  virtual String getModuleName() const = 0;

  // Optional methods with default implementations
  virtual String getModuleVersion() const { return "1.0.0"; }
  virtual String getModuleDescription() const { return "Web-enabled module"; }

  // Convenience method for modules with identical HTTP/HTTPS routes
  virtual std::vector<WebRoute> getWebRoutes() { return getHttpRoutes(); }
};

// Utility functions for HTTP method conversion
inline String httpMethodToString(WebModule::Method method) {
  switch (method) {
  case WebModule::WM_GET:
    return "GET";
  case WebModule::WM_POST:
    return "POST";
  case WebModule::WM_PUT:
    return "PUT";
  case WebModule::WM_DELETE:
    return "DELETE";
  case WebModule::WM_PATCH:
    return "PATCH";
  default:
    return "UNKNOWN";
  }
}

inline WebModule::Method stringToHttpMethod(const String &method) {
  if (method == "GET")
    return WebModule::WM_GET;
  if (method == "POST")
    return WebModule::WM_POST;
  if (method == "PUT")
    return WebModule::WM_PUT;
  if (method == "DELETE")
    return WebModule::WM_DELETE;
  if (method == "PATCH")
    return WebModule::WM_PATCH;
  return WebModule::WM_GET; // Default fallback
}

#endif // WEB_MODULE_INTERFACE_H