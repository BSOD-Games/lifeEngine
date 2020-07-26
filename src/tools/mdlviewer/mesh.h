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
	bool						Save();
	bool						SaveAs( const QString& Path );
	void						Clear();
	void						SetEdit( bool isEdit );

	le::IMesh*					GetMesh();
	std::vector<std::string>	GetMaterialPaths();
	bool						GetEdited();

private:
	le::IMesh*					mesh;
	MDLDoc						mdlDoc;
	std::string					path;

	bool						isEdited = false;;
};

