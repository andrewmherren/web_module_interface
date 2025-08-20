#include "example_module.h"
#include "web_module_interface.h"

// Static assets stored in PROGMEM to save RAM
const char EXAMPLE_JS[] PROGMEM = R"(
// Example JavaScript for interactive features
class DeviceController {
    constructor() {
        this.statusElement = document.getElementById('device-status');
        this.initializeEventListeners();
        this.startStatusPolling();
    }
    
    initializeEventListeners() {
        // Bind button clicks
        document.getElementById('refresh-btn').addEventListener('click', () => {
            this.refreshStatus();
        });
        
        document.getElementById('toggle-led').addEventListener('click', () => {
            this.toggleLED();
        });
    }
    
    async refreshStatus() {
        try {
            const response = await fetch('/api/status');
            const data = await response.json();
            this.updateStatusDisplay(data);
        } catch (error) {
            console.error('Failed to fetch status:', error);
            this.showError('Failed to connect to device');
        }
    }
    
    async toggleLED() {
        try {
            const response = await fetch('/api/led/toggle', { method: 'POST' });
            const data = await response.json();
            this.showMessage(data.message);
        } catch (error) {
            console.error('Failed to toggle LED:', error);
            this.showError('Failed to toggle LED');
        }
    }
    
    updateStatusDisplay(data) {
        this.statusElement.innerHTML = `
            <div class="status-item">
                <strong>Uptime:</strong> ${data.uptime}
            </div>
            <div class="status-item">
                <strong>Free Memory:</strong> ${data.freeMemory} bytes
            </div>
            <div class="status-item">
                <strong>WiFi RSSI:</strong> ${data.wifiRSSI} dBm
            </div>
        `;
    }
    
    startStatusPolling() {
        // Refresh status every 10 seconds
        setInterval(() => this.refreshStatus(), 10000);
        // Initial load
        this.refreshStatus();
    }
    
    showMessage(message) {
        this.showNotification(message, 'success');
    }
    
    showError(message) {
        this.showNotification(message, 'error');
    }
    
    showNotification(message, type) {
        const notification = document.createElement('div');
        notification.className = `notification ${type}`;
        notification.textContent = message;
        
        document.body.appendChild(notification);
        
        // Auto-remove after 3 seconds
        setTimeout(() => {
            if (notification.parentElement) {
                notification.parentElement.removeChild(notification);
            }
        }, 3000);
    }
}

// Initialize when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    new DeviceController();
});
)";

const char EXAMPLE_CSS[] PROGMEM = R"(
/* Module-specific CSS styles */
.device-controls {
    background: var(--card-bg, #ffffff);
    border: 1px solid var(--border-color, #dee2e6);
    border-radius: 8px;
    padding: 20px;
    margin: 20px 0;
}

.status-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 15px;
    margin: 20px 0;
}

.status-item {
    background: var(--secondary-bg, #f8f9fa);
    padding: 15px;
    border-radius: 6px;
    border-left: 4px solid var(--primary-color, #007bff);
}

.control-buttons {
    display: flex;
    gap: 10px;
    margin-top: 20px;
}

.notification {
    position: fixed;
    top: 20px;
    right: 20px;
    padding: 15px 20px;
    border-radius: 6px;
    color: white;
    font-weight: bold;
    z-index: 1000;
    animation: slideIn 0.3s ease-out;
}

.notification.success {
    background: #28a745;
}

.notification.error {
    background: #dc3545;
}

@keyframes slideIn {
    from {
        transform: translateX(100%);
        opacity: 0;
    }
    to {
        transform: translateX(0);
        opacity: 1;
    }
}

/* Responsive design */
@media (max-width: 768px) {
    .control-buttons {
        flex-direction: column;
    }
    
    .status-grid {
        grid-template-columns: 1fr;
    }
}
)";

// Small example logo (1x1 transparent pixel in PNG format, base64 encoded)
const char EXAMPLE_LOGO[] PROGMEM = 
    "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChAGAWA0ddwAAAABJRU5ErkJggg==";

// Example module implementation
ExampleModule exampleModule;

void ExampleModule::setupStaticAssets() {
    // Add JavaScript with PROGMEM
    IWebModule::addJavaScript("/assets/example.js", String(FPSTR(EXAMPLE_JS)), true);
    
    // Add module-specific CSS
    IWebModule::addStaticAsset("/assets/example.css", String(FPSTR(EXAMPLE_CSS)), "text/css", true);
    
    // Add example logo
    IWebModule::addImage("/assets/example-logo.png", String(FPSTR(EXAMPLE_LOGO)), "png", true);
    
    // Add a sample JSON file
    String sampleConfig = R"({
        "module": "example",
        "version": "1.0.0",
        "features": ["status_polling", "led_control", "responsive_ui"],
        "refresh_interval": 10000
    })";
    
    IWebModule::addStaticAsset("/assets/config.json", sampleConfig, "application/json");
}

