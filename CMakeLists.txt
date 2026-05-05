cmake_minimum_required(VERSION 3.15)
project(AIChatbotCLI VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
    add_definitions(-DPLATFORM_WINDOWS)
else()
    set(PLATFORM_LINUX TRUE)
    add_definitions(-DPLATFORM_LINUX)
endif()

# Find packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# MySQL/MariaDB detection
if(WIN32)
    # Windows: MySQL typically installed in Program Files
    set(MYSQL_DIR "C:/Program Files/MySQL/MySQL Server 8.0" CACHE PATH "MySQL installation directory")
    
    find_path(MYSQL_INCLUDE_DIR
        NAMES mysql.h
        PATHS "${MYSQL_DIR}/include"
        PATH_SUFFIXES mysql
    )
    
    find_library(MYSQL_LIBRARY
        NAMES libmysql mysqlclient
        PATHS "${MYSQL_DIR}/lib"
    )
else()
    # Linux: Use pkg-config or find directly
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(MARIADB mariadb)
        pkg_check_modules(MYSQL mysqlclient)
    endif()
    
    if(MARIADB_FOUND)
        set(MYSQL_INCLUDE_DIR ${MARIADB_INCLUDE_DIRS})
        set(MYSQL_LIBRARY ${MARIADB_LIBRARIES})
    elseif(MYSQL_FOUND)
        set(MYSQL_INCLUDE_DIR ${MYSQL_INCLUDE_DIRS})
        set(MYSQL_LIBRARY ${MYSQL_LIBRARIES})
    else()
        find_path(MYSQL_INCLUDE_DIR
            NAMES mysql.h
            PATHS /usr/include/mysql /usr/include/mariadb /usr/local/include/mysql
        )
        find_library(MYSQL_LIBRARY
            NAMES mysqlclient mariadb libmysql
            PATHS /usr/lib /usr/lib64 /usr/local/lib
            PATH_SUFFIXES mysql
        )
    endif()
endif()

if(NOT MYSQL_INCLUDE_DIR OR NOT MYSQL_LIBRARY)
    message(WARNING "MySQL/MariaDB not found. Database features will be disabled.")
    add_definitions(-DNO_DATABASE)
endif()

# Thread library
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/chatbot.cpp
    src/api_handler.cpp
    src/database.cpp
    src/history.cpp
    src/utils.cpp
)

set(HEADERS
    include/chatbot.h
    include/api_handler.h
    include/database.h
    include/history.h
    include/utils.h
    include/env_loader.h
)

# Create executable
add_executable(chatbot ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(chatbot PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
    ${MYSQL_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(chatbot PRIVATE
    ${CURL_LIBRARIES}
    Threads::Threads
    nlohmann_json::nlohmann_json
)

if(MYSQL_LIBRARY)
    target_link_libraries(chatbot PRIVATE ${MYSQL_LIBRARY})
endif()

# Platform-specific settings
if(WIN32)
    # Windows: Set console to UTF-8
    target_compile_options(chatbot PRIVATE /utf-8)
    # Link winsock for networking
    target_link_libraries(chatbot PRIVATE ws2_32)
else()
    # Linux: Additional warnings
    target_compile_options(chatbot PRIVATE -Wall -Wextra)
endif()

# Install rules
install(TARGETS chatbot DESTINATION bin)
install(DIRECTORY data/ DESTINATION share/ai-chatbot-cli/data)