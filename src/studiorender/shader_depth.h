//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_DEPTH_H
#define SHADER_DEPTH_H

#include <unordered_map>

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class GPUProgram;

    //---------------------------------------------------------------------//

    class ShaderDepth
    {
    public:

        //---------------------------------------------------------------------//

        enum GEOMETRY_TYPE
        {
            GT_UNKNOWN,
            GT_SPHERE,
            GT_CONE
        };

        //---------------------------------------------------------------------//

        ShaderDepth();
        ~ShaderDepth();

		bool				Create();
		void				Delete();
		inline void			Bind()
		{
			if ( !gpuProgram ) return;
			gpuProgram->Bind();
		}

		inline void			Unbind()
		{
			if ( !gpuProgram ) return;
			gpuProgram->Unbind();
		}        

        inline void         SetType( GEOMETRY_TYPE Type )
        {
            gpuProgram = gpuPrograms[ Type ];
            activeType = Type;
        }

        inline void         SetPVTMatrix( const Matrix4x4_t& PVTMatrix )
        {
            if ( !gpuProgram ) return;
            gpuProgram->Bind();
            gpuProgram->SetUniform( "pvtMatrix", PVTMatrix );
            gpuProgram->Unbind();
        }

        inline void         SetRadius( float Radius )
        {
            if ( !gpuProgram || ( activeType != GT_SPHERE && activeType != GT_CONE ) ) return;
            gpuProgram->Bind();
            gpuProgram->SetUniform( "radius", Radius );
            gpuProgram->Unbind();
        }

        inline void         SetHeight( float Height )
        {
            if ( !gpuProgram || activeType != GT_CONE ) return;
            gpuProgram->Bind();
            gpuProgram->SetUniform( "height", Height );
            gpuProgram->Unbind();
        }

    private:
        GEOMETRY_TYPE                                           activeType;
        GPUProgram*			                                    gpuProgram;

        std::unordered_map< GEOMETRY_TYPE, GPUProgram* >        gpuPrograms;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_DEPTH_H