cmake_minimum_required( VERSION 2.6 )
set( MODULE_NAME Core )

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

# -- Private files --
# Root private files
file( GLOB PRIVATE_FILES "Private/*.h" "Private/*.cpp" )
source_group( "Private Files" FILES ${PRIVATE_FILES} )

# Logging private files
file( GLOB PRIVATE_FILES_LOGGING "Private/Logging/*.h" "Private/Logging/*.cpp" )
source_group( "Private Files\\Logging" FILES ${PRIVATE_FILES_LOGGING} )

# ================================================
#
#   --- Project settings ---
#

set( SOURCE_FILES ${PUBLIC_FILES} ${PRIVATE_FILES} ${PUBLIC_FILES_PLATFORM} ${PUBLIC_FILES_MISC} ${PUBLIC_FILES_LOGGING} ${PRIVATE_FILES_LOGGING} ${PUBLIC_FILES_TEMPLATES} )
add_library( ${MODULE_NAME} STATIC ${SOURCE_FILES} )

set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
set_target_properties( ${MODULE_NAME} PROPERTIES DEBUG_POSTFIX "-Debug" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( Public )
include_directories( Private )
include_directories( ../ )