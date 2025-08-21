#include "../include/web_module_interface.h"

// Initialize static variables
String IWebModule::globalCSS = WEB_UI_DEFAULT_CSS;
bool IWebModule::globalCSSSet = false;
String IWebModule::baseCSS = "";
String IWebModule::themeCSS = "";
bool IWebModule::themeInitialized = false;
std::vector<NavigationItem> IWebModule::navigationMenu;
String IWebModule::currentPath = "";
std::map<int, String> IWebModule::errorPages;
std::vector<RedirectRule> IWebModule::redirectRules;
std::vector<StaticAsset> IWebModule::staticAssets;

// Phase 1: Custom CSS System (BACKWARD COMPATIBILITY)
void IWebModule::setGlobalCSS(const String &css) {
  globalCSS = css;
  globalCSSSet = true;
  // When using legacy method, disable modern theme system
  themeInitialized = false;
}

String IWebModule::getGlobalCSS() {
  // If modern theme system is active, return combined CSS
  if (themeInitialized && !baseCSS.isEmpty()) {
    return baseCSS + "\n" + themeCSS;
  }
  // Otherwise return legacy CSS
  return globalCSS;
}

WebRoute IWebModule::getCSSRoute() {
  return WebRoute(
      "/assets/style.css", WebModule::WM_GET,
      [](const String &, const std::map<String, String> &) {
        return IWebModule::getGlobalCSS();
      },
      "text/css", "Global CSS stylesheet");
}

// Phase 6: Theme System Modernization
void IWebModule::initializeBaseTheme() {
  // Only initialize once
  if (themeInitialized)
    return;

  // Set base CSS framework (this would come from theme_assets.h or similar)
  // For now, use the existing WEB_UI_DEFAULT_CSS as base
  baseCSS = WEB_UI_DEFAULT_CSS;
  themeCSS = ""; // No theme overlay by default
  themeInitialized = true;
  globalCSSSet = false; // Disable legacy system
}

void IWebModule::setTheme(const String &newBaseCSS, const String &newThemeCSS) {
  baseCSS = newBaseCSS;
  themeCSS = newThemeCSS;
  themeInitialized = true;
  globalCSSSet = false; // Disable legacy system when using modern themes
}

std::vector<WebRoute> IWebModule::getThemeAssetRoutes() {
  std::vector<WebRoute> routes;

  // Serve base CSS at /assets/base.css
  if (!baseCSS.isEmpty()) {
    WebRoute baseRoute(
        "/assets/base.css", WebModule::WM_GET,
        [](const String &, const std::map<String, String> &) {
          return baseCSS;
        },
        "text/css", "Base CSS framework");
    routes.push_back(baseRoute);
  }

  // Serve theme CSS at /assets/theme.css
  if (!themeCSS.isEmpty()) {
    WebRoute themeRoute(
        "/assets/theme.css", WebModule::WM_GET,
        [](const String &, const std::map<String, String> &) {
          return themeCSS;
        },
        "text/css", "Theme overlay CSS");
    routes.push_back(themeRoute);
  }

  return routes;
}

String IWebModule::injectCSSLink(const String &htmlContent) {
  // Find the closing head tag or insert at the beginning if no head tag
  int headPos = htmlContent.indexOf("</head>");
  if (headPos == -1) {
    int htmlPos = htmlContent.indexOf("<html>");
    if (htmlPos != -1) {
      // Insert after <html> tag with a new head section
      return htmlContent.substring(0, htmlPos + 6) +
             "\n<head>\n<link rel=\"stylesheet\" "
             "href=\"/assets/style.css\">\n</head>\n" +
             htmlContent.substring(htmlPos + 6);
    } else {
      // No HTML tag, insert at the beginning
      return "<head>\n<link rel=\"stylesheet\" "
             "href=\"/assets/style.css\">\n</head>\n" +
             htmlContent;
    }
  } else {
    // Insert before the closing head tag
    return htmlContent.substring(0, headPos) +
           "\n<link rel=\"stylesheet\" href=\"/assets/style.css\">\n" +
           htmlContent.substring(headPos);
  }
}

