cmake_minimum_required( VERSION 2.6 )

#
#   --- Задаем переменные ---
#

file( GLOB_RECURSE SOURCE_FILES "*.h" "*.cpp" )
set( MODULE_NAME phyloader )

#
#   --- Настройки проекта ---
#

#source_group("Source Files\\module" FILES ${SOURCE_FILES})
add_library( ${MODULE_NAME} STATIC ${SOURCE_FILES} )
set_target_properties( ${MODULE_NAME} PROPERTIES FOLDER Libraries )