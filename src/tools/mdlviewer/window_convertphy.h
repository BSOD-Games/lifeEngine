#ifndef WINDOW_CONVERT_PHY_H
#define WINDOW_CONVERT_PHY_H

#include <qdialog.h>
#include <qvector3d.h>

namespace Ui
{
	class Window_ConvertPHY;
}

class Window_ConvertPHY : public QDialog
{
	Q_OBJECT

public:
	Window_ConvertPHY( QWidget* Parent = nullptr );
	~Window_ConvertPHY();

private slots:
	void		on_toolButton_InputPath_clicked();
	void		on_toolButton_OutputPath_clicked();
	void		on_checkBox_Static_clicked();
	void		on_checkBox_HullShape_clicked();
	void		on_pushButton_Convert_clicked();
	void		on_pushButton_Cancel_clicked();


private:
	Ui::Window_ConvertPHY*				ui;
	QString								inputPath;
	QString								outputPath;
	float								mass;
	QVector3D							inertia;
	bool								isStatic;
	bool								isHullShape;


};

#endif // !WINDOW_CONVERT_PHY_H