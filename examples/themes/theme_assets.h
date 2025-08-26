#ifndef THEME_ASSETS_H
#define THEME_ASSETS_H

#include <Arduino.h>

// This file demonstrates CSS variables approach for theming
// NOTE: This is an advanced example - for simple theming, use the complete
// themes in example_themes.h instead

// Base CSS framework using CSS variables for easy theming
const char CSS_VARIABLES_BASE[] PROGMEM = R"css(
/* CSS Variables-based Theme System */
:root {
  --primary-color: #3d5af1;
  --secondary-color: #2944d2;
  --success-color: #4CAF50;
  --info-color: #2196F3;
  --warning-color: #FF9800;
  --error-color: #F44336;
  
  --bg-color: #ffffff;
  --card-bg: #ffffff;
  --text-color: #333;
  --border-color: #dee2e6;
  --shadow-color: rgba(0,0,0,0.1);
  
  --font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  --border-radius: 8px;
  --transition: all 0.3s ease;
}

body {
  font-family: var(--font-family);
  margin: 0;
  padding: 20px;
  background: var(--bg-color);
  color: var(--text-color);
  min-height: 100vh;
  line-height: 1.6;
}

.container {
  max-width: 800px;
  margin: 0 auto;
  background: var(--card-bg);
  padding: 30px;
  border-radius: 15px;
  box-shadow: 0 4px 16px var(--shadow-color);
  border: 1px solid var(--border-color);
}

h1 {
  text-align: center;
  font-size: 2.5em;
  margin-bottom: 30px;
  color: var(--primary-color);
}

.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 20px;
  margin-bottom: 30px;
}

.status-card {
  background: var(--card-bg);
  border: 1px solid var(--border-color);
  border-radius: var(--border-radius);
  padding: 20px;
  box-shadow: 0 2px 4px var(--shadow-color);
  transition: var(--transition);
}

.status-card h3 {
  margin-top: 0;
  color: var(--primary-color);
  font-size: 1.3em;
}

.btn {
  background: var(--card-bg);
  color: var(--primary-color);
  text-decoration: none;
  padding: 12px 24px;
  margin: 4px;
  border-radius: 25px;
  border: 1px solid var(--primary-color);
  cursor: pointer;
  transition: var(--transition);
  display: inline-block;
}

.btn:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 16px var(--shadow-color);
}

.btn-primary {
  background: var(--primary-color);
  color: white;
}

.success { color: var(--success-color); font-weight: bold; }
.info { color: var(--info-color); font-weight: bold; }
.warning { color: var(--warning-color); font-weight: bold; }
.error { color: var(--error-color); font-weight: bold; }

.nav-links {
  display: flex;
  justify-content: center;
  gap: 20px;
  margin-top: 30px;
  flex-wrap: wrap;
}

.nav-links a {
  background: var(--card-bg);
  color: var(--primary-color);
  text-decoration: none;
  padding: 12px 24px;
  border-radius: 25px;
  border: 1px solid var(--primary-color);
  transition: var(--transition);
}

.nav-links a:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 16px var(--shadow-color);
}

.nav-links a.active {
  background: var(--primary-color);
  color: white;
}
)css";

// Example: Dark theme using CSS variable overrides
const char DARK_VARIABLE_OVERRIDE[] PROGMEM = R"css(
/* Dark Theme - Override CSS Variables */
:root {
  --primary-color: #66ccff;
  --secondary-color: #4da6cc;
  --success-color: #00ff88;
  --info-color: #00d4ff;
  --warning-color: #ffaa00;
  --error-color: #ff5566;
  
  --bg-color: #1a1a2e;
  --card-bg: #16213e;
  --text-color: #ffffff;
  --border-color: #334155;
  --shadow-color: rgba(0,0,0,0.3);
}

body {
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
}
)css";

#endif // THEME_ASSETS_H