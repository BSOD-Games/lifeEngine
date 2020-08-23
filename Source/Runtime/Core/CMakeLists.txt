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

# Modules files
file( GLOB PUBLIC_FILES_MODULES "Public/Modules/*.h" )
source_group( "Public Files\\Modules" FILES ${PUBLIC_FILES_MODULES} )

# Logging files
file( GLOB PUBLIC_FILES_LOGGING "Public/Logging/*.h" )
source_group( "Public Files\\Logging" FILES ${PUBLIC_FILES_LOGGING} )

# -- Private files --
# Root private files
file( GLOB PRIVATE_FILES "Private/*.h" "Private/*.cpp" )
source_group( "Private Files" FILES ${PRIVATE_FILES} )

# Logging private files
file( GLOB PRIVATE_FILES_LOGGING "Private/Logging/*.h" "Private/Logging/*.cpp" )
source_group( "Private Files\\Logging" FILES ${PRIVATE_FILES_LOGGING} )

# Misc private files
file( GLOB PRIVATE_FILES_MISC "Private/Misc/*.h" "Private/Misc/*.cpp" )
source_group( "Private Files\\Misc" FILES ${PRIVATE_FILES_MISC} )

# ================================================
#
#   --- Project settings ---
#

set( SOURCE_FILES ${PUBLIC_FILES} ${PRIVATE_FILES} ${PUBLIC_FILES_PLATFORM} ${PUBLIC_FILES_MISC} ${PUBLIC_FILES_MODULES} ${PUBLIC_FILES_LOGGING} ${PRIVATE_FILES_LOGGING} ${PRIVATE_FILES_MISC} )

if( NOT IS_MONOLIT )
	add_definitions( -DCORE_EXPORT )
	add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} )
	install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/Binaries/${ARCH_TYPE} )
else()
	add_library( ${MODULE_NAME} STATIC ${SOURCE_FILES} )
endif()

set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
set_target_properties( ${MODULE_NAME} PROPERTIES DEBUG_POSTFIX "-Debug" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( Public )
include_directories( Private )
include_directories( ../ )