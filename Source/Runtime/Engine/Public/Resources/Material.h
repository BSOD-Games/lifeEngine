// Copyright BSOD-Games, All Rights Reserved.
// Authors: Yehor Pohuliaka (zombiHello)

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>

#include "Misc/EngineGlobals.h"
#include "Misc/Types.h"
#include "Rendering/BaseShader.h"
#include "Rendering/ShaderFactory.h"
#include "Rendering/ShaderVar.h"
#include "Resources/Resource.h"

namespace le
{
	class Material : public Resource
	{
	public:
		/* Constructor */
		Material();

		/* Constructor of copy */
		Material( const Material& InCopy );

		/* Destructor */
		~Material();

		/* Serialize resource */
		bool Serialize( const Path& InPath ) override;

		/* Deserialize resource */
		bool Deserialize( const Path& InPath ) override;

		/* Add shader var */
		FORCEINLINE void AddVar( const ShaderVar& InShaderVar )
		{
			vars.push_back( InShaderVar );
			vars.back().SubscribeMaterial( this );
		}

		/* Remove shader var by name*/
		void RemoveVar( const std::string& InName );	
		
		/* Remove shader var by index */
		FORCEINLINE void RemoveVar( uint32 InIndex )
		{
			LIFEENGINE_ASSERT( InIndex < vars.size() );
			
			vars[ InIndex ].UnsubscribeMaterial( this );
			vars.erase( InIndex + vars.begin() );
		}

		/* Find shader var */
		ShaderVar* FindVar( const std::string& InName ) const;

		/* Refresh material */
		void Refresh();

		/* Update shader */
		FORCEINLINE bool UpdateShader();

		/* Need update shader */
		FORCEINLINE void NeadUpdateShader()
		{
			isNeadUpdateShader = true;
		}

		/* Clear material */
		void Clear();

		/* Set shader */
		FORCEINLINE void SetShader( BaseShader* InShader )
		{
			if ( shader )		shader->ReleaseRef();

			shader = InShader;
			if ( InShader )		InShader->AddRef();
		}

		/* Set shader by name */
		FORCEINLINE void SetShader( const std::string& InName )				{ shader = GShaderFactory->Create( InName ); }

		/* Get resource type */
		EResourceType GetType() const override;

		/* Get shader name */
		FORCEINLINE FBaseShaderRef GetShader() const						{ return shader; }

		/* Get vars */
		FORCEINLINE const std::vector< ShaderVar >& GetVars() const			{ return vars; }

		/* Operator = */
		FORCEINLINE Material& operator=( const Material& InRight )
		{
			Clear();
			
			vars = InRight.vars;
			shader = InRight.shader;
			if ( shader )		shader->AddRef();		
			
			return *this;
		}

	private:
		bool							isNeadUpdateShader;
		FBaseShaderRef					shader;
		std::vector< ShaderVar >		vars;
	};
}

#endif // !MATERIAL_H
