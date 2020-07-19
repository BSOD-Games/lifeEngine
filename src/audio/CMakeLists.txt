cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" )
file ( GLOB PUBLIC_FILES "../public/audio/*.h" )	
file ( GLOB PRIVATE_FILES "../private/audio/*.h" )					
set( MODULE_NAME audio )

set( VORBIS_PATH ${EXTLIBS_DIR}/vorbis CACHE PATH "Path to Vorbis" )
set( OGG_PATH ${EXTLIBS_DIR}/ogg CACHE PATH "Path to Ogg" )
set( OPENAL_PATH ${EXTLIBS_DIR}/OpenAL CACHE PATH "Path to OpenAL" )
set( GLM_PATH ${EXTLIBS_DIR} CACHE PATH "Path to GLM" )

#   Файл ресурсов для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ICON resource.rc )
endif()

#
#   --- Задаем комманды препроцессора ---
#

add_definitions( -DLIFEENGINE_EXPORT )

#
#   --- Настройки проекта ---
#

source_group( "Public Files" FILES ${PUBLIC_FILES} )
source_group( "Private Files" FILES ${PRIVATE_FILES} )

add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} ${PUBLIC_FILES} ${PRIVATE_FILES} ${ICON} )

set_target_properties (${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( ../public )
include_directories( ../private )
include_directories( ../ )

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )

#
#   --- Ищим и подключаем зависимости ---
#

#---------------
#	Vorbis

find_package( Vorbis REQUIRED )
if ( NOT VORBIS_FOUND )
    message( SEND_ERROR "Failed to find Vorbis" )
    return()
else()
    include_directories( ${VORBIS_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${VORBIS_LIB} ${VORBISFILE_LIB} ${VORBISENC_LIB}  )
	
endif()

#---------------
#	ogg

find_package( Ogg REQUIRED )
if ( NOT OGG_FOUND)
    message( SEND_ERROR "Failed to find Ogg" )
    return()
else()
    include_directories( ${OGG_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${OGG_LIB} )
endif()

#---------------
#   OpenAL

find_package( OpenAL REQUIRED )
if ( NOT OPENAL_FOUND)
    message( SEND_ERROR "Failed to find OpenAL" )
    return()
else()
    include_directories( ${OPENAL_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${OPENAL_LIB} )
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