std::vector<WebRoute> ExampleModule::getHttpRoutes() {
    std::vector<WebRoute> routes;
    
    // Main page
    routes.push_back(WebRoute("/", WebModule::WM_GET, 
        [](const String &, const std::map<String, String> &) {
            return generateMainPage();
        }, "text/html", "Example module main page"));
    
    // API endpoints
    routes.push_back(WebRoute("/api/status", WebModule::WM_GET,
        [](const String &, const std::map<String, String> &) {
            return getStatusJSON();
        }, "application/json", "Device status API"));
        
    routes.push_back(WebRoute("/api/led/toggle", WebModule::WM_POST,
        [](const String &, const std::map<String, String> &) {
            return toggleLEDJSON();
        }, "application/json", "LED toggle API"));
    
    // Add static asset routes
    auto assetRoutes = IWebModule::getStaticAssetRoutes();
    routes.insert(routes.end(), assetRoutes.begin(), assetRoutes.end());
    
    return routes;
}

std::vector<WebRoute> ExampleModule::getHttpsRoutes() {
    // Same routes for HTTPS
    return getHttpRoutes();
}

String ExampleModule::generateMainPage() {
    String html = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Example Module</title>
    <link rel="stylesheet" href="/assets/style.css">
    <link rel="stylesheet" href="/assets/example.css">
</head>
<body>
    <div class="container">
        <!-- Navigation menu will be auto-injected here -->
        
        <div class="page-header">
            <img src="/assets/example-logo.png" alt="Example" class="module-logo">
            <h1>Example Module</h1>
            <p>Demonstrating static asset management with interactive features</p>
        </div>
        
        <div class="device-controls">
            <h2>Device Status</h2>
            <div id="device-status" class="status-grid">
                <div class="status-item">Loading...</div>
            </div>
            
            <div class="control-buttons">
                <button id="refresh-btn" class="btn btn-primary">Refresh Status</button>
                <button id="toggle-led" class="btn btn-secondary">Toggle LED</button>
            </div>
        </div>
        
        <div class="device-controls">
            <h2>Static Asset Features</h2>
            <ul>
                <li>✅ Custom JavaScript for interactivity</li>
                <li>✅ Module-specific CSS styling</li>
                <li>✅ Embedded images and logos</li>
                <li>✅ JSON configuration files</li>
                <li>✅ PROGMEM support for memory efficiency</li>
            </ul>
            
            <p>
                <a href="/assets/config.json" target="_blank" class="btn btn-outline">
                    View Configuration JSON
                </a>
            </p>
        </div>
    </div>
    
    <script src="/assets/example.js"></script>
</body>
</html>)";

    // Apply global CSS and navigation injection
    html = IWebModule::injectCSSLink(html);
    html = IWebModule::injectNavigationMenu(html);
    
    return html;
}

String ExampleModule::getStatusJSON() {
    // Simulate device status data
    String json = "{";
    json += "\"uptime\":\"" + String(millis() / 1000) + " seconds\",";
    json += "\"freeMemory\":" + String(ESP.getFreeHeap()) + ",";
    json += "\"wifiRSSI\":" + String(WiFi.RSSI()) + ",";
    json += "\"timestamp\":" + String(millis());
    json += "}";
    
    return json;
}

String ExampleModule::toggleLEDJSON() {
    // Simulate LED toggle (you would implement actual LED control here)
    static bool ledState = false;
    ledState = !ledState;
    
    // In real implementation, you would control actual LED pin here
    // digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    
    String json = "{";
    json += "\"success\":true,";
    json += "\"ledState\":" + String(ledState ? "true" : "false") + ",";
    json += "\"message\":\"LED " + String(ledState ? "ON" : "OFF") + "\"";
    json += "}";
    
    return json;
}