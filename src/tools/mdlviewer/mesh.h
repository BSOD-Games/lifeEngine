//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Жулай Иван (ZhulaiIvan)
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <qstring.h>
#include <string>
#include <vector>

#include "studiorender/imesh.h"
#include "ui_window_viewer.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool						Load( const QString& Path );
	bool						LoadMaterial(	const QString& Path, le::UInt32_t Index	);

	le::IMesh*					GetMesh();
	std::vector<QString>		GetPaths();

private:
	le::IMesh*					mesh;
	std::vector<QString>		paths;
};

