cmake_minimum_required(VERSION 2.6)

project(launcher)
set(PROJECT launcher)

set(${PROJECT}_HEADERS
    mainwindow.h
    configdialog.h
)

set(${PROJECT}_SOURCES
    main.cpp
    mainwindow.cpp
    configdialog.cpp
)

set(${PROJECT}_FORMS
    mainwindow.ui
    configdialog.ui
)

set(${PROJECT}_RESOURCES
    recources.qrc
)

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_ui(${PROJECT}_UI ${${PROJECT}_FORMS})
qt4_wrap_cpp(${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})
qt4_add_resources(${PROJECT}_QRC_SOURCES ${${PROJECT}_RESOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT} ${${PROJECT}_UI} ${${PROJECT}_SOURCES} ${${PROJECT}_MOC_SOURCES} ${${PROJECT}_QRC_SOURCES})
target_link_libraries(${PROJECT} ${QT_LIBRARIES})

install(TARGETS ${PROJECT}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

