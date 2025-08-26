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

// Redirect structure for managing URL redirects (simplified for embedded use)
struct RedirectRule {
  String fromPath; // Source path to redirect from
  String toPath;   // Destination path to redirect to

  // Constructor for convenience
  RedirectRule(const String &from, const String &to)
      : fromPath(from), toPath(to) {}
};

// Static asset structure for serving files like JS, images, fonts
struct StaticAsset {
  String path;     // Asset path (e.g., "/assets/script.js", "/assets/logo.png")
  String content;  // Asset content (can be stored in PROGMEM)
  String mimeType; // MIME type (e.g., "application/javascript", "image/png")
  bool useProgmem; // Whether content is stored in PROGMEM

  // Constructors for convenience
  StaticAsset(const String &p, const String &c, const String &m)
      : path(p), content(c), mimeType(m), useProgmem(false) {}

  StaticAsset(const String &p, const String &c, const String &m, bool progmem)
      : path(p), content(c), mimeType(m), useProgmem(progmem) {}
};

// Abstract interface that all web modules must implement
class IWebModule {
private:
  static bool defaultThemeInitialized; // Track default theme initialization
  static std::vector<NavigationItem> navigationMenu;
  static String
      currentPath; // Store the current request path for auto-active detection
  static std::map<int, String> errorPages; // Custom error pages by status code
  static std::vector<RedirectRule> redirectRules; // URL redirect rules
  static std::vector<StaticAsset>
      staticAssets; // Static assets (JS, images, etc.)

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

  // Initialize CSS for /assets/style.css - can use default or custom CSS
  static void initializeCSS(const String &customCSS = "");

  // Add additional CSS to existing stylesheet (must be called after
  // initializeCSS)
  static void addCustomCSS(const String &additionalCSS);

  // Backward compatibility - calls initializeCSS() with default CSS
  static void initializeDefaultTheme() { initializeCSS(); }

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

  // Custom error page management
  static void setErrorPage(int statusCode, const String &html);
  static String getErrorPage(int statusCode);

  // Helper method to generate default error pages with global CSS
  static String generateDefaultErrorPage(int statusCode,
                                         const String &message = "");

  // Phase 3: Route Redirection System (simplified for embedded use)
  // Add URL redirect rule (302 temporary redirect)
  static void addRedirect(const String &fromPath, const String &toPath);

  // Check if a path matches any redirect rule and return the redirect target
  // Returns empty string if no redirect matches
  static String getRedirectTarget(const String &requestPath);

  // Phase 5: Static Asset Management
  // Add static asset (JavaScript, images, fonts, etc.)
  static void addStaticAsset(const String &path, const String &content,
                             const String &mimeType, bool useProgmem = false);

  // Get static asset by path - returns empty StaticAsset if not found
  static StaticAsset getStaticAsset(const String &path);

  // Check if a path matches any static asset
  static bool hasStaticAsset(const String &path);

  // Get all static asset routes for registration with web router
  static std::vector<WebRoute> getStaticAssetRoutes();

  // Helper methods for common static assets
  static void addJavaScript(const String &path, const String &jsCode,
                            bool useProgmem = false);
  static void addImage(const String &path, const String &imageData,
                       const String &imageType = "png",
                       bool useProgmem = false);
  static void addFont(const String &path, const String &fontData,
                      const String &fontType = "ttf", bool useProgmem = false);
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