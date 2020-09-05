cmake_minimum_required( VERSION 2.6 )
set( MODULE_NAME OpenGL4RHI )

# ================================================
#
#	--- Paths to extlibs
#

set( GLEW_PATH ${EXTLIBS_DIR}/GLEW CACHE PATH "Path to GLEW" )

#
#   --- Set values ---
#
# ================================================

# -- Public files --
# Root files
file( GLOB PUBLIC_FILES "Public/*.h" )
source_group( "Public Files" FILES ${PUBLIC_FILES} )

# -- Private files --
# Root private files
file( GLOB PRIVATE_FILES "Private/*.h" "Private/*.cpp" )
source_group( "Private Files" FILES ${PRIVATE_FILES} )

# Platform/<OS> private files
if ( ${PLATFORM_TYPE} STREQUAL "Windows" )
	file( GLOB PRIVATE_FILES_PLATFORM "Private/Platform/Windows/*.h" "Private/Platform/Windows/*.cpp" )
	source_group( "Private Files\\Platform" FILES ${PRIVATE_FILES_PLATFORM} )
endif()

# ================================================
#
#   --- Project settings ---
#

set( SOURCE_FILES ${PUBLIC_FILES} ${PRIVATE_FILES} ${PRIVATE_FILES_PLATFORM} )
add_library( ${MODULE_NAME} STATIC ${SOURCE_FILES} )

set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
set_target_properties( ${MODULE_NAME} PROPERTIES DEBUG_POSTFIX "-Debug" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( Public )
include_directories( Private )
include_directories( ../ )
include_directories( ../Engine/Public )

# ================================================
#
#   --- Find and link extlibs ---
#

# OpenGL
find_package( OpenGL REQUIRED )
if( NOT OPENGL_FOUND )
    message( SEND_ERROR "Failed to find OpenGL" )
    return()
else()
    include_directories( ${OPENGL_INCLUDE_DIR} )
    target_link_libraries( ${MODULE_NAME} ${OPENGL_LIBRARIES} )
endif()

# GLEW
find_package( GLEW REQUIRED )
if( NOT GLEW_FOUND )
    message( SEND_ERROR "Failed to find GLEW" )
    return()
else()
	add_definitions( -DGLEW_STATIC )
    include_directories( ${GLEW_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${GLEW_LIB} )
endif()