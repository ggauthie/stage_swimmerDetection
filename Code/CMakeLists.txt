cmake_minimum_required(VERSION 2.6)

# *******************************************
# ************* CMake Content ***************
# *******************************************
# This CMake create a workspace containing the following projects
#
# Programs
#  - SwimmerDetection_c

# IDE dependent config
if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
	MESSAGE("Add definition for ${CMAKE_GENERATOR}")
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

# Add definition for relative path into project
add_definitions( -DPROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

project(SwimmerDetection_c)

# Set to 1 to activate debug compilation (0 for release)
if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
	set(DEBUG 0)

	if(${DEBUG})
			MESSAGE("Generate Debug project")
			set(CMAKE_BUILD_TYPE Debug)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
			set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -pg -Wall")
	else()
			MESSAGE("Generate Release project")
			set(CMAKE_BUILD_TYPE ../Release)
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
			set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")
	endif()
	#add libmath during non visual studio builds
	set(CMAKE_EXTRA_LIB m)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake_modules/")

# *******************************************
# ************** SDL2 LIBRARY ***************
# *******************************************
# Set variables before finding SDL Library
# Helps to find where SDL libraries are saved (win32 only)
if(WIN32)
    # find the SDL2-x.y.z folder in the lib directory.
	file(GLOB SDL2DIR "${LIBS_DIR}/SDL2-[\\.|0-9]*")
	set(ENV{SDL2DIR} ${SDL2DIR})
endif()

Find_Package (SDL2 REQUIRED )

if(NOT SDL2_FOUND)
  MESSAGE(FATAL_ERROR "SDL not found !")
endif()

if (WIN32)
	file(GLOB
		SDL2_DLL
		${SDL2DIR}/bin/*.dll
		${SDL2DIR}/lib/*.dll
	)

	MESSAGE("Copy SDL DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
		file(COPY ${SDL2_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	else()
		file(COPY ${SDL2_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
		file(COPY ${SDL2_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	endif()
endif()


# *******************************************
# ************ SDL2TTF LIBRARY **************
# ******************************************
# Set variables before finding SDL2TTF Library
# Helps to find where SDL2TTF libraries are saved (win32 only)
if(WIN32)
	# find the SDL2_ttf-x.y.z folder in the lib directory.
	file(GLOB SDL2TTFDIR "${LIBS_DIR}/SDL2_ttf-[\\.|0-9]*")
	set(ENV{SDL2TTFDIR} ${SDL2TTFDIR})
endif()


Find_Package (SDL2TTF REQUIRED )

if(NOT SDL2TTF_FOUND)
	MESSAGE(FATAL_ERROR "SDL2TTF not found !")
endif()

if (WIN32)
	file(GLOB
			SDL2TTF_DLL
			${SDL2TTFDIR}/bin/*.dll
			${SDL2TTFDIR}/lib/*.dll
			)

	MESSAGE("Copy SDL2_ttf DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
		file(COPY ${SDL2TTF_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	else()
		file(COPY ${SDL2TTF_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
		file(COPY ${SDL2TTF_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	endif()
endif()


# *******************************************
# ************ Pthread LIBRARY **************
# *******************************************
if (WIN32)
  set(THREADS_USE_PTHREADS_WIN32 true)
	# pthread included AFTER Sdl to avoid unnecessary warnings
	file(GLOB PTHREADDIR "${LIBS_DIR}/pthread-[\\.|0-9]*")
	set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PTHREADDIR})
	Find_Package ( THREADS REQUIRED )
else()
	Find_Package ( Threads )
endif()

if(NOT THREADS_FOUND)
	MESSAGE(FATAL_ERROR "Pthread not found !")
endif()


if (WIN32)
	file(GLOB
		PTHREADS_DLL
		${CMAKE_PREFIX_PATH}/lib/*.dll
	)

	MESSAGE("Copy Pthreads DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
		file(COPY ${PTHREADS_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	else()
		file(COPY ${PTHREADS_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
		file(COPY ${PTHREADS_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
	endif()
endif()

# *******************************************
# ************ Spider LIBRARY ***************
# *******************************************
 
# find the spider folder in the libspider directory.
file(GLOB SPIDERDIR "${LIBS_DIR}/spider")
set(ENV{SPIDERDIR} ${SPIDERDIR})
 
Find_Package (Spider REQUIRED )
 
if(NOT SPIDER_FOUND)
  MESSAGE(FATAL_ERROR "SPIDER not found !")
endif()
 
if (WIN32)
    file(GLOB
        SPIDER_DLL
        ${SPIDERDIR}/lib/*.dll
    )
     
    MESSAGE("Copy SPIDER DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
        file(COPY ${SPIDER_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    else()
        file(COPY ${SPIDER_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        file(COPY ${SPIDER_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endif()
endif()


# *******************************************
# ************ OpenCV LIBRARY **************
# *******************************************

find_package(OpenCV REQUIRED )
find_package(jsoncpp CONFIG REQUIRED)

# *******************************************
# ************ Object		   **************
# *******************************************
option(STATIC "Static Project" ON)

if(STATIC)
	add_library(Image OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/Image.cpp)
	add_library(ImageWrapper OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageWrapper.cpp)
endif()

# *******************************************
# **************** SwimmerDetection.exe  ****************
# *******************************************
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/generated ${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${THREADS_PTHREADS_INCLUDE_DIR} ${SPIDER_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

if(STATIC)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/clock.cpp")
		file(RENAME src/clock.cpp src/clock.c)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/crop.cpp")
		file(RENAME src/crop.cpp src/crop.c)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/GMM.cpp")
		file(RENAME src/GMM.cpp src/GMM.c)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/mp4Read.cpp")
		file(RENAME src/mp4Read.cpp src/mp4Read.c)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/resize.cpp")
		file(RENAME src/resize.cpp src/resize.c)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/splitMerge.cpp")
		file(RENAME src/splitMerge.cpp src/splitMerge.c)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/voteSystem.cpp")
		file(RENAME src/voteSystem.cpp src/voteSystem.c)
	endif()
	file(
	GLOB_RECURSE
	source_files
	./src/*.c
	./src/*.h
	./include/*.h
	./generated/*.c
	./generated/*.h
)
else()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/clock.c")
		file(RENAME src/clock.c src/clock.cpp)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/crop.c")
		file(RENAME src/crop.c src/crop.cpp)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/GMM.c")
		file(RENAME src/GMM.c src/GMM.cpp)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/mp4Read.c")
		file(RENAME src/mp4Read.c src/mp4Read.cpp)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/resize.c")
		file(RENAME src/resize.c src/resize.cpp)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/splitMerge.c")
		file(RENAME src/splitMerge.c src/splitMerge.cpp)
	endif()
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/voteSystem.c")
		file(RENAME src/voteSystem.c src/voteSystem.cpp)
	endif()
	file(
	GLOB_RECURSE
	source_files
	./src/*.cpp
	./src/*.c
	./src/*.h
	./include/*.h
	./generated/*.cpp
	./generated/*.h
)
endif()

add_executable(SwimmerDetection_c ${source_files} ${header_files})
if(STATIC)
	target_link_libraries(SwimmerDetection_c ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_EXTRA_LIB} ${OpenCV_LIBS} ${SPIDER_LIBRARY} jsoncpp Image ImageWrapper)
else()
	target_link_libraries(SwimmerDetection_c ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_EXTRA_LIB} ${OpenCV_LIBS} ${SPIDER_LIBRARY} jsoncpp)
endif()