#ifndef WINDOW_IMPORTSETTINGS_H
#define WINDOW_IMPORTSETTINGS_H

#include <qdialog>

namespace Ui
{
	class Window_ImportSettings;
}

class Window_ImportSettings : public QDialog
{
	Q_OBJECT

public:

	enum RESULT_TYPE
	{
		RT_OK,
		RT_CANCEL
	};

	Window_ImportSettings( QWidget* Parent = nullptr );
	~Window_ImportSettings();

protected:
	// QWidget
	virtual void	closeEvent( QCloseEvent* Event );

private slots:
	void	on_buttonPathIn_clicked();
	void	on_buttonPathOut_clicked();
	void	on_buttonConvert_clicked();
	void	on_buttonCancel_clicked();

private:
	Ui::Window_ImportSettings*					ui;
	QString										inputPath;
	QString										outputPath;

};

#endif // !WINDOW_IMPORTSETTINGS_H