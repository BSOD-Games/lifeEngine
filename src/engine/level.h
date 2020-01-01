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
#include "bitset.h"

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

		int						FindLeaf( const Vector3D_t& Position ) const;
		inline int				FindLeaf( Camera* Camera ) const
		{
			return FindLeaf( Camera->GetPosition() );
		}

		bool					IsClusterVisible( int CurrentCluster, int TestCluster ) const;

	private:
		bool								isLoaded;
		BSPVisData							visData;
		Bitset								facesDraw;
		IMesh*								mesh;
	
		std::vector< Camera* >				arrayCameras;
		std::vector< ITexture* >			arrayLightmaps;
		std::vector< BSPNode >				arrayBspNodes;
		std::vector< BSPLeaf >				arrayBspLeafs;
		std::vector< BSPPlane >				arrayBspPlanes;
		std::vector< BSPModel >				arrayBspModels;
		std::vector< int >					arrayBspLeafsFaces;

	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LEVEL_H
