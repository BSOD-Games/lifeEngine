cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB_RECURSE SOURCE_FILES "*.h" "*.cpp"
								"../public/common/*.h"
								"../public/engine/*.h" )
set( MODULE_NAME engine )

set( SDL2_PATH ${EXTLIBS_DIR}/SDL2 CACHE PATH "Path to SDL2" )
set( RAPIDJSON_PATH ${EXTLIBS_DIR} CACHE PATH "Path to RapidJSON" )
set( GLM_PATH ${EXTLIBS_DIR} CACHE PATH "Path to GLM" )
set( FREEIMAGE_PATH ${EXTLIBS_DIR}/FreeImage CACHE PATH "Path to FreeImage" )
set( FREETYPE_PATH ${EXTLIBS_DIR}/freetype CACHE PATH "Path to Freetype" )
set( TCC_PATH ${EXTLIBS_DIR}/tcc CACHE PATH "Path to Tiny C Compiler" )

#
#   --- Название модулей движка ---
#

set( ENGINE_DLL "engine" )
set( STUDIORENDER_DLL "studiorender" )
set( PHYSICSSYSTEM_DLL "physics" )
set( PATHS_H paths.h )

#   Название модулей для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ENGINE_DLL ${ENGINE_DLL}.dll )
    set( PHYSICSSYSTEM_DLL ${PHYSICSSYSTEM_DLL}.dll )
    set( STUDIORENDER_DLL ${STUDIORENDER_DLL}.dll )
    set( ICON resource.rc )

 #   Название модулей для Linux
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( ENGINE_DLL ${ENGINE_DLL}.so )
    set( PHYSICSSYSTEM_DLL ${PHYSICSSYSTEM_DLL}.so )
    set( STUDIORENDER_DLL ${STUDIORENDER_DLL}.so )

else()
    message( SEND_ERROR "Unknow platform" )
endif()

#
#   --- Задаем комманды препроцессора ---
#

add_definitions( -DLIFEENGINE_EXPORT )

#
#   --- Настройки проекта ---
#

configure_file( ${PATHS_H}.in ${PATHS_H} )
add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} ${ICON} )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( ../public )
include_directories( ../ )

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )

install( DIRECTORY "scripts" DESTINATION ${BUILD_DIR}/engine )
install( DIRECTORY "shaders/shaders" DESTINATION ${BUILD_DIR}/engine )

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
#   FreeImage

find_package( FreeImage REQUIRED )
if ( NOT FREEIMAGE_FOUND )
    message( SEND_ERROR "Failed to FreeImage" )
    return()
else()
    include_directories( ${FREEIMAGE_INCLUDE} )
	target_link_libraries( ${MODULE_NAME} ${FREEIMAGE_LIB} )
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

#---------------
#   Freetype

find_package( Freetype REQUIRED )
if( NOT FREETYPE_FOUND )
    message( SEND_ERROR "Failed to find Freetype" )
    return()
else()
    include_directories( ${FREETYPE_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${FREETYPE_LIB} )
endif()  

#---------------
#   Tiny C Compiler

find_package( TCC REQUIRED )
if( NOT TCC_FOUND )
    message( SEND_ERROR "Failed to find Tiny C Compiler" )
    return()
else()
    include_directories( ${TCC_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${TCC_LIB} ${CMAKE_DL_LIBS} )
endif()
