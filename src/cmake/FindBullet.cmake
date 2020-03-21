# 	---------------------------------
#	[in] 	BULLET_PATH		- root dir bullet
#	[out] 	BULLET_INCLUDE		- dir with includes
#	[out]	BULLET_LIB		- lib bullet
#	[out]	BULLET_FOUND		- is found bullet
# 	---------------------------------

SET( BULLET_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	/lib
	${BULLET_PATH}
)

find_path( 		BULLET_INCLUDE
			NAMES "bullet/btBulletDynamicsCommon.h"
			PATH_SUFFIXES src include
			PATHS ${BULLET_SEARCH_PATHS} )

find_library( 		BULLET_LIB_DYNAMICS 
                	NAMES BulletDynamics
			PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                	PATHS ${BULLET_SEARCH_PATHS} )

find_library( 		BULLET_LIB_COLLISION 
                	NAMES BulletCollision
			PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                	PATHS ${BULLET_SEARCH_PATHS} )

find_library( 		BULLET_LIB_LINEEARMATH
                	NAMES LinearMath
			PATH_SUFFIXES lib lib32 lib64 lib/x86_64-linux-gnu
                	PATHS ${BULLET_SEARCH_PATHS} )

if ( NOT BULLET_INCLUDE OR NOT BULLET_LIB_DYNAMICS OR NOT BULLET_LIB_COLLISION OR NOT BULLET_LIB_LINEEARMATH )
    message( SEND_ERROR "Failed to find Bullet3" )
    return()
else()
	set( BULLET_FOUND true )	
endif()