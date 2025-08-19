#include <web_module_interface.h>
#include <Arduino.h>
#include <map>

// Example function to generate different error pages based on context
String generateContextualErrorPage(int statusCode, const String &context) {
  String errorMessage;
  
  // Customize error message based on the specific context
  if (context == "api") {
    switch (statusCode) {
      case 400:
        errorMessage = "Bad request: The API request is malformed or missing required parameters.";
        break;
      case 401:
        errorMessage = "Unauthorized: Authentication required for this API endpoint.";
        break;
      case 404:
        errorMessage = "API endpoint not found. Check the documentation for available endpoints.";
        break;
      case 500:
        errorMessage = "API server error: The request couldn't be processed due to an internal issue.";
        break;
      default:
        errorMessage = "API error: An unexpected error occurred while processing your request.";
    }
  } 
  else if (context == "file") {
    switch (statusCode) {
      case 404:
        errorMessage = "File not found: The requested file does not exist on this system.";
        break;
      case 403:
        errorMessage = "Access denied: You don't have permission to access this file.";
        break;
      default:
        errorMessage = "File operation error: An error occurred while processing your file request.";
    }
  }
  else if (context == "user") {
    switch (statusCode) {
      case 404:
        errorMessage = "User not found: The specified user does not exist in the system.";
        break;
      case 403:
        errorMessage = "Access denied: You don't have permission to view this user's information.";
        break;
      default:
        errorMessage = "User operation error: An error occurred while processing your user request.";
    }
  }
  else {
    // Default generic messages
    switch (statusCode) {
      case 404:
        errorMessage = "Resource not found: The requested resource does not exist.";
        break;
      case 500:
        errorMessage = "Server error: The server encountered an unexpected condition.";
        break;
      default:
        errorMessage = "An error occurred while processing your request.";
    }
  }
  
  // Generate the error page with the custom message
  return IWebModule::generateDefaultErrorPage(statusCode, errorMessage);
}

// Example API handler that uses contextual error pages
String handleApiRequest(const String &requestBody, const std::map<String, String> &params) {
  // Parse request and check for errors
  bool isValidRequest = false; // Your validation logic here
  bool hasPermission = false;  // Your authentication logic here
  bool resourceExists = false; // Your resource check logic here
  
  // Set current path for navigation menu
  IWebModule::setCurrentPath("/api");
  
  if (!isValidRequest) {
    // Return 400 Bad Request with API-specific error message
    return IWebModule::injectNavigationMenu(generateContextualErrorPage(400, "api"));
  }
  
  if (!hasPermission) {
    // Return 403 Forbidden with API-specific error message
    return IWebModule::injectNavigationMenu(generateContextualErrorPage(403, "api"));
  }
  
  if (!resourceExists) {
    // Return 404 Not Found with API-specific error message
    return IWebModule::injectNavigationMenu(generateContextualErrorPage(404, "api"));
  }
  
  // Normal API response handling...
  return "API response data";
}

// Example file handler that uses contextual error pages
String handleFileRequest(const String &requestBody, const std::map<String, String> &params) {
  // Get filename parameter
  auto it = params.find("filename");
  if (it == params.end()) {
    // No filename provided
    return IWebModule::injectNavigationMenu(
        generateContextualErrorPage(400, "file"));
  }
  
  String filename = it->second;
  bool fileExists = false; // Your file check logic here
  bool hasFileAccess = true; // Your file permission check here
  
  // Set current path for navigation menu
  IWebModule::setCurrentPath("/files");
  
  if (!fileExists) {
    // Return 404 Not Found with file-specific error message
    return IWebModule::injectNavigationMenu(generateContextualErrorPage(404, "file"));
  }
  
  if (!hasFileAccess) {
    // Return 403 Forbidden with file-specific error message
    return IWebModule::injectNavigationMenu(generateContextualErrorPage(403, "file"));
  }
  
  // Normal file serving...
  return "File content";
}