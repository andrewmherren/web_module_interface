// Example showing how to integrate static asset management in main.cpp

#include <Arduino.h>
#include <WiFi.h>
#include "web_module_interface.h"
#include "web_router.h"
#include "example_module.h"

// WiFi credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

void setupStaticAssets() {
    Serial.println("Setting up static assets...");
    
    // Setup assets for the example module
    exampleModule.setupStaticAssets();
    
    // Add global JavaScript utilities that all modules can use
    String globalUtils = R"(
        // Global utility functions available to all pages
        window.ESP32Utils = {
            formatBytes: function(bytes) {
                if (bytes === 0) return '0 Bytes';
                const k = 1024;
                const sizes = ['Bytes', 'KB', 'MB', 'GB'];
                const i = Math.floor(Math.log(bytes) / Math.log(k));
                return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
            },
            
            formatUptime: function(seconds) {
                const days = Math.floor(seconds / (24 * 60 * 60));
                const hours = Math.floor((seconds % (24 * 60 * 60)) / (60 * 60));
                const minutes = Math.floor((seconds % (60 * 60)) / 60);
                const secs = seconds % 60;
                
                let result = [];
                if (days > 0) result.push(days + 'd');
                if (hours > 0) result.push(hours + 'h');
                if (minutes > 0) result.push(minutes + 'm');
                result.push(secs + 's');
                
                return result.join(' ');
            },
            
            async apiCall: async function(endpoint, method = 'GET', data = null) {
                try {
                    const options = {
                        method: method,
                        headers: {
                            'Content-Type': 'application/json',
                        }
                    };
                    
                    if (data && method !== 'GET') {
                        options.body = JSON.stringify(data);
                    }
                    
                    const response = await fetch(endpoint, options);
                    return await response.json();
                } catch (error) {
                    console.error('API call failed:', error);
                    throw error;
                }
            }
        };
    )";
    
    IWebModule::addJavaScript("/assets/global-utils.js", globalUtils);
    
    // Add a favicon
    String faviconSVG = R"(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 32 32">
        <circle cx="16" cy="16" r="14" fill="#007bff"/>
        <text x="16" y="22" text-anchor="middle" fill="white" font-family="Arial" font-size="18" font-weight="bold">E</text>
    </svg>)";
    
    IWebModule::addStaticAsset("/favicon.svg", faviconSVG, "image/svg+xml");
    
    // Add global CSS enhancements
    String enhancedCSS = IWebModule::getGlobalCSS() + R"(
        /* Additional global enhancements */
        .module-logo {
            height: 32px;
            width: auto;
            margin-right: 10px;
            vertical-align: middle;
        }
        
        .page-header {
            text-align: center;
            margin-bottom: 30px;
            padding: 20px;
            background: var(--card-bg, #ffffff);
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        
        .page-header h1 {
            margin: 10px 0;
            color: var(--primary-color, #007bff);
        }
        
        .btn-outline {
            background: transparent;
            border: 2px solid var(--primary-color, #007bff);
            color: var(--primary-color, #007bff);
        }
        
        .btn-outline:hover {
            background: var(--primary-color, #007bff);
            color: white;
        }
    )";
    
    IWebModule::setGlobalCSS(enhancedCSS);
    
    Serial.println("Static assets setup complete");
}

void setupNavigation() {
    Serial.println("Setting up navigation menu...");
    
    std::vector<NavigationItem> navItems = {
        NavigationItem("Home", "/"),
        NavigationItem("Device Status", "/api/status"),
        NavigationItem("Configuration", "/assets/config.json")
    };
    
    IWebModule::setNavigationMenu(navItems);
    
    Serial.println("Navigation menu setup complete");
}

void setupErrorPages() {
    Serial.println("Setting up custom error pages...");
    
    // Custom 404 page with asset links
    String custom404 = R"(<!DOCTYPE html>
<html>
<head>
    <title>Page Not Found - ESP32</title>
    <link rel="stylesheet" href="/assets/style.css">
</head>
<body>
    <div class="container">
        <div class="error-page">
            <h1 class="error">404 - Page Not Found</h1>
            <p>The requested page could not be found on this ESP32 device.</p>
            
            <div class="status-message error">
                <strong>Available Resources:</strong><br>
                • <a href="/">Main Page</a><br>
                • <a href="/api/status">Device Status API</a><br>
                • <a href="/assets/config.json">Configuration</a><br>
                • <a href="/assets/global-utils.js">JavaScript Utilities</a>
            </div>
            
            <div class="button-group">
                <a href="/" class="btn btn-primary">Go Home</a>
                <a href="javascript:history.back()" class="btn btn-secondary">Go Back</a>
            </div>
        </div>
    </div>
    <script src="/assets/global-utils.js"></script>
</body>
</html>)";
    
    IWebModule::setErrorPage(404, custom404);
    
    Serial.println("Error pages setup complete");
}

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    
    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    // Setup web interface components
    setupStaticAssets();
    setupNavigation();
    setupErrorPages();
    
    // Initialize web router
    Serial.println("Starting web router...");
    WebRouter::begin();
    
    // Register modules
    WebRouter::registerModule("/example", &exampleModule);
    
    Serial.println("Web server is running!");
    Serial.println("Visit http://" + WiFi.localIP().toString() + "/example");
}

void loop() {
    // Handle web server
    WebRouter::handleClient();
    
    // Add any other loop processing here
    delay(10);
}