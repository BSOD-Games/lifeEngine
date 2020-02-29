cmake_minimum_required( VERSION 2.6 )
project( lifeEngine )

#
#   --- Название проектов ---
#

set( PROJECT_LAUNCHER launcher )
set( PROJECT_ENGINE engine )
set( PROJECT_STUDIORENDER studiorender )
set( PROJECT_STDSHADERS stdshaders )

#
#   --- Настройки сборки ---
#

set( EXTLIBS_DIR ${PROJECT_SOURCE_DIR}/extlibs )

option( LIFEENGINE_DEBUG "Enable debug mode" OFF )
option( BUILD_LAUNCHER "Build launcher engine" OFF )
option( BUILD_ENGINE "Build engine" OFF )
option( BUILD_STUDIORENDER "Build studiorender" OFF )
option( BUILD_STDSHADERS "Build stdshaders" OFF )

if( LIFEENGINE_DEBUG )
	message( STATUS "Debug mode enabled" )
	add_definitions( -DLIFEENGINE_DEBUG )
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    if ( MINGW )
        set( CMAKE_RC_COMPILER_INIT windres )
        enable_language( RC )
        set( CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -i <SOURCE> -o <OBJECT>" )
    endif()
else()
    message( SEND_ERROR "Unknow platform")
endif()

#
#   --- Пути к каталогам ---
#

set( BUILD_DIR ${PROJECT_SOURCE_DIR}/../build )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )
set( CMAKE_PREFIX_PATH ${QT5_DIR} )

#
# Добавляем подпроекты
#

include_directories( ${CMAKE_BINARY_DIR} )

if ( BUILD_LAUNCHER )
	add_subdirectory( ${PROJECT_LAUNCHER} )
endif()

if ( BUILD_ENGINE )
	add_subdirectory( ${PROJECT_ENGINE} )
endif()

if ( BUILD_STUDIORENDER )
	add_subdirectory( ${PROJECT_STUDIORENDER} )
endif()

if ( BUILD_STDSHADERS )
	add_subdirectory( ${PROJECT_STDSHADERS} )
endif()
