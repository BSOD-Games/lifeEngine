//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-Games/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <qmessagebox.h>
#include <qdebug.h>

#include "errors.h"

// ------------------------------------------------------------------------------------ //
// Error: critical
// ------------------------------------------------------------------------------------ //
void Error_Critical( const char* Message )
{
	std::ofstream			fileLog( "lmdviewer.log", std::ios::app );

	QMessageBox::critical( nullptr, "Error LMDViewer", Message );
	fileLog << "\nCritical error: " << Message;
	exit( 1 );
}

// ------------------------------------------------------------------------------------ //
// Error: info
// ------------------------------------------------------------------------------------ //
void Error_Info( const char* Message )
{
	qCritical() << Message;
	QMessageBox::critical( nullptr, "Error LMDViewer", Message );
}