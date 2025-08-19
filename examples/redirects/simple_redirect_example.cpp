/**
 * Simple Redirect Example
 * 
 * Demonstrates the simplified embedded redirection system
 * that provides lightweight exact path matching only.
 */

#include <Arduino.h>
#include <web_module_interface.h>

// Example module implementing the interface
class SimpleWebModule : public IWebModule {
private:
  String getMainPage() {
    String html = R"html(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Simple Module</title>
      <link rel="stylesheet" href="/assets/style.css">
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
          <a href="/external" class="btn btn-warning">Test External Redirect</a>
        </div>
      </div>
    </body>
    </html>
    )html";
    
    // Set current path and inject navigation
    IWebModule::setCurrentPath("/simple/");
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
  
  // Configure redirects
  
  // Internal redirects - pages within your application
  IWebModule::addRedirect("/", "/simple/");        // Redirect root to main module
  IWebModule::addRedirect("/old-url", "/simple/"); // Redirect legacy URL
  IWebModule::addRedirect("/main", "/simple/");    // Shortcut to main page
  IWebModule::addRedirect("/config", "/settings/"); // Intuitive alias for settings
  
  // External redirect - to another website
  IWebModule::addRedirect("/external", "https://www.example.com");
  
  // Module shortcuts - shorter URLs to access modules
  IWebModule::addRedirect("/s", "/simple/");       // Short module alias
  IWebModule::addRedirect("/o", "/other/");        // Short module alias
  
  Serial.println("Redirect rules configured:");
  Serial.println("  / -> /simple/");
  Serial.println("  /old-url -> /simple/");
  Serial.println("  /main -> /simple/");
  Serial.println("  /config -> /settings/");
  Serial.println("  /external -> https://www.example.com");
  Serial.println("  /s -> /simple/");
  Serial.println("  /o -> /other/");
}

void loop() {
  // Your main loop code
  delay(100);
}