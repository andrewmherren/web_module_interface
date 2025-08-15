#ifndef WEB_MODULE_INTERFACE_H
#define WEB_MODULE_INTERFACE_H

#include <vector>
#include <functional>
#include <map>
#include <Arduino.h>

// HTTP Methods enum
enum HTTPMethod {
  HTTP_GET = 0,
  HTTP_POST = 1,
  HTTP_PUT = 2,
  HTTP_DELETE = 3,
  HTTP_PATCH = 4
};

// Route handler function signature
typedef std::function<String(const String& requestBody, const std::map<String, String>& params)> RouteHandler;

// Web route structure
struct WebRoute {
  String path;          // Route path (e.g., "/status", "/config")
  HTTPMethod method;    // HTTP method
  RouteHandler handler; // Function pointer to handler
  String contentType;   // Optional: "text/html", "application/json"
  String description;   // Optional: Human-readable description
  
  // Constructors for convenience
  WebRoute() : method(HTTP_GET), contentType("text/html") {}
  
  WebRoute(const String& p, HTTPMethod m, RouteHandler h) 
    : path(p), method(m), handler(h), contentType("text/html") {}
    
  WebRoute(const String& p, HTTPMethod m, RouteHandler h, const String& ct)
    : path(p), method(m), handler(h), contentType(ct) {}
    
  WebRoute(const String& p, HTTPMethod m, RouteHandler h, const String& ct, const String& desc)
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
  virtual std::vector<WebRoute> getWebRoutes() {
    return getHttpRoutes();
  }
};

// Utility functions for HTTP method conversion
inline String httpMethodToString(HTTPMethod method) {
  switch (method) {
    case HTTP_GET: return "GET";
    case HTTP_POST: return "POST";
    case HTTP_PUT: return "PUT";
    case HTTP_DELETE: return "DELETE";
    case HTTP_PATCH: return "PATCH";
    default: return "UNKNOWN";
  }
}

inline HTTPMethod stringToHttpMethod(const String& method) {
  if (method == "GET") return HTTP_GET;
  if (method == "POST") return HTTP_POST;
  if (method == "PUT") return HTTP_PUT;
  if (method == "DELETE") return HTTP_DELETE;
  if (method == "PATCH") return HTTP_PATCH;
  return HTTP_GET; // Default fallback
}

#endif // WEB_MODULE_INTERFACE_H