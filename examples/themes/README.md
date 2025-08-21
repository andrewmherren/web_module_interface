# Web Module Interface - Theme Examples

This directory contains example theme files that can be used with the Web Module Interface's static asset system.

## How to Use Custom CSS

1. **Create your CSS content** as a PROGMEM constant:
   ```cpp
   const char MY_CUSTOM_CSS[] PROGMEM = R"css(
   /* Your custom CSS here */
   :root {
     --primary-color: #8e44ad;
     --secondary-color: #6c3483;
     --bg-color: #f0f0f0;
   }
   
   body {
     background: var(--bg-color);
     color: var(--text-color);
   }
   )css";
   ```

2. **Add the CSS as a static asset** in your setup function:
   ```cpp
   void setup() {
     // Initialize default theme first
     IWebModule::initializeDefaultTheme();
     
     // Override with your custom CSS
     IWebModule::addStaticAsset("/assets/style.css", 
                                String(FPSTR(MY_CUSTOM_CSS)), 
                                "text/css", true);
     
     // Continue with setup...
   }
   ```

3. **Link to CSS in your HTML templates**:
   ```html
   <head>
     <meta charset="UTF-8">
     <meta name="viewport" content="width=device-width, initial-scale=1.0">
     <title>My Module</title>
     <link rel="stylesheet" href="/assets/style.css">
   </head>
   ```
   ## Static Asset Approach

The static asset system manages all CSS files consistently:

```cpp
void setupCustomTheme() {
  // Initialize the default theme first
  IWebModule::initializeDefaultTheme();
  
  // Add additional CSS files
  IWebModule::addStaticAsset("/assets/custom-theme.css", 
                             String(FPSTR(MY_CUSTOM_THEME)), 
                             "text/css", true);
  
  // Add JavaScript for theme switching (optional)
  IWebModule::addJavaScript("/assets/theme-switcher.js", 
                            String(FPSTR(THEME_SWITCHER_JS)), true);
}
```

## Multiple CSS Files

You can organize your styling into multiple CSS files:

```cpp
// Base framework
IWebModule::addStaticAsset("/assets/base.css", String(FPSTR(BASE_CSS)), "text/css", true);

// Theme-specific colors
IWebModule::addStaticAsset("/assets/theme.css", String(FPSTR(THEME_CSS)), "text/css", true);

// Module-specific styling
IWebModule::addStaticAsset("/assets/module.css", String(FPSTR(MODULE_CSS)), "text/css", true);
```

Then include all files in your HTML:
```html
<link rel="stylesheet" href="/assets/base.css">
<link rel="stylesheet" href="/assets/theme.css">
<link rel="stylesheet" href="/assets/module.css">
```

## Available Themes

### Dark Fuschia Theme
A sleek dark theme with fuschia/pink accents. Perfect for a modern, striking interface.

### Ocean Blue Theme
A calming blue theme with aqua accents. Good for a professional, serene look.

### Light Theme
A clean, light-colored theme with blue accents. Traditional and easy to read.

## Customizing Themes

To create your own theme:

1. Create a new CSS constant with your theme styles
2. Add it as a static asset during initialization
3. Modify the CSS values to match your desired color scheme
4. Link to your theme CSS in HTML templates

The most important elements to customize are:
- Background colors and gradients
- Container styling (cards, borders)
- Text colors
- Button styles
- Status indicators (success, error, etc.)

## CSS Class Reference

The themes include styling for these common UI elements:

- `.container` - Main content container
- `.status-grid` - Grid layout for status cards
- `.status-card` - Individual status information cards
- `.nav-links` - Navigation menu container
- `.btn` - Buttons (including variants like `.btn-primary`)
- `.success`, `.info`, `.warning`, `.error` - Status indicators
- `.form-control` - Form input elements
- `.pdo-card` - Used for power delivery option cards

## Tips for Theme Development

1. **Use PROGMEM**: All CSS should be stored in flash memory using PROGMEM to save RAM
2. **Use FPSTR()**: When passing CSS to addStaticAsset(), always use FPSTR() to reference PROGMEM strings
3. **Consistent classes**: Maintain consistent class names across themes for compatibility
4. **Test on device**: Check themes on actual devices as colors may appear differently
5. **Consider performance**: Complex CSS (like extensive gradients) may impact rendering on ESP devices
6. **Organize assets**: Group related CSS files logically (base, theme, module-specific)
