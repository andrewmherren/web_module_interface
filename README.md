# Web Module Interface

A flexible, unified interface for web-enabled modules in embedded projects with comprehensive theming and navigation systems.

## Overview

The Web Module Interface provides a standardized way to integrate web-enabled modules into a single web server. It defines a common interface pattern that all modules must implement, enabling a modular architecture for ESP32/ESP8266 projects.

## Core Components

- `IWebModule`: Base interface that all web modules must implement
- `WebRoute`: Structure defining HTTP/HTTPS routes and handlers
- `WebModule` namespace: Contains method enums and handler function signatures
- `NavigationItem`: Structure for defining navigation menu items

## Features

### Phase 1: Custom CSS System
- **Global CSS Management**: Centralized styling with default glass morphism theme
- **CSS Link Injection**: Automatic CSS inclusion in HTML responses
- **Embedded Style Guide**: Built-in default CSS following design standards
- **Theme Customization**: Easy custom CSS application across all modules

### Phase 2: Navigation Menu System
- **Consistent Navigation**: Unified navigation across all modules
- **Auto-Active Detection**: Automatic highlighting of current page
- **Flexible Menu Items**: Support for internal links and external links
- **Easy Integration**: Simple HTML injection system

### Core Interface Features
- **Protocol Agnostic**: Support for both HTTP and HTTPS routes
- **Type Safety**: Compile-time enforcement of interface implementation
- **Flexible Routing**: Support for GET, POST, PUT, DELETE, and PATCH methods
- **Content Type Aware**: Built-in content type specification
- **Documentation Friendly**: Route descriptions for API documentation
- **Namespace Isolation**: Uses `WebModule` namespace to avoid platform conflicts

## Interface Implementation

To create a web-enabled module, extend the `IWebModule` interface and implement these required methods:

```cpp
// Return HTTP routes for this module
std::vector<WebRoute> getHttpRoutes() override;

// Return HTTPS routes for this module (can be identical to HTTP)
std::vector<WebRoute> getHttpsRoutes() override; 

// Return module name for identification
String getModuleName() const override;
```

Optional methods with default implementations:
```cpp
// Return module version
String getModuleVersion() const override { return "1.0.0"; }

// Return module description
String getModuleDescription() const override { return "Web-enabled module"; }
```

## Enhanced Features

### 1. Custom CSS System

The interface provides a global CSS management system:

```cpp
// Set custom CSS for all modules
IWebModule::setGlobalCSS(customCssString);

// Get current global CSS
String css = IWebModule::getGlobalCSS();

// Get route for serving CSS at /assets/style.css
WebRoute cssRoute = IWebModule::getCSSRoute();

// Helper to inject CSS link into HTML content
String htmlWithCss = IWebModule::injectCSSLink(htmlContent);
```

Default CSS is based on a glass morphism design with a comprehensive set of components and utility classes.

### 2. Navigation Menu System

A consistent navigation system across all modules:

```cpp
// Set up navigation menu items
std::vector<NavigationItem> navItems = {
    NavigationItem("Home", "/"),
    NavigationItem("Settings", "/settings/"),
    NavigationItem("Docs", "https://example.com", "_blank")
};
IWebModule::setNavigationMenu(navItems);

// Get current navigation menu
auto menu = IWebModule::getNavigationMenu();

// Set current path for auto-active detection
IWebModule::setCurrentPath("/current/path/");

// Generate HTML for navigation menu
String navHtml = IWebModule::generateNavigationHtml();

// Inject navigation menu into HTML content
String htmlWithNav = IWebModule::injectNavigationMenu(htmlContent);
```

The navigation system automatically determines which item is active based on the current path, eliminating the need to manually track active states.

## Module Registration

Modules are registered with a web router that manages the HTTP/HTTPS server:

```cpp
// Initialize your module
MyModule myModule;
myModule.begin();

// Register with the web router
webRouter.registerModule("/my-module", &myModule);

// Start the web router
webRouter.begin(80, 443);
```

## Example Usage

```cpp
class MyModule : public IWebModule {
public:
  std::vector<WebRoute> getHttpRoutes() override {
    std::vector<WebRoute> routes;
    
    // Add main page route with navigation
    routes.push_back({
      "/", 
      WebModule::WM_GET,
      [this](const String &body, const std::map<String, String> &params) {
        // Set current path for navigation
        IWebModule::setCurrentPath("/my-module/");
        
        // Prepare HTML content
        String html = "<html><body><div class=\"container\">My content</div></body></html>";
        
        // Add CSS and navigation
        html = IWebModule::injectCSSLink(html);
        html = IWebModule::injectNavigationMenu(html);
        
        return html;
      },
      "text/html"
    });
    
    return routes;
  }
  
  std::vector<WebRoute> getHttpsRoutes() override {
    return getHttpRoutes();
  }
  
  String getModuleName() const override {
    return "MyModule";
  }
};
```

