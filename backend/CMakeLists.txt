cmake_minimum_required(VERSION 3.15)
project(ScientificEventsPlatform CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Trantor REQUIRED)
find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(jwt-cpp CONFIG REQUIRED)
find_package(CURL REQUIRED)

set(BACKEND_SOURCES
    main.cpp
    filters/jwt/JwtFilter.cpp
    services/email/EmailService.cpp
    controllers/auth/AuthController.cpp
    controllers/files/FilesController.cpp
    controllers/parser/ParserController.cpp
    controllers/events/EventController.cpp
    controllers/user/UserController.cpp
    controllers/moderator/ModeratorController.cpp
    controllers/organizer/OrganizerController.cpp
    controllers/universities/UniversitiesController.cpp
)

add_executable(${PROJECT_NAME} ${BACKEND_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/filters
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/services
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/controllers/auth
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/controllers/files
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/controllers/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/controllers/events
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/controllers/user
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/controllers/moderator
    ${CMAKE_CURRENT_SOURCE_DIR}/backend/controllers/organizer
    ${Drogon_INCLUDE_DIRS}
    ${PostgreSQL_INCLUDE_DIRS}
    ${jwt-cpp_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Drogon::Drogon
    PostgreSQL::PostgreSQL
    jwt-cpp::jwt-cpp
    CURL::libcurl
    bcrypt
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif ()