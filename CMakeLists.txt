cmake_minimum_required(VERSION 3.16)

project(ContactsDisplay VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appContactsDisplay
    main.cpp
)

qt_add_qml_module(appContactsDisplay
    URI ContactsDisplay
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES contacts.h contacts.cpp
)

set_target_properties(appContactsDisplay PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android

)

target_link_libraries(appContactsDisplay
    PRIVATE Qt6::Quick
)

install(TARGETS appContactsDisplay
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_add_resources(appContactsDisplay "android"
    PREFIX /
    FILES
        android/src/com/example/contactsdisplay/MainActivity.java
        android/src/com/example/contactsdisplay/Contacts.java
        android/src/com/example/contactsdisplay/Contact.java
        android/AndroidManifest.xml
        android/build.gradle
        android/res/values/libs.xml
)
