#ifndef EXAMPLE_MODULE_H
#define EXAMPLE_MODULE_H

#include "web_module_interface.h"
#include <WiFi.h>

class ExampleModule : public IWebModule {
public:
    ExampleModule() = default;
    ~ExampleModule() = default;
    
    // IWebModule implementation
    std::vector<WebRoute> getHttpRoutes() override;
    std::vector<WebRoute> getHttpsRoutes() override;
    String getModuleName() const override { return "Example Module"; }
    String getModuleVersion() const override { return "1.0.0"; }
    String getModuleDescription() const override { 
        return "Example module demonstrating static asset management"; 
    }
    
    // Setup method to initialize static assets
    void setupStaticAssets();

private:
    // Helper methods for generating responses
    static String generateMainPage();
    static String getStatusJSON();
    static String toggleLEDJSON();
};

// Global instance
extern ExampleModule exampleModule;

#endif // EXAMPLE_MODULE_H