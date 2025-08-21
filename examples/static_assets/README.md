# Static Asset Management Example

This example demonstrates how to use the Static Asset Management feature of the web module interface to serve JavaScript files, images, and other static content from your ESP32/ESP8266 device.

## Overview

Static Asset Management allows you to serve files like JavaScript, CSS, images, and fonts directly from your device's memory. This is particularly useful for creating rich web interfaces without requiring external CDNs or separate file servers.

## Features

- **JavaScript Files**: Serve custom JavaScript for interactive web pages
- **Images**: Embed logos, icons, and images directly in firmware  
- **Fonts**: Provide custom fonts for consistent branding
- **Any Static Content**: Serve any file type with proper MIME types
- **PROGMEM Support**: Store assets in flash memory to save RAM

## Basic Usage

### 1. Adding Static Assets

```cpp
#include "web_module_interface.h"

void setupStaticAssets() {
    // Add JavaScript file
    String jsCode = R"(
        function updateStatus() {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('status').textContent = data.status;
                });
        }
        
        // Update status every 5 seconds
        setInterval(updateStatus, 5000);
    )";
    
    IWebModule::addJavaScript("/assets/app.js", jsCode);
    
    // Add a small image (base64 encoded for this example)
    String logoData = "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChAGAWA0ddwAAAABJRU5ErkJggg==";
    IWebModule::addImage("/assets/logo.png", logoData, "png");
    
    // Add custom CSS (separate from global CSS)
    String moduleCSS = R"(
        .module-specific {
            background: linear-gradient(45deg, #ff6b6b, #4ecdc4);
            padding: 20px;
            border-radius: 8px;
        }
    )";
    
    IWebModule::addStaticAsset("/assets/module.css", moduleCSS, "text/css");
}
```

### 2. Using Assets in HTML

```cpp
String generateWebPage() {
    String html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>My ESP32 App</title>
    <link rel="stylesheet" href="/assets/style.css">
    <link rel="stylesheet" href="/assets/module.css">
</head>
<body>
    <div class="container">
        <img src="/assets/logo.png" alt="Logo" style="height: 50px;">
        <h1>Device Status</h1>
        <div id="status" class="module-specific">Loading...</div>
    </div>
    
    <script src="/assets/app.js"></script>
</body>
</html>
    )";
    
    return IWebModule::injectNavigationMenu(html);
}
```

### 3. Integration with Web Router

The static assets are automatically registered as routes. In your module:

```cpp
std::vector<WebRoute> MyModule::getHttpRoutes() {
    std::vector<WebRoute> routes;
    
    // Add your module's regular routes
    routes.push_back(WebRoute("/", WebModule::WM_GET, [](const String&, const std::map<String, String>&) {
        return generateWebPage();
    }));
    
    // Get static asset routes and add them
    auto assetRoutes = IWebModule::getStaticAssetRoutes();
    routes.insert(routes.end(), assetRoutes.begin(), assetRoutes.end());
    
    return routes;
}
```

## Advanced Usage

### Using PROGMEM for Large Assets

For larger assets, use PROGMEM to store them in flash memory instead of RAM:

```cpp
// Store large assets in PROGMEM
const char largeScript[] PROGMEM = R"(
    // Large JavaScript library or framework
    function complexFunction() {
        // ... lots of code ...
    }
)";

void setupLargeAssets() {
    // The fourth parameter (true) indicates PROGMEM usage
    IWebModule::addJavaScript("/assets/large-script.js", 
                             String(FPSTR(largeScript)), 
                             true);  // Use PROGMEM
}
```

### Helper Methods

The interface provides convenient helper methods:

```cpp
// JavaScript files
IWebModule::addJavaScript("/assets/utils.js", jsCode);
IWebModule::addJavaScript("/assets/big-lib.js", jsCode, true); // PROGMEM

// Images with automatic MIME type detection
IWebModule::addImage("/assets/icon.png", pngData, "png");
IWebModule::addImage("/assets/photo.jpg", jpegData, "jpeg");
IWebModule::addImage("/assets/logo.gif", gifData, "gif");

// Fonts with proper MIME types
IWebModule::addFont("/assets/custom.woff2", fontData, "woff2");
IWebModule::addFont("/assets/custom.ttf", fontData, "ttf");
```

### Checking Asset Existence

```cpp
if (IWebModule::hasStaticAsset("/assets/app.js")) {
    Serial.println("JavaScript file is available");
}

StaticAsset asset = IWebModule::getStaticAsset("/assets/app.js");
if (asset.path != "") {
    Serial.println("Asset found: " + asset.mimeType);
}
```

## Memory Considerations

- **RAM Usage**: Assets stored without PROGMEM use valuable RAM
- **Flash Usage**: PROGMEM assets use flash memory, which is more abundant
- **Asset Size**: Keep individual assets reasonably small (< 64KB recommended)
- **Total Assets**: Monitor total memory usage across all assets

## Best Practices

1. **Use PROGMEM**: For assets > 1KB, always use PROGMEM
2. **Organize Paths**: Use consistent path structure (`/assets/js/`, `/assets/img/`, etc.)
3. **Optimize Assets**: Minify JavaScript, compress images before embedding
4. **Cache Headers**: Consider adding cache control in future enhancements
5. **Error Handling**: Always check asset availability before referencing in HTML

## Example Project Structure

```
/assets/
├── style.css          (Default CSS via IWebModule::initializeDefaultTheme)
├── app.js             (Main application JavaScript)
├── utils.js           (Utility functions)
├── logo.png           (Site logo)
├── favicon.ico        (Browser icon)
└── fonts/
    ├── custom.woff2   (Custom font)
    └── icons.ttf      (Icon font)
```

This structure provides a complete static asset foundation for professional web interfaces running entirely on your ESP32/ESP8266 device.