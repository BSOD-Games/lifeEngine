//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <qapplication.h>
#include <qfile.h>
#include <qtextstream.h>

#include "window_selectgame.h"

// ------------------------------------------------------------------------------------ //
// Main function
// ------------------------------------------------------------------------------------ //
int main( int argc, char** argv )
{
	QApplication			application( argc, argv );

	// Loading style
	{
		QFile			file( "styles/dark/style.css" );
		if ( file.exists() )
		{
			file.open( QFile::ReadOnly | QFile::Text );
			QTextStream			textStream( &file );
			application.setStyleSheet( textStream.readAll() );
		}
	}

	Window_SelectGame		window_SelectGame;
	return window_SelectGame.exec();
}
