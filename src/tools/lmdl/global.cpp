//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "global.h"

CONVERT_TYPE			g_convertType = CT_MESH;
std::string				g_sourceFile;
std::string				g_outputFile;
std::string				g_materialsDir;
float					g_masa = 0.f;
le::Vector3D_t			g_inertia = le::Vector3D_t( 0.f, 0.f, 0.f );
bool					g_isGenHullShape = false;
bool					g_isStaticCollision = false;
