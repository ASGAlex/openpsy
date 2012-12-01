cmake_minimum_required(VERSION 2.6)

project(TestWindow)
set(PROJECT TestWindow)

subdirs(quazip)

set(${PROJECT}_HEADERS
    helpdialog.h
    imagedialog.h
    testwindow.h
    qwebviewscriptable.h
    buttonstate.h
)

set(${PROJECT}_SOURCES
    main.cpp
    helpdialog.cpp
    imagedialog.cpp
    testwindow.cpp
    qwebviewscriptable.cpp
    buttonstate.cpp
)

set(${PROJECT}_FORMS
    help.ui
    image.ui
    testwindow.ui
)

set(${PROJECT}_RESOURCES
    resources.qrc
)

include_directories(${openpsy_SOURCE_DIR})

#find_package(QuaZip REQUIRED)
#include_directories(${QUAZIP_INCLUDE_DIR} ${QUAZIP_ZLIB_INCLUDE_DIR})

include_directories(quazip quazip/quazip/zlib-1.2.7/)

find_package(Qt4 COMPONENTS QtCore QtGui QtWebKit QtScript QtScriptTools REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

qt4_wrap_ui(${PROJECT}_UI ${${PROJECT}_FORMS})
qt4_wrap_cpp(${PROJECT}_MOC_SOURCES ${${PROJECT}_HEADERS})
qt4_add_resources(${PROJECT}_QRC_SOURCES ${${PROJECT}_RESOURCES})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT} ${${PROJECT}_UI} ${${PROJECT}_SOURCES} ${${PROJECT}_MOC_SOURCES} ${${PROJECT}_QRC_SOURCES})
target_link_libraries(${PROJECT} ${QT_LIBRARIES} quazip)

install(TARGETS ${PROJECT}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

