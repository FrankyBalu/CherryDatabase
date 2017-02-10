cmake_minimum_required(VERSION 3.0.0)

project(CherryDatabase)

set(DB_VERSION_MAJOR 0)
set(DB_VERSION_MINOR 1)
set(DB_VERSION_PATCH 0)
set(DB_VERSION ${DB_VERSION_MAJOR}.${DB_VERSION_MINOR}.${DB_VERSION_PATCH})


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)

file(GLOB SOURCES src/*cpp)

include_directories(include)
include_directories(src)

add_library(${PROJECT_NAME} SHARED ${SOURCES} )
target_link_libraries(${PROJECT_NAME} Qt5::Widgets )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${DB_VERSION} SOVERSION ${DB_VERSION_MAJOR})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

file(GLOB HEADERS include/*.h)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})
