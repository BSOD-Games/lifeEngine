cmake_minimum_required( VERSION 3.7 )

#
#   --- Задаем переменные ---
#

file( GLOB SOURCE_FILES "*.h" "*.cpp" "../public/stdshaders/*.h" "glsl/*.glsl" )
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

#
#   --- Настройки компиляции шейдеров ---
#

file( MAKE_DIRECTORY shaders )
add_custom_target( shaders COMMAND $<TARGET_FILE:shadercompile> -s "glsl/unlitgeneric_vertex.glsl" -o "shaders/unlitgeneric_vertex"
    COMMAND  $<TARGET_FILE:shadercompile> -s "glsl/unlitgeneric_fragment.glsl" -o "shaders/unlitgeneric_fragment"
    COMMAND $<TARGET_FILE:shadercompile> -s "glsl/textgeneric_vertex.glsl" -o "shaders/textgeneric_vertex"
    COMMAND  $<TARGET_FILE:shadercompile> -s "glsl/textgeneric_fragment.glsl" -o "shaders/textgeneric_fragment"
    COMMAND $<TARGET_FILE:shadercompile> -s "glsl/testshader_vertex.glsl" -o "shaders/testshader_vertex"
    COMMAND  $<TARGET_FILE:shadercompile> -s "glsl/testshader_fragment.glsl" -o "shaders/testshader_fragment"
    COMMAND $<TARGET_FILE:shadercompile> -s "glsl/spritegeneric_vertex.glsl" -o "shaders/spritegeneric_vertex"
    COMMAND  $<TARGET_FILE:shadercompile> -s "glsl/spritegeneric_fragment.glsl" -o "shaders/spritegeneric_fragment"
    COMMAND $<TARGET_FILE:shadercompile> -s "glsl/lightmappedgeneric_vertex.glsl" -o "shaders/lightmappedgeneric_vertex"
    COMMAND  $<TARGET_FILE:shadercompile> -s "glsl/lightmappedgeneric_fragment.glsl" -o "shaders/lightmappedgeneric_fragment"

    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "*** Generated shaders ***" )
add_dependencies( ${MODULE_NAME} shaders )
