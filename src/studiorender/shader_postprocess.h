//////////////////////////////////////////////////////////////////////////
//
//			        *** lifeEngine ***
//				Copyright (C) 2018-2020
//
// Repository engine:   https://github.com/zombihello/lifeEngine
// Authors:				Egor Pogulyaka (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SHADER_POSTPROCESS_H
#define SHADER_POSTPROCESS_H

//---------------------------------------------------------------------//

namespace le
{
    //---------------------------------------------------------------------//

    class GPUProgram;

    //---------------------------------------------------------------------//

    class ShaderPostprocess
    {
    public:
        ShaderPostprocess();
        ~ShaderPostprocess();

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

        inline void			SetSizeViewport( const Vector2D_t& SizeViewport )
		{
			if ( !gpuProgram ) return;
			
			Bind();
			gpuProgram->SetUniform( "screenSize", SizeViewport );
			Unbind();
		}

    private:
        GPUProgram*		        gpuProgram;
    };

    //---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SHADER_POSTPROCESS_H