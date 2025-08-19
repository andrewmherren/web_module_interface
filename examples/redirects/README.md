# URL Redirection Examples

This directory contains examples of how to use the URL redirection functionality in the web_module_interface library.

## Features

- Simple URL redirection from one path to another
- Support for both internal and external redirects
- Easy configuration through the IWebModule interface
- 302 Temporary redirects optimized for embedded systems

## Examples

### 1. Simple Redirect Example

`simple_redirect_example.cpp` demonstrates basic redirects setup:
- Internal redirects (one page to another within your application)
- External redirects (from your application to an external website)
- Module shortcuts (simplified URLs to module pages)

### 2. Embedded Redirect Integration

`embedded_redirect_integration.cpp` shows how to integrate redirects into a complete embedded web application:
- Setting up redirects during initialization
- Defining a home page redirect
- Creating shortcuts to commonly used module pages
- Handling external redirects

## Usage

To set up redirects in your project:

1. Add redirects during initialization:
   ```cpp
   // Internal redirection
   IWebModule::addRedirect("/home", "/");
   
   // Module shortcut
   IWebModule::addRedirect("/pd", "/usb_pd/");
   
   // External redirection
   IWebModule::addRedirect("/github", "https://github.com/yourusername/yourrepo");
   ```

2. The web router will automatically detect redirect rules and perform 302 redirects when a matching URL is requested.

## Integration with Web Router

The redirection system works seamlessly with the web_router component:

1. When a request comes in, web_router checks if the URL matches any redirect rule
2. If a match is found, a 302 Temporary Redirect response is sent
3. The browser automatically follows the redirect to the new location

## Redirect Best Practices

- Use redirects sparingly - they add processing overhead
- Create shortcuts for commonly accessed pages
- Use readable, memorable paths for redirects
- Avoid redirect chains (redirects pointing to other redirects)
- Consider using a home page redirect to point users to your main module