#include "window_importsettings.h"
#include "ui_window_importsettings.h"

#include <qfiledialog.h>
#include <qprocess.h>
#include <qdebug.h>
#include <qevent.h>

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
}

// ------------------------------------------------------------------------------------ //
// Event: choose output path
// ------------------------------------------------------------------------------------ //
void Window_ImportSettings::on_buttonPathOut_clicked()
{
	QString		fileName = QFileDialog::getSaveFileName( this, "Save as..",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Model file (*.mdl)" );

	if ( fileName.isEmpty() ) return;

	ui->lineEdit_OutputPath->setText( fileName );
	outputPath = fileName;
}

// ------------------------------------------------------------------------------------ //
// Event: choose output path
// ------------------------------------------------------------------------------------ //
void Window_ImportSettings::on_buttonConvert_clicked()
{
	if ( inputPath.isEmpty() || outputPath.isEmpty() ) return;

	QProcess		procces;
	procces.execute( "lmdl", QStringList() << "-s " << inputPath << " -o " << outputPath );
	qDebug() << QStringList() << "-s " << inputPath << " -o " << outputPath;
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

