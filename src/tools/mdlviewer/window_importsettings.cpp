#include "window_importsettings.h"
#include "ui_window_importsettings.h"

#include <qfiledialog.h>
#include <qprocess.h>
#include <qdebug.h>
#include <qevent.h>
#include <qmessagebox.h>

#include "engineapi.h"
#include "common/gameinfo.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_ImportSettings::Window_ImportSettings( QWidget* Parent ) :
	QDialog( Parent ),
	ui( new Ui::Window_ImportSettings() )
{
	ui->setupUi( this );

	ui->lineEdit_InputPath->setDisabled( true );
	ui->lineEdit_OutputPath->setDisabled( true );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_ImportSettings::~Window_ImportSettings()
{
	inputPath.clear();
	outputPath.clear();
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: close window
// ------------------------------------------------------------------------------------ //
void Window_ImportSettings::closeEvent( QCloseEvent* Event )
{
	if ( Event->type() == QCloseEvent::Close )
		done( RT_CANCEL );

	Event->accept();
}

// ------------------------------------------------------------------------------------ //
// Event: choose model path
// ------------------------------------------------------------------------------------ //
void Window_ImportSettings::on_buttonPathIn_clicked()
{	
	QString		fileName = QFileDialog::getOpenFileName( this, "Choose model for convert",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Model files (*.*)" );

	if ( fileName.isEmpty() ) return;

	ui->lineEdit_InputPath->setText( fileName );
	inputPath = fileName;

	int found = fileName.lastIndexOf( '/' );
	std::string out = fileName.toStdString().substr( 0, found + 1 );
	ui->lineEdit_OutputPath->setText( out.c_str() );
}

// ------------------------------------------------------------------------------------ //
// Event: choose output path
// ------------------------------------------------------------------------------------ //
void Window_ImportSettings::on_buttonPathOut_clicked()
{
	QString		fileName = QFileDialog::getExistingDirectory( this, "Save as..",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir );

	if ( fileName.isEmpty() ) return;

	ui->lineEdit_OutputPath->setText( fileName );
	outputPath = fileName;
}

// ------------------------------------------------------------------------------------ //
// Event: start convert
// ------------------------------------------------------------------------------------ //
void Window_ImportSettings::on_buttonConvert_clicked()
{
	if ( inputPath.isEmpty() )
	{
		QMessageBox::critical( this, "Import error", "No import file selected. Select a file" );
		return;
	}

	if ( outputPath.isEmpty() )
	{
		QMessageBox::critical( this, "Import error", "No output directory selected. Select the directory where the file will be imported" );
		return;
	}

	QProcess		procces;
	
	procces.execute( "./lmdl", QStringList() << "-s" << inputPath << "-o" << outputPath );
	procces.waitForFinished();
	this->close();
}

// ------------------------------------------------------------------------------------ //
// Event: cancel import
// ------------------------------------------------------------------------------------ //
void Window_ImportSettings::on_buttonCancel_clicked()
{
	inputPath.clear();
	outputPath.clear();
	this->close();
}

