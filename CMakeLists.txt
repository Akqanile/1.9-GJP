cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ANDROID_STL gnustl_static)
set(ANDROID_ABI armeabi-v7a)

project(scorpio)

FILE(GLOB SCORPIO_SRC src/*.h src/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL Windows)
   FILE(GLOB KITTYMEMORY_SRC external/KittyMemory/KittyMemory/*.cpp external/KittyMemory/KittyMemory/*.h)
   add_compile_definitions(kNO_KEYSTONE)

   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/KittyMemory/KittyMemory)

   set(SCORPIO_SRC ${SCORPIO_SRC} ${KITTYMEMORY_SRC})
endif()

add_library(scorpio SHARED ${SCORPIO_SRC})

add_subdirectory(external/base64)
target_link_libraries(scorpio base64)

if (NOT DEFINED GD_VERSION)
	message(SEND_ERROR "GD Version is not defined")
endif()

if (NOT ${GD_VERSION} STREQUAL "1.91" AND NOT ${GD_VERSION} STREQUAL "1.92" AND NOT ${GD_VERSION} STREQUAL "1.93")
	message(SEND_ERROR "GD Version ${GD_VERSION} is invalid")
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
   add_subdirectory(external/minhook)
   target_link_libraries(scorpio minhook)

   add_compile_definitions(PLATFORM_WINDOWS)
   add_compile_definitions(GD_VERSION="${GD_VERSION}")

   if (${GD_VERSION} STREQUAL "1.93")
	   message(SEND_ERROR "GD Version ${GD_VERSION} is invalid for Windows")
   endif()
else() # Android
   set(DOBBY_DEBUG OFF CACHE INTERNAL "" FORCE)
   set(DOBBY_GENERATE_SHARED OFF CACHE INTERNAL "" FORCE)
   add_subdirectory(external/dobby) # 16b99a55dc219114832c33f3258f48ca2958ed70
   target_link_libraries(scorpio dobby)

   add_compile_definitions(PLATFORM_ANDROID)
endif()
