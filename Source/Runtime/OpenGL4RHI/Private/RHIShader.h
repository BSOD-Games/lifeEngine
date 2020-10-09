// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef RHISHADER_H
#define RHISHADER_H

#include <unordered_map>

#include "Misc/Types.h"
#include "Rendering/RHI/IRHIShader.h"
#include "GLState.h"

namespace le
{
	class RHIShader : public IRHIShader
	{
	public:
		/* Constructor */
		RHIShader();

		/* Destructor */
		~RHIShader();

		/* Compile shader */
		bool Compile( const std::string& InCode, EShaderType InShaderType, std::vector< std::string >* InDefines = nullptr, std::string* OutError = nullptr ) override;
		
		/* Link shader */
		bool Link( std::string* OutError = nullptr ) override;
		
		/* Bind shader */
		FORCEINLINE void Bind()
		{
			if ( !IsLoaded() ) return;
			GLState::BindShader( gpuProgram );
		}

		/* Unbind shader */
		FORCEINLINE static void Unbind()
		{
			GLState::BindShader( 0 );
		}

		/* Destroy shader */
		void Destroy() override;

		/* Set uniform */
		void SetUniform( const std::string& InName, int InValue ) override;
		void SetUniform( const std::string& InName, float InValue ) override;
		void SetUniform( const std::string& InName, bool InValue ) override;
		void SetUniform( const std::string& InName, const SColor& InValue ) override;
		void SetUniform( const std::string& InName, const FVector2D& InValue ) override;
		void SetUniform( const std::string& InName, const FVector3D& InValue ) override;
		void SetUniform( const std::string& InName, const FVector4D& InValue ) override;
		void SetUniform( const std::string& InName, const FQuaternion& InValue ) override;
		void SetUniform( const std::string& InName, const FMatrix4x4& InValue ) override;
		void SetUniform( const std::string& InName, const FSRectFloat& InValue ) override;
		void SetUniform( const std::string& InName, const FSRectInt& InValue ) override;

		/* Is loaded shader */
		bool IsLoaded() const override;

		/* Get handle shader */
		FORCEINLINE uint32 GetHandle() const
		{
			return gpuProgram;
		}

	private:
		/* Insert defines to shader code */
		void InsertDefinesToCode( std::string& InOutCode, const std::string& InDefinesSection );

		/* Destroy shader */
		void Destroy( EShaderType InShaderType );

		/* Get error compilation */
		void GetErrorCompilation( EShaderType InShaderType, std::string& OutError );

		/* Is uniform exists */
		FORCEINLINE bool IsUniformExists( std::string& InName )
		{
			return GetUniformLocation( InName ) > -1;
		}
		
		/* Get unifrom location */
		int GetUniformLocation( const std::string& InName );

		uint32		vertexShader;
		uint32		geometryShader;
		uint32		pixelShader;
		uint32		gpuProgram;

		std::unordered_map< std::string, int >		uniforms;
	};
}

#endif // !RHISHADER_H
