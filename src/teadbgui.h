//
// Created by 张晟铖 on 2025/4/25.
//

#ifndef TEADB_TEADBGUI_H
#define TEADB_TEADBGUI_H

#include "query_db.h"
#include "qwaiting.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QWidget>
#include <QtConcurrent/QtConcurrentRun>

QT_BEGIN_NAMESPACE
namespace Ui {
    class TeaDbGui;
}
QT_END_NAMESPACE

class TeaDbGui : public QWidget {
    Q_OBJECT

public:
    explicit TeaDbGui(QWidget *parent = nullptr);
    ~TeaDbGui() override;

private:
    Ui::TeaDbGui *ui;
    void loadFile();
    void getTop10Similar();
    QStandardItemModel *tblModel = new QStandardItemModel();
    std::string queryFile;
};


#endif//TEADB_TEADBGUI_H
