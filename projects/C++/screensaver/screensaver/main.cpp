#include "screensaver.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	screensaver w;
	w.show();
	return a.exec();
}
