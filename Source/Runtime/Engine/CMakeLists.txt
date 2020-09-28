cmake_minimum_required( VERSION 2.6 )
set( MODULE_NAME Engine )

# ================================================
#
#	--- Paths to extlibs
#

set( SDL2_PATH ${EXTLIBS_DIR}/SDL2 CACHE PATH "Path to SDL2" )
set( STB_PATH ${EXTLIBS_DIR}/STB CACHE PATH "Path to STB" )
set( RAPIDJSON_PATH ${EXTLIBS_DIR}/RapidJSON CACHE PATH "Path to RapidJSON" )

#
#   --- Set values ---
#
# ================================================

# -- Public files --
# Root files
file( GLOB PUBLIC_FILES "Public/*.h" )
source_group( "Public Files" FILES ${PUBLIC_FILES} )

# Platform/<OS> files
if ( ${PLATFORM_TYPE} STREQUAL "Windows" )
	file( GLOB PUBLIC_FILES_PLATFORM "Public/Platform/Windows/*.h" )
	source_group( "Public Files\\Platform" FILES ${PUBLIC_FILES_PLATFORM} )
endif()

# Misc files
file( GLOB PUBLIC_FILES_MISC "Public/Misc/*.h" )
source_group( "Public Files\\Misc" FILES ${PUBLIC_FILES_MISC} )

# Logging files
file( GLOB PUBLIC_FILES_LOGGING "Public/Logging/*.h" )
source_group( "Public Files\\Logging" FILES ${PUBLIC_FILES_LOGGING} )

# Templates files
file( GLOB PUBLIC_FILES_TEMPLATES "Public/Templates/*.h" )
source_group( "Public Files\\Templates" FILES ${PUBLIC_FILES_TEMPLATES} )

# System files
file( GLOB PUBLIC_FILES_SYSTEM "Public/System/*.h" )
source_group( "Public Files\\System" FILES ${PUBLIC_FILES_SYSTEM} )

# Rendering files
file( GLOB PUBLIC_FILES_RENDERING "Public/Rendering/*.h" )
source_group( "Public Files\\Rendering" FILES ${PUBLIC_FILES_RENDERING} )

# Rendering/RHI files
file( GLOB PUBLIC_FILES_RENDERING_RHI "Public/Rendering/RHI/*.h" )
source_group( "Public Files\\Rendering\\RHI" FILES ${PUBLIC_FILES_RENDERING_RHI} )

# Math files
file( GLOB PUBLIC_FILES_MATH "Public/Math/*.h" )
source_group( "Public Files\\Math" FILES ${PUBLIC_FILES_MATH} )

# Resources files
file( GLOB PUBLIC_FILES_RESOURCES "Public/Resources/*.h" )
source_group( "Public Files\\Resources" FILES ${PUBLIC_FILES_RESOURCES} )

# Resources/Parsers files
file( GLOB PUBLIC_FILES_RESOURCES_PARSERS "Public/Resources/Parsers/*.h" )
source_group( "Public Files\\Resources\\Parsers" FILES ${PUBLIC_FILES_RESOURCES_PARSERS} )

# World files
file( GLOB PUBLIC_FILES_WORLD "Public/World/*.h" )
source_group( "Public Files\\World" FILES ${PUBLIC_FILES_WORLD} )

# World/Components files
file( GLOB PUBLIC_FILES_WORLD_COMPONENTS "Public/World/Components/*.h" )
source_group( "Public Files\\World\\Components" FILES ${PUBLIC_FILES_WORLD_COMPONENTS} )

# -- Private files --
# Root private files
file( GLOB PRIVATE_FILES "Private/*.h" "Private/*.cpp" )
source_group( "Private Files" FILES ${PRIVATE_FILES} )

# Logging private files
file( GLOB PRIVATE_FILES_LOGGING "Private/Logging/*.h" "Private/Logging/*.cpp" )
source_group( "Private Files\\Logging" FILES ${PRIVATE_FILES_LOGGING} )

# System private files
file( GLOB PRIVATE_FILES_SYSTEM "Private/System/*.h" "Private/System/*.cpp" )
source_group( "Private Files\\System" FILES ${PRIVATE_FILES_SYSTEM} )

