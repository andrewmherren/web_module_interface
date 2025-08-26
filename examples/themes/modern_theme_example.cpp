#include <Arduino.h>
#include <web_module_interface.h>

// Include theme examples
#include "example_themes.h"

// Example 1: Using default CSS only
void setupDefaultTheme() {
  // Case 1: Use built-in default CSS
  IWebModule::initializeCSS();
}

// Example 2: Complete custom theme replacement
void setupCustomTheme() {
  // Case 2: Replace with entirely custom CSS
  IWebModule::initializeCSS(String(FPSTR(THEME_DARK_FUSCHIA)));
  // Could also use THEME_OCEAN_BLUE or THEME_LIGHT
}

// Example 3: Default CSS + additional styling
void setupEnhancedTheme() {
  // Define some additional CSS
  const char EXTRA_STYLES[] PROGMEM = R"css(
  /* Add some custom enhancements */
  .custom-highlight {
    background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
    color: white;
    padding: 10px;
    border-radius: 8px;
    margin: 10px 0;
  }
  
  .btn:hover {
    transform: translateY(-3px) !important;
    box-shadow: 0 6px 20px rgba(0,0,0,0.3) !important;
  }
  
  .status-card {
    transition: all 0.3s ease;
  }
  
  .status-card:hover {
    transform: scale(1.02);
  }
  )css";

  // Case 3: Start with default CSS, then add custom styles
  IWebModule::initializeCSS();                           // Use default CSS
  IWebModule::addCustomCSS(String(FPSTR(EXTRA_STYLES))); // Add enhancements

  // Or even simpler (auto-initializes default if not done):
  // IWebModule::addCustomCSS(String(FPSTR(EXTRA_STYLES)));
} // Example of how to create HTML that uses the CSS system
String createThemedPage() {
  String html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>CSS System Example</title>
  <!-- All CSS is served from a single file -->
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    
    <h1>CSS Initialization Examples</h1>
    <p>This page demonstrates the three CSS approaches available in the Web Module Interface system.</p>
    
    <div class="status-grid">
      <div class="status-card">
        <h3>Default CSS</h3>
        <p>Built-in styling for consistent appearance</p>
        <p class="info">initializeCSS()</p>
      </div>
      
      <div class="status-card">
        <h3>Custom CSS</h3>
        <p>Complete theme replacement</p>
        <p class="warning">initializeCSS(customCSS)</p>
      </div>
      
      <div class="status-card">
        <h3>Enhanced CSS</h3>
        <p>Default + additional styling</p>
        <p class="success">addCustomCSS(extraCSS)</p>
      </div>
    </div>
    
    <!-- Example of enhanced styling if using Case 3 -->
    <div class="custom-highlight">
      <strong>Enhanced Styling Example</strong><br>
      This element uses custom CSS added via addCustomCSS().
    </div>
    
    <div class="nav-links">
      <a href="#" class="btn">Standard Button</a>
      <a href="#" class="btn btn-primary">Primary Button</a>
    </div>
  </div>
</body>
</html>
  )";

  return html;
}

// Practical usage in a web module
void handleThemeDemo() {
  // Set current path for navigation
  IWebModule::setCurrentPath("/demo");

  // Generate the page HTML
  String html = createThemedPage();

  // Inject navigation menu
  html = IWebModule::injectNavigationMenu(html);

  // Return the complete HTML
  // (In a real web module, this would be returned from a route handler)
}
