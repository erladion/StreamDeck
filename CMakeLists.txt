cmake_minimum_required(VERSION 3.5)

project(streamdeck VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-fpermissive")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(hidapi)
add_subdirectory(phantomstyle)

include_directories(gui)
include_directories(actions)
include_directories(style)

set(PROJECT_SOURCES
        main.cpp
        resources.qrc
        streamdeckinterface.h
        streamdeck.h streamdeck.cpp
        deckhandler.h deckhandler.cpp
        configurationhandler.h configurationhandler.cpp
        configuration.h configuration.cpp
        gui/mainwindow.h gui/mainwindow.cpp
        gui/deckbutton.h gui/deckbutton.cpp
        gui/imageselectiondialog.h gui/imageselectiondialog.cpp
        actions/action.h actions/action.cpp
        actions/brightnessaction.h actions/brightnessaction.cpp
        actions/nextpageaction.h actions/nextpageaction.cpp
        actions/previouspageaction.h actions/previouspageaction.cpp
)

include_directories(hidapi/hidapi)
include_directories(phantomstyle/src/phantom)

if(WIN32)
  set(HIDAPI
    hidapi/windows/hid.c
    hidapi/hidapi/hidapi.h
  )
else()
  set(HIDAPI
    hidapi/linux/hid.c
    hidapi/hidapi/hidapi.h
  )
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(streamdeck
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES} ${HIDAPI}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET streamdeck APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(streamdeck SHARED
            ${PROJECT_SOURCES} ${HIDAPI}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(streamdeck
            ${PROJECT_SOURCES} ${HIDAPI}
        )
    endif()
endif()

target_link_libraries(streamdeck PRIVATE Qt${QT_VERSION_MAJOR}::Widgets libphantomstyle)

if(WIN32)
  target_link_libraries(streamdeck PRIVATE wsock32)
endif()

set_target_properties(streamdeck PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS streamdeck
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(streamdeck)
endif()
