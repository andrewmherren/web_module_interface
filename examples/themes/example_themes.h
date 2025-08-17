#ifndef EXAMPLE_THEMES_H
#define EXAMPLE_THEMES_H

#include <Arduino.h>

// Theme: Dark Fuschia
// A sleek dark theme with fuschia/pink accents
const char THEME_DARK_FUSCHIA[] PROGMEM = R"css(
/* Custom Dark Theme for Web UI */
body {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  margin: 0;
  padding: 20px;
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
  min-height: 100vh;
  color: #e94560;
}

.container {
  max-width: 800px;
  margin: 0 auto;
  background: rgba(233, 69, 96, 0.1);
  padding: 30px;
  border-radius: 15px;
  backdrop-filter: blur(10px);
  box-shadow: 0 8px 32px rgba(233, 69, 96, 0.3);
  border: 1px solid rgba(233, 69, 96, 0.3);
}

h1 {
  text-align: center;
  margin-bottom: 30px;
  font-size: 2.5em;
  text-shadow: 2px 2px 4px rgba(233, 69, 96, 0.5);
  color: #e94560;
}

.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 20px;
  margin-bottom: 30px;
}

.status-card {
  background: rgba(233, 69, 96, 0.15);
  padding: 20px;
  border-radius: 10px;
  border: 1px solid rgba(233, 69, 96, 0.3);
}

.status-card h3 {
  margin: 0 0 10px 0;
  color: #e94560;
  font-size: 1.2em;
}

