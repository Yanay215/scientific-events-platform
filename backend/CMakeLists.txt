cmake_minimum_required(VERSION 3.15)
project(ScientificEventsPlatform CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Trantor REQUIRED)
find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)

add_executable(${PROJECT_NAME} 
    backend/main.cpp
    backend/controllers/AuthController.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Drogon::Drogon
    PostgreSQL::PostgreSQL
    jwt-cpp::jwt-cpp
)