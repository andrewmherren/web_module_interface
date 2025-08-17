# Web Module Interface

A standardized interface library for creating web-enabled ESP32/ESP8266 modules that can be dynamically registered with web routers. Includes enhancements for custom CSS theming system.

## Overview

This library provides an abstract interface (`IWebModule`) that ensures consistent implementation across different web-enabled modules. It supports both HTTP and HTTPS protocols, flexible route registration, and a unified CSS theming system for consistent web UI styling across modules.

## Features

- **Protocol Agnostic**: Support for both HTTP and HTTPS routes
- **Type Safety**: Compile-time enforcement of interface implementation
- **Flexible Routing**: Support for GET, POST, PUT, DELETE, and PATCH methods
- **Content Type Aware**: Built-in content type specification
- **Documentation Friendly**: Route descriptions for API documentation
- **Namespace Isolation**: Uses `WebModule` namespace to avoid platform conflicts
- **Global CSS System**: Centralized styling with default glass morphism theme
- **CSS Link Injection**: Automatic CSS inclusion in HTML responses
- **Embedded Style Guide**: Built-in default CSS following design standards

## Usage

### Implementing the Interface

```cpp
#include <web_module_interface.h>

class MyWebModule : public IWebModule {
private:
  String handleStatus() {
    return "{\"status\":\"online\"}";
  }
  
  String handleConfig(const String& requestBody, const std::map<String, String>& params) {
    // Process configuration update
    return "{\"success\":true}";
  }
  
  String getMainPage() {
    String html = R"html(
    <!DOCTYPE html>
    <html>
    <head>
      <title>My Module</title>
    </head>
    <body>
      <div class="container">
        <h1>My Web Module</h1>
        <div class="status-card">
          <h3>Status</h3>
          <p class="success">Online</p>
        </div>
      </div>
    </body>
    </html>
    )html";
    
    // Inject CSS link automatically
    return IWebModule::injectCSSLink(html);
  }

public:
  std::vector<WebRoute> getHttpRoutes() override {
    return {
      WebRoute("/", WebModule::WM_GET, [this](const String& body, const std::map<String, String>& params) {
        return getMainPage();
      }, "text/html", "Module main page"),
      
      WebRoute("/api/status", WebModule::WM_GET, [this](const String& body, const std::map<String, String>& params) {
        return handleStatus();
      }, "application/json", "Get module status"),
      
      WebRoute("/api/config", WebModule::WM_POST, [this](const String& body, const std::map<String, String>& params) {
        return handleConfig(body, params);
      }, "application/json", "Update module configuration")
    };
  }
  
  std::vector<WebRoute> getHttpsRoutes() override {
    // For this example, HTTPS routes are identical to HTTP routes
    return getHttpRoutes();
  }
  
  String getModuleName() const override {
    return "MyWebModule";
  }
  
  String getModuleVersion() const override {
    return "1.0.0";
  }
};
```

### Route Registration with Web Router

```cpp
MyWebModule myModule;

// Register module with web router (recommended approach)
webRouter.registerModule("/mymodule", &myModule);

// The web router will automatically:
// 1. Register the CSS route at /assets/style.css
// 2. Register all module routes with proper CSS injection
// 3. Handle both HTTP and HTTPS if certificates are available
```

### CSS Theming System

```cpp
// Set custom global CSS in main.cpp setup()
void setup() {
  // Set custom CSS theme for all modules
  String customCSS = R"css(
    body {
      background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
      color: #e94560;
    }
    .container {
      background: rgba(233, 69, 96, 0.1);
      border: 1px solid rgba(233, 69, 96, 0.3);
    }
  )css";
  
  IWebModule::setGlobalCSS(customCSS);
  
  // Initialize other components...
}
```

### HTML with CSS Integration

```cpp
// In your module's HTML route handler:
String getMainPage() {
  String html = R"html(
  <!DOCTYPE html>
  <html>
  <head>
    <title>My Module</title>
    <!-- CSS link will be injected here -->
  </head>
  <body>
    <div class="container">
      <h1>My Module</h1>
      <!-- Use standard CSS classes for consistent styling -->
      <div class="status-card">
        <h3>Status</h3>
        <p class="success">System Online</p>
      </div>
      <div class="button-group">
        <button class="btn btn-primary">Primary Action</button>
        <button class="btn btn-secondary">Secondary</button>
      </div>
    </div>
  </body>
  </html>
  )html";
  
  // This automatically injects <link rel="stylesheet" href="/assets/style.css">
  return IWebModule::injectCSSLink(html);
}
```

## Interface Methods

### Required Methods (Pure Virtual)

- `getHttpRoutes()`: Returns vector of HTTP routes
- `getHttpsRoutes()`: Returns vector of HTTPS routes  
- `getModuleName()`: Returns module name string

### Optional Methods (Default Implementation Provided)

- `getModuleVersion()`: Returns version string (default: "1.0.0")
- `getModuleDescription()`: Returns description string
- `getWebRoutes()`: Convenience method returning HTTP routes

