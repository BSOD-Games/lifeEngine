//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ILEVEL_H
#define ILEVEL_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class ICamera;
	class IModel;
	class IEntity;
	class IPointLight;
	class ISpotLight;
	class IDirectionalLight;

	//---------------------------------------------------------------------//

	class ILevel
	{
	public:
		virtual bool					Load( const char* Path, IFactory* GameFactory ) = 0;
		virtual void					Update( UInt32_t DeltaTime ) = 0;
		virtual void					Clear() = 0;
		virtual void					AddCamera( ICamera* Camera ) = 0;
		virtual void					AddModel( IModel* Model ) = 0;
		virtual void					AddEntity( IEntity* Entity ) = 0;
		virtual void					AddPointLight( IPointLight* PointLight ) = 0;
		virtual void					AddSpotLight( ISpotLight* SpotLight ) = 0;
		virtual void					AddDirectionalLight( IDirectionalLight* DirectionalLight ) = 0;
		virtual void					RemoveCamera( ICamera* Camera ) = 0;
		virtual void					RemoveCamera( UInt32_t Index ) = 0;
		virtual void					RemoveModel( IModel* Model ) = 0;
		virtual void					RemoveModel( UInt32_t Index ) = 0;
		virtual void					RemoveEntity( IEntity* Entity ) = 0;
		virtual void					RemoveEntity( UInt32_t Index ) = 0;
		virtual void					RemovePointLight( IPointLight* PointLight ) = 0;
		virtual void					RemovePointLight( UInt32_t Index ) = 0;
		virtual void					RemoveSpotLight( ISpotLight* SpotLight ) = 0;
		virtual void					RemoveSpotLight( UInt32_t Index ) = 0;
		virtual void					RemoveDirectionalLight( IDirectionalLight* DirectionalLight ) = 0;
		virtual void					RemoveDirectionalLight( UInt32_t Index ) = 0;

		virtual bool					IsLoaded() const = 0;
		virtual const char*				GetNameFormat() const = 0;
		virtual UInt32_t				GetCountCameras() const = 0;
		virtual ICamera*				GetCamera( UInt32_t Index ) const = 0;
		virtual UInt32_t				GetCountModels() const = 0;
		virtual IModel*					GetModel( UInt32_t Index ) const = 0;
		virtual UInt32_t				GetCountEntityes() const = 0;
		virtual IEntity*				GetEntity( UInt32_t Index ) const = 0;
		virtual UInt32_t				GetCountPointLights() const = 0;
		virtual IPointLight*			GetPointLight( UInt32_t Index ) const = 0;
		virtual UInt32_t				GetCountSpotLights() const = 0;
		virtual ISpotLight*				GetSpotLight( UInt32_t Index ) const = 0;
		virtual UInt32_t				GetCountDirectionalLight() const = 0;
		virtual IDirectionalLight*		GetDirectionalLight( UInt32_t Index ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ILEVEL_H

