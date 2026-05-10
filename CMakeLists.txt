cmake_minimum_required(VERSION 3.29)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")

project(
	ConsiderateFollowers
	VERSION 1.0.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

file(
	GLOB_RECURSE
	headers
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

file(
	GLOB_RECURSE
	sources
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	FILES ${headers} ${sources}
)

set(BUILD_TESTS OFF)
add_subdirectory(extern/CommonLibSSE-NG)
include(extern/CommonLibSSE-NG/cmake/CommonLibSSE.cmake)

add_commonlibsse_plugin(
	${PROJECT_NAME}
	AUTHOR "AirWolf359"
	SOURCES ${headers} ${sources}
)

find_path(SIMPLEINI_INCLUDE_DIRS "SimpleIni.h")
find_package(jsoncpp CONFIG REQUIRED)

target_include_directories(
	"${PROJECT_NAME}"
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		${SIMPLEINI_INCLUDE_DIRS}
)

target_precompile_headers(
	"${PROJECT_NAME}"
	PRIVATE
		src/common/PCH.h
)

target_link_libraries(
	"${PROJECT_NAME}"
	PRIVATE
		JsonCpp::JsonCpp
)

# ---- Post build ----
if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
	set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

if(DEFINED OUTPUT_FOLDER)
	set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
		VERBATIM
	)
endif()

set(DATA_DIR "${PROJECT_SOURCE_DIR}/data")

install(
	FILES "${DATA_DIR}/SKSE/Plugins/ConsiderateFollowers.ini"
	DESTINATION "SKSE/Plugins"
)

include(components.cmake)
