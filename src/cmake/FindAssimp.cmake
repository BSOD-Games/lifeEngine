# 	---------------------------------
#	[in] 	ASSIMP_PATH		- root dir Assimp
#	[out] 	ASSIMP_INCLUDE		- dir with includes
#	[out]	ASSIMP_LIB		- lib Assimp
#	[out]	ASSIMP_ZLIB_LIB		- lib zlib
#	[out]	ASSIMP_IRRXML_LIB	- lib IrrXML
#	[out]	ASSIMP_FOUND		- is found Assimp
# 	---------------------------------

SET( ASSIMP_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${ASSIMP_PATH}
)

find_path( 		ASSIMP_INCLUDE
				NAMES assimp/postprocess.h assimp/scene.h assimp/version.h assimp/config.h assimp/cimport.h
				PATH_SUFFIXES include
				PATHS ${ASSIMP_SEARCH_PATHS} )	
	
find_library( 		ASSIMP_LIB 
                		NAMES assimp
				PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                		PATHS ${ASSIMP_SEARCH_PATHS} )
				
if ( NOT ASSIMP_INCLUDE OR NOT ASSIMP_LIB )
    message( SEND_ERROR "Failed to find Assimp" )
    return()
else()
	set( ASSIMP_FOUND true )
endif()