#include <web_module_interface.h>
#include <Arduino.h>
#include <map>

// Define a custom 404 error page
const char custom404Html[] PROGMEM = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>404 - Page Not Found</title>
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <div class="error-page">
      <h1 class="error">😵 404 - Not Found</h1>
      <p class="error-description">
        We couldn't find what you were looking for. Maybe it was never here?
      </p>
      
      <div class="status-message error">
        <strong>Technical Info:</strong> The requested URL was not found on this server.
      </div>
      
      <div class="error-actions button-group">
        <a href="/" class="btn btn-primary">Home</a>
        <a href="javascript:history.back()" class="btn btn-secondary">Go Back</a>
      </div>
    </div>
  </div>
</body>
</html>
)html";

// Set up error pages during initialization
void setupErrorPages() {
  // Set custom 404 page
  IWebModule::setErrorPage(404, FPSTR(custom404Html));
  
  // Note: Other status codes will use default theme-aware error pages
}

// Handler for testing error pages
String handleErrorTest(const String &requestBody, const std::map<String, String> &params) {
  // Check for error parameter
  auto it = params.find("error");
  if (it != params.end()) {
    String errorType = it->second;
    
    // Set current path for navigation active state
    IWebModule::setCurrentPath("/error-test");

    if (errorType == "404") {
      // Return the custom 404 page
      return IWebModule::injectNavigationMenu(IWebModule::getErrorPage(404));
    } 
    else if (errorType == "500") {
      // Return the default 500 page
      return IWebModule::injectNavigationMenu(IWebModule::getErrorPage(500));
    } 
    else if (errorType == "401") {
      // Return the default 401 page
      return IWebModule::injectNavigationMenu(IWebModule::getErrorPage(401));
    }
    else if (errorType == "custom") {
      // Example of generating a custom error with specific message
      return IWebModule::injectNavigationMenu(
          IWebModule::generateDefaultErrorPage(418, "I'm a teapot! This server refuses to brew coffee."));
    }
  }
  
  // Normal test page if no error parameter
  String html = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Error Page Test</title>
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <h1>Error Page Testing</h1>
    <p>Test different error pages by clicking the links below:</p>
    
    <div class="button-group">
      <a href="/error-test?error=404" class="btn btn-secondary">Test 404 (Custom)</a>
      <a href="/error-test?error=500" class="btn btn-secondary">Test 500 (Default)</a>
      <a href="/error-test?error=401" class="btn btn-secondary">Test 401 (Default)</a>
      <a href="/error-test?error=custom" class="btn btn-warning">Test 418 (Teapot)</a>
      <a href="/this-page-does-not-exist" class="btn btn-danger">Natural 404 Error</a>
    </div>
    
    <div class="status-message info mt-3">
      <strong>How it works:</strong><br>
      • Custom 404 page replaces the default error page<br>
      • Other status codes use theme-aware default pages<br>
      • All error pages inherit your current theme<br>
      • Error pages include navigation and styling
    </div>
  </div>
</body>
</html>)html";

  // Set current path for navigation active state
  IWebModule::setCurrentPath("/error-test");
  
  // Return page with navigation menu injected
  return IWebModule::injectNavigationMenu(html);
}

// Add this to your WebModule implementation
std::vector<WebRoute> getErrorTestRoutes() {
  std::vector<WebRoute> routes;
  
  // Add test route
  routes.push_back(WebRoute(
    "/error-test",
    WebModule::WM_GET,
    handleErrorTest,
    "text/html",
    "Error page testing route"
  ));
  
  return routes;
}