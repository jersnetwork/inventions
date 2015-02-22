#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);  
    
  MainWindow window;

  window.resize(300, 200);
  window.move(300, 300);  
  window.setWindowTitle("QToolBar");
  window.show();

  return app.exec();
}
