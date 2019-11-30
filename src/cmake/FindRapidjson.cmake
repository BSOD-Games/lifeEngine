# 	---------------------------------
#	[in] 	RAPIDJSON_PATH		- root dir rapidjson
#	[out] 	RAPIDJSON_INCLUDE	- dir with includes
#	[out]	RAPIDJSON_FOUND		- is found rapidjson
# 	---------------------------------

SET( RAPIDJSON_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${RAPIDJSON_PATH}
)

find_path( 		RAPIDJSON_INCLUDE
				NAMES "rapidjson/rapidjson.h"
				PATH_SUFFIXES include
				PATHS ${RAPIDJSON_SEARCH_PATHS} )		
		
if ( NOT RAPIDJSON_INCLUDE )
    message( SEND_ERROR "Failed to find Rapidjson" )
    return()
else()
	set( RAPIDJSON_FOUND true )
endif()