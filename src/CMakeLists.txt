cmake_minimum_required( VERSION 2.6 )
project( lifeEngine )

#
#   --- Название проектов ---
#

set( PROJECT_LAUNCHER launcher )
set( PROJECT_ENGINE engine )
set( PROJECT_STUDIORENDER studiorender )
set( PROJECT_STDSHADERS stdshaders )
set( PROJECT_MATERIALSYSTEM materialsystem )

#
#   --- Настройки сборки ---
#

set( EXTLIBS_DIR ${PROJECT_SOURCE_DIR}/extlibs )

option( LIFEENGINE_DEBUG "Enable debug mode" OFF )
option( BUILD_LAUNCHER "Build launcher engine" OFF )
option( BUILD_ENGINE "Build engine" OFF )
option( BUILD_STUDIORENDER "Build studiorender" OFF )
option( BUILD_STDSHADERS "Build stdshaders" OFF )
option( BUILD_MATERIALSYSTEM "Build materialsystem" OFF )

if( LIFEENGINE_DEBUG )
	message( STATUS "Debug mode enabled" )
	add_definitions( -DLIFEENGINE_DEBUG )
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

if ( BUILD_MATERIALSYSTEM )
	add_subdirectory( ${PROJECT_MATERIALSYSTEM} )
endif()