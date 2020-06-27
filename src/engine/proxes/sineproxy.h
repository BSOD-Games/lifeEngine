//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SINE_PROXY_H
#define SINE_PROXY_H

#include "engine/materialproxydescriptor.h"
#include "engine/imaterialproxy.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class SineProxy : public IMaterialProxy
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

        // SineProxy
        SineProxy();
        ~SineProxy();

		static MaterialProxyDescriptor		GetDescriptor();

    private:
        bool                            isInitialized;
        bool                            isNeadUpdate;
        float                           currentSine;
        UInt32_t                        countReference;

        IMaterialProxyVar*              sinemin;
        IMaterialProxyVar*              sinemax;
        IMaterialProxyVar*              sineoffset;
        IMaterialProxyVar*              resultVar;
        IMaterialProxyVar*              changeComponent;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SINE_PROXY_H
