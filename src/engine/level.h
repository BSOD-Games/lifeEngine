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

	class Model;

	//---------------------------------------------------------------------//

	class Level : public ILevel
	{
	public:
		// ILevel
		virtual bool					Load( const char* Path, IFactory* GameFactory );
		virtual void					Update( UInt32_t DeltaTime );
		virtual void					Clear();
		virtual void					AddCamera( ICamera* Camera );
		virtual void					AddModel( IModel* Model );
		virtual void					AddEntity( IEntity* Entity );
		virtual void					AddPointLight( IPointLight* PointLight );
		virtual void					AddSpotLight( ISpotLight* SpotLight );
		virtual void					AddDirectionalLight( IDirectionalLight* DirectionalLight );
		virtual void					RemoveCamera( ICamera* Camera );
		virtual void					RemoveCamera( UInt32_t Index );
		virtual void					RemoveModel( IModel* Model );
		virtual void					RemoveModel( UInt32_t Index );
		virtual void					RemoveEntity( IEntity* Entity );
		virtual void					RemoveEntity( UInt32_t Index );
		virtual void					RemovePointLight( IPointLight* PointLight );
		virtual void					RemovePointLight( UInt32_t Index );
		virtual void					RemoveSpotLight( ISpotLight* SpotLight );
		virtual void					RemoveSpotLight( UInt32_t Index );
		virtual void					RemoveDirectionalLight( IDirectionalLight* DirectionalLight );
		virtual void					RemoveDirectionalLight( UInt32_t Index );

		virtual bool					IsLoaded() const;
		virtual const char*				GetNameFormat() const;
		virtual UInt32_t				GetCountCameras() const;
		virtual ICamera*				GetCamera( UInt32_t Index ) const;
		virtual UInt32_t				GetCountModels() const;
		virtual IModel*					GetModel( UInt32_t Index ) const;
		virtual UInt32_t				GetCountEntityes() const;
		virtual IEntity*				GetEntity( UInt32_t Index ) const;
		virtual UInt32_t				GetCountPointLights() const;
		virtual IPointLight*			GetPointLight( UInt32_t Index ) const;
		virtual UInt32_t				GetCountSpotLights() const;
		virtual ISpotLight*				GetSpotLight( UInt32_t Index ) const;
		virtual UInt32_t				GetCountDirectionalLight() const;
		virtual IDirectionalLight*		GetDirectionalLight( UInt32_t Index ) const;

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

		//---------------------------------------------------------------------//

		struct ModelDescriptor
		{
			bool			isBspModel;
			Model*			model;
		};

		//---------------------------------------------------------------------//

		struct Entity
		{
			std::string											className;
			std::unordered_map< std::string, std::string >		values;
		};

		//---------------------------------------------------------------------//

		void					EntitiesParse( std::vector< Entity >& ArrayEntities, BSPEntities& BSPEntities, UInt32_t Size );

		bool								isLoaded;
		BSPVisData							visData;
		Bitset								facesDraw;
		IMesh*								mesh;
				
		std::vector< BSPNode >				arrayBspNodes;
		std::vector< BSPLeaf >				arrayBspLeafs;
		std::vector< BSPPlane >				arrayBspPlanes;	
		std::vector< int >					arrayBspLeafsFaces;

		std::vector< ITexture* >			arrayLightmaps;
		std::vector< Camera* >				arrayCameras;
		std::vector< ModelDescriptor >		arrayModels;
		std::vector< IEntity* >				arrayEntities;
		std::vector< IPointLight* >			arrayPointLights;
		std::vector< ISpotLight* >			arraySpotLights;
		std::vector< IDirectionalLight* >	arrayDirectionalLights;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !LEVEL_H
