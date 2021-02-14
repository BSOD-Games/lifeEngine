# 	---------------------------------
#	[in] 	GLM_PATH		- root dir glm
#	[out] 	GLM_INCLUDE		- dir with includes
#	[out]	GLM_FOUND		- is found glm
# 	---------------------------------

SET( GLM_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${GLM_PATH}
)

find_path( 		GLM_INCLUDE
				NAMES "glm/glm.hpp"
				PATH_SUFFIXES include
				PATHS ${GLM_SEARCH_PATHS} )		
		
if ( NOT GLM_INCLUDE )
    message( SEND_ERROR "Failed to find GLM" )
    return()
else()
	set( GLM_FOUND true )
endif()