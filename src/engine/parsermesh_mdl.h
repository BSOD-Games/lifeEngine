//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PARSERMESH_MDL_H
#define PARSERMESH_MDL_H

#include "engine/iparsermesh.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ParserMeshMDL : public IParserMesh
	{
	public:
		// IParser
		virtual void					Release();

		virtual const char*				GetVersion() const;
		virtual const char*				GetName() const;
		virtual UInt32_t				GetCountFileExtensions() const;
		virtual const char**			GetFileExtensions() const;
		virtual const char*				GetAuthor() const;
		
		// IParserMesh
		virtual IMesh*					Read( const char* Path, IResourceSystem* ResourceSystem, IFactory* StudioRenderFactory );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !PARSERMESH_MDL_H