## Usage Examples

### Basic Module Implementation

```cpp
#include <web_module_interface.h>

class MyWebModule : public IWebModule {
private:
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
        
        <!-- Navigation menu will be auto-injected here -->
        
        <div class="status-card">
          <h3>Status</h3>
          <p class="success">Online</p>
        </div>
      </div>
    </body>
    </html>
    )html";
    
    // Set current path for navigation highlighting
    IWebModule::setCurrentPath("/my-module/");
    
    // Inject CSS and navigation
    html = IWebModule::injectCSSLink(html);
    html = IWebModule::injectNavigationMenu(html);
    
    return html;
  }

public:
  std::vector<WebRoute> getHttpRoutes() override {
    return {
      WebRoute("/", WebModule::WM_GET, [this](const String& body, const std::map<String, String>& params) {
        return getMainPage();
      }, "text/html", "Module main page")
    };
  }
  
  std::vector<WebRoute> getHttpsRoutes() override {
    return getHttpRoutes();
  }
  
  String getModuleName() const override {
    return "MyWebModule";
  }
};
```

### Setting Up Global Navigation in main.cpp

```cpp
void setup() {
  // Set up the global navigation menu
  std::vector<NavigationItem> navItems = {
      NavigationItem("Home", "/"),
      NavigationItem("USB PD Control", "/usb_pd/"),
      NavigationItem("WiFi Settings", "/wifi/"),
      NavigationItem("System Info", "/web_router")
  };
  IWebModule::setNavigationMenu(navItems);
  
  // Set custom CSS theme (optional)
  IWebModule::setGlobalCSS(FPSTR(THEME_DARK_FUSCHIA));
  
  // Initialize other components...
}
```

### Module Registration with Web Router

```cpp
MyWebModule myModule;

// Register module with web router
webRouter.registerModule("/mymodule", &myModule);

// The web router will automatically:
// 1. Register the CSS route at /assets/style.css
// 2. Register all module routes with proper navigation injection
// 3. Handle both HTTP and HTTPS if certificates are available
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

### Navigation Menu System Methods (Static)

- `setNavigationMenu(const std::vector<NavigationItem>& items)`: Set global navigation menu
- `getNavigationMenu()`: Get current navigation menu items
- `setCurrentPath(const String& path)`: Set current path for active link detection
- `getCurrentPath()`: Get current path
- `generateNavigationHtml()`: Generate navigation HTML
- `injectNavigationMenu(const String& html)`: Inject navigation into HTML content

## Data Structures

### WebRoute Structure

```cpp
struct WebRoute {
  String path;                     // Route path (e.g., "/status", "/config")
  WebModule::Method method;        // HTTP method enum
  WebModule::RouteHandler handler; // Function pointer to handler
  String contentType;              // MIME type (e.g., "application/json")
  String description;              // Human-readable description
};
```

### NavigationItem Structure

```cpp
struct NavigationItem {
  String name;   // Display name for the menu item
  String url;    // URL the menu item links to
  String target; // Optional: target attribute for the link (e.g., "_blank")
  
  // Constructors for convenience
  NavigationItem(const String &n, const String &u);
  NavigationItem(const String &n, const String &u, const String &t);
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

## Future Enhancements

Planned enhancements for this interface include:

### Phase 3: Route Redirection System
- `addRedirect(const String& fromPath, const String& toPath)` for URL redirection
- Support for 302 temporary redirects (appropriate for embedded firmware)
- Main page redirect configuration

### Phase 4: Error Page Customization  
- `setErrorPage(int statusCode, const String& html)` for custom 404, 500, etc.
- Default error pages with global CSS applied
- Consistent error handling across modules

### Phase 5: Asset Management
- `addStaticAsset(const String& path, const String& content, const String& mimeType)`
- Support for JavaScript, images, fonts alongside CSS
- Rich web interface development support

### Phase 6: HTTPS Auto-Redirect (WEB_ROUTER ENHANCEMENT)
- `setHttpsRedirectMode(bool enabled)` in WebRouter class
- Automatic 301 redirect of HTTP requests to HTTPS
- Simplified module development (single route definition)

## Version History

### v1.2.0 - Phase 2 Navigation System
- Added navigation menu system with NavigationItem structure
- Added auto-active detection based on current path
- Added navigation HTML generation and injection
- Updated example documentation for navigation integration

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

This project is licensed under the MIT License - see LICENSE file for details.

MIT License - see LICENSE file for details