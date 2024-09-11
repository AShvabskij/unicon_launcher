cmake_minimum_required(VERSION 3.16)

project(unicon_launcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick WebView)

qt_standard_project_setup(REQUIRES 6.5)


file(GLOB IP_DISCOVERY_SOURCES "./RSP_IP_WIN_discovery/*.cpp")
file(GLOB IP_DISCOVERY_HEADERS "./RSP_IP_WIN_discovery/*.h")

qt_add_executable(appunicon_launcher
    main.cpp
    ${IP_DISCOVERY_HEADERS}
)

# SET_TARGET_PROPERTIES(appunicon_launcher PROPERTIES LINK_FLAGS    "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS")

target_include_directories(appunicon_launcher PRIVATE ${IP_DISCOVERY_HEADERS})

target_sources(appunicon_launcher PRIVATE ${IP_DISCOVERY_SOURCES})

set_source_files_properties(Constants.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
    )

set_source_files_properties(Colors.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(appunicon_launcher
    URI unicon_launcher
    VERSION 1.0
    QML_FILES
        Main.qml
        ApplicationFlow.qml
        ApplicationFlowForm.ui.qml
        Constants.qml
        Colors.qml
        Home.qml
        CustomButton.qml
        CustomButtonForm.ui.qml
        CustomToolBar.qml
        CustomToolBarForm.ui.qml
        Local.qml
        Remote.qml
        ProgressItem.qml
        NodeItem.qml
        RemoteNode.qml
        WarningBox.qml
        Settings.qml
        CustomRowSingleTextInput.qml
        CustomRowDblTextInput.qml

    RESOURCES
        qtquickcontrols2.conf
        images/icons/home_dark.svg
        images/icons/home_light.svg
        images/icons/rst_symbol.svg
        images/icons/home.svg
        images/icons/keyboard_backspace_black.svg
        images/icons/keyboard_backspace_black_left.svg
        images/icons/keyboard_backspace_black_right.svg
        images/icons/keyboard_backspace_white_left.svg
        images/icons/keyboard_backspace_white_right.svg
        images/icons/light_mode_black_24dp.svg
        images/icons/dark_mode_black_24dp.svg
        images/icons/check.svg
        images/icons/ellipse_dark.svg
        images/icons/ellipse_light.svg
        images/icons/node.svg
        images/icons/flag.svg
        images/icons/logo.png
        images/icons/logo_window.png
        images/icons/settings.svg
        images/icons/stats.svg
        images/icons/refresh.svg
        images/icons/email.svg
        images/icons/folder-plus.svg

        fonts/montserrat/montserrat-latin-600-normal.ttf
        fonts/montserrat/montserrat-latin-400-normal.ttf
        fonts/montserrat/montserrat-thin.ttf
        fonts/montserrat/montserrat-regular.ttf
        fonts/montserrat/montserrat-bold.ttf
        fonts/montserrat/montserrat-semibold.ttf

    SOURCES
        qtunnel.h
        qtunnel.cpp
        nodelistmodel.h
        nodelistmodel.cpp
        licenseservice.h
        licenseservice.cpp
        tunnelservice.h
        tunnelservice.cpp
        localmodel.h
        localmodel.cpp
        licensemodel.h
        licensemodel.cpp

    RESOURCES
        resources.qrc
        images/icons/logo_window.png
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appunicon_launcher PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appunicon_launcher
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appunicon_launcher
    PRIVATE Qt6::Quick
    Qt6::WebView
)

include(GNUInstallDirs)
install(TARGETS appunicon_launcher
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
