#ifndef WEB_MODULE_INTERFACE_H
#define WEB_MODULE_INTERFACE_H

#include "web_ui_styles.h"
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

// Navigation menu item structure
struct NavigationItem {
  String name;   // Display name for the menu item
  String url;    // URL the menu item links to
  String target; // Optional: target attribute for the link (e.g., "_blank")
  
  // Constructors for convenience
  NavigationItem(const String &n, const String &u)
      : name(n), url(u), target("") {}
      
  NavigationItem(const String &n, const String &u, const String &t)
      : name(n), url(u), target(t) {}
};

// Abstract interface that all web modules must implement
class IWebModule {
private:
  static String globalCSS;
  static bool globalCSSSet;
  static std::vector<NavigationItem> navigationMenu;
  static String currentPath;  // Store the current request path for auto-active detection

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

  // Phase 1: Custom CSS System
  // Global CSS management - affects all modules
  static void setGlobalCSS(const String &css);
  static String getGlobalCSS();

  // Helper method to get CSS route for serving at /assets/style.css
  static WebRoute getCSSRoute();

  // Helper method to inject CSS link into HTML pages
  static String injectCSSLink(const String &htmlContent);

  // Phase 2: Navigation Menu System
  // Navigation menu management
  static void setNavigationMenu(const std::vector<NavigationItem> &items);
  static std::vector<NavigationItem> getNavigationMenu();
  
  // Set current path for auto-active detection in navigation
  static void setCurrentPath(const String &path);
  static String getCurrentPath();
  
  // Helper methods for navigation menu
  static String generateNavigationHtml();
  static String injectNavigationMenu(const String &htmlContent);
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