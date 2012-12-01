cmake_minimum_required(VERSION 2.6)

project(plotter)
set(PROJECT plotter)

set(${PROJECT}_HEADERS
    plotter.h
    plotterinterface.h
)

set(${PROJECT}_SOURCES
    plotter.cpp
)

find_package(Qt4 COMPONENTS QtCore QtGui QtScript REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_cpp(${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${PROJECT} ${${PROJECT}_SOURCES} ${${PROJECT}_MOC_SOURCES})
target_link_libraries(${PROJECT} ${QT_LIBRARIES})

install(TARGETS ${PROJECT}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

