# Error Pages Examples

This directory contains examples of how to use the error page customization functionality in the web_module_interface library.

## Features

- Custom error pages for specific HTTP status codes (404, 500, etc.)
- Default theme-aware error pages for all status codes
- Navigation menu integration with error pages
- Custom error messages for specific scenarios

## Examples

### 1. Basic Error Page Setup

`basic_error_setup.cpp` demonstrates how to set up a custom 404 error page and use the default error pages for other status codes.

### 2. Error Page Test Routes

`error_test_routes.cpp` shows how to create test routes to preview and test your error pages, similar to the implementation in `error_page_test.cpp`.

### 3. Custom Error Messages

`custom_error_messages.cpp` demonstrates how to generate error pages with custom messages for specific scenarios.

## Usage

To use custom error pages in your project:

1. Define your custom error page HTML
2. Call `IWebModule::setErrorPage(statusCode, htmlContent)` for each status code you want to customize
3. For status codes without custom pages, the system will use theme-aware default error pages

Example:

```cpp
// Set a custom 404 error page
IWebModule::setErrorPage(404, FPSTR(myCustom404Html));

// Use the default error page for 500 errors
String errorPage500 = IWebModule::getErrorPage(500);

// Generate a custom error message
String teapotError = IWebModule::generateDefaultErrorPage(418, "I'm a teapot!");
```

## Integration with Navigation and CSS

All error pages, both custom and default, automatically:
- Include the global CSS stylesheet
- Support navigation menu injection
- Adapt to the current theme

To ensure navigation is correctly displayed on error pages, be sure to:
- Set the current path before generating the error page
- Inject the navigation menu if not using auto-injection

Example:
```cpp
// Set the current path for proper navigation highlighting
IWebModule::setCurrentPath("/error-test");

// Get the error page with navigation
String errorPageWithNav = IWebModule::injectNavigationMenu(IWebModule::getErrorPage(404));
```