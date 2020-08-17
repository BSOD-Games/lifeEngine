cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" )
set( MODULE_NAME lmdl )

set( ASSIMP_PATH ${EXTLIBS_DIR}/Assimp CACHE PATH "Path to Assimp" )
set( BULLET_PATH ${EXTLIBS_DIR}/bullet3 CACHE PATH "Path to Bullet3" )

#
#   --- Платформозависимые настройки ---
#

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( RESOURCE resource.rc )

elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
     set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie" )
     set( RESOURCE "" )
else()
    message( SEND_ERROR "Unknow platform" )
endif()

#
#   --- Настройки проекта ---
#

add_executable( ${MODULE_NAME} ${SOURCE_FILES} ${RESOURCE} )
set_target_properties (${MODULE_NAME} PROPERTIES FOLDER Tools )

target_link_libraries( ${MODULE_NAME} ${CMAKE_DL_LIBS} )
target_link_libraries( ${MODULE_NAME} mdlloader )
target_link_libraries( ${MODULE_NAME} phyloader )
include_directories( ../mdlloader )
include_directories( ../phyloader )
include_directories( ../../public )
include_directories( ../../private )

install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/binaries )

#
#   --- Ищим и подключаем зависимости ---
#

#---------------
#   Assimp

find_package( Assimp REQUIRED )
if ( NOT ASSIMP_FOUND )
    message( SEND_ERROR "Failed to Assimp" )
    return()
else()
    include_directories( ${ASSIMP_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${ASSIMP_LIB} )
	
	if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
		target_link_libraries( ${MODULE_NAME} ${ASSIMP_ZLIB_LIB} ${ASSIMP_IRRXML_LIB} )
	endif()
endif()

find_package( Bullet REQUIRED )
if ( NOT BULLET_FOUND )
    message( SEND_ERROR "Failed to find Bullet3" )
    return()
else()
    include_directories( ${BULLET_INCLUDE} )
    target_link_libraries( ${MODULE_NAME} ${BULLET_LIB_LINEEARMATH} )
endif()