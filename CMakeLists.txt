cmake_minimum_required(VERSION 3.14)

project(QuickVtkDemo VERSION 0.1 LANGUAGES CXX)
set(Qt5_DIR "/home/allen/soft/Qt6.2/5.15.2/gcc_64/")
set(QTDIR "/home/allen/soft/Qt6.2/5.15.2/gcc_64/")
set(VTK_DIR "/home/allen/usr/local/lib/cmake/vtk-9.1")
set(CMAKE_PREFIX_PATH ${Qt5_DIR} ${VTK_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5  REQUIRED COMPONENTS Core Quick Qml )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Qml )
#target_compile_features(
#    myQmlVtkPlugin
#    PRIVATE
#        cxx_auto_type
#        cxx_generic_lambdas
#)
set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        qquickvtkrenderitem.h
        qquickvtkrenderitem.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QuickVtkDemo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QuickVtkDemo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QuickVtkDemo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QuickVtkDemo
          ${PROJECT_SOURCES}
          resources.qrc
        )
    endif()
endif()
# VTK Libraries
set(VTK_DIR $ENV{VTK_DIR})
find_package(VTK REQUIRED NO_MODULE)
add_library(myQmlVtkPlugin SHARED Plugin/myqmlvtkplugin.h Plugin/myqmlvtkplugin.cpp)
list (APPEND RESOURCES TestQQuickVTK.qrc)
qt5_add_resources (PROJECT_SOURCES ${RESOURCES})
#target_compile_features(
#    add_library
#    PRIVATE
#        cxx_auto_type
#        cxx_generic_lambdas
#)
target_link_libraries(
    myQmlVtkPlugin
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::Qml
        ${VTK_LIBRARIES}
)
#add_library(myQmlVtkPlugin SHARED /home/allen/qtprojects/Test/QuickVtkDemo/qt-project.org/imports/VTK.9.1/libqmlvtkplugin.so)
include(${VTK_USE_FILE})
target_compile_definitions(QuickVtkDemo
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(QuickVtkDemo
  PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Qml
  ${VTK_LIBRARIES}
  ${SYSTEM_LIBS}
  /home/allen/qtprojects/Test/QuickVtkDemo/build/QuickVtkDemo-Desktop_Qt_5_15_2_GCC_64bit-Release/libmyQmlVtkPlugin.so)


set_target_properties(QuickVtkDemo PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(QuickVtkDemo)
    qt_finalize_executable(QuickVtkDemo)
endif()
