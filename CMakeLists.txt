cmake_minimum_required(VERSION 3.16)

project(unicon_launcher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick WebView)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appunicon_launcher
    main.cpp
)

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

    RESOURCES
        qtquickcontrols2.conf
        images/icons/Qt-logo-white-transparent.svg
        images/icons/home_dark.svg
        images/icons/home_light.svg
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

    SOURCES
        qtunnel.h
        qtunnel.cpp
        nodelistmodel.h
        nodelistmodel.cpp
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
