#include "../include/web_module_interface.h"

// Static member definitions - defined once in the library
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