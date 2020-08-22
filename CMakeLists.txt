cmake_minimum_required( VERSION 2.6 )
project( lifeEngine )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#
#   --- Modules ---
#

set( MODULE_CORE Source/Runtime/Core )

#
#   --- Build settings ---
#

# Get OS name
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
	set( PLATFORM_TYPE "Windows" )
else()
	message( SEND_ERROR "Unknow platform" )
endif()

# Get architecture type
if ( ${CMAKE_SIZEOF_VOID_P} MATCHES 8 )
	set( ARCH_TYPE x64 )
else()
	set( ARCH_TYPE x86 )
endif()

set( BUILD_DIR ${CMAKE_INSTALL_PREFIX}/Engine )
set( CMAKE_CONFIGURATION_TYPES "Debug;Release" )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

option( IS_MONOLIT "Is monolit" OFF )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )	
    if ( MINGW )
        set( CMAKE_RC_COMPILER_INIT windres )
        enable_language( RC )
        set( CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -i <SOURCE> -o <OBJECT>" )
        set( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -static )
        set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -static )		
    endif()
else()
    message( SEND_ERROR "Unknow platform" )
endif()

if( IS_MONOLIT )
	add_definitions( -DLIFEENGINE_MONOLIT )
endif()

#
#	--- Add modules ---
#

add_subdirectory( ${MODULE_CORE} )