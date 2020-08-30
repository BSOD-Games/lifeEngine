cmake_minimum_required( VERSION 2.6 )
set( MODULE_NAME Engine )

# ================================================
#
#	--- Paths to extlibs
#

set( SDL2_PATH ${EXTLIBS_DIR}/SDL2 CACHE PATH "Path to SDL2" )

#
#   --- Set values ---
#
# ================================================

# -- Public files --
# Root files
file( GLOB PUBLIC_FILES "Public/*.h" )
source_group( "Public Files" FILES ${PUBLIC_FILES} )

# System files
file( GLOB PUBLIC_FILES_SYSTEM "Public/System/*.h" )
source_group( "Public Files\\System" FILES ${PUBLIC_FILES_SYSTEM} )

# Rendering files
file( GLOB PUBLIC_FILES_RENDERING "Public/Rendering/*.h" )
source_group( "Public Files\\Rendering" FILES ${PUBLIC_FILES_RENDERING} )

# Rendering/RHI files
file( GLOB PUBLIC_FILES_RENDERING_RHI "Public/Rendering/RHI/*.h" )
source_group( "Public Files\\Rendering\\RHI" FILES ${PUBLIC_FILES_RENDERING_RHI} )

# -- Private files --
# Root private files
file( GLOB PRIVATE_FILES "Private/*.h" "Private/*.cpp" )
source_group( "Private Files" FILES ${PRIVATE_FILES} )

# System private files
file( GLOB PRIVATE_FILES_SYSTEM "Private/System/*.h" "Private/System/*.cpp" )
source_group( "Private Files\\System" FILES ${PRIVATE_FILES_SYSTEM} )

# Rendering private files
file( GLOB PRIVATE_FILES_RENDERING "Private/Rendering/*.h" "Private/Rendering/*.cpp" )
source_group( "Private Files\\Rendering" FILES ${PRIVATE_FILES_RENDERING} )

# ================================================
#
#   --- Project settings ---
#

set( SOURCE_FILES ${PUBLIC_FILES} ${PRIVATE_FILES} ${PUBLIC_FILES_SYSTEM} ${PRIVATE_FILES_SYSTEM} ${PUBLIC_FILES_RENDERING} ${PRIVATE_FILES_RENDERING} ${PUBLIC_FILES_RENDERING_RHI} )
add_library( ${MODULE_NAME} STATIC ${SOURCE_FILES} )
target_link_libraries( ${MODULE_NAME} Core )

set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
set_target_properties( ${MODULE_NAME} PROPERTIES DEBUG_POSTFIX "-Debug" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( Public )
include_directories( Private )
include_directories( ../ )
include_directories( ../Core/Public )

# OpenGL 4.5 RHI
include_directories( ../OpenGL4RHI/Public )
target_link_libraries( ${MODULE_NAME} OpenGL4RHI )

# ================================================
#
#   --- Find and link extlibs ---
#

# SDL2
find_package( SDL2 REQUIRED )
if( NOT SDL2_FOUND )
    message( SEND_ERROR "Failed to find SDL2" )
    return()
else()
    include_directories( ${SDL2_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} 
		debug ${SDL2_DEBUG_LIB} debug ${SDL2MAIN_DEBUG_LIB} 
		optimized ${SDL2_LIB} optimized ${SDL2MAIN_LIB} )
endif()

# Other default extlibs
if ( ${PLATFORM_TYPE} MATCHES "Windows" )
	target_link_libraries( ${MODULE_NAME} winmm version imm32 setupapi )
endif()