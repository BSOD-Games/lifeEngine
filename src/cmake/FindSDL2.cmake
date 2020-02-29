# 	---------------------------------
#	[in] 	SDL2_PATH		- root dir sdl2
#	[out] 	SDL2_INCLUDE	- dir with includes
#	[out]	SDL2_LIB		- lib sdl2
#	[out]	SDL2MAIN_LIB	- lib sdl2main
#	[out]	SDL2_FOUND		- is found sdl2
# 	---------------------------------

SET( SDL2_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${SDL2_PATH}
)

find_path( 		SDL2_INCLUDE
				NAMES "SDL2/SDL.h"
				PATH_SUFFIXES include
				PATHS ${SDL2_SEARCH_PATHS} )		
find_library( 	SDL2_LIB 
                NAMES SDL2
                PATH_SUFFIXES lib
                PATHS ${SDL2_SEARCH_PATHS} )

find_library( 	SDL2MAIN_LIB
                NAMES SDL2main
                PATH_SUFFIXES lib
                PATHS ${SDL2_SEARCH_PATHS} )
		
if ( NOT SDL2_INCLUDE OR NOT SDL2_LIB OR NOT SDL2MAIN_LIB )
    message( SEND_ERROR "Failed to find SDL2" )
    return()
else()
	set( SDL2_FOUND true )
endif()
