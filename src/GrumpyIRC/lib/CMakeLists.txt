PROJECT(grumpy_lib)

SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSCRIPT TRUE)
SET(CMAKE_BUILD_TYPE Release)
SET(CMAKE_AUTOMOC ON)

if(QT5_BUILD)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(Qt5Network REQUIRED)
  find_package(Qt5Script REQUIRED)
  set(QT_INCLUDES ${Qt5Gui_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Script_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})
  include_directories(${QT_INCLUDES})
else()
  find_package(Qt4 REQUIRED)
endif()

file (GLOB ui "*.ui")
file (GLOB headers "*.h")
file (GLOB src "*.cpp")
file (GLOB resources "*.qrc")

if(QT5_BUILD)
    QT5_WRAP_CPP(grumpy_lib_HEADERS_MOC ${grumpy_lib_HEADERS})
    QT5_WRAP_UI(grumpy_lib_FORMS_HEADERS ${ui})
    QT5_ADD_RESOURCES(grumpy_lib_RESOURCES_RCC ${resources})
else()
    QT4_WRAP_CPP(grumpy_lib_HEADERS_MOC ${irc_HEADERS})
    QT4_WRAP_UI(grumpy_lib_FORMS_HEADERS ${ui})
    QT4_ADD_RESOURCES(grumpy_lib_RESOURCES_RCC ${resources})
endif()

if (NOT QT5_BUILD)
  INCLUDE(${QT_USE_FILE})
endif()

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS( -DGRUMPY_LIBRARY -DQT_USE_QSTRINGBUILDER )

ADD_LIBRARY(grumpy_lib SHARED ${src} ${grumpy_lib_HEADERS_MOC} ${headers} ${grumpy_lib_RESOURCES_RCC} ${grumpy_lib_FORMS_HEADERS})
if (QT5_BUILD)
    qt5_use_modules(grumpy_lib Core Multimedia Gui Script Widgets Xml)
endif()
TARGET_LINK_LIBRARIES(grumpy_lib grumpycore irc2htmlcode ${QT_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "grumpy")
if (NOT WIN32)
    INSTALL(TARGETS grumpy_lib LIBRARY DESTINATION lib)
endif()
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES("../../")
