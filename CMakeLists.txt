cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include("cmake/HunterGate.cmake")

HunterGate(
    URL  "https://github.com/yadisk-ops/hunter/archive/v0.18.47.2.tar.gz"
    SHA1 "fe0f3a4d337ae08dbea640fd1ebc1e448919fcd3"
    LOCAL
)

project(yadisk-cli)

set(YDCLI_VERSION_MAJOR 0)
set(YDCLI_VERSION_MINOR 1)
set(YDCLI_VERSION_PATCH 0)
set(YDCLI_VERSION_TWEAK 0)
set(YDCLI_VERSION ${YDCLI_VERSION_MAJOR}.${YDCLI_VERSION_MINOR}.${YDCLI_VERSION_PATCH})
set(YDCLI_VERSION_STRING "v${YDCLI_VERSION}")

add_definitions(-DYDCLI_VERSION_STRING="${YDCLI_VERSION_STRING}")

hunter_add_package(yadisk-client)
find_package(yadisk-client)

hunter_add_package(Boost COMPONENTS system filesystem program_options)
find_package(Boost CONFIG REQUIRED system filesystem program_options)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

hunter_add_package(CURL)
find_package(CURL CONFIG REQUIRED)

hunter_add_package(nlohmann-json)
find_package(nlohmann-json REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sources/*) 

add_executable(yad ${${PROJECT_NAME}_SOURCES})

target_link_libraries(yad
    yadisk-client::yadisk-client
	Boost::filesystem Boost::system Boost::program_options
	OpenSSL::SSL OpenSSL::Crypto 
	CURL::libcurl 
	nlohmann-json::nlohmann-json
)

install(TARGETS yad	RUNTIME DESTINATION bin)
