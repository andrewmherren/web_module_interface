#ifndef WEB_REQUEST_H
#define WEB_REQUEST_H

#include "auth_types.h"
#include <Arduino.h>
#include <map>

// Forward declarations to avoid circular dependencies
#if defined(ESP32) || defined(ESP8266)
class WebServerClass;
#endif

#if defined(ESP32)
struct httpd_req;
#endif

/**
 * WebRequest - Unified request abstraction for HTTP/HTTPS handlers
 *
 * Part of the web_module_interface to provide a consistent interface
 * for accessing request data across Arduino WebServer and ESP-IDF
 * HTTP server implementations without modules needing to know about
 * WebPlatform internals.
 */
class WebRequest {
private:
  String path;
  String method;
  String body;
  std::map<String, String> params;
  std::map<String, String> headers;
  AuthContext authContext; // Authentication information

public:
  // Constructor for HTTP server (Arduino WebServer)
#if defined(ESP32) || defined(ESP8266)
  WebRequest(WebServerClass *server);
#endif

  // Constructor for HTTPS server (ESP-IDF)
#if defined(ESP32)
  WebRequest(httpd_req *req);
#endif

  // Request information
  String getPath() const { return path; }
  String getMethod() const { return method; }
  String getBody() const { return body; }

  // URL parameters (query string and POST form data)
  String getParam(const String &name) const;
  bool hasParam(const String &name) const;
  std::map<String, String> getAllParams() const { return params; }

  // Headers
  String getHeader(const String &name) const;
  bool hasHeader(const String &name) const;

  // Convenience methods
  String getQueryString() const;
  String getContentType() const;
  size_t getContentLength() const;

  // Authentication context
  const AuthContext &getAuthContext() const { return authContext; }
  void setAuthContext(const AuthContext &context) { authContext = context; }

private:
  void parseQueryParams(const String &query);
  void parseFormData(const String &formData);
  void parseHeaders();
};

#endif // WEB_REQUEST_H