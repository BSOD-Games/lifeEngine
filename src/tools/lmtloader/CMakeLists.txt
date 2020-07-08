cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB_RECURSE SOURCE_FILES "*.h" "*.cpp" )

set( MODULE_NAME lmtloader )
set( RAPIDJSON_PATH ${EXTLIBS_DIR} CACHE PATH "Path to RapidJSON" )

#
#   --- Настройки проекта ---
#

#source_group("Source Files\\module" FILES ${SOURCE_FILES})
add_library( ${MODULE_NAME} STATIC ${SOURCE_FILES} )

#---------------
#   RapidJSON

find_package( Rapidjson REQUIRED )
if ( NOT RAPIDJSON_FOUND )
    message( SEND_ERROR "Failed to find RapidJSON" )
    return()
else()
    include_directories( ${RAPIDJSON_INCLUDE} )
endif()