// Phase 2: Navigation Menu System
void IWebModule::setNavigationMenu(const std::vector<NavigationItem> &items) {
  navigationMenu = items;
}

std::vector<NavigationItem> IWebModule::getNavigationMenu() {
  return navigationMenu;
}

void IWebModule::setCurrentPath(const String &path) { currentPath = path; }

String IWebModule::getCurrentPath() { return currentPath; }

String IWebModule::generateNavigationHtml() {
  if (navigationMenu.empty()) {
    return "";
  }

  String html = "<div class=\"nav-links\">\n";
  for (const auto &item : navigationMenu) {
    html += "  <a href=\"" + item.url + "\"";

    // Auto-detect active state by comparing with current path
    if (!currentPath.isEmpty() &&
        (currentPath == item.url ||
         (item.url != "/" && currentPath.startsWith(item.url)))) {
      html += " class=\"active\"";
    }

    if (item.target.length() > 0) {
      html += " target=\"" + item.target + "\"";
    }

    html += ">" + item.name + "</a>\n";
  }
  html += "</div>\n";

  return html;
}

String IWebModule::injectNavigationMenu(const String &htmlContent) {
  // Generate the navigation HTML
  String navHtml = generateNavigationHtml();
  if (navHtml.length() == 0) {
    return htmlContent; // No navigation menu to inject
  }

  // Look for the placeholder comment first (most preferred)
  int placeholderPos = htmlContent.indexOf(
      "<!-- Navigation menu will be auto-injected here -->");
  if (placeholderPos == -1) {
    placeholderPos = htmlContent.indexOf(
        "<!-- Navigation will be automatically injected here -->");
  }

  if (placeholderPos != -1) {
    // Replace the placeholder with navigation HTML
    int placeholderEnd = htmlContent.indexOf("-->", placeholderPos) + 3;
    return htmlContent.substring(0, placeholderPos) + navHtml +
           htmlContent.substring(placeholderEnd);
  }

  // Try to find container to inject into
  int containerPos = htmlContent.indexOf("<div class=\"container\">");
  if (containerPos != -1) {
    // Find the first div closing tag after the container opening
    int divClosePos = htmlContent.indexOf(">", containerPos);
    if (divClosePos != -1) {
      return htmlContent.substring(0, divClosePos + 1) + "\n" + navHtml +
             htmlContent.substring(divClosePos + 1);
    }
  }

  // Fallback: Try to find the body tag
  int bodyPos = htmlContent.indexOf("<body>");
  if (bodyPos != -1) {
    return htmlContent.substring(0, bodyPos + 6) + "\n" + navHtml +
           htmlContent.substring(bodyPos + 6);
  }

  // Last resort: just prepend to the content
  return navHtml + htmlContent;
}

// Phase 4: Error Page Customization
void IWebModule::setErrorPage(int statusCode, const String &html) {
  errorPages[statusCode] = html;
}

String IWebModule::getErrorPage(int statusCode) {
  auto it = errorPages.find(statusCode);
  if (it != errorPages.end()) {
    return it->second;
  }

  // Return default error page if no custom page is set
  return generateDefaultErrorPage(statusCode);
}

