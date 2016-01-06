PROJECT (core)
SET(CMAKE_AUTOMOC ON)
SET(QT_USE_QTNETWORK TRUE)

if(QT5_BUILD)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Network REQUIRED)
  set(QT_INCLUDES ${Qt5Network_INCLUDE_DIRS})
  include_directories(${QT_INCLUDES})
else()
  find_package(Qt4 REQUIRED)
endif()

file (GLOB src "*.cpp")
file (GLOB headers "*.h")
file (GLOB resources "*.qrc")

if(QT5_BUILD)
    QT5_WRAP_CPP(core_HEADERS_MOC ${core_HEADERS})
    QT5_ADD_RESOURCES(core_RESOURCES_RCC ${resources})
else()
    QT4_WRAP_CPP(core_HEADERS_MOC ${core_HEADERS})
    QT4_ADD_RESOURCES(core_RESOURCES_RCC ${resources})
endif()

if (NOT QT5_BUILD)
  INCLUDE(${QT_USE_FILE})
endif()

ADD_DEFINITIONS(${QT_DEFINITIONS})

if (SQLITE)
  set(src ${src} "../sqlite/sqlite3.c")
  ADD_DEFINITIONS(-DGRUMPY_SQLITE)
endif()

ADD_DEFINITIONS( -DLIBCORE_LIBRARY -DQT_USE_QSTRINGBUILDER )


ADD_LIBRARY(core SHARED ${src} ${headers})
if (QT5_BUILD)
    qt5_use_modules(core Core Network)
endif()
TARGET_LINK_LIBRARIES(core gp ircclient ${QT_LIBRARIES})