# Rendering private files
file( GLOB PRIVATE_FILES_RENDERING "Private/Rendering/*.h" "Private/Rendering/*.cpp" )
source_group( "Private Files\\Rendering" FILES ${PRIVATE_FILES_RENDERING} )

# Math private files
file( GLOB PRIVATE_FILES_MATH "Private/Math/*.h" "Private/Math/*.cpp" )
source_group( "Private Files\\Math" FILES ${PRIVATE_FILES_MATH} )

# Resources private files
file( GLOB PRIVATE_FILES_RESOURCES "Private/Resources/*.h" "Private/Resources/*.cpp" )
source_group( "Private Files\\Resources" FILES ${PRIVATE_FILES_RESOURCES} )

# Resources/Parsers private files
file( GLOB PRIVATE_FILES_RESOURCES_PARSERS "Private/Resources/Parsers/*.h" "Private/Resources/Parsers/*.cpp" )
source_group( "Private Files\\Resources\\Parsers" FILES ${PRIVATE_FILES_RESOURCES_PARSERS} )

# Misc private files
file( GLOB PRIVATE_FILES_MISC "Private/Misc/*.h" "Private/Misc/*.cpp" )
source_group( "Private Files\\Misc" FILES ${PRIVATE_FILES_MISC} )

# World private files
file( GLOB PRIVATE_FILES_WORLD "Private/World/*.h" "Private/World/*.cpp" )
source_group( "Private Files\\World" FILES ${PRIVATE_FILES_WORLD} )

# World/Components private files
file( GLOB PRIVATE_FILES_WORLD_COMPONENTS "Private/World/Components/*.h" "Private/World/Components/*.cpp" )
source_group( "Private Files\\World\\Components" FILES ${PRIVATE_FILES_WORLD_COMPONENTS} )

# ================================================
#
#   --- Project settings ---
#

set( SOURCE_FILES 	${PUBLIC_FILES} ${PUBLIC_FILES_PLATFORM} 
					${PUBLIC_FILES_MISC} ${PRIVATE_FILES_MISC} 
					${PUBLIC_FILES_LOGGING} ${PRIVATE_FILES_LOGGING} 
					${PUBLIC_FILES_TEMPLATES} ${PRIVATE_FILES} 
					${PUBLIC_FILES_SYSTEM} ${PRIVATE_FILES_SYSTEM} 
					${PUBLIC_FILES_RENDERING} ${PRIVATE_FILES_RENDERING} 
					${PUBLIC_FILES_RENDERING_RHI} ${PUBLIC_FILES_MATH} 
					${PRIVATE_FILES_MATH} ${PUBLIC_FILES_RESOURCES} 
					${PRIVATE_FILES_RESOURCES} ${PUBLIC_FILES_RESOURCES_PARSERS} 
					${PRIVATE_FILES_RESOURCES_PARSERS} 
					${PUBLIC_FILES_WORLD} ${PUBLIC_FILES_WORLD_COMPONENTS}
					${PRIVATE_FILES_WORLD} ${PRIVATE_FILES_WORLD_COMPONENTS} )
add_library( ${MODULE_NAME} STATIC ${SOURCE_FILES} )

set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
set_target_properties( ${MODULE_NAME} PROPERTIES DEBUG_POSTFIX "-Debug" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( Public )
include_directories( Private )
include_directories( ../ )

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

# STB
find_package( STB REQUIRED )
if( NOT STB_FOUND )
	message( SEND_ERROR "Failed to find STB" )
	return()
else()
	include_directories( ${STB_INCLUDE} )
endif()

# RapidJSON
find_package( RapidJSON REQUIRED )
if( NOT RAPIDJSON_FOUND )
	message( SEND_ERROR "Failed to find RapidJSON" )
	return()
else()
	include_directories( ${RAPIDJSON_INCLUDE} )
endif()

# Other default extlibs
if ( ${PLATFORM_TYPE} MATCHES "Windows" )
	target_link_libraries( ${MODULE_NAME} winmm version imm32 setupapi )
endif()