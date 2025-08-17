# Web Module Interface - Theme Examples

This directory contains example theme files that can be used with the Web Module Interface's CSS theming system.

## How to Use These Themes

1. **Copy the example_themes.h file** to your project's source directory

2. **Include the themes file** in your main.cpp:
   ```cpp
   #include "example_themes.h" // Or your renamed version
   ```

3. **Apply the theme** in your setup function:
   ```cpp
   void setup() {
     // Initialize serial and other components...
     
     // Apply one of the themes (choose one):
     IWebModule::setGlobalCSS(FPSTR(THEME_DARK_FUSCHIA));
     // or
     // IWebModule::setGlobalCSS(FPSTR(THEME_OCEAN_BLUE));
     // or
     // IWebModule::setGlobalCSS(FPSTR(THEME_LIGHT));
     
     // Continue with the rest of your setup code...
   }
   ```

4. **Create your own themes** by modifying the examples or creating new ones with similar structure.

## Available Themes

### Dark Fuschia Theme
A sleek dark theme with fuschia/pink accents. Perfect for a modern, striking interface.

### Ocean Blue Theme
A calming blue theme with aqua accents. Good for a professional, serene look.

### Light Theme
A clean, light-colored theme with blue accents. Traditional and easy to read.

## Customizing Themes

To create your own theme:

1. Copy one of the existing theme constants in the file
2. Rename it (e.g., `THEME_MY_CUSTOM`)
3. Modify the CSS values to match your desired color scheme
4. Use your new theme with `IWebModule::setGlobalCSS(FPSTR(THEME_MY_CUSTOM))`

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

1. **Use PROGMEM**: All themes should be stored in flash memory using PROGMEM to save RAM
2. **Use FPSTR()**: When passing the theme to setGlobalCSS(), always use FPSTR() to reference PROGMEM strings
3. **Consistent classes**: Maintain consistent class names across themes for compatibility
4. **Test on device**: Check themes on actual devices as colors may appear differently
5. **Consider performance**: Complex CSS (like extensive gradients) may impact rendering on ESP devices