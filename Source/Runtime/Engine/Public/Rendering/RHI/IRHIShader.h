// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef IRHISHADER_H
#define IRHISHADER_H

#include <string>
#include <vector>

#include "Misc/Object.h"
#include "Misc/RefCounted.h"
#include "Math/Math.h"

namespace le
{
	struct SColor;

	enum EShaderType
	{
		ST_Vertex,
		ST_Geometry,
		ST_Pixel
	};

	class IRHIShader : public Object, public RefCounted
	{
	public:
		/* Destructor */
		virtual ~IRHIShader() {}

		/* Compile shader */
		virtual bool Compile( const std::string& InCode, EShaderType InShaderType, std::vector< std::string >* InDefines = nullptr, std::string* OutError = nullptr ) = 0;

		/* Link shader */
		virtual bool Link( std::string* OutError = nullptr ) = 0;

		/* Destroy shader */
		virtual void Destroy() = 0;

		/* Set uniform */
		virtual void SetUniform( const std::string& InName, int InValue ) = 0;
		virtual void SetUniform( const std::string& InName, float InValue ) = 0;
		virtual void SetUniform( const std::string& InName, bool InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const SColor& InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const FVector2D& InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const FVector3D& InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const FVector4D& InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const FQuaternion& InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const FMatrix4x4& InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const FSRectFloat& InValue ) = 0;
		virtual void SetUniform( const std::string& InName, const FSRectInt& InValue ) = 0;

		/* Is loaded shader */
		virtual bool IsLoaded() const = 0;
	};
}

#endif // !IRHISHADER_H
