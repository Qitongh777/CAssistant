cmake_minimum_required (VERSION 3.8)

project ("CAssistant")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 设置Qt环境变量
set(CMAKE_PREFIX_PATH /home/kubuntu/Qt5.9.9/5.9.9/gcc_64)
# moc自动编译
set(CMAKE_AUTOMOC ON)
# qrc自动编译
set(CMAKE_AUTORCC ON)
# ui自动编译
set(CMAKE_AUTOUIC ON)

# 包含文件
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. DIR_SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_CASS_SRC)

set(REQUIRED_LIBS Core Gui Widgets PrintSupport)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::PrintSupport)

find_package(Qt5 COMPONENTS ${REQUIRED_LIBS} REQUIRED)
file(GLOB MOCS ${PROJECT_SOURCE_DIR}/include/*.h)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O0")
    message("Debug mode = ${CMAKE_C_FLAGS_DEBUG}")
    add_executable(${PROJECT_NAME} WIN32  ${MOCS} ${DIR_SRC} ${DIR_CASS_SRC})
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O2")
    message("Release mode = ${CMAKE_C_FLAGS_RELEASE}")
    add_executable(${PROJECT_NAME} WIN32  ${MOCS} ${DIR_SRC} ${DIR_CASS_SRC})
else()
    message("else = ${CMAKE_BUILD_TYPE}")
    message("else = ${CMAKE_C_FLAGS_DEBUG}")
    set(CMAKE_C_FLAGS_DEBUG"${CMAKE_C_FLAGS_DEBUG} -Wall -O0")
    add_executable(${PROJECT_NAME} WIN32  ${MOCS} ${DIR_SRC} ${DIR_CASS_SRC})
endif()

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})