String IWebModule::generateDefaultErrorPage(int statusCode,
                                            const String &message) {
  // Common error messages
  String title, description;
  String statusClass = "error"; // Default to error class for styling

  switch (statusCode) {
  case 400:
    title = "400 Bad Request";
    description = message.isEmpty()
                      ? "The request could not be understood by the server."
                      : message;
    break;
  case 401:
    title = "401 Unauthorized";
    description = message.isEmpty()
                      ? "Authentication is required to access this resource."
                      : message;
    break;
  case 403:
    title = "403 Forbidden";
    description = message.isEmpty()
                      ? "You don't have permission to access this resource."
                      : message;
    break;
  case 404:
    title = "404 Page Not Found";
    description = message.isEmpty()
                      ? "The requested page could not be found on this server."
                      : message;
    break;
  case 405:
    title = "405 Method Not Allowed";
    description = message.isEmpty()
                      ? "The request method is not allowed for this resource."
                      : message;
    break;
  case 500:
    title = "500 Internal Server Error";
    description = message.isEmpty()
                      ? "The server encountered an unexpected condition."
                      : message;
    break;
  case 502:
    title = "502 Bad Gateway";
    description = message.isEmpty() ? "The server received an invalid "
                                      "response from an upstream server."
                                    : message;
    break;
  case 503:
    title = "503 Service Unavailable";
    description =
        message.isEmpty() ? "The server is temporarily unavailable." : message;
    statusClass = "warning"; // Service unavailable is more of a warning
    break;
  default:
    title = String(statusCode) + " Error";
    description = message.isEmpty()
                      ? "An error occurred while processing your request."
                      : message;
    break;
  }

  // Generate HTML error page that adapts to any theme
  String html = "<!DOCTYPE html>\n";
  html += "<html lang=\"en\">\n";
  html += "<head>\n";
  html += "  <meta charset=\"UTF-8\">\n";
  html += "  <meta name=\"viewport\" content=\"width=device-width, "
          "initial-scale=1.0\">\n";
  html += "  <title>" + title + "</title>\n";
  html += "  <link rel=\"stylesheet\" href=\"/assets/style.css\">\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "  <div class=\"container\">\n";
  html += "    <!-- Navigation menu will be auto-injected here -->\n";
  html += "    <div class=\"error-page\">\n";
  html += "      <h1 class=\"" + statusClass + "\">" + title + "</h1>\n";
  html += "      <p class=\"error-description\">" + description + "</p>\n";
  html += "      <div class=\"status-message " + statusClass + "\">\n";
  html += "        <strong>What can you do?</strong><br>\n";
  html += "        • Check the URL for typos<br>\n";
  html += "        • Use the navigation menu above<br>\n";
  html += "        • Return to the home page\n";
  html += "      </div>\n";
  html += "      <div class=\"error-actions button-group\">\n";
  html +=
      "        <a href=\"/\" class=\"btn btn-primary\">Return to Home</a>\n";
  html += "        <a href=\"javascript:history.back()\" class=\"btn "
          "btn-secondary\">Go Back</a>\n";
  html += "      </div>\n";
  html += "    </div>\n";
  html += "  </div>\n";
  html += "</body>\n";
  html += "</html>\n";

  // DO NOT auto-inject navigation - we'll do that explicitly when we use the
  // error page This allows the caller to set the current path first
  return html;
}

// Phase 3: Route Redirection System (simplified for embedded use)
void IWebModule::addRedirect(const String &fromPath, const String &toPath) {
  // Simple exact path matching only - no dynamic manipulation needed
  redirectRules.push_back(RedirectRule(fromPath, toPath));
}

String IWebModule::getRedirectTarget(const String &requestPath) {
  // Simple exact path matching only
  for (const auto &rule : redirectRules) {
    if (rule.fromPath == requestPath) {
      return rule.toPath;
    }
  }

  // No redirect found
  return "";
}

// Phase 5: Static Asset Management
void IWebModule::addStaticAsset(const String &path, const String &content,
                                const String &mimeType, bool useProgmem) {
  staticAssets.push_back(StaticAsset(path, content, mimeType, useProgmem));
}

StaticAsset IWebModule::getStaticAsset(const String &path) {
  for (const auto &asset : staticAssets) {
    if (asset.path == path) {
      return asset;
    }
  }

  // Return empty asset if not found
  return StaticAsset("", "", "");
}

bool IWebModule::hasStaticAsset(const String &path) {
  for (const auto &asset : staticAssets) {
    if (asset.path == path) {
      return true;
    }
  }
  return false;
}

std::vector<WebRoute> IWebModule::getStaticAssetRoutes() {
  std::vector<WebRoute> routes;

  for (const auto &asset : staticAssets) {
    WebRoute route(
        asset.path, WebModule::WM_GET,
        [asset](const String &, const std::map<String, String> &) {
          return asset.content;
        },
        asset.mimeType, "Static asset: " + asset.path);
    routes.push_back(route);
  }

  return routes;
}

