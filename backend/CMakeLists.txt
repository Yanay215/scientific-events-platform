cmake_minimum_required(VERSION 3.15)
project(ScientificEventsPlatform CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Trantor REQUIRED)
find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(bcrypt REQUIRED IMPORTED_TARGET libbcrypt)

add_executable(${PROJECT_NAME} 
    backend/main.cpp
    backend/controllers/auth/AuthController.cpp
    backend/controllers/files/FilesController.cpp
    backend/controllers/parser/ParserController.cpp
    backend/controllers/events/EventsController.cpp
    backend/controllers/user/UserController.cpp
    backend/controllers/moderator/ModeratorController.cpp
)
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Drogon_INCLUDE_DIRS}
    ${PostgreSQL_INCLUDE_DIRS}
    ${jwt-cpp_INCLUDE_DIRS}
    ${bcrypt_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Drogon::Drogon
    PostgreSQL::PostgreSQL
    jwt-cpp::jwt-cpp
    bcrypt::libbcrypt
)