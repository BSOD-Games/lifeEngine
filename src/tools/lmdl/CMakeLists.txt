cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" )
set( MODULE_NAME lmdl )

#
#   --- Платформозависимые настройки ---
#

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ICON resource.rc )

elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
     set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie" )
     set( ICON "" )
else()
    message( SEND_ERROR "Unknow platform" )
endif()

#
#   --- Настройки проекта ---
#

add_executable( ${MODULE_NAME} WIN32 ${SOURCE_FILES} ${ICON} )
target_link_libraries( ${MODULE_NAME} ${CMAKE_DL_LIBS} )
install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )
include_directories( ../../public )

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
endif()
