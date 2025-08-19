/**
 * Simple Redirect Example
 * 
 * Demonstrates the simplified embedded redirection system
 * that provides lightweight exact path matching only.
 */

#include <Arduino.h>
#include "../include/web_module_interface.h"

// Example module implementing the interface
class SimpleWebModule : public IWebModule {
private:
  String getMainPage() {
    String html = R"html(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Simple Module</title>
    </head>
    <body>
      <div class="container">
        <!-- Navigation menu will be auto-injected here -->
        
        <h1>Simple Web Module</h1>
        <p class="success">This is the main module page.</p>
        
        <div class="button-group">
          <a href="/old-url" class="btn btn-secondary">Test Old URL (should redirect)</a>
          <a href="/main" class="btn btn-secondary">Test /main (should redirect)</a>
          <a href="/config" class="btn btn-secondary">Test /config (should redirect)</a>
        </div>
      </div>
    </body>
    </html>
    )html";
    
    // Set current path and inject navigation/CSS
    IWebModule::setCurrentPath("/simple/");
    html = IWebModule::injectCSSLink(html);
    html = IWebModule::injectNavigationMenu(html);
    
    return html;
  }

public:
  std::vector<WebRoute> getHttpRoutes() override {
    return {
      WebRoute("/", WebModule::WM_GET, [this](const String& body, const std::map<String, String>& params) {
        return getMainPage();
      }, "text/html", "Simple module main page")
    };
  }
  
  std::vector<WebRoute> getHttpsRoutes() override {
    return getHttpRoutes();
  }
  
  String getModuleName() const override {
    return "SimpleWebModule";
  }
};

// Global module instance
SimpleWebModule simpleModule;

void setup() {
  Serial.begin(115200);
  Serial.println("Simple Redirect Example");
  
  // Set up basic navigation menu
  std::vector<NavigationItem> navItems = {
    NavigationItem("Home", "/simple/"),
    NavigationItem("Other Module", "/other/"),
    NavigationItem("Settings", "/settings/")
  };
  IWebModule::setNavigationMenu(navItems);
  
  // Configure simple redirects - typical embedded use case
  // These are set once at startup and never changed
  
  // Redirect root to main module
  IWebModule::addRedirect("/", "/simple/");
  
  // Redirect old URLs to new locations
  IWebModule::addRedirect("/old-url", "/simple/");
  IWebModule::addRedirect("/main", "/simple/");
  IWebModule::addRedirect("/config", "/settings/");
  
  Serial.println("Redirect rules configured:");
  Serial.println("  / -> /simple/");
  Serial.println("  /old-url -> /simple/");
  Serial.println("  /main -> /simple/");
  Serial.println("  /config -> /settings/");
}

void loop() {
  // Test the redirect system
  static unsigned long lastTest = 0;
  if (millis() - lastTest > 10000) { // Test every 10 seconds
    lastTest = millis();
    
    // Test some paths to see if they redirect
    testRedirect("/");
    testRedirect("/main");
    testRedirect("/config");
    testRedirect("/nonexistent"); // Should not redirect
    
    Serial.println("---");
  }
  
  delay(100);
}

void testRedirect(const String& path) {
  String target = IWebModule::getRedirectTarget(path);
  if (!target.isEmpty()) {
    Serial.printf("✓ Redirect: %s -> %s\n", path.c_str(), target.c_str());
  } else {
    Serial.printf("✗ No redirect: %s\n", path.c_str());
  }
}