cmake_minimum_required (VERSION 3.8)

set(PROJECT_NAME Haste_n_Slash)
project (${PROJECT_NAME})

# Установить путь для исходного кода lua
set(LUA_SRC lib/lua-5.4.7)

# Получить исходные файлы lua
file(GLOB LUA_SOURCES ${LUA_SRC}/src/*.c)
list(REMOVE_ITEM LUA_SOURCES ${LUA_SRC}/src/lua.c ${LUA_SRC}/src/luac.c)

# Добавить библиотеку lua и добавить include директории для проекта
add_library(lua STATIC ${LUA_SOURCES})
target_include_directories(lua PUBLIC ${LUA_SRC}/src)

# Установка значений для luacpp
set(LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/${LUA_SRC}/src)
set(LUA_LIBRARIES lua)

# Добавление luacpp
add_subdirectory(lib/luacpp-master)

# Добавление SDL

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL-main EXCLUDE_FROM_ALL)

# Добавление исполняемых файлов
file(GLOB SRC "./src/*")
message("Findes sources ${SRC}")
add_executable (${PROJECT_NAME} ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Haste_n_Slash PROPERTY CXX_STANDARD 20)
endif()

# Линковка библиотек с проектом
target_link_libraries(${PROJECT_NAME} PRIVATE lua luacpp_static SDL3::SDL3)
target_include_directories(${PROJECT_NAME} PRIVATE lib/entt-master/src/entt)
target_include_directories(${PROJECT_NAME} PRIVATE lib/rapidjson-master/include/rapidjson)