cmake_minimum_required( VERSION 3.7 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" "../public/stdshaders/*.h" )
set( MODULE_NAME stdshaders )

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

add_library( ${MODULE_NAME} SHARED ${SOURCE_FILES} )
install( TARGETS ${MODULE_NAME} DESTINATION ${BUILD_DIR}/engine )