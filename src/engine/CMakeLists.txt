cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" "../public/engine/*.h" "../public/common/*.h" )
set( MODULE_NAME engine )

set( SDL2_PATH ${EXTLIBS_DIR}/SDL2 CACHE PATH "Path to SDL2" )
set( RAPIDJSON_PATH ${EXTLIBS_DIR} CACHE PATH "Path to RapidJSON" )
set( GLM_PATH ${EXTLIBS_DIR} CACHE PATH "Path to GLM" )

#
#   --- Название модулей движка ---
#

set( ENGINE_DLL "engine" )
set( STUDIORENDER_DLL "studiorender" )
set( GAME_DLL "game" )
set( STDSHADERS_DLL "stdshaders" )
set( MATERIALSYSTEM_DLL "materialsystem" )
set( PATHS_H paths.h )

#   Название модулей для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ENGINE_DLL ${ENGINE_DLL}.dll )
	set( STUDIORENDER_DLL ${STUDIORENDER_DLL}.dll )
    set( GAME_DLL ${GAME_DLL}.dll )
	set( STDSHADERS_DLL ${STDSHADERS_DLL}.dll )
	set( MATERIALSYSTEM_DLL ${MATERIALSYSTEM_DLL}.dll )
else()
    message( SEND_ERROR "Unknow platform")
endif()

#
#   --- Задаем комманды препроцессора ---
#

add_definitions( -DLIFEENGINE_EXPORT )

#
#   --- Настройки проекта ---
#

configure_file( ${PATHS_H}.in ${PATHS_H} )
add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( ../public )
include_directories( ../ )

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )

#
#   --- Ищим и подключаем зависимости ---
#

#---------------
#   RapidJSON

find_package( Rapidjson REQUIRED )
if ( NOT RAPIDJSON_FOUND )
    message( SEND_ERROR "Failed to find RapidJSON" )
    return()
else()
    include_directories( ${RAPIDJSON_INCLUDE} )
endif()

#---------------
#   SDL2

find_package( SDL2 REQUIRED )
if( NOT SDL2_FOUND )
    message( SEND_ERROR "Failed to find SDL2" )
    return()
else()
    include_directories( ${SDL2_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${SDL2_LIB} ${SDL2MAIN_LIB} )
endif()

#---------------
#   GLM

find_package( GLM REQUIRED )
if ( NOT GLM_FOUND )
    message( SEND_ERROR "Failed to find GLM" )
    return()
else()
    include_directories( ${GLM_INCLUDE} )
endif()