cmake_minimum_required( VERSION 2.6 )
project( lifeEngine )

set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" )
set( EXTLIBS_DIR ${CMAKE_BINARY_DIR}/Extlibs )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

include( ${CMAKE_ROOT}/Modules/ExternalProject.cmake )
include( Git )
	
git_init()	

#
#   --- Modules ---
#
# ================================================

set( MODULE_ENGINE Source/Runtime/Engine )
set( MODULE_OPENGL4RHI Source/Runtime/OpenGL4RHI )
set( EDITOR Source/Editor )

# ================================================
#
#   --- Build settings ---
#

# Download extlibs from repo
if ( NOT EXISTS ${EXTLIBS_DIR} )
	set( EXTLIBS_REP https://github.com/BSOD-Games/lifeEngine-extlibs.git )	
	git_current_branch( CURRENT_BRANCH )
	execute_process( COMMAND ${GIT_EXECUTABLE} clone --branch ${CURRENT_BRANCH} -v ${EXTLIBS_REP} ${EXTLIBS_DIR} )

	if ( NOT EXISTS ${EXTLIBS_DIR} )
		message( SEND_ERROR "Failed loading extlibs from [${EXTLIBS_REP}]" )
	endif()
endif( )

# If there is a directory of dependencies, set up the binary system in
# build directory
if ( EXISTS ${EXTLIBS_DIR} )
	execute_process( COMMAND ${GIT_EXECUTABLE} -C ${EXTLIBS_DIR} pull origin )

	include( ${EXTLIBS_DIR}/cmake/install_extlibs.cmake )
	set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${EXTLIBS_DIR}/CMake" )
endif()

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

set( BUILD_DIR ${CMAKE_INSTALL_PREFIX} )
set( CMAKE_CONFIGURATION_TYPES "Debug;Release" )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Platform specific settings
if( ${PLATFORM_TYPE} MATCHES "Windows" )	
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

# ================================================
#
#	--- Add modules ---
#

add_subdirectory( ${MODULE_ENGINE} )
add_subdirectory( ${EDITOR} )
add_subdirectory( ${MODULE_OPENGL4RHI} )