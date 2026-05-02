![Aliza MS](package/archive/usr/share/icons/hicolor/128x128/apps/alizams.png)

Aliza MS - DICOM Viewer
=======================

Get
---

See latest release: [Releases](https://github.com/AlizaMedicalImaging/AlizaMS/releases)

S. [www.aliza-dicom-viewer.com](https://www.aliza-dicom-viewer.com) for other operating systems and versions.

Quick start
-----------

Select _DICOM scanner_ tab, open directory with DICOM files or DICOMDIR file (or drag-and-drop).

Select one or more series and click _arrow_ action (or double-click selected row) to load.

![Open](package/art/start0.png)

View short introductory videos on [Youtube Channel](https://www.youtube.com/channel/UCPGvoSYX7PC5XCp-81Q4MAg) _(new, WIP)_

Highlights
----------

 * 2D and 3D views with many tools
 * 3D view also for non-uniform images
 * Volume Rendering
 * Multi-planar reconstruction (MPR)
 * Fast directory scanner, DICOMDIR
 * Consistently de-identify DICOM, maintain integrity
 * RTSTRUCT contours
 * DICOM Study multi-view with intersection lines and side-by-side view
 * Proper measurement in Ultrasound Calibrated Regions
 * 2D+t and 3D+t animations
 * DICOM metadata viewer, DICOM 2025e dictionary
 * Encapsulated transfer syntaxes incl. HTJ2K
 * Most IODs are supported incl. Structured Reports, Key Objects Selection, Grayscale Softcopy Presentation State

![](https://github.com/AlizaMedicalImaging/AlizaMS/blob/master/package/art/alizams_scr1.jpg)

![](https://github.com/AlizaMedicalImaging/AlizaMS/blob/master/package/art/alizams_scr2.jpg)

![](https://github.com/AlizaMedicalImaging/AlizaMS/blob/master/package/art/alizams_scr3.jpg)

Build prerequisites
-------------------

 * CMake >= 3.11
 * C++ compiler with C++14 support (GCC, Clang, MSVC)
 * Qt 5 or Qt 6 (Core, Gui, Widgets, Svg, PrintSupport, Sql, OpenGLWidgets for Qt6)
 * ITK (Insight Toolkit) — modules: Common, ImageFilterBase, ImageFunction, ImageGrid, ImageIntensity, SpatialObjects, Statistics, Transform
 * OpenGL 3.2+ (Core Profile)

### Debian / Ubuntu (Qt6)

    sudo apt-get install cmake build-essential \
      qt6-base-dev qt6-svg-dev libqt6openglwidgets6 libqt6sql6 libqt6core5compat6-dev \
      libgl-dev libglu1-mesa-dev libinsighttoolkit5-dev

### Debian / Ubuntu (Qt5)

    sudo apt-get install cmake build-essential \
      qtbase5-dev libqt5svg5-dev libqt5sql5 libqt5opengl5-dev \
      libgl-dev libglu1-mesa-dev libinsighttoolkit5-dev

### Fedora

    sudo dnf install cmake gcc-c++ \
      qt6-qtbase-devel qt6-qtsvg-devel qt6-qt5compat-devel \
      mesa-libGL-devel InsightToolkit-devel

Build
-----

    cmake -B build -DALIZA_QT_VERSION=6 -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j$(nproc)

For Qt5, use `-DALIZA_QT_VERSION=5`. For Qt4, use `-DALIZA_QT_VERSION=4`.

S. [wiki](https://github.com/AlizaMedicalImaging/AlizaMS/wiki) for more build examples.

### Optional CMake options

 * `-DALIZA_COREGL=ON` — Use OpenGL Core Profile (default: ON)
 * `-DALIZA_DISABLE_SIMDMATH=ON` — Disable SSE/SSE2
 * `-DALIZA_VERBOSE=ON` — Print messages to stdout
 * `-DBUILD_TESTING=ON` — Build unit tests

### Running tests

    cmake -B build -DALIZA_QT_VERSION=6 -DBUILD_TESTING=ON
    cmake --build build
    cd build && ctest --output-on-failure

Supported platforms
-------------------

 * Linux (primary) — Debian, Ubuntu, Fedora, openSUSE
 * macOS — Qt6 only
 * Windows — MSVC

Bundled libraries
-----------------

 * Little CMS 2.19 (MIT) — ICC color management
 * Bullet Physics 3.25 (zlib) — collision detection for 3D view
 * MDCM 1.6.3 (BSD) — DICOM parser (fork of GDCM)
 * OpenJPEG 2.5.4 (BSD-2) — JPEG 2000 codec
 * CharLS 2.4.3 (BSD-3) — JPEG-LS codec
 * zlib 1.3.1 (zlib) — compression
 * IJG JPEG 6b (IJG) — JPEG codec

Contributing
------------

 1. Fork the repository
 2. Create a feature branch (`git checkout -b feature/my-change`)
 3. Commit your changes
 4. Push to the branch (`git push origin feature/my-change`)
 5. Open a Pull Request

License
-------

GPL-3.0. See [LICENSE](LICENSE) for details.
