//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "engine/ilevel.h"
#include "engine/camera.h"
#include "studiorender/istudiorender.h"
#include "studiorender/imesh.h"
#include "bsp.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Level : public ILevel
	{
	public:
		// ILevel
		virtual bool			Load( const char* Path, IFactory* GameFactory );
		virtual void			Update( UInt32_t DeltaTime );
		virtual void			Clear();
		virtual void			AddCamera( ICamera* Camera );
		virtual void			RemoveCamera( ICamera* Camera );
		virtual void			RemoveCamera( UInt32_t Index );

		virtual bool			IsLoaded() const;
		virtual const char*		GetNameFormat() const;
		virtual UInt32_t		GetCountCameras() const;
		virtual ICamera*		GetCamera( UInt32_t Index ) const;

		// Level
		Level();
		~Level();

	private:
		std::vector< BSPTexture >			arrayBspTextures;
		Camera*								mainCamera;
		IMesh*								mesh;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LEVEL_H
