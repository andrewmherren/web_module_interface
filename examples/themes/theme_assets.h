#ifndef THEME_ASSETS_H
#define THEME_ASSETS_H

#include <Arduino.h>

// Base CSS framework - provides foundation classes and standard naming conventions
const char BASE_CSS[] PROGMEM = R"css(
/* Base CSS Framework - Standard class and ID naming conventions */

/* CSS Variables for theming */
:root {
  --primary-color: #3d5af1;
  --secondary-color: #2944d2;
  --accent-color: #007bff;
  --success-color: #4CAF50;
  --info-color: #2196F3;
  --warning-color: #FF9800;
  --error-color: #F44336;
  
  --bg-color: #ffffff;
  --card-bg: #ffffff;
  --text-color: #333;
  --text-muted: #666;
  --border-color: #dee2e6;
  --shadow-color: rgba(0,0,0,0.1);
  
  --font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  --border-radius: 8px;
  --transition: all 0.3s ease;
}

/* Base typography */
body {
  font-family: var(--font-family);
  margin: 0;
  padding: 20px;
  background: var(--bg-color);
  color: var(--text-color);
  min-height: 100vh;
  line-height: 1.6;
}

/* Container and layout */
.container {
  max-width: 800px;
  margin: 0 auto;
  background: var(--card-bg);
  padding: 30px;
  border-radius: 15px;
  box-shadow: 0 4px 16px var(--shadow-color);
  border: 1px solid var(--border-color);
}

/* Typography */
h1, h2, h3, h4, h5, h6 {
  color: var(--primary-color);
  margin-top: 0;
  margin-bottom: 1rem;
}

h1 {
  text-align: center;
  font-size: 2.5em;
  margin-bottom: 30px;
}

/* Grid system */
.grid {
  display: grid;
  gap: 20px;
}

.grid-2 { grid-template-columns: repeat(2, 1fr); }
.grid-3 { grid-template-columns: repeat(3, 1fr); }
.grid-4 { grid-template-columns: repeat(4, 1fr); }
.grid-auto { grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); }

/* Status grid */
.status-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 20px;
  margin-bottom: 30px;
}

/* Cards */
.card {
  background: var(--card-bg);
  border: 1px solid var(--border-color);
  border-radius: var(--border-radius);
  padding: 20px;
  box-shadow: 0 2px 4px var(--shadow-color);
  transition: var(--transition);
}

.card:hover {
  transform: translateY(-2px);
  box-shadow: 0 4px 8px var(--shadow-color);
}

.card h3 {
  margin-top: 0;
  color: var(--primary-color);
  font-size: 1.3em;
}

.status-card { /* Alias for card */ }
.status-card h3 { /* Inherit from card h3 */ }

.pdo-card {
  border: 2px solid var(--border-color);
  background: var(--card-bg);
}

.pdo-card.active {
  border-color: var(--success-color);
  background: rgba(76, 175, 80, 0.1);
}

/* Buttons */
.btn {
  background: var(--card-bg);
  color: var(--primary-color);
  text-decoration: none;
  padding: 12px 24px;
  margin: 4px 4px 4px 0;
  border-radius: 25px;
  border: 1px solid var(--primary-color);
  cursor: pointer;
  font-size: 16px;
  font-weight: 500;
  min-width: 100px;
  transition: var(--transition);
  text-align: center;
  display: inline-block;
}

.btn:hover:not(:disabled) {
  background: rgba(61, 90, 241, 0.1);
  transform: translateY(-2px);
  box-shadow: 0 4px 16px rgba(61, 90, 241, 0.2);
}

.btn-primary {
  background: var(--primary-color);
  color: white;
  border: 1px solid var(--primary-color);
}

.btn-primary:hover:not(:disabled) {
  background: var(--secondary-color);
  box-shadow: 0 4px 16px rgba(61, 90, 241, 0.3);
}

.btn:disabled {
  opacity: 0.6;
  cursor: not-allowed;
}

/* Forms */
.form-control {
  width: 100%;
  padding: 12px;
  border: 1px solid var(--border-color);
  border-radius: var(--border-radius);
  background: var(--card-bg);
  color: var(--text-color);
  font-size: 16px;
  transition: var(--transition);
}

