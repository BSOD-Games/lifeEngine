cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" )
file( GLOB PUBLIC_FILES "../public/physics/*.h" "../public/physics/*.h" )
set( MODULE_NAME physics )

set( BULLET_PATH ${EXTLIBS_DIR}/bullet3 CACHE PATH "Path to Bullet3" )
set( GLM_PATH ${EXTLIBS_DIR} CACHE PATH "Path to GLM" )

#   Файл ресурсов для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ICON resource.rc )
endif()

#
#   --- Задаем комманды препроцессора ---
#

add_definitions( -DLIFEENGINE_EXPORT )

#
#   --- Настройки проекта ---
#

source_group( "Public Files" FILES ${PUBLIC_FILES} )

add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} ${PUBLIC_FILES} ${ICON} )

set_target_properties (${MODULE_NAME} PROPERTIES FOLDER Engine )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )

include_directories( ${CMAKE_BINARY_DIR}/${MODULE_NAME} )
include_directories( ../public )
include_directories( ../ )

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )

#
#   --- Ищим и подключаем зависимости ---
#

#---------------
#   Bullet

find_package( Bullet REQUIRED )
if ( NOT BULLET_FOUND )
    message( SEND_ERROR "Failed to find Bullet3" )
    return()
else()
    include_directories( ${BULLET_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${BULLET_LIB_DYNAMICS} ${BULLET_LIB_COLLISION} ${BULLET_LIB_LINEEARMATH} )
endif()

#---------------
#   GLM

find_package( GLM REQUIRED )
if ( NOT GLM_FOUND )
    message( SEND_ERROR "Failed to find GLM" )
    return()
else()
    include_directories( ${GLM_INCLUDE} )
endif()
