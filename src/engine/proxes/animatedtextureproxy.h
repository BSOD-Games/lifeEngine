//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_PROXY_H
#define MATERIAL_PROXY_H

#include "engine/imaterialproxy.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class AnimatedTextureProxy : public IMaterialProxy
    {
    public:
        // IMaterialProxy
        virtual void                        Update();
        virtual void                        NeadUpdate();
        virtual void                        ClearVar( const char* NameVar );
        virtual void                        ClearAllVars();

        virtual void                        SetVar( IMaterialProxyVar* MaterialProxyVar );

        virtual bool                        IsNeadUpdate() const;
        virtual const char*                 GetName() const;
        virtual IMaterialProxyVar*          GetVar( const char* NameVar ) const;

        // IReferenceObject
        virtual void                        IncrementReference();
        virtual void                        DecrementReference();
        virtual void                        Release();
        virtual UInt32_t                    GetCountReferences() const;

        // AnimatedTextureProxy
        AnimatedTextureProxy();
        ~AnimatedTextureProxy();

    private:
        bool                            isInitialized;
        bool                            isNeadUpdate;
        float                           currentFrameFire;
        UInt32_t                        countReference;

        IMaterialProxyVar*              frames;
        IMaterialProxyVar*              textureRectVar;
        IMaterialProxyVar*              delay;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_PROXY_H