// Helper methods for common static assets
void IWebModule::addJavaScript(const String &path, const String &jsCode,
                               bool useProgmem) {
  addStaticAsset(path, jsCode, "application/javascript", useProgmem);
}

void IWebModule::addImage(const String &path, const String &imageData,
                          const String &imageType, bool useProgmem) {
  String mimeType = "image/" + imageType;
  addStaticAsset(path, imageData, mimeType, useProgmem);
}

void IWebModule::addFont(const String &path, const String &fontData,
                         const String &fontType, bool useProgmem) {
  String mimeType;
  if (fontType == "ttf" || fontType == "otf") {
    mimeType = "font/" + fontType;
  } else if (fontType == "woff") {
    mimeType = "font/woff";
  } else if (fontType == "woff2") {
    mimeType = "font/woff2";
  } else {
    mimeType = "application/octet-stream"; // Fallback for unknown font types
  }

  addStaticAsset(path, fontData, mimeType, useProgmem);
}

// Phase 6: Theme System Modernization
void IWebModule::initializeBaseTheme() {
  // Only initialize once
  if (themeInitialized)
    return;

  // Set base CSS framework (using existing default CSS as base)
  baseCSS = WEB_UI_DEFAULT_CSS;
  themeCSS = ""; // No theme overlay by default
  themeInitialized = true;
  globalCSSSet = false; // Disable legacy system
}

void IWebModule::setTheme(const String &newBaseCSS, const String &newThemeCSS) {
  baseCSS = newBaseCSS;
  themeCSS = newThemeCSS;
  themeInitialized = true;
  globalCSSSet = false; // Disable legacy system when using modern themes
}

std::vector<WebRoute> IWebModule::getThemeAssetRoutes() {
  std::vector<WebRoute> routes;

  // Serve base CSS at /assets/base.css
  if (!baseCSS.isEmpty()) {
    WebRoute baseRoute(
        "/assets/base.css", WebModule::WM_GET,
        [](const String &, const std::map<String, String> &) {
          return baseCSS;
        },
        "text/css", "Base CSS framework");
    routes.push_back(baseRoute);
  }

  // Serve theme CSS at /assets/theme.css
  if (!themeCSS.isEmpty()) {
    WebRoute themeRoute(
        "/assets/theme.css", WebModule::WM_GET,
        [](const String &, const std::map<String, String> &) {
          return themeCSS;
        },
        "text/css", "Theme overlay CSS");
    routes.push_back(themeRoute);
  }

  return routes;
}

// Phase 6: Theme System Modernization Implementation
void IWebModule::initializeBaseTheme() {
  // Only initialize once
  if (themeInitialized)
    return;

  // Set base CSS framework (this would come from theme_assets.h or similar)
  // For now, use the existing WEB_UI_DEFAULT_CSS as base
  baseCSS = WEB_UI_DEFAULT_CSS;
  themeCSS = ""; // No theme overlay by default
  themeInitialized = true;
  globalCSSSet = false; // Disable legacy system
}

void IWebModule::setTheme(const String &newBaseCSS, const String &newThemeCSS) {
  baseCSS = newBaseCSS;
  themeCSS = newThemeCSS;
  themeInitialized = true;
  globalCSSSet = false; // Disable legacy system when using modern themes
}

std::vector<WebRoute> IWebModule::getThemeAssetRoutes() {
  std::vector<WebRoute> routes;

  // Serve base CSS at /assets/base.css
  if (!baseCSS.isEmpty()) {
    WebRoute baseRoute(
        "/assets/base.css", WebModule::WM_GET,
        [](const String &, const std::map<String, String> &) {
          return baseCSS;
        },
        "text/css", "Base CSS framework");
    routes.push_back(baseRoute);
  }

  // Serve theme CSS at /assets/theme.css
  if (!themeCSS.isEmpty()) {
    WebRoute themeRoute(
        "/assets/theme.css", WebModule::WM_GET,
        [](const String &, const std::map<String, String> &) {
          return themeCSS;
        },
        "text/css", "Theme overlay CSS");
    routes.push_back(themeRoute);
  }

  return routes;
}
