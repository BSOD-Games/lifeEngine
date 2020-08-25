cmake_minimum_required( VERSION 2.6 )
set( MODULE_NAME Engine )

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

# ================================================
#
#   --- Project settings ---
#

set( SOURCE_FILES ${PUBLIC_FILES} ${PRIVATE_FILES} )
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