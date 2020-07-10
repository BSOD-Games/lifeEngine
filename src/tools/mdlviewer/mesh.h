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

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool						Load( const QString& Path );
	le::IMesh*					GetMesh();

private:
	le::IMesh*					mesh;
	std::vector<QString>		paths;
};

