//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IENTITY_H
#define IENTITY_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class ILevel;
	class IModel;
	class IBody;
	class ICamera;
	class IStudioRender;

    //---------------------------------------------------------------------//

	class IEntity : public IReferenceObject
    {
    public:
        virtual ~IEntity() {}		
        virtual void				KeyValue( const char* Key, const char* Value ) = 0;
		virtual void				Update() = 0;
		virtual void				Render( IStudioRender* StudioRender ) = 0;

		virtual void				SetModel( IModel* Model, IBody* Body ) = 0;
		virtual void				SetPosition( const Vector3D_t& Position ) = 0;
		virtual void				SetLevel( ILevel* Level ) = 0;

		virtual bool				IsVisible( ICamera* Camera ) const = 0;
		virtual Vector3D_t			GetCenter() const = 0;
		virtual const Vector3D_t&	GetPosition() const = 0;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IENTITY_H

