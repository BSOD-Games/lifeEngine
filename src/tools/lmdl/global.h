//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

#include "common/types.h"

//---------------------------------------------------------------------//

enum CONVERT_TYPE
{
	CT_MESH,
	CT_COLLISION
};

//---------------------------------------------------------------------//

extern CONVERT_TYPE				g_convertType;
extern std::string				g_sourceFile;
extern std::string				g_outputFile;
extern std::string				g_materialsDir;
extern float					g_masa;
extern le::Vector3D_t			g_inertia;
extern bool						g_isGenHullShape;
extern bool						g_isStaticCollision;

//---------------------------------------------------------------------//

#endif // !GLOBAL_H
