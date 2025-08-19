/**
 * Embedded Redirect Integration Example
 * 
 * Shows how to integrate the redirection system into a complete
 * embedded web application with multiple modules.
 */

#include <Arduino.h>
#include <web_module_interface.h>

// Forward declarations of our modules
class MainModule : public IWebModule {
public:
  std::vector<WebRoute> getHttpRoutes() override;
  std::vector<WebRoute> getHttpsRoutes() override { return getHttpRoutes(); }
  String getModuleName() const override { return "MainModule"; }
};

class SettingsModule : public IWebModule {
public:
  std::vector<WebRoute> getHttpRoutes() override;
  std::vector<WebRoute> getHttpsRoutes() override { return getHttpRoutes(); }
  String getModuleName() const override { return "SettingsModule"; }
};

// Global instances
MainModule mainModule;
SettingsModule settingsModule;

// Main application setup
void setupRedirects() {
  // Home page configuration
  IWebModule::addRedirect("/", "/main/");      // Root redirects to main module
  
  // Module shortcuts
  IWebModule::addRedirect("/m", "/main/");     // Short alias for main module
  IWebModule::addRedirect("/s", "/settings/"); // Short alias for settings
  
  // Legacy URL support (hypothetical example of app evolution)
  IWebModule::addRedirect("/config", "/settings/");
  IWebModule::addRedirect("/preferences", "/settings/");
  IWebModule::addRedirect("/dashboard", "/main/");
  
  // External redirects
  IWebModule::addRedirect("/github", "https://github.com/yourusername/yourrepo");
  IWebModule::addRedirect("/docs", "https://example.com/documentation");
  
  // Feature-specific shortcuts
  IWebModule::addRedirect("/wifi", "/settings/wifi/");
  IWebModule::addRedirect("/theme", "/settings/appearance/");
  
  Serial.println("Redirect system configured with 10 rules");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Embedded Redirect Integration Example");
  
  // Set up global CSS theme
  IWebModule::setGlobalCSS(R"CSS(
    body { font-family: Arial, sans-serif; margin: 0; padding: 0; }
    .container { max-width: 1200px; margin: 0 auto; padding: 20px; }
    .btn { display: inline-block; padding: 8px 16px; margin: 5px; text-decoration: none;
           background: #f0f0f0; border-radius: 4px; color: #333; }
    .btn:hover { background: #e0e0e0; }
    .success { color: green; }
    .error { color: red; }
  )CSS");
  
  // Set up navigation menu
  std::vector<NavigationItem> navItems = {
    NavigationItem("Home", "/main/"),
    NavigationItem("Settings", "/settings/"),
    NavigationItem("GitHub", "/github", "_blank"),
    NavigationItem("Documentation", "/docs", "_blank")
  };
  IWebModule::setNavigationMenu(navItems);
  
  // Set up redirect rules
  setupRedirects();
  
  // Initialize web router and register modules
  // (simplified - actual implementation would use WebRouter class)
  Serial.println("Registering modules:");
  Serial.println("  /main/ -> MainModule");
  Serial.println("  /settings/ -> SettingsModule");
  
  // Start the web server
  Serial.println("Starting web server...");
  Serial.println("Server started at http://localhost:80");
}

void loop() {
  // Simulate handling requests
  static unsigned long lastSimulation = 0;
  if (millis() - lastSimulation > 5000) { // Every 5 seconds
    lastSimulation = millis();
    
    // Simulate some requests to demonstrate redirects
    simulateRequest("/");
    simulateRequest("/m");
    simulateRequest("/config");
    simulateRequest("/wifi");
    simulateRequest("/nonexistent");
    
    Serial.println("---");
  }
  
  delay(100);
}

// Simulated route handling
void simulateRequest(const String& path) {
  String redirectTarget = IWebModule::getRedirectTarget(path);
  
  if (!redirectTarget.isEmpty()) {
    Serial.printf("Redirect: %s -> %s\n", path.c_str(), redirectTarget.c_str());
    
    // Determine if this is an external redirect
    if (redirectTarget.startsWith("http://") || redirectTarget.startsWith("https://")) {
      Serial.println("  External redirect - sending 302 to browser");
    } else {
      // For internal redirects, we could simulate following the redirect
      Serial.println("  Internal redirect - following");
      
      // In a real implementation, this would be handled by the web router
      if (redirectTarget.startsWith("/main/")) {
        Serial.println("  Routing to MainModule");
      } else if (redirectTarget.startsWith("/settings/")) {
        Serial.println("  Routing to SettingsModule");
      }
    }
  } else {
    // No redirect - normal request handling
    Serial.printf("Direct request: %s\n", path.c_str());
    
    // In a real implementation, this would be handled by the web router
    if (path.startsWith("/main/")) {
      Serial.println("  Routing to MainModule");
    } else if (path.startsWith("/settings/")) {
      Serial.println("  Routing to SettingsModule");
    } else {
      Serial.println("  No matching route - 404 Not Found");
    }
  }
}

// Module implementations
std::vector<WebRoute> MainModule::getHttpRoutes() {
  return {
    WebRoute("/", WebModule::WM_GET, 
      [](const String& body, const std::map<String, String>& params) {
        return "Main Module Homepage";
      }, "text/html")
  };
}

std::vector<WebRoute> SettingsModule::getHttpRoutes() {
  return {
    WebRoute("/", WebModule::WM_GET, 
      [](const String& body, const std::map<String, String>& params) {
        return "Settings Module Homepage";
      }, "text/html"),
    WebRoute("/wifi/", WebModule::WM_GET, 
      [](const String& body, const std::map<String, String>& params) {
        return "WiFi Settings Page";
      }, "text/html"),
    WebRoute("/appearance/", WebModule::WM_GET, 
      [](const String& body, const std::map<String, String>& params) {
        return "Appearance Settings Page";
      }, "text/html")
  };
}