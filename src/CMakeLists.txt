cmake_minimum_required( VERSION 2.6 )
project( lifeEngine )

include( ${CMAKE_ROOT}/Modules/ExternalProject.cmake )
find_package( Git REQUIRED )	

#
#   --- Название проектов ---
#

set( PROJECT_LAUNCHER launcher )
set( PROJECT_ENGINE engine )
set( PROJECT_STUDIORENDER studiorender )
set( PROJECT_PHYSICS physics )
set( PROJECT_LMDL tools/lmdl )
set( PROJECT_MATERIALS_EDITOR tools/lmteditor ) 

#
#   --- Настройки сборки и пути к каталогам ---
#

set( EXTLIBS_DIR ${CMAKE_BINARY_DIR}/extlibs )
set( QT5_DIR CACHE PATH "Qt5 dir (ex. D:/Soft/Qt/5.12.2/mingw73_64)" )
set( BUILD_DIR ${PROJECT_SOURCE_DIR}/../build )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )
set( CMAKE_PREFIX_PATH ${QT5_DIR} )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

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
	install_extlibs( ${EXTLIBS_DIR} ${BUILD_DIR}/engine )
endif()

option( LIFEENGINE_DEBUG "Enable debug mode" OFF )
option( BUILD_LAUNCHER "Build launcher engine" OFF )
option( BUILD_ENGINE "Build engine" OFF )
option( BUILD_STUDIORENDER "Build studiorender" OFF )
option( BUILD_PHYSICS "Build physics" OFF )
option( BUILD_CONVERTER_MODELS "Build converter models" OFF )
option( BUILD_MATERIALS_EDITOR "Build materials editor" OFF )

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

if ( BUILD_CONVERTER_MODELS )
	add_subdirectory( ${PROJECT_LMDL} )
endif()

if ( BUILD_MATERIALS_EDITOR )
	add_subdirectory( ${PROJECT_MATERIALS_EDITOR} )
endif()
