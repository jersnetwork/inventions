#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QtWidgets/QMainWindow>
#include "ui_screensaver.h"

class screensaver : public QMainWindow
{
	Q_OBJECT

public:
	screensaver(QWidget *parent = 0);
	~screensaver();

private:
	Ui::screensaverClass ui;
};

#endif // SCREENSAVER_H
