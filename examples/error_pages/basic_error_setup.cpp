#include <web_module_interface.h>
#include <Arduino.h>

// Custom 404 Not Found error page
const char myCustom404Html[] PROGMEM = R"html(
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
      <h1 class="error">404 - Page Not Found</h1>
      <p class="error-description">
        Sorry, the page you're looking for doesn't exist.
      </p>
      
      <div class="status-message error">
        <strong>Technical Info:</strong> The requested URL was not found on this server.
      </div>
      
      <div class="error-actions button-group">
        <a href="/" class="btn btn-primary">Home Page</a>
        <a href="javascript:history.back()" class="btn btn-secondary">Go Back</a>
      </div>
    </div>
  </div>
</body>
</html>
)html";

void setupErrorPages() {
  // Set custom 404 page
  IWebModule::setErrorPage(404, FPSTR(myCustom404Html));
  
  // Note: Other error codes (500, 403, etc.) will use the default error pages
  // which automatically adapt to your theme
}

// Example of how to use error pages in a WebRouter or custom module
String handleRequest(const String &requestBody, const std::map<String, String> &params) {
  // Check if the request should return an error
  bool resourceExists = false; // Your logic to check if resource exists
  
  if (!resourceExists) {
    // Set the current path for proper navigation highlighting
    IWebModule::setCurrentPath("/your/current/path");
    
    // Return the 404 error page with navigation menu injected
    return IWebModule::injectNavigationMenu(IWebModule::getErrorPage(404));
  }
  
  // Normal request handling...
  return "Resource content";
}