# 	---------------------------------
#	[in] 	TCC_PATH	- root dir TCC
#	[out] 	TCC_INCLUDE	- dir with includes
#	[out]	TCC_LIB		- lib tcc
#	[out]	TCC_FOUND	- is found tcc
# 	---------------------------------

SET( TCC_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${TCC_PATH}
)

find_path( 	TCC_INCLUDE
		NAMES "libtcc.h"
		PATH_SUFFIXES include
		PATHS ${TCC_SEARCH_PATHS} )
		
find_library( 	TCC_LIB 
                NAMES tcc
                PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                PATHS ${TCC_SEARCH_PATHS} )

if ( NOT TCC_INCLUDE OR NOT TCC_LIB )
    message( SEND_ERROR "Failed to find Tiny C Compiler" )
    return()
else()
	set( TCC_FOUND true )
endif()
