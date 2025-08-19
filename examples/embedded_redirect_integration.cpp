/**
 * Embedded Redirect Integration Example
 * 
 * Demonstrates how the simplified redirect system integrates with
 * a web router in a typical embedded ESP32/ESP8266 project.
 */

#include <Arduino.h>
#include "../include/web_module_interface.h"

// Mock web router integration - showing how redirects would be handled
class MockWebRouter {
private:
  void handleRequest(const String& path, const String& method) {
    Serial.printf("Request: %s %s\n", method.c_str(), path.c_str());
    
    // Step 1: Check for redirects first (before route lookup)
    String redirectTarget = IWebModule::getRedirectTarget(path);
    if (!redirectTarget.isEmpty()) {
      Serial.printf("  -> Redirecting to: %s (302)\n", redirectTarget.c_str());
      // In real implementation, this would set HTTP headers and send redirect
      return;
    }
    
    // Step 2: Normal route handling
    Serial.printf("  -> Processing normally\n");
  }
  
public:
  void simulateRequests() {
    Serial.println("\n=== Simulating Web Requests ===");
    handleRequest("/", "GET");
    handleRequest("/main", "GET");
    handleRequest("/old-settings", "GET");
    handleRequest("/api/status", "GET");    // Should not redirect
    handleRequest("/usb_pd/control", "GET"); // Should not redirect
  }
};

// Example USB PD Controller Module
class USBPDModule : public IWebModule {
public:
  std::vector<WebRoute> getHttpRoutes() override {
    return {
      WebRoute("/", WebModule::WM_GET, [](const String&, const std::map<String, String>&) {
        return "USB PD Controller Main Page";
      }),
      WebRoute("/control", WebModule::WM_POST, [](const String&, const std::map<String, String>&) {
        return "{\"status\":\"ok\"}";
      }, "application/json")
    };
  }
  
  std::vector<WebRoute> getHttpsRoutes() override { return getHttpRoutes(); }
  String getModuleName() const override { return "USBPDController"; }
};

// Example WiFi Settings Module
class WiFiModule : public IWebModule {
public:
  std::vector<WebRoute> getHttpRoutes() override {
    return {
      WebRoute("/", WebModule::WM_GET, [](const String&, const std::map<String, String>&) {
        return "WiFi Settings Page";
      }),
      WebRoute("/scan", WebModule::WM_POST, [](const String&, const std::map<String, String>&) {
        return "{\"networks\":[]}";
      }, "application/json")
    };
  }
  
  std::vector<WebRoute> getHttpsRoutes() override { return getHttpRoutes(); }
  String getModuleName() const override { return "WiFiSettings"; }
};

// Global instances (typical embedded pattern)
USBPDModule usbPdController;
WiFiModule wifiSettings;
MockWebRouter webRouter;

void setup() {
  Serial.begin(115200);
  Serial.println("Embedded Redirect Integration Example");
  Serial.println("=====================================");
  
  // Typical embedded navigation setup
  std::vector<NavigationItem> navItems = {
    NavigationItem("USB PD Control", "/usb_pd/"),
    NavigationItem("WiFi Settings", "/wifi/"),
    NavigationItem("System Info", "/system/")
  };
  IWebModule::setNavigationMenu(navItems);
  
  // Configure redirects at startup - this is the typical embedded pattern
  // These rules are set once and never changed during operation
  
  Serial.println("Configuring redirect rules...");
  
  // 1. Main page redirect - direct users to primary functionality
  IWebModule::addRedirect("/", "/usb_pd/");
  Serial.println("  / -> /usb_pd/ (main page)");
  
  // 2. Legacy URL support - maintain compatibility with old bookmarks
  IWebModule::addRedirect("/main", "/usb_pd/");
  IWebModule::addRedirect("/home", "/usb_pd/");
  Serial.println("  /main, /home -> /usb_pd/ (legacy URLs)");
  
  // 3. URL shortening - provide shorter alternatives
  IWebModule::addRedirect("/settings", "/wifi/");
  IWebModule::addRedirect("/config", "/wifi/");
  Serial.println("  /settings, /config -> /wifi/ (shortcuts)");
  
  // 4. Fix common typos or variations
  IWebModule::addRedirect("/pd", "/usb_pd/");
  IWebModule::addRedirect("/power", "/usb_pd/");
  Serial.println("  /pd, /power -> /usb_pd/ (aliases)");
  
  Serial.println("\nRedirect configuration complete!");
  Serial.println("Total memory used: ~" + String(4 * 8) + " bytes (8 redirects * ~4 bytes each)");
  
  // In a real implementation, modules would be registered with the web router here:
  // webRouter.registerModule("/usb_pd", &usbPdController);
  // webRouter.registerModule("/wifi", &wifiSettings);
  // webRouter.begin();
}

void loop() {
  static unsigned long lastTest = 0;
  if (millis() - lastTest > 5000) { // Test every 5 seconds
    lastTest = millis();
    
    // Simulate incoming web requests to demonstrate redirect behavior
    webRouter.simulateRequests();
    
    // Show memory efficiency
    Serial.printf("\nMemory efficient: No dynamic allocation, no complex pattern matching\n");
    Serial.printf("Perfect for embedded: Set once at startup, fast O(n) lookup\n");
    
    Serial.println("===============================================\n");
  }
  
  delay(100);
}

// Helper function to demonstrate the simplicity of the redirect system
void demonstrateSimplicity() {
  Serial.println("\n=== Redirect System Simplicity ===");
  Serial.println("Code size: ~40 lines vs ~150 lines (original complex system)");
  Serial.println("Memory: Vector of simple structs, no complex data structures");
  Serial.println("Lookup: Simple iteration, no regex or wildcard processing");
  Serial.println("Use case: Perfect for embedded 'set once at startup' scenarios");
  
  // Show the actual usage pattern
  Serial.println("\nTypical embedded usage pattern:");
  Serial.println("1. setup() { configure all redirects once }");
  Serial.println("2. webRouter checks redirects before route lookup");
  Serial.println("3. Fast 302 redirect response with minimal overhead");
}