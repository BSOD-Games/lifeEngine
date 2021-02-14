//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPARSERMATERIAL_H
#define IPARSERMATERIAL_H

#include "engine/iparser.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;
	class IResourceSystem;
	class IMaterialSystem;
	class IMaterial;

	//---------------------------------------------------------------------//

	class IParserMaterial : public IParser
	{
	public:
		virtual ~IParserMaterial() {}
		virtual IMaterial*				Read( const char* Path, IResourceSystem* ResourceSystem, IMaterialSystem* MaterialSystem, IFactory* EngineFactory ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IPARSERMATERIAL_H
