//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <qmessagebox.h>

#include "errors.h"

// ------------------------------------------------------------------------------------ //
// Error: critical
// ------------------------------------------------------------------------------------ //
void Error_Critical( const char* Message )
{
	std::ofstream			fileLog( "lmteditor.log", std::ios::app );

	QMessageBox::critical( nullptr, "Error LMTEditor", Message );
	fileLog << "\nCritical error: " << Message;
	exit( 1 );
}