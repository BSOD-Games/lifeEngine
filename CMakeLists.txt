cmake_minimum_required( VERSION 2.6 )
project( lifeEngine )

include( ${CMAKE_ROOT}/Modules/ExternalProject.cmake )
find_package( Git REQUIRED )	
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#
#   --- Название проектов ---
#

set( PROJECT_LAUNCHER src/launcher )
set( PROJECT_ENGINE src/engine )
set( PROJECT_STUDIORENDER src/studiorender )
set( PROJECT_PHYSICS src/physics )
set( PROJECT_AUDIO src/audio )
set( PROJECT_LMDL src/tools/lmdl )
set( PROJECT_MATERIALS_EDITOR src/tools/lmteditor ) 
set( PROJECT_MODEL_VIEWER src/tools/mdlviewer )
set( PROJECT_LMTLOADER src/tools/lmtloader )
set( PROJECT_MDLLOADER src/tools/mdlloader )
set( PROJECT_PHYLOADER src/tools/phyloader )

#
#   --- Настройки сборки и пути к каталогам ---
#

set( EXTLIBS_DIR ${CMAKE_BINARY_DIR}/extlibs )
set( QT5_DIR CACHE PATH "Qt5 dir (ex. D:/Soft/Qt/5.12.2/mingw73_64)" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )
set( CMAKE_PREFIX_PATH ${QT5_DIR} )

set( BUILD_DIR ${CMAKE_INSTALL_PREFIX} )
set( BUILD_BIN_DIR bin )
set( BUILD_ENGINE_DIR engine )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

include_directories( ${CMAKE_BINARY_DIR}/src )

# Загрузка зависимостей из репозитория
if ( NOT EXISTS ${EXTLIBS_DIR} )
	set( EXTLIBS_REP https://github.com/BSOD-Games/lifeEngine-extlibs.git )	
	execute_process( COMMAND ${GIT_EXECUTABLE} clone ${EXTLIBS_REP} ${EXTLIBS_DIR} )

	if ( NOT EXISTS ${EXTLIBS_DIR} )
		message( SEND_ERROR Failed loading extlibs from [${EXTLIBS_REP}] )
	endif()
endif( )

# Если каталог с зависимостями присудствует - настраиваем установку бинарников в
# каталог билда
if ( EXISTS ${EXTLIBS_DIR} )
	execute_process( COMMAND ${GIT_EXECUTABLE} -C ${EXTLIBS_DIR} pull origin )

	include( ${EXTLIBS_DIR}/cmake/install_extlibs.cmake )
	install_extlibs( ${EXTLIBS_DIR} ${BUILD_BIN_DIR} )
endif()

option( LIFEENGINE_DEBUG "Enable debug mode" OFF )
option( BUILD_LAUNCHER "Build launcher engine" OFF )
option( BUILD_ENGINE "Build engine" OFF )
option( BUILD_STUDIORENDER "Build studiorender" OFF )
option( BUILD_PHYSICS "Build physics" OFF )
option( BUILD_AUDIO "Build audio" OFF )
option( BUILD_CONVERTER_MODELS "Build converter models" OFF )
option( BUILD_MATERIALS_EDITOR "Build materials editor" OFF )
option( BUILD_MODEL_VIEWER "Build model viewer" OFF )

if( LIFEENGINE_DEBUG )
    message( STATUS "Debug mode enabled" )
    add_definitions( -DLIFEENGINE_DEBUG )
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )	
    if ( MINGW )
        set( CMAKE_RC_COMPILER_INIT windres )
        enable_language( RC )
        set( CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -i <SOURCE> -o <OBJECT>" )
        set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -static )
        set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -static )		
    endif()
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
else()
    message( SEND_ERROR "Unknow platform" )
endif()

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

if ( BUILD_PHYSICS )
	add_subdirectory( ${PROJECT_PHYSICS} )
endif()

if ( BUILD_AUDIO )
	add_subdirectory( ${PROJECT_AUDIO} )
endif()

if ( BUILD_CONVERTER_MODELS )
	add_subdirectory( ${PROJECT_LMDL} )
endif()

if ( BUILD_MATERIALS_EDITOR )
	add_subdirectory( ${PROJECT_MATERIALS_EDITOR} )
endif()

if ( BUILD_MODEL_VIEWER )
	add_subdirectory( ${PROJECT_MODEL_VIEWER} )
endif()

add_subdirectory( ${PROJECT_LMTLOADER} )
add_subdirectory( ${PROJECT_MDLLOADER} )
add_subdirectory( ${PROJECT_PHYLOADER} )