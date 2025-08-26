# Web Module Interface - Theme Examples

This directory contains example theme files that demonstrate the three CSS approaches available in the Web Module Interface system.

## CSS Initialization Options

There are three ways to handle CSS in your application:

### 1. Use Default CSS Only
```cpp
void setup() {
  // Uses built-in default CSS
  IWebModule::initializeCSS();
  // or equivalent:
  // IWebModule::initializeDefaultTheme();
}
```

### 2. Replace with Entirely Custom CSS
```cpp
const char MY_CUSTOM_CSS[] PROGMEM = R"css(
/* Your complete custom CSS here */
body {
  font-family: Arial, sans-serif;
  background: #f0f0f0;
  color: #333;
}
/* ... rest of your CSS ... */
)css";

void setup() {
  // Replace default CSS completely
  IWebModule::initializeCSS(String(FPSTR(MY_CUSTOM_CSS)));
}
```

### 3. Use Default CSS + Additional CSS (NEW!)
```cpp
const char EXTRA_STYLING[] PROGMEM = R"css(
/* Additional CSS to enhance default theme */
.custom-card {
  background: linear-gradient(45deg, #667eea 0%, #764ba2 100%);
  color: white;
}

.special-button {
  border-radius: 20px;
  box-shadow: 0 4px 15px rgba(102, 126, 234, 0.3);
}
)css";

void setup() {
  // Start with default CSS, then add custom styles
  IWebModule::initializeCSS();
  IWebModule::addCustomCSS(String(FPSTR(EXTRA_STYLING)));
  
  // Or even simpler (auto-initializes default if not done):
  // IWebModule::addCustomCSS(String(FPSTR(EXTRA_STYLING)));
}
```

## HTML Template Usage

Once CSS is initialized, reference it in your HTML templates:

```html
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>My Module</title>
  <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
  <div class="container">
    <!-- Navigation menu will be auto-injected here -->
    <h1>My Application</h1>
    <!-- Your content here -->
  </div>
</body>
</html>
```

## Complete Theme Examples

The `example_themes.h` file contains three complete theme implementations:

### Dark Fuschia Theme
A sleek dark theme with fuschia/pink accents. Perfect for a modern, striking interface.
```cpp
#include "example_themes.h"

void setup() {
  IWebModule::initializeCSS(String(FPSTR(THEME_DARK_FUSCHIA)));
}
```

### Ocean Blue Theme
A calming blue theme with aqua accents. Good for a professional, serene look.
```cpp
#include "example_themes.h"

void setup() {
  IWebModule::initializeCSS(String(FPSTR(THEME_OCEAN_BLUE)));
}
```

### Light Theme
A clean, light-colored theme with blue accents. Traditional and easy to read.
```cpp
#include "example_themes.h"

void setup() {
  IWebModule::initializeCSS(String(FPSTR(THEME_LIGHT)));
}
```

## Adding Custom Styles to Themes

You can enhance any theme (including default) with additional CSS:

```cpp
const char CUSTOM_ENHANCEMENTS[] PROGMEM = R"css(
/* Add glowing effects to buttons */
.btn:hover {
  box-shadow: 0 0 20px rgba(61, 90, 241, 0.5);
}

/* Custom status indicator animations */
.success {
  animation: pulse 2s infinite;
}

@keyframes pulse {
  0% { opacity: 1; }
  50% { opacity: 0.7; }
  100% { opacity: 1; }
}
)css";

void setup() {
  // Use any base theme
  IWebModule::initializeCSS(String(FPSTR(THEME_DARK_FUSCHIA)));
  
  // Add your enhancements
  IWebModule::addCustomCSS(String(FPSTR(CUSTOM_ENHANCEMENTS)));
}
```

## Creating Your Own Themes

To create a completely custom theme:

1. **Copy one of the existing themes** as a starting point
2. **Modify colors and styles** to match your desired appearance
3. **Test on actual hardware** - colors may appear differently on ESP devices
4. **Store in PROGMEM** to save RAM

Key elements to customize:
- Background colors and gradients
- Container styling (cards, borders)
- Text colors and fonts
- Button styles and hover effects
- Status indicators (success, error, warning, info)
- Navigation menu appearance

## CSS Class Reference

All themes support these common UI elements:

- `.container` - Main content container with consistent padding and styling
- `.status-grid` - CSS Grid layout for status cards (auto-fit, responsive)
- `.status-card` - Individual status information cards
- `.nav-links` - Navigation menu container (flexbox layout)
- `.btn` - Base button styling
- `.btn-primary` - Primary action buttons with enhanced styling
- `.success`, `.info`, `.warning`, `.error` - Status message colors
- `.form-control` - Form input elements (text fields, selects, etc.)
- `.pdo-card` - Special cards used for power delivery options
- `.pdo-card.active` - Active state for PDO cards

## Best Practices

1. **Use PROGMEM**: Store all CSS in flash memory to preserve RAM
   ```cpp
   const char MY_CSS[] PROGMEM = R"css(/* CSS here */)css";
   ```

2. **Use FPSTR()**: Always use FPSTR() when passing PROGMEM strings
   ```cpp
   IWebModule::initializeCSS(String(FPSTR(MY_CSS)));
   ```

3. **Test on hardware**: Colors and performance may differ on ESP devices

4. **Keep it simple**: Complex CSS animations may impact performance on embedded devices

5. **Maintain compatibility**: Use the standard CSS classes to ensure compatibility across modules

6. **Consider memory**: Each CSS theme uses flash memory - balance features with available space
