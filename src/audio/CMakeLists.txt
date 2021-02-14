cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.cpp" "*.h" )
file( GLOB PUBLIC_HEADERS_FILES "../public/audio/*.h" )
file( GLOB PRIVATE_HEADERS_FILES "../private/audio/*.h" )	
	
set( MODULE_NAME audio )
set( VORBIS_PATH ${EXTLIBS_DIR}/vorbis CACHE PATH "Path to Vorbis" )
set( OGG_PATH ${EXTLIBS_DIR}/ogg CACHE PATH "Path to Ogg" )
set( OPENAL_PATH ${EXTLIBS_DIR}/OpenAL CACHE PATH "Path to OpenAL" )
set( GLM_PATH ${EXTLIBS_DIR} CACHE PATH "Path to GLM" )

#   Файл ресурсов для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ICON resource.rc )
endif()

set( ALL_SOURCE_FILES ${SOURCE_FILES} ${PUBLIC_HEADERS_FILES} ${PRIVATE_HEADERS_FILES} ${ICON} )

#
#   --- Задаем комманды препроцессора ---
#

add_definitions( -DLIFEENGINE_EXPORT )

#
#   --- Настройки проекта ---
#

source_group( "" FILES ${SOURCE_FILES} ${ICON} )
source_group( "Public" FILES ${PUBLIC_HEADERS_FILES} )
source_group( "Private" FILES ${PRIVATE_HEADERS_FILES} )

add_library( ${MODULE_NAME} SHARED ${ALL_SOURCE_FILES} )
set_target_properties (${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )

include_directories( ../../public )
include_directories( ../../private )
include_directories( ../ )

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/${BUILD_BIN_DIR} )

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
