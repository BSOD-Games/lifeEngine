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

#include "engine/imodel.h"
#include "studiorender/imesh.h"
#include "scene.h"
#include "errors.h"
#include "ui_window_viewer.h"

class Model
{
public:
	Model();
	~Model();

	bool						Load( const QString& Path );
	bool						LoadMaterial(	const QString& Path, le::UInt32_t Index	);
	bool						Save();
	bool						SaveAs( const QString& Path );
	void						Clear();
	void						SetEdited( bool isEdited );

	le::IMesh*					GetMesh();
	std::vector<std::string>	GetMaterialPaths();
	void						RotateByMouse( const le::Vector2D_t& MouseOffset, float MouseSensitivity );
	bool						GetEdited();
	int							GetCountTriangles();
	int							GetCountVerteces();

private:
	le::IMesh*					mesh;
	le::IModel*					model;
	MDLDoc						mdlDoc;
	std::string					path;
	le::Vector3D_t				eulerRotation;
	le::Quaternion_t			quatRotation;
	bool						isEdit = false;
};

