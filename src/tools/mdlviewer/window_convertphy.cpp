#include "window_convertphy.h"
#include "ui_window_convertphy.h"

#include <qfiledialog.h>
#include <qdebug.h>
#include <qprocess.h>
#include <qstringlist.h>
#include <iostream>
#include <sstream>


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
}

// ------------------------------------------------------------------------------------ //
// Event: select input path
// ------------------------------------------------------------------------------------ //
void Window_ConvertPHY::on_toolButton_OutputPath_clicked()
{
	QString		fileName = QFileDialog::getSaveFileName( this, "Save as..",
		EngineAPI::GetInstance()->GetEngine()->GetGameInfo().gameDir, "Physics file (*.phy)" );

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
	mass = ui->doubleSpinBox_Mass->value();

	inertia.setX( ui->doubleSpinBox_x->value() );
	inertia.setY( ui->doubleSpinBox_y->value() );
	inertia.setZ( ui->doubleSpinBox_z->value() );

	QProcess			process;
	std::stringstream	arguments;

	arguments << "-s " << inputPath.toStdString() << " -o " << outputPath.toStdString();
	arguments << " -c" << " -m " << mass << " -i " << inertia.x() << " " << inertia.y() << " " << inertia.z();

	if ( isStatic )
		arguments << " -static";
	if ( isHullShape )
		arguments << " -ghs";

	qDebug() << QStringList() <<  arguments.str().c_str();
	qDebug() << process.execute( "lmdl.exe", QStringList() << arguments.str().c_str() );
	process.waitForFinished();
	this->close();
}
