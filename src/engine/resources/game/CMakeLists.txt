cmake_minimum_required( VERSION 2.6 )
project( Game )

#
#   --- Задаем переменные и пути к исходникам ---
#

option( EDITOR_BUILD "Is build game logic for editor" OFF )
set( LIFEENGINE_INCLUDE CACHE PATH "lifeEngine include directory" )
set( INSTALL_DIR CACHE PATH "Install to directory" )
set( GAME_NAME "Game" CACHE STRING "Game name" )
set( GAME_VERSION "v0.1" CACHE STRING "Game version" )	
set( COMPANY_NAME "" CACHE STRING "Company name" )	

file( GLOB SOURCE_FILES "*.h" "*.cpp" )
string( TOLOWER ${GAME_NAME} GAME_NAME_LOWER )

#
#   --- Настройки проекта ---
#

if ( EDITOR_BUILD )
	add_definitions( -DLIFEENGINE_EDITOR )
	add_library( ${GAME_NAME} SHARED ${SOURCE_FILES} )
	set_target_properties( ${GAME_NAME} PROPERTIES OUTPUT_NAME ${GAME_NAME_LOWER}-editor )
else()
	if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
		set( LAUNCHER_FILE core/launcher/launcher_win32.cpp )
		set( RESOURCE ${CMAKE_BINARY_DIR}/resource.rc )
		
		if ( MINGW )
			set( CMAKE_RC_COMPILER_INIT windres )
			enable_language( RC )
			set( CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -i <SOURCE> -o <OBJECT>" )
			set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -static )
			set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -static )		
		endif()
		
		set( ICON "core/launcher/lifeEngine.ico" CACHE FILEPATH "Path to icon" )						
		configure_file( core/launcher/resource.rc.in resource.rc )
	elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
		 set( LAUNCHER_FILE core/launcher/launcher_unix.cpp )
		 set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie" )
		 set( RESOURCE "" )
	else()
		message( SEND_ERROR "Unknow platform" )
	endif()
	
	add_executable( ${GAME_NAME} WIN32 ${LAUNCHER_FILE} ${SOURCE_FILES} ${RESOURCE} )
	set_target_properties( ${GAME_NAME} PROPERTIES OUTPUT_NAME ${GAME_NAME_LOWER} )
endif()

set_target_properties( ${GAME_NAME} PROPERTIES FOLDER Game )

include_directories( ./ )
include_directories( ${CMAKE_BINARY_DIR}/${GAME_NAME} )
include_directories( ${LIFEENGINE_INCLUDE} )

install( TARGETS ${GAME_NAME} DESTINATION ${INSTALL_DIR} )