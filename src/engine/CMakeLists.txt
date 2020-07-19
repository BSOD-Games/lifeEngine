cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" )	
file( GLOB PROXES_FILES "proxes/*.h" "proxes/*.cpp" )	
file( GLOB_RECURSE SCRIPTS_FILES "scripts/*.h" "scripts/*.cpp" )
file( GLOB SHADERS_FILES "shaders/*.h" "shaders/*.cpp" )						
file( GLOB PUBLIC_FILES "../public/common/*.h" "../public/engine/*.h" )								
file( GLOB PRIVATE_FILES "../private/engine/*.h" )		
	
set( MODULE_NAME engine )

set( SDL2_PATH ${EXTLIBS_DIR}/SDL2 CACHE PATH "Path to SDL2" )
set( RAPIDJSON_PATH ${EXTLIBS_DIR} CACHE PATH "Path to RapidJSON" )
set( GLM_PATH ${EXTLIBS_DIR} CACHE PATH "Path to GLM" )
set( FREEIMAGE_PATH ${EXTLIBS_DIR}/FreeImage CACHE PATH "Path to FreeImage" )
set( FREETYPE_PATH ${EXTLIBS_DIR}/freetype CACHE PATH "Path to Freetype" )
set( TCC_PATH ${EXTLIBS_DIR}/tcc CACHE PATH "Path to Tiny C Compiler" )
set( OGG_PATH ${EXTLIBS_DIR}/ogg CACHE PATH "Path to ogg" )
set( VORBIS_PATH ${EXTLIBS_DIR}/vorbis CACHE PATH "Path to vorbis" )

#
#   --- Название модулей движка ---
#

set( ENGINE_DLL "engine" )
set( STUDIORENDER_DLL "studiorender" )
set( PHYSICSSYSTEM_DLL "physics" )
set( AUDIOSYSTEM_DLL "audio" )
set( PATHS_H paths.h )

#   Название модулей для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ENGINE_DLL ${ENGINE_DLL}.dll )
    set( PHYSICSSYSTEM_DLL ${PHYSICSSYSTEM_DLL}.dll )
    set( STUDIORENDER_DLL ${STUDIORENDER_DLL}.dll )
	set( AUDIOSYSTEM_DLL ${AUDIOSYSTEM_DLL}.dll )
    set( ICON resource.rc )

 #   Название модулей для Linux
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( ENGINE_DLL ${ENGINE_DLL}.so )
    set( PHYSICSSYSTEM_DLL ${PHYSICSSYSTEM_DLL}.so )
    set( STUDIORENDER_DLL ${STUDIORENDER_DLL}.so )
	set( AUDIOSYSTEM_DLL ${AUDIOSYSTEM_DLL}.so )

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

file( GLOB SOURCE_FILES "*.h" "*.cpp" )	
file( GLOB PROXES_FILES "proxes/*.h" "proxes/*.cpp" )	
file( GLOB_RECURSE SCRIPTS_FILES "scripts/*.h" "scripts/*.cpp" )
file( GLOB SHADERS_FILES "shaders/*.h" "shaders/*.cpp" )						
file( GLOB PUBLIC_FILES "../public/common/*.h" "../public/engine/*.h" )	

source_group( "Public Files" FILES ${PUBLIC_FILES} )
source_group( "Private Files" FILES ${PRIVATE_FILES} )
source_group( "Proxes Files" FILES ${PROXES_FILES} )
source_group( "Scripts Files" FILES ${SCRIPTS_FILES} )
source_group( "Shaders Files" FILES ${SHADERS_FILES} )

configure_file( ${PATHS_H}.in ${PATHS_H} )
add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} ${PUBLIC_FILES} ${PRIVATE_FILES} ${PROXES_FILES} ${SCRIPTS_FILES} ${SHADERS_FILES} ${ICON} )

set_target_properties (${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
target_link_libraries( ${MODULE_NAME} phyloader )
target_link_libraries( ${MODULE_NAME} mdlloader )
target_link_libraries( ${MODULE_NAME} lmtloader )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( ../public )
include_directories( ../private )
include_directories( ../tools/mdlloader )
include_directories( ../tools/phyloader )
include_directories( ../tools/lmtloader )
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

#--------------
#	OGG

find_package( Ogg REQUIRED )
if( NOT OGG_FOUND )
	message( SEND_ERROR "Failed to find Ogg" )
	return()
else()
	include_directories( ${OGG_INCLUDE} )
	target_link_libraries( ${MODULE_NAME} ${OGG_LIB} )
endif()

#--------------
#	VORBIS

find_package( Vorbis REQUIRED )
if( NOT VORBIS_FOUND )
	message( SEND_ERROR "Failed to find Vorbis" )
	return()
else()
	include_directories( ${VORBIS_INCLUDE} )
	target_link_libraries( ${MODULE_NAME} ${VORBIS_LIB} ${VORBISFILE_LIB} )
endif()