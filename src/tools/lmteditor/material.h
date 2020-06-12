//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef LMT_MATERIAL_H
#define LMT_MATERIAL_H

#include <qstring.h>
#include <vector>
#include <memory>

#include "engine/imaterial.h"
#include "shaderparameter.h"

//---------------------------------------------------------------------//

class Material
{
public:
	Material();
	~Material();

	bool							Load( const QString& Path );
	bool							Save( const QString& Path );
	void							Clear();
	void							EnableDepthTest( bool Enable = true );
	void							EnableDepthWrite( bool Enable = true );
	void							EnableBlend( bool Enable = true );
	void							EnableCullFace( bool Enable = true );
	void							AddParameter( const QString& Name, le::SHADER_PARAMETER_TYPE Type );
	void							RemoveParameter( quint32 Index );

	void							SetSurface( const QString& Name );
	void							SetShader( const QString& Name );
	void							SetCullFaceType( le::CULLFACE_TYPE CullFaceType );

	bool							HasParameter( const QString& Name ) const;
	inline quint32					GetCountParameters() const		{ return parameters.size(); }
	inline bool						IsEnabledDepthTest() const		{ return material->IsDepthTest(); }
	inline bool						IsEnabledDepthWrite() const		{ return material->IsDepthWrite(); }
	inline bool						IsEnabledBlend() const			{ return material->IsBlend(); }
	inline bool						IsEnabledCullFace() const		{ return material->IsCullFace(); }
	inline le::CULLFACE_TYPE		GetCullFaceType() const			{ return material->GetCullFaceType(); }
	inline QString					GetSurface() const				{ return material->GetSurfaceName(); }
	inline ShaderParameterPtr		GetParameter( quint32 Index ) const
	{
		if ( Index >= parameters.size() ) return nullptr;
		return parameters[ Index ];
	}
	inline const std::vector< ShaderParameterPtr >&			GetParameters() const	{ return parameters; }
	inline std::string				GetShader() const				{ return material->GetNameShader(); }
	inline le::IMaterial*			GetHandle() const				{ return material; }

private:
	le::IMaterial*							material;
	std::vector< ShaderParameterPtr >		parameters;
};

//---------------------------------------------------------------------//

#endif // !MATERIAL_H
