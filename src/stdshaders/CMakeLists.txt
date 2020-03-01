cmake_minimum_required( VERSION 3.7 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" "../public/stdshaders/*.h" )
set( MODULE_NAME stdshaders )

#   Платформозависимые исходники для Windows
if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    set( ICON resource.rc )

#   Платформозависимые исходники для Linux
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set( ICON "" )
else()
    message( SEND_ERROR "Unknow platform")
endif()

#
#   --- Задаем комманды препроцессора ---
#

add_definitions( -DLIFEENGINE_EXPORT )

#
#   --- Настройки проекта ---
#

include_directories( ../public )
include_directories( ../ )
include_directories( ./ )

add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} ${ICON} )
set_target_properties( ${MODULE_NAME} PROPERTIES PREFIX "" )
install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )
