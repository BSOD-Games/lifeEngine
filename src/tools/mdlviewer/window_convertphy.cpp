#include "window_convertphy.h"
#include "ui_window_convertphy.h"

#include <qfiledialog.h>
#include <qdebug.h>
#include <qprocess.h>
#include <qstringlist.h>
#include <iostream>
#include <sstream>
#include <qmessagebox>

#include "common/gameinfo.h"
#include "engineapi.h"

// ------------------------------------------------------------------------------------ //
// Constructor
// ------------------------------------------------------------------------------------ //
Window_ConvertPHY::Window_ConvertPHY( QWidget* Parent ) :
	QDialog( Parent ),
	ui( new Ui::Window_ConvertPHY() )
{
	ui->setupUi( this );

	ui->lineEdit_InputPath->setDisabled( true );
	ui->lineEdit_OutputPath->setDisabled( true );
}

// ------------------------------------------------------------------------------------ //
// Destructor
// ------------------------------------------------------------------------------------ //
Window_ConvertPHY::~Window_ConvertPHY()
{
	delete ui;
}

// ------------------------------------------------------------------------------------ //
// Event: select input path
// ------------------------------------------------------------------------------------ //
void Window_ConvertPHY::on_toolButton_InputPath_clicked()
{
	QString		fileName = QFileDialog::getOpenFileName( this, "Choose model for convert",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Model files (*.*)" );

	if ( fileName.isEmpty() ) return;

	ui->lineEdit_InputPath->setText( fileName );
	inputPath = fileName;

	int found = fileName.toStdString().find_last_of( '/' );
	std::string out = fileName.toStdString().substr( 0, found + 1 );

	ui->lineEdit_OutputPath->setText( out.c_str() );
}

// ------------------------------------------------------------------------------------ //
// Event: select input path
// ------------------------------------------------------------------------------------ //
void Window_ConvertPHY::on_toolButton_OutputPath_clicked()
{
	QString		fileName = QFileDialog::getExistingDirectory( this, "Save as..",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir );

	if ( fileName.isEmpty() ) return;

	ui->lineEdit_OutputPath->setText( fileName );
	outputPath = fileName;
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on check box static
// ------------------------------------------------------------------------------------ //
void Window_ConvertPHY::on_checkBox_Static_clicked()
{
	if ( ui->checkBox_Static->checkState() == Qt::CheckState::Checked )
		isStatic = true;
	else
		isStatic = false;
}

// ------------------------------------------------------------------------------------ //
// Event: clicked on check box generate hull shape
// ------------------------------------------------------------------------------------ //
void Window_ConvertPHY::on_checkBox_HullShape_clicked()
{
	if ( ui->checkBox_HullShape->checkState() == Qt::CheckState::Checked )
		isHullShape = true;
	else
		isHullShape = false;
}

// ------------------------------------------------------------------------------------ //
// Event: start convertation
// ------------------------------------------------------------------------------------ //
void Window_ConvertPHY::on_pushButton_Convert_clicked()
{
	if ( inputPath.isEmpty() )
	{
		QMessageBox::critical( this, "Convert Error", "No input file was selected. Please select an input file to convert", QMessageBox::Button::Ok );
		return;
	}

	if ( outputPath.isEmpty() )
	{
		QMessageBox::critical( this, "Convert Error", "No save directory has been selected. Please choose where to save the new file", QMessageBox::Button::Ok );
		return;
	}

	mass = ui->doubleSpinBox_Mass->value();

	if ( mass == 0.0f )
		mass = 10.0f;

	inertia.setX( ui->doubleSpinBox_x->value() );
	inertia.setY( ui->doubleSpinBox_y->value() );
	inertia.setZ( ui->doubleSpinBox_z->value() );

	QProcess			process;
	QStringList			arguments;

	arguments << "-c" <<"-s" << inputPath << "-o" << outputPath;
	arguments << "-masa" << QString::number( mass ) << "-i" << QString::number( inertia.x() ) << " " << QString::number( inertia.y() )
		<< QString::number( inertia.z() );

	if ( isStatic )
		arguments << "-static";

	if ( isHullShape )
		arguments << "-ghs";

	process.execute( "./lmdl", arguments );
	process.waitForFinished();
	this->close();
}

// ------------------------------------------------------------------------------------ //
// Event: cancel convertation
// ------------------------------------------------------------------------------------ //
void Window_ConvertPHY::on_pushButton_Cancel_clicked()
{
	inputPath.clear();
	outputPath.clear();
	this->close();
}