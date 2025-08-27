#include "web_request.h"

#if defined(ESP32) || defined(ESP8266)
#if defined(ESP32)
#include <WebServer.h>
typedef WebServer WebServerClass;
#elif defined(ESP8266)
#include <ESP8266WebServer.h>
typedef ESP8266WebServer WebServerClass;
#endif
#endif

#if defined(ESP32)
#include <esp_http_server.h>
#endif

// Constructor for Arduino WebServer
#if defined(ESP32) || defined(ESP8266)
WebRequest::WebRequest(WebServerClass* server) {
    if (!server) return;
    
    path = server->uri();
    method = server->method() == HTTP_GET ? "GET" : 
             server->method() == HTTP_POST ? "POST" :
             server->method() == HTTP_PUT ? "PUT" :
             server->method() == HTTP_DELETE ? "DELETE" :
             server->method() == HTTP_PATCH ? "PATCH" : "GET";
    
    // Get request body for POST requests
    if (server->method() == HTTP_POST) {
        body = server->arg("plain");
    }
    
    // Parse URL parameters (query string)
    for (int i = 0; i < server->args(); i++) {
        params[server->argName(i)] = server->arg(i);
    }
    
    // Parse headers
    for (int i = 0; i < server->headers(); i++) {
        headers[server->headerName(i)] = server->header(i);
    }
}
#endif

// Constructor for ESP-IDF HTTPS server
#if defined(ESP32)
WebRequest::WebRequest(httpd_req* req) {
    if (!req) return;
    
    path = String(req->uri);
    
    // Convert HTTP method
    switch (req->method) {
        case HTTP_GET:
            method = "GET";
            break;
        case HTTP_POST:
            method = "POST";
            break;
        case HTTP_PUT:
            method = "PUT";
            break;
        case HTTP_DELETE:
            method = "DELETE";
            break;
        default:
            method = "GET";
            break;
    }
    
    // Parse query string
    size_t query_len = httpd_req_get_url_query_len(req);
    if (query_len > 0) {
        char* query = new char[query_len + 1];
        if (httpd_req_get_url_query_str(req, query, query_len + 1) == ESP_OK) {
            parseQueryParams(String(query));
        }
        delete[] query;
    }
    
    // Get request body for POST requests
    if (req->method == HTTP_POST && req->content_len > 0) {
        char* content = new char[req->content_len + 1];
        int received = httpd_req_recv(req, content, req->content_len);
        if (received > 0) {
            content[received] = '\0';
            body = String(content);
            
            // Parse form data if content type is form-urlencoded
            String contentType = getHeader("Content-Type");
            if (contentType.indexOf("application/x-www-form-urlencoded") >= 0) {
                parseFormData(body);
            }
        }
        delete[] content;
    }
    
    // Parse headers
    size_t header_count = httpd_req_get_hdr_value_len(req, "");
    // Note: ESP-IDF doesn't provide easy iteration over all headers
    // Individual headers need to be requested by name
    // Common headers can be pre-populated here if needed
}
#endif

String WebRequest::getParam(const String& name) const {
    auto it = params.find(name);
    return (it != params.end()) ? it->second : String();
}

bool WebRequest::hasParam(const String& name) const {
    return params.find(name) != params.end();
}

String WebRequest::getHeader(const String& name) const {
    auto it = headers.find(name);
    return (it != headers.end()) ? it->second : String();
}

bool WebRequest::hasHeader(const String& name) const {
    return headers.find(name) != headers.end();
}

String WebRequest::getQueryString() const {
    String query = "";
    bool first = true;
    
    for (const auto& param : params) {
        if (!first) {
            query += "&";
        }
        query += param.first + "=" + param.second;
        first = false;
    }
    
    return query;
}

String WebRequest::getContentType() const {
    return getHeader("Content-Type");
}

size_t WebRequest::getContentLength() const {
    String lengthStr = getHeader("Content-Length");
    return lengthStr.length() > 0 ? lengthStr.toInt() : 0;
}

void WebRequest::parseQueryParams(const String& query) {
    if (query.length() == 0) return;
    
    int start = 0;
    int end = query.indexOf('&');
    
    while (start < (int)query.length()) {
        String param = (end == -1) ? query.substring(start) : query.substring(start, end);
        
        int equalPos = param.indexOf('=');
        if (equalPos > 0) {
            String key = param.substring(0, equalPos);
            String value = param.substring(equalPos + 1);
            
            // URL decode (basic implementation)
            value.replace("+", " ");
            value.replace("%20", " ");
            
            params[key] = value;
        }
        
        if (end == -1) break;
        
        start = end + 1;
        end = query.indexOf('&', start);
    }
}

void WebRequest::parseFormData(const String& formData) {
    parseQueryParams(formData); // Form data uses same format as query params
}