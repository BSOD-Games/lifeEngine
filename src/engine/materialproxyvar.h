//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_PROXY_VAR_H
#define MATERIAL_PROXY_VAR_H

#include <string>
#include <vector>

#include "engine/imaterialproxyvar.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class MaterialProxyVar : public IMaterialProxyVar
    {
    public:
        // IReferenceObject
        virtual void                            IncrementReference();
        virtual void                            DecrementReference();
        virtual void                            Release();
        virtual UInt32_t                        GetCountReferences() const;

        // IMaterialProxyVar
        virtual void                            Clear();

        virtual void                            SetName( const char* Name );
        virtual void                            SetType( MATERIAL_PROXY_VAR_TYPE Type );
        virtual void                            SetValueInt( int Value );
        virtual void                            SetValueFloat( float Value );
        virtual void                            SetValueBool( bool Value );
        virtual void                            SetValueVector2D( const Vector2D_t& Value );
        virtual void                            SetValueVector3D( const Vector3D_t& Value );
        virtual void                            SetValueVector4D( const Vector4D_t& Value );
        virtual void                            SetValueShaderParameter( IShaderParameter* Value ) ;
        virtual void                            SetValueArrayFloat( float* Array, UInt32_t Count );
        virtual void                            SetValueArrayInt( int* Array, UInt32_t Count );
        virtual void                            SetValueArrayVector2D( Vector2D_t* Array, UInt32_t Count );
        virtual void                            SetValueArrayVector3D( Vector3D_t* Array, UInt32_t Count );
        virtual void                            SetValueArrayVector4D( Vector4D_t* Array, UInt32_t Count );

        virtual bool                            IsDefined() const;
        virtual const char*                     GetName() const;
        virtual MATERIAL_PROXY_VAR_TYPE         GetType() const;
        virtual int                             GetValueInt() const;
        virtual float                           GetValueFloat() const;
        virtual bool                            GetValueBool() const;
        virtual Vector2D_t                      GetValueVector2D() const;
        virtual Vector3D_t                      GetValueVector3D() const;
        virtual Vector4D_t                      GetValueVector4D() const;
        virtual IShaderParameter*               GetValueShaderParameter() const;
        virtual float*                          GetValueArrayFloat( UInt32_t& Count );
        virtual int*                            GetValueArrayInt( UInt32_t& Count );
        virtual Vector2D_t*                     GetValueArrayVector2D( UInt32_t& Count );
        virtual Vector3D_t*                     GetValueArrayVector3D( UInt32_t& Count );
        virtual Vector4D_t*                     GetValueArrayVector4D( UInt32_t& Count );

        // MaterialProxyVar
        MaterialProxyVar();
        ~MaterialProxyVar();

    private:
        bool                            isDefined;
        UInt32_t                        countReferences;

        std::string                     name;
        MATERIAL_PROXY_VAR_TYPE         type;
        void*                           value;        
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // IMATERIAL_PROXY_VAR_H
