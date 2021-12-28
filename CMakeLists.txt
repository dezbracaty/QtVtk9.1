###########
# Project #
###########
set(EXENAME QtVtk)
variable_watch(VTK_DIR)

if(NOT CONFIGURED)
        cmake_minimum_required(VERSION 3.4)
        project(${EXENAME})
        set(PROJECT_SOURCE_DIR ../)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
        link_directories(${PROJECT_SOURCE_DIR}/build)
        # Indicate that previous configuration exists
        set(CONFIGURED TRUE)
endif(NOT CONFIGURED)

if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()


#########################
# External Dependencies #
#########################

# Find the Qt libraries
set(Qt5_DIR "/home/allen/soft/Qt6.2/5.15.2/gcc_64/")
set(QTDIR "/home/allen/soft/Qt6.2/5.15.2/gcc_64/")
set(VTK_DIR "/home/allen/soft/src/VTK-9.1.0/Dbuild/")
set(CMAKE_PREFIX_PATH ${Qt5_DIR} ${VTK_DIR})

#set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt5Quick REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Qml REQUIRED)

find_package(Qt5QuickControls2 REQUIRED)
find_package(Qt5Quick REQUIRED)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# VTK Libraries
set(VTK_DIR $ENV{VTK_DIR})
find_package(VTK REQUIRED NO_MODULE)
include(${VTK_USE_FILE})

# Headers directories
include_directories(${PROJECT_SOURCE_DIR}
                        ${PROJECT_SOURCE_DIR}/src
)

###########
# Targets #
###########

# Add QML files to project tree in Qt Creator
file(GLOB QML_DESKTOP "../resources/*.qml")
add_custom_target(QML_files SOURCES ${QML_DESKTOP})

# Sources
set (SOURCES
    main.cpp
    qml.qrc
    qtvtkitem.h
    qtvtkitem.cpp
    vtkinteractorstyletrackballcamerastyle2.h
    vtkinteractorstyletrackballcamerastyle2.cpp
    CommandModel.cpp
    CommandModel.h
    CommandModelAdd.cpp
    CommandModelAdd.h
    Model.cpp
    Model.h
    ProcessingEngine.cpp
    ProcessingEngine.h
    qquickvtkrenderitem.h
    qquickvtkrenderitem.cpp
)

if (NOT APPLE)
        add_definitions(-std=c++11 -fext-numeric-literals -DPTHREADS_USED)
else()
        add_definitions(-std=c++11)
endif()
add_definitions(-DQT_NO_VERSION_TAGGING)    #compiler errorundefined reference to `qt_version_tag@Qt_5.15'
# Qt Resources
qt5_add_resources(RESOURCES qml.qrc)

# Build executable
if (WIN32)
    add_executable(${EXENAME} WIN32 ${HEADERS} ${SOURCES} ${RESOURCES})
else()
    add_executable(${EXENAME} ${HEADERS} ${SOURCES} ${RESOURCES})
endif()
#set (EXCLUDE_DIR "/usr/lib/x86_64-linux-gnu/")      #compiler errorundefined reference to `qt_version_tag@Qt_5.15'

# Link to libraries
target_link_libraries(${EXENAME} Qt5::Quick Qt5::Widgets Qt5::Qml  ${VTK_LIBRARIES} ${SYSTEM_LIBS})
#target_link_libraries(${EXENAME} Qt5::Quick Qt5::Widgets Qt5::Qml  ${VTK_LIBRARIES} ${SYSTEM_LIBS})



#/**  second method of cmake */
#cmake_minimum_required(VERSION 3.14)

#project(QuickVtkDemo VERSION 0.1 LANGUAGES CXX)

#set(Qt5_DIR "/home/allen/soft/Qt6.2/5.15.2/gcc_64/")
#set(QTDIR "/home/allen/soft/Qt6.2/5.15.2/gcc_64/")
#set(VTK_DIR "/home/allen/soft/src/VTK-9.1.0/Dbuild")
#set(CMAKE_PREFIX_PATH ${Qt5_DIR} ${VTK_DIR})
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 Qt5  REQUIRED COMPONENTS Core Quick Qml )
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Qml )
#find_package(Qt${QT_VERSION_MAJOR}QuickControls2 REQUIRED)

#file(GLOB QML_DESKTOP "../resources/*.qml")
#add_custom_target(QML_files SOURCES ${QML_DESKTOP})

#set(PROJECT_SOURCES
#        main.cpp
#        qml.qrc
#        qtvtkitem.h
#        qtvtkitem.cpp
#        vtkinteractorstyletrackballcamerastyle2.h
#        vtkinteractorstyletrackballcamerastyle2.cpp
#        CommandModel.cpp
#        CommandModel.h
#        CommandModelAdd.cpp
#        CommandModelAdd.h
#        Model.cpp
#        Model.h
#        ProcessingEngine.cpp
#        ProcessingEngine.h

#)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(QuickVtkDemo
#        MANUAL_FINALIZATION
#        ${PROJECT_SOURCES}
#    )
## Define target properties for Android with Qt 6 as:
##    set_property(TARGET QuickVtkDemo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
##                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
## For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
#else()
#    if(ANDROID)
#        add_library(QuickVtkDemo SHARED
#            ${PROJECT_SOURCES}
#        )
## Define properties for Android with Qt 5 after find_package() calls as:
##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(QuickVtkDemo
#          ${PROJECT_SOURCES}
##          resources.qrc
#        )
#    endif()
#endif()
## VTK Libraries
#set(VTK_DIR $ENV{VTK_DIR})
#find_package(VTK REQUIRED NO_MODULE)

#qt5_add_resources (PROJECT_SOURCES ${RESOURCES})
#target_compile_definitions(QuickVtkDemo
#  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
#target_link_libraries(QuickVtkDemo
#  PRIVATE
#  Qt${QT_VERSION_MAJOR}::Core
#  Qt${QT_VERSION_MAJOR}::Quick
#  Qt${QT_VERSION_MAJOR}::Qml
#  Qt${QT_VERSION_MAJOR}::Widgets
#  ${VTK_LIBRARIES}
#  ${SYSTEM_LIBS}
#  )


#set_target_properties(QuickVtkDemo PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#)

#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_import_qml_plugins(QuickVtkDemo)
#    qt_finalize_executable(QuickVtkDemo)
#endif()
