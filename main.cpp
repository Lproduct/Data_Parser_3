#include <QApplication>

#include "FenPrincipale.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Data_parser_3);

    QApplication app(argc, argv);
    FenPrincipale mainWin;
    mainWin.show();
    return app.exec();
}