### CSS System Methods (Static)

- `setGlobalCSS(const String& css)`: Set custom CSS for all modules
- `getGlobalCSS()`: Get current CSS (custom or default)
- `getCSSRoute()`: Get WebRoute for serving CSS at /assets/style.css
- `injectCSSLink(const String& html)`: Inject CSS link into HTML content

## WebRoute Structure

```cpp
struct WebRoute {
  String path;                     // Route path (e.g., "/status", "/config")
  WebModule::Method method;        // HTTP method enum
  WebModule::RouteHandler handler; // Function pointer to handler
  String contentType;              // MIME type (e.g., "application/json")
  String description;              // Human-readable description
};
```

## Standard CSS Classes

The default CSS provides these standard classes for consistent styling:

### Layout
- `.container`: Main content wrapper with glass morphism effect
- `.status-grid`: Responsive grid for status cards

### Cards
- `.status-card`: Glass morphism card container
- `.pdo-card`: Specialized card for power delivery objects
- `.pdo-card.active`: Active state styling

### Typography & Status
- `.success`: Green success text
- `.error`: Red error text  
- `.warning`: Orange warning text
- `.info`: Blue info text

### Buttons
- `.btn`: Base button class
- `.btn-primary`: Primary action buttons (green)
- `.btn-secondary`: Secondary buttons (glass)
- `.btn-warning`: Warning buttons (orange)
- `.btn-danger`: Danger buttons (red)
- `.button-group`: Flex container for button groups

### Forms
- `.form-group`: Form field container
- `.form-control`: Input/select styling
- `.nav-links`: Navigation link container

### Utilities
- `.hidden`: Hide elements
- `.text-center`, `.text-left`, `.text-right`: Text alignment
- `.mb-0` through `.mb-3`: Margin bottom spacing
- `.mt-0` through `.mt-3`: Margin top spacing

## HTTP Methods

Supported HTTP methods via enum:
- `WebModule::WM_GET`
- `WebModule::WM_POST` 
- `WebModule::WM_PUT`
- `WebModule::WM_DELETE`
- `WebModule::WM_PATCH`

## Installation

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps = 
  https://github.com/yourusername/web_module_interface.git
```

### Direct Integration (Development/Testing)

For testing or when you need to modify the interface:

1. Copy `web_module_interface/` folder to your project root
2. Add static variable definitions to your main project:

```cpp
// In src/web_ui_globals.cpp or main.cpp
#include "../web_module_interface/include/web_module_interface.h"

// Static member definitions
String IWebModule::globalCSS = "";
bool IWebModule::globalCSSSet = false;

// Method implementations
void IWebModule::setGlobalCSS(const String& css) {
  globalCSS = css;
  globalCSSSet = true;
}

String IWebModule::getGlobalCSS() {
  if (globalCSSSet) {
    return globalCSS;
  }
  return String(FPSTR(WEB_UI_DEFAULT_CSS));
}

WebRoute IWebModule::getCSSRoute() {
  return WebRoute(
    "/assets/style.css",
    WebModule::WM_GET,
    [](const String& body, const std::map<String, String>& params) -> String {
      return getGlobalCSS();
    },
    "text/css",
    "Global CSS stylesheet"
  );
}

String IWebModule::injectCSSLink(const String& htmlContent) {
  String result = htmlContent;
  int headEndPos = result.indexOf("</head>");
  
  if (headEndPos != -1) {
    String cssLink = "\n  <link rel=\"stylesheet\" href=\"/assets/style.css\" type=\"text/css\">\n";
    result = result.substring(0, headEndPos) + cssLink + result.substring(headEndPos);
  }
  
  return result;
}
```

3. Include the header in your modules:

```cpp
#include "../web_module_interface/include/web_module_interface.h"
```

### Arduino IDE

1. Download or clone this repository
2. Copy to your Arduino libraries folder
3. Restart Arduino IDE

## Architecture Notes

### Static Variable Management

The CSS system uses static variables that must be defined exactly once in your project. When using this library:

1. **As PlatformIO Library**: Static variables are automatically managed
2. **Direct Integration**: You must provide the static variable definitions (see installation instructions)

### CSS Route Registration

The `/assets/style.css` route should be registered by your web router before any module routes to ensure CSS is available when pages load.

### Thread Safety

The CSS system is designed for single-threaded Arduino/ESP32 environments. The static variables are not protected by mutexes.

## Compatibility

- **Platforms**: ESP32, ESP8266
- **Framework**: Arduino
- **C++ Standard**: C++17 or later
- **Memory**: Requires sufficient heap for CSS content (default ~8KB)

## Version History

### v1.1.0 - Phase 1 CSS System
- Added global CSS theming system
- Added default glass morphism CSS
- Added CSS link injection helper
- Added standard CSS class library
- Added web_ui_styles.h with default theme

### v1.0.0 - Initial Release
- Basic IWebModule interface
- HTTP/HTTPS route support
- WebRoute structure
- Method enumeration

## License

MIT License - see LICENSE file for details