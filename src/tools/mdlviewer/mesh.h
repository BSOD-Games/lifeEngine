//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-Games/lifeEngine
// ������:				����� ���� (ZhulaiIvan)
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

	le::IMesh*					GetMesh();
	std::vector<std::string>	GetMaterialPaths();

private:
	le::IMesh*					mesh;
	MDLDoc						mdlDoc;
	std::string					path;
};