.success { color: #00ff88; font-weight: bold; }
.info { color: #00d4ff; font-weight: bold; }
.warning { color: #ffaa00; font-weight: bold; }
.error { color: #ff0055; font-weight: bold; }

.btn-primary {
  background: rgba(233, 69, 96, 0.8);
  color: white;
  border: 1px solid rgba(233, 69, 96, 0.5);
}

.btn-primary:hover:not(:disabled) {
  background: rgba(233, 69, 96, 1);
  box-shadow: 0 4px 16px rgba(233, 69, 96, 0.4);
}

/* Navigation and button styling */
.nav-links {
  display: flex;
  justify-content: center;
  gap: 20px;
  margin-top: 30px;
  flex-wrap: wrap;
}

.nav-links a, .btn {
  background: rgba(233, 69, 96, 0.2);
  color: #e94560;
  text-decoration: none;
  padding: 12px 24px;
  margin: 4px 4px 4px 0;
  border-radius: 25px;
  border: 1px solid rgba(233, 69, 96, 0.4);
  cursor: pointer;
  font-size: 16px;
  font-weight: 500;
  min-width: 100px;
  transition: all 0.3s ease;
  text-align: center;
  display: inline-block;
}

.nav-links a:hover, .btn:hover:not(:disabled) {
  background: rgba(233, 69, 96, 0.4);
  transform: translateY(-2px);
  box-shadow: 0 4px 16px rgba(233, 69, 96, 0.3);
}

.pdo-card {
  border: 2px solid rgba(233, 69, 96, 0.3);
  background: rgba(233, 69, 96, 0.1);
}

.pdo-card.active {
  border-color: rgba(0, 255, 136, 0.8);
  background: rgba(0, 255, 136, 0.2);
}

.form-control {
  background: rgba(233, 69, 96, 0.1);
  border: 1px solid rgba(233, 69, 96, 0.3);
  color: #e94560;
}

.form-control:focus {
  border-color: rgba(233, 69, 96, 0.6);
  background: rgba(233, 69, 96, 0.2);
}
)css";

// Theme: Ocean Blue
// A calming blue theme with aqua accents
const char THEME_OCEAN_BLUE[] PROGMEM = R"css(
/* Ocean Blue Theme for Web UI */
body {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  margin: 0;
  padding: 20px;
  background: linear-gradient(135deg, #1a2a6c 0%, #2a4858 50%, #003366 100%);
  min-height: 100vh;
  color: #66ccff;
}

.container {
  max-width: 800px;
  margin: 0 auto;
  background: rgba(102, 204, 255, 0.1);
  padding: 30px;
  border-radius: 15px;
  backdrop-filter: blur(10px);
  box-shadow: 0 8px 32px rgba(102, 204, 255, 0.3);
  border: 1px solid rgba(102, 204, 255, 0.3);
}

h1 {
  text-align: center;
  margin-bottom: 30px;
  font-size: 2.5em;
  text-shadow: 2px 2px 4px rgba(102, 204, 255, 0.5);
  color: #66ccff;
}

.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 20px;
  margin-bottom: 30px;
}

.status-card {
  background: rgba(102, 204, 255, 0.15);
  padding: 20px;
  border-radius: 10px;
  border: 1px solid rgba(102, 204, 255, 0.3);
}

.status-card h3 {
  margin: 0 0 10px 0;
  color: #66ccff;
  font-size: 1.2em;
}

.success { color: #00ff88; font-weight: bold; }
.info { color: #ffffff; font-weight: bold; }
.warning { color: #ffaa00; font-weight: bold; }
.error { color: #ff5566; font-weight: bold; }

.btn-primary {
  background: rgba(102, 204, 255, 0.8);
  color: white;
  border: 1px solid rgba(102, 204, 255, 0.5);
}

.btn-primary:hover:not(:disabled) {
  background: rgba(102, 204, 255, 1);
  box-shadow: 0 4px 16px rgba(102, 204, 255, 0.4);
}

/* Navigation and button styling */
.nav-links {
  display: flex;
  justify-content: center;
  gap: 20px;
  margin-top: 30px;
  flex-wrap: wrap;
}

.nav-links a, .btn {
  background: rgba(102, 204, 255, 0.2);
  color: #ffffff;
  text-decoration: none;
  padding: 12px 24px;
  margin: 4px 4px 4px 0;
  border-radius: 25px;
  border: 1px solid rgba(102, 204, 255, 0.4);
  cursor: pointer;
  font-size: 16px;
  font-weight: 500;
  min-width: 100px;
  transition: all 0.3s ease;
  text-align: center;
  display: inline-block;
}

.nav-links a:hover, .btn:hover:not(:disabled) {
  background: rgba(102, 204, 255, 0.4);
  transform: translateY(-2px);
  box-shadow: 0 4px 16px rgba(102, 204, 255, 0.3);
}

.pdo-card {
  border: 2px solid rgba(102, 204, 255, 0.3);
  background: rgba(102, 204, 255, 0.1);
}

.pdo-card.active {
  border-color: rgba(0, 255, 136, 0.8);
  background: rgba(0, 255, 136, 0.2);
}

.form-control {
  background: rgba(102, 204, 255, 0.1);
  border: 1px solid rgba(102, 204, 255, 0.3);
  color: #ffffff;
}

.form-control:focus {
  border-color: rgba(102, 204, 255, 0.6);
  background: rgba(102, 204, 255, 0.2);
}
)css";

// Theme: Light Mode
// A clean, light-colored theme
const char THEME_LIGHT[] PROGMEM = R"css(
/* Light Theme for Web UI */
body {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  margin: 0;
  padding: 20px;
  background: #f5f7fa;
  min-height: 100vh;
  color: #333;
}

.container {
  max-width: 800px;
  margin: 0 auto;
  background: white;
  padding: 30px;
  border-radius: 15px;
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.1);
  border: 1px solid rgba(0, 0, 0, 0.05);
}

h1 {
  text-align: center;
  margin-bottom: 30px;
  font-size: 2.5em;
  color: #3d5af1;
}

.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 20px;
  margin-bottom: 30px;
}

.status-card {
  background: #f8f9ff;
  padding: 20px;
  border-radius: 10px;
  border: 1px solid rgba(61, 90, 241, 0.2);
}

.status-card h3 {
  margin: 0 0 10px 0;
  color: #3d5af1;
  font-size: 1.2em;
}

.success { color: #4CAF50; font-weight: bold; }
.info { color: #2196F3; font-weight: bold; }
.warning { color: #FF9800; font-weight: bold; }
.error { color: #F44336; font-weight: bold; }

.btn-primary {
  background: #3d5af1;
  color: white;
  border: 1px solid #3d5af1;
}

.btn-primary:hover:not(:disabled) {
  background: #2944d2;
  box-shadow: 0 4px 16px rgba(61, 90, 241, 0.3);
}

/* Navigation and button styling */
.nav-links {
  display: flex;
  justify-content: center;
  gap: 20px;
  margin-top: 30px;
  flex-wrap: wrap;
}

.nav-links a, .btn {
  background: white;
  color: #3d5af1;
  text-decoration: none;
  padding: 12px 24px;
  margin: 4px 4px 4px 0;
  border-radius: 25px;
  border: 1px solid #3d5af1;
  cursor: pointer;
  font-size: 16px;
  font-weight: 500;
  min-width: 100px;
  transition: all 0.3s ease;
  text-align: center;
  display: inline-block;
}

.nav-links a:hover, .btn:hover:not(:disabled) {
  background: #f5f7ff;
  transform: translateY(-2px);
  box-shadow: 0 4px 16px rgba(61, 90, 241, 0.2);
}

.pdo-card {
  border: 2px solid rgba(61, 90, 241, 0.2);
  background: white;
}

.pdo-card.active {
  border-color: #4CAF50;
  background: rgba(76, 175, 80, 0.1);
}

.form-control {
  background: white;
  border: 1px solid rgba(61, 90, 241, 0.3);
  color: #333;
}

.form-control:focus {
  border-color: #3d5af1;
  background: white;
  box-shadow: 0 0 0 3px rgba(61, 90, 241, 0.1);
}
)css";

#endif // EXAMPLE_THEMES_H