.form-control:focus {
  outline: none;
  border-color: var(--primary-color);
  box-shadow: 0 0 0 3px rgba(61, 90, 241, 0.1);
}

/* Status classes */
.success { color: var(--success-color); font-weight: bold; }
.info { color: var(--info-color); font-weight: bold; }
.warning { color: var(--warning-color); font-weight: bold; }
.error { color: var(--error-color); font-weight: bold; }

/* Navigation */
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
  margin: 4px 4px 4px 0;
  border-radius: 25px;
  border: 1px solid var(--primary-color);
  transition: var(--transition);
}

.nav-links a:hover {
  background: rgba(61, 90, 241, 0.1);
  transform: translateY(-2px);
  box-shadow: 0 4px 16px rgba(61, 90, 241, 0.2);
}

.nav-links a.active {
  background: var(--primary-color);
  color: white;
}

/* Footer */
.footer {
  text-align: center;
  margin-top: 40px;
  opacity: 0.7;
  font-size: 0.9em;
  padding-top: 20px;
  border-top: 1px solid var(--border-color);
}

/* Responsive design */
@media (max-width: 768px) {
  .container {
    padding: 20px;
  }
  
  h1 {
    font-size: 2em;
  }
  
  .grid-2, .grid-3, .grid-4 {
    grid-template-columns: 1fr;
  }
  
  .nav-links {
    gap: 10px;
  }
  
  .btn {
    min-width: 80px;
    font-size: 14px;
  }
}
)css";

// Light theme - overrides CSS variables for light appearance
const char LIGHT_THEME_CSS[] PROGMEM = R"css(
/* Light Theme - Clean, professional appearance */
:root {
  --primary-color: #3d5af1;
  --secondary-color: #2944d2;
  --accent-color: #007bff;
  --success-color: #4CAF50;
  --info-color: #2196F3;
  --warning-color: #FF9800;
  --error-color: #F44336;
  
  --bg-color: #f5f7fa;
  --card-bg: #ffffff;
  --text-color: #333;
  --text-muted: #666;
  --border-color: #dee2e6;
  --shadow-color: rgba(0,0,0,0.1);
}
)css";

// Dark theme - overrides CSS variables for dark appearance
const char DARK_THEME_CSS[] PROGMEM = R"css(
/* Dark Theme - Easy on the eyes */
:root {
  --primary-color: #66ccff;
  --secondary-color: #4da6cc;
  --accent-color: #5599dd;
  --success-color: #00ff88;
  --info-color: #00d4ff;
  --warning-color: #ffaa00;
  --error-color: #ff5566;
  
  --bg-color: #1a1a2e;
  --card-bg: #16213e;
  --text-color: #ffffff;
  --text-muted: #cccccc;
  --border-color: #334155;
  --shadow-color: rgba(0,0,0,0.3);
}

body {
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
}

.container {
  backdrop-filter: blur(10px);
}
)css";

// Fuschia theme - overrides CSS variables for fuschia/pink appearance
const char FUSCHIA_THEME_CSS[] PROGMEM = R"css(
/* Fuschia Theme - Bold and vibrant */
:root {
  --primary-color: #e94560;
  --secondary-color: #cc3951;
  --accent-color: #ff6b8a;
  --success-color: #00ff88;
  --info-color: #00d4ff;
  --warning-color: #ffaa00;
  --error-color: #ff0055;
  
  --bg-color: #1a1a2e;
  --card-bg: #16213e;
  --text-color: #e94560;
  --text-muted: #cc7788;
  --border-color: rgba(233, 69, 96, 0.3);
  --shadow-color: rgba(233, 69, 96, 0.3);
}

body {
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
}

.container {
  background: rgba(233, 69, 96, 0.1);
  backdrop-filter: blur(10px);
  border: 1px solid rgba(233, 69, 96, 0.3);
}

h1 {
  text-shadow: 2px 2px 4px rgba(233, 69, 96, 0.5);
}
)css";

#endif // THEME_ASSETS_H