#include <web_module_interface.h>
#include <Arduino.h>

// Include theme assets
#include "theme_assets.h"

void setupModernTheme() {
  // Initialize the base theme framework (must be called before setting theme)
  IWebModule::initializeBaseTheme();
  
  // Set the theme using base CSS framework and a theme overlay
  // This replaces the older setGlobalCSS() approach
  IWebModule::setTheme(
    String(FPSTR(BASE_CSS)),    // Base CSS framework with standard classes
    String(FPSTR(DARK_THEME_CSS))  // Theme overlay (could also use LIGHT_THEME_CSS or FUSCHIA_THEME_CSS)
  );
  
  // Register theme asset routes with your web server
  // This will serve the CSS files at /assets/base.css and /assets/theme.css
  auto themeRoutes = IWebModule::getThemeAssetRoutes();
  
  // In a real application, you would register these routes with your server
  // For example:
  /*
  for (const auto &route : themeRoutes) {
    webRouter.addStaticContent(
      route.path.c_str(),
      route.handler("", std::map<String, String>()).c_str(),
      route.contentType.c_str()
    );
  }
  */
  
  // The combined CSS is still available at the legacy location
  // for backward compatibility:
  auto legacyCssRoute = IWebModule::getCSSRoute();
  // This serves at /assets/style.css
}

// Example of how to create HTML that uses the modern theme system
String createThemedPage() {
  String html = R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Modern Theme Example</title>
  <!-- Modern theme system uses two CSS files -->
  <link rel="stylesheet" href="/assets/base.css">
  <link rel="stylesheet" href="/assets/theme.css">
  <!-- Legacy compatibility is preserved via style.css -->
  <!-- <link rel="stylesheet" href="/assets/style.css"> -->
</head>
<body>
  <div class="container">
    <!-- Navigation will be automatically injected here -->
    
    <h1>Modern Theme System</h1>
    <p>This page demonstrates the new modular theme system with base CSS framework and theme overlays.</p>
    
    <div class="status-grid">
      <div class="status-card">
        <h3>Base Framework</h3>
        <p>Provides standard classes and structure</p>
        <p class="info">Consistent naming conventions</p>
      </div>
      
      <div class="status-card">
        <h3>Theme Overlay</h3>
        <p>Customizes colors and appearance</p>
        <p class="success">Easy theme switching</p>
      </div>
    </div>
    
    <div class="button-group">
      <button class="btn">Standard Button</button>
      <button class="btn btn-primary">Primary Button</button>
    </div>
  </div>
</body>
</html>
  )";
  
  return html;
}