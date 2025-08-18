#include "web_module_interface.h"

// Example demonstrating Error Page Customization (Phase 4)
// This shows how to create custom error pages that work with any theme

void setupCustomErrorPages() {
  // Example 1: Custom 404 page with helpful navigation
  String custom404 = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Page Not Found</title>
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <div class="error-page">
      <h1 class="error">🔍 Page Not Found</h1>
      <p class="error-description">
        Oops! The page you're looking for seems to have wandered off into the digital void.
      </p>
      
      <div class="status-grid">
        <div class="status-card">
          <h3>Quick Links</h3>
          <div class="button-group">
            <a href="/" class="btn btn-primary">Home</a>
            <a href="/usb_pd/" class="btn btn-secondary">USB PD Controller</a>
            <a href="/wifi/" class="btn btn-secondary">WiFi Settings</a>
          </div>
        </div>
        
        <div class="status-card">
          <h3>Search Tips</h3>
          <p>Try:</p>
          <ul style="margin: 10px 0; padding-left: 20px; color: inherit;">
            <li>Checking your URL spelling</li>
            <li>Using the navigation menu</li>
            <li>Starting from the home page</li>
          </ul>
        </div>
      </div>
      
      <div class="status-message info">
        <strong>💡 Developer Tip:</strong> This error page adapts to your current theme automatically!
      </div>
    </div>
  </div>
</body>
</html>)html";

  // Example 2: Custom 500 error page with diagnostic info
  String custom500 = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Server Error</title>
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <div class="error-page">
      <h1 class="error">⚠️ Internal Server Error</h1>
      <p class="error-description">
        Something went wrong on our end. The server encountered an unexpected condition.
      </p>
      
      <div class="status-message error">
        <strong>Error 500:</strong> Internal Server Error<br>
        This is usually a temporary issue. Please try again in a moment.
      </div>
      
      <div class="status-grid">
        <div class="status-card">
          <h3>What you can do:</h3>
          <p>
            • Refresh the page<br>
            • Try again in a few minutes<br>
            • Contact support if the issue persists
          </p>
        </div>
        
        <div class="status-card">
          <h3>System Status</h3>
          <p class="info">Most services are operational</p>
          <div class="button-group">
            <a href="/" class="btn btn-primary">Return Home</a>
            <a href="javascript:location.reload()" class="btn btn-secondary">Retry</a>
          </div>
        </div>
      </div>
    </div>
  </div>
</body>
</html>)html";

  // Example 3: Custom 403 Forbidden page
  String custom403 = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Access Forbidden</title>
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <div class="error-page">
      <h1 class="warning">🚫 Access Forbidden</h1>
      <p class="error-description">
        You don't have permission to access this resource on this server.
      </p>
      
      <div class="status-message warning">
        <strong>HTTP 403:</strong> This area requires special permissions or authentication.
      </div>
      
      <div class="error-actions button-group">
        <a href="/" class="btn btn-primary">Go to Home</a>
        <a href="/login" class="btn btn-secondary">Login</a>
        <a href="javascript:history.back()" class="btn btn-secondary">Go Back</a>
      </div>
      
      <div class="footer">
        <p>If you believe you should have access, please contact your administrator.</p>
      </div>
    </div>
  </div>
</body>
</html>)html";

  // Apply the custom error pages
  IWebModule::setErrorPage(404, custom404);
  IWebModule::setErrorPage(500, custom500);
  IWebModule::setErrorPage(403, custom403);
  
  // Note: Pages not set will use the default error page generator
  // which automatically adapts to the current theme
}

void setupNavigationAndThemeExample() {
  // Set up navigation menu that will appear in error pages too
  std::vector<NavigationItem> nav;
  nav.push_back(NavigationItem("Home", "/"));
  nav.push_back(NavigationItem("USB PD", "/usb_pd/"));
  nav.push_back(NavigationItem("WiFi", "/wifi/"));
  nav.push_back(NavigationItem("Settings", "/settings/"));
  
  IWebModule::setNavigationMenu(nav);
  
  // Example of how error pages work with different themes
  // Uncomment one of these to test with different themes:
  
  // Option 1: Use default theme (from web_ui_styles.h)
  // IWebModule::setGlobalCSS(WEB_UI_DEFAULT_CSS);
  
  // Option 2: Use custom theme (you would include example_themes.h)
  // IWebModule::setGlobalCSS(FPSTR(THEME_DARK_FUSCHIA));
  // IWebModule::setGlobalCSS(FPSTR(THEME_OCEAN_BLUE));
  // IWebModule::setGlobalCSS(FPSTR(THEME_LIGHT));
}

// Example of how to use error pages in your web router
String handleExampleRoute(const String &requestBody, const std::map<String, String> &params) {
  // Simulate different error conditions for testing
  auto it = params.find("error");
  if (it != params.end()) {
    String errorType = it->second;
    
    if (errorType == "404") {
      // This would normally be handled by the web router automatically
      // but you can manually trigger it for testing:
      return IWebModule::getErrorPage(404);
    }
    else if (errorType == "500") {
      return IWebModule::getErrorPage(500);
    }
    else if (errorType == "403") {
      return IWebModule::getErrorPage(403);
    }
    else if (errorType == "custom") {
      // Example of using generateDefaultErrorPage with custom message
      return IWebModule::generateDefaultErrorPage(418, "I'm a teapot! This is a custom error message.");
    }
  }
  
  // Normal response
  String html = R"html(
<!DOCTYPE html>
<html>
<head>
  <title>Error Page Test</title>
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <h1>Error Page Testing</h1>
    <p>Click these links to test different error pages:</p>
    
    <div class="button-group">
      <a href="/test?error=404" class="btn btn-secondary">Test 404 (Custom)</a>
      <a href="/test?error=500" class="btn btn-secondary">Test 500 (Custom)</a>
      <a href="/test?error=403" class="btn btn-secondary">Test 403 (Custom)</a>
      <a href="/test?error=custom" class="btn btn-secondary">Test 418 (Default)</a>
    </div>
    
    <div class="status-message info">
      All error pages automatically inherit your current theme and include navigation!
    </div>
  </div>
</body>
</html>)html";

  return IWebModule::injectNavigationMenu(html);
}

// Usage in main.cpp setup():
/*
void setup() {
  // ... other initialization ...
  
  // Set up custom error pages
  setupCustomErrorPages();
  setupNavigationAndThemeExample();
  
  // ... register modules with web router ...
}
*/