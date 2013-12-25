#include <QtGui>
#include <QApplication>

#include "mainwindow.h"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(diagramscene);

    QApplication *app = new QApplication(argv, args);
    MainWindow mw;
    mw.showMaximized();
		
    return app->exec();
}
