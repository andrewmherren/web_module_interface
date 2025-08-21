/*
 * Basic Implementation Example
 *
 * This example demonstrates how to implement the IWebModule interface
 * for a simple web-enabled module.
 */

#include <web_module_interface.h>

class ExampleWebModule : public IWebModule {
private:
  int counter = 0;

  String handleHomePage() {
    // Set current path for navigation active state
    IWebModule::setCurrentPath("/");

    String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Example Module</title>
    <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
    <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <h1>Example Web Module</h1>
    <p>Counter: )" + String(counter) + R"(</p>
    <button onclick="fetch('/api/increment', {method: 'POST'}).then(() => location.reload())">
        Increment Counter
    </button>
    </div>
</body>
</html>)";
    
    // Inject navigation menu into HTML
    return IWebModule::injectNavigationMenu(html);
  }

  String handleStatus() {
    return "{\"module\":\"" + getModuleName() + "\",\"version\":\"" +
           getModuleVersion() + "\",\"counter\":" + String(counter) + "}";
  }

  String handleIncrement(const String &requestBody,
                         const std::map<String, String> &params) {
    counter++;
    return "{\"success\":true,\"counter\":" + String(counter) + "}";
  }

  String handleReset(const String &requestBody,
                     const std::map<String, String> &params) {
    counter = 0;
    return "{\"success\":true,\"counter\":" + String(counter) + "}";
  }

public:
  std::vector<WebRoute> getHttpRoutes() override {
    return {
        WebRoute(
            "/", WebModule::WM_GET,
            [this](const String &body, const std::map<String, String> &params) {
              return handleHomePage();
            },
            "text/html", "Home page with counter display"),

        WebRoute(
            "/api/status", WebModule::WM_GET,
            [this](const String &body, const std::map<String, String> &params) {
              return handleStatus();
            },
            "application/json", "Get module status and counter value"),

        WebRoute(
            "/api/increment", WebModule::WM_POST,
            [this](const String &body, const std::map<String, String> &params) {
              return handleIncrement(body, params);
            },
            "application/json", "Increment the counter"),

        WebRoute(
            "/api/reset", WebModule::WM_POST,
            [this](const String &body, const std::map<String, String> &params) {
              return handleReset(body, params);
            },
            "application/json", "Reset the counter to zero")};
  }

  std::vector<WebRoute> getHttpsRoutes() override {
    // For this example, HTTPS routes are identical to HTTP routes
    return getHttpRoutes();
  }

  String getModuleName() const override { return "ExampleWebModule"; }

  String getModuleVersion() const override { return "1.0.0"; }

  String getModuleDescription() const override {
    return "Example web module demonstrating interface implementation";
  }
};

// Global instance
ExampleWebModule exampleModule;

void setup() {
  Serial.begin(115200);
  Serial.println("Example Web Module Interface Demo");

  // Initialize default theme
  IWebModule::initializeDefaultTheme();

  // Set up navigation menu
  std::vector<NavigationItem> navItems = {
      NavigationItem("Home", "/"), NavigationItem("API Status", "/api/status")};
  IWebModule::setNavigationMenu(navItems);

  // Print module information
  Serial.println("Module Name: " + exampleModule.getModuleName());
  Serial.println("Module Version: " + exampleModule.getModuleVersion());
  Serial.println("Module Description: " + exampleModule.getModuleDescription());

  // Print available routes
  Serial.println("\nAvailable HTTP Routes:");
  auto httpRoutes = exampleModule.getHttpRoutes();
  for (const auto &route : httpRoutes) {
    Serial.println("  " + httpMethodToString(route.method) + " " + route.path +
                   " (" + route.contentType + ") - " + route.description);
  }

  Serial.println("\nAvailable HTTPS Routes:");
  auto httpsRoutes = exampleModule.getHttpsRoutes();
  for (const auto &route : httpsRoutes) {
    Serial.println("  " + httpMethodToString(route.method) + " " + route.path +
                   " (" + route.contentType + ") - " + route.description);
  }
}

void loop() {
  // In a real implementation, you would:
  // 1. Initialize WiFi connection
  // 2. Create and configure web router
  // 3. Register routes from this module
  // 4. Call webRouter.handle() in the loop

  delay(1000);
}