cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" )
set( MODULE_NAME shadercompile )

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
target_link_libraries( ${MODULE_NAME} ${CMAKE_DL_LIBS} )
install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )
include_directories( ../../public )
