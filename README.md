# Codenheimer ☢️💥💣

Codenheimer is a powerful and feature-rich application designed to help students and developers specially CP enthusiasts manage their coding workflow efficiently. It provides a seamless environment for organizing code snippets, syntax highlighting, and much more. The application is built using Qt and offers a fully dependency-free installer for easy setup.

## Features

### 🔹 **Code Management**
- Syntax highlighting for multiple programming languages (C, C++, Py, Java, CSS)
- Integrated file encryption using OpenSSL for secure storage
- Built-in code editor to save and see what's saved
- Quick access copy button
- Launch on system startup support
- Minimize to system tray
- Search engine for finding snippet easily
- Filters to browse efficiently
- Ability to add & remove user-defined color coded tags to the snippets

### 🔹 **UI & Customization**
- Styled with QSS (Qt Style Sheets) for a clean and modern look
- Smooth UI transitions with Qt animations
- System tray integration for quick access

### 🔹 **Cross-Platform Compatibility**
- Windows & macOS supported
- Uses Qt framework, ensuring high performance and portability

### 🔹 **Installation & Build Options**
- **Fully dependency-free installers** available in [Releases](https://github.com/Deatrax/codenheimer/releases)
- **Source code includes all dependencies** (except Qt), allowing easy compilation

## Installation

### 🔹 **Using Prebuilt Installers (Recommended)**
The easiest way to install Codenheimer is by downloading the latest release from the [Releases](https://github.com/Deatrax/codenheimer/releases) page. These installers contain all required dependencies, so no additional setup is needed.

1. Download the latest version for your platform.
2. Run the installer and follow the on-screen instructions. Now we are still students and do not have the means to buy a code-signing certificate. So please ignore the warning given by windows when the installer is run. Just Select the **"More info"** option.   - Then, click on **"Run anyway"** to proceed with the installation.
3. Launch Codenheimer and start coding!

### 🔹 **Building from Source**
If you prefer to build Codenheimer yourself, follow these steps:

#### **Prerequisites**
- Qt (at least Qt 6.x)
- OpenSSL (bundled with the source code, so no manual installation is needed)

#### **Steps to Build**
1. Clone the repository:
   ```sh
   git clone https://github.com/Deatrax/codenheimer.git
   cd Codenheimer
   ```
2. Open the project in Qt Creator or build via terminal:
   ```sh
   qmake Codenheimer.pro
   make  # or mingw32-make on Windows
   ```
3. Run the application:
   ```sh
   ./Codenheimer
   ```

## License
Codenheimer is licensed under **GPL 2.0**, allowing free use and modification while ensuring source code openness. However we would want you to credit us in accordance with the license T&Cs.

## Contributing
Contributions are welcome! If you'd like to improve Codenheimer, feel free to fork the repository, make changes, and submit a pull request.

## Contact & Support
For any issues or feature requests, please open an [issue](https://github.com/Deatrax/codenheimer/issues) or reach out to the development team.

---

🚀 Happy Coding with Codenheimer!

