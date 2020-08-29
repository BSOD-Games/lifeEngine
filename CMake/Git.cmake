# Initialize Git
function( git_init )
	find_package( Git REQUIRED )
	
	if ( GIT_FOUND )
		message( STATUS "Git found: ${GIT_EXECUTABLE}" )
	else()
		message( FATAL_ERROR "Git found: not founded" )
	endif()	
endfunction()

# Get branch name
function( git_current_branch OUT_VALUE )
	execute_process( COMMAND ${GIT_EXECUTABLE} branch --show-current
						WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
						OUTPUT_VARIABLE CURRENT_BRANCH
						ERROR_QUIET
						OUTPUT_STRIP_TRAILING_WHITESPACE )	

	set( ${OUT_VALUE} ${CURRENT_BRANCH} PARENT_SCOPE )
endfunction()

# Generation version from Git
function( git_generation_version OUT_VALUE )
	
	# Get current tag
	execute_process( COMMAND ${GIT_EXECUTABLE} describe --tags --exact-match
					WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
					OUTPUT_VARIABLE VERSION_TAG
					ERROR_QUIET
					OUTPUT_STRIP_TRAILING_WHITESPACE )
	
	# If tag not founded - getting brunch name
	if ( VERSION_TAG STREQUAL "" )
		execute_process( COMMAND ${GIT_EXECUTABLE} branch --show-current
						WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
						OUTPUT_VARIABLE VERSION_TAG
						ERROR_QUIET
						OUTPUT_STRIP_TRAILING_WHITESPACE )		
	endif()
	
	# Getting count commits
	execute_process( COMMAND ${GIT_EXECUTABLE} rev-list --count HEAD
					WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
					OUTPUT_VARIABLE VERSION_PATH
					ERROR_QUIET
					OUTPUT_STRIP_TRAILING_WHITESPACE )	
	
	# Getting hash last commit
	execute_process( COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
				WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
				OUTPUT_VARIABLE VERSION_HASH
				ERROR_QUIET
				OUTPUT_STRIP_TRAILING_WHITESPACE )	
	
	# Return version
	set( ${OUT_VALUE} "${VERSION_TAG}-${VERSION_HASH}.${VERSION_PATH}" PARENT_SCOPE )
endfunction()