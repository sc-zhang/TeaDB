#include "teadbgui.h"
#include <QApplication>
#include <QtWidgets>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TeaDbGui w;
    QString dbpath = "db/Tea.db";
    QFileInfo fi(dbpath);
    if (!fi.isFile()) {
        QMessageBox::critical(&w, "Fatal error", "Database file not found, please put Tea.db file into db folder in the same location with this program!");
        exit(-1);
    } else {
        w.setFixedSize(900, 640);
        w.resize(900, 640);
        w.show();
    }
    return QApplication::exec();
}
