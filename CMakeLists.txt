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

# spdlog's cmake version file has a bug: PACKAGE_FIND_VERSION_MAJOR STREQUAL CVF_VERSION_MAJOR
# returns FALSE when no version is requested (PACKAGE_FIND_VERSION_MAJOR is ""). Patch it.
set(_spdlog_ver_file
    "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows-skse/share/spdlog/spdlogConfigVersion.cmake")
if(EXISTS "${_spdlog_ver_file}")
    file(READ "${_spdlog_ver_file}" _spdlog_ver_content)
    string(REPLACE
        "if(PACKAGE_FIND_VERSION_MAJOR STREQUAL CVF_VERSION_MAJOR)"
        "if(PACKAGE_FIND_VERSION_MAJOR STREQUAL \"\" OR PACKAGE_FIND_VERSION_MAJOR STREQUAL CVF_VERSION_MAJOR)"
        _spdlog_ver_content "${_spdlog_ver_content}")
    file(WRITE "${_spdlog_ver_file}" "${_spdlog_ver_content}")
endif()

add_subdirectory(extern/CommonLibSSE-NG)
include(extern/CommonLibSSE-NG/cmake/CommonLibSSE.cmake)

add_commonlibsse_plugin(
	${PROJECT_NAME}
	AUTHOR "AirWolf359"
	SOURCES ${headers} ${sources}
)

find_path(SIMPLEINI_INCLUDE_DIRS "SimpleIni.h")
find_package(jsoncpp CONFIG REQUIRED)

target_compile_definitions(
	"${PROJECT_NAME}"
	PRIVATE
		NOMINMAX
		WIN32_LEAN_AND_MEAN
)

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
