cmake_minimum_required (VERSION 3.8)

set(PROJECT_NAME Haste_n_Slash)
project (${PROJECT_NAME})

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif(MSVC)

# Установить путь для исходного кода lua
set(LUA_SRC lib/lua-5.4.7)

# Получить исходные файлы lua
file(GLOB LUA_SOURCES ${LUA_SRC}/src/*.c)
list(REMOVE_ITEM LUA_SOURCES ${LUA_SRC}/src/lua.c ${LUA_SRC}/src/luac.c)

# Добавить библиотеку lua и добавить include директории для проекта
add_library(lua STATIC ${LUA_SOURCES})
target_include_directories(lua PUBLIC ${LUA_SRC}/src)

# Установка значений для luabridge
set(LUA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/${LUA_SRC}/src)
set(LUA_LIBRARIES lua)

# Добавление SDL
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL-main EXCLUDE_FROM_ALL)

set(SDLTTF_VENDORED ON)
add_subdirectory(lib/SDL3_ttf-3.2.2 EXCLUDE_FROM_ALL)

set(SDLIMAGE_VENDORED ON)
set(SDLIMAGE_AVIF OFF)	# disable formats we don't use to make the build faster and smaller.
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_TIF OFF)
add_subdirectory(lib/SDL_image-main EXCLUDE_FROM_ALL)

set(SDLMIXER_MIDI_NATIVE OFF)     # disable formats we don't use to make the build faster and smaller. Also some of these don't work on all platforms so you'll need to do some experimentation.
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)     
set(SDLMIXER_MOD OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_VENDORED ON)   # tell SDL_mixer to build its own dependencies
add_subdirectory(lib/SDL_mixer-main EXCLUDE_FROM_ALL)

# Добавление исполняемых файлов
file(GLOB_RECURSE SRC_CPP "./src/*.cpp")
file(GLOB_RECURSE SRC_HPP "./src/*.hpp")
message("Findes sources: ${SRC_CPP} ${SRC_HPP}")
add_executable (${PROJECT_NAME} ${SRC_CPP} ${SRC_HPP})

# Линковка библиотек с проектом
target_link_libraries(${PROJECT_NAME} PRIVATE
SDL3_mixer::SDL3_mixer
SDL3_image::SDL3_image
SDL3_ttf::SDL3_ttf
lua
SDL3::SDL3
)

target_include_directories(${PROJECT_NAME} PRIVATE lib/LuaBridge3-master/Source)
target_include_directories(${PROJECT_NAME} PRIVATE lib/entt-master/src)
target_include_directories(${PROJECT_NAME} PRIVATE lib/rapidjson-1.1.0/include)

# Копирование ресурсов после сборки

file(GLOB_RECURSE RESOURCE_FILES ${PROJECT_SOURCE_DIR}/resources/*)

# Создаем кастомную цель для копирования ресурсов
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/resources
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
    COMMENT "Copying resources..."
)

# Делаем так, чтобы цель LuaExample зависела от копирования ресурсов
add_dependencies(${PROJECT_NAME} copy_resources)