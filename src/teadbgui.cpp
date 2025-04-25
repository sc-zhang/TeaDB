//
// Created by 张晟铖 on 2025/4/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TeaDbGui.h" resolved

#include "teadbgui.h"
#include "ui_teadbgui.h"

void TeaDbGui::loadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file"), ".", tr("Jac files(*.jac);;Text files(*.txt);;All files(*.*)"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Failed to load file!");
    } else {
        this->queryFile = fileName.toStdString();
        this->ui->txtQueryFile->setText(fileName);
    }
}

void TeaDbGui::getTop10Similar() {
    const char *dbpath = "db/Tea.db";
    this->ui->btnSelect->setEnabled(false);
    this->ui->btnQuery->setEnabled(false);
    this->tblModel->setRowCount(0);
    QWaiting *qwait = new QWaiting(this);
    qwait->show();
    QFuture<std::vector<Sample>> future = QtConcurrent::run(query_db, this->queryFile.c_str(), dbpath, 0);
    QFutureWatcher<std::vector<Sample>> watcher;
    if (qApp != nullptr) {
        QEventLoop loop;
        QObject::connect(&watcher, &QFutureWatcher<std::vector<Sample>>::finished, &loop, &QEventLoop::quit);
        watcher.setFuture(future);
        loop.exec();
    } else {
        watcher.setFuture(future);
        watcher.waitForFinished();
    }
    // std::vector<Sample> top10_similar_samples = query_db(this->queryFile.c_str(), dbpath, 0);

    int row_idx = 0;
    for (const Sample &smp: future.result()) {
        this->tblModel->setItem(row_idx, 0, new QStandardItem(QString::fromStdString(smp.id)));
        this->tblModel->setItem(row_idx, 1, new QStandardItem(QString::fromStdString(smp.name)));
        this->tblModel->setItem(row_idx, 2, new QStandardItem(QString::fromStdString(smp.englishname)));
        this->tblModel->setItem(row_idx, 3, new QStandardItem(QString::fromStdString(smp.species)));
        this->tblModel->setItem(row_idx, 4, new QStandardItem(QString::fromStdString(smp.group)));
        this->tblModel->setItem(row_idx, 5, new QStandardItem(QString::fromStdString(smp.country)));
        this->tblModel->setItem(row_idx, 6, new QStandardItem(QString::fromStdString(smp.area)));
        this->tblModel->setItem(row_idx, 7, new QStandardItem(QString::fromStdString(smp.similarity)));
        ++row_idx;
        QCoreApplication::processEvents();
    }
    qwait->close();

    this->ui->btnSelect->setEnabled(true);
    this->ui->btnQuery->setEnabled(true);
}
TeaDbGui::TeaDbGui(QWidget *parent) : QWidget(parent), ui(new Ui::TeaDbGui) {
    ui->setupUi(this);
    connect(this->ui->btnSelect, &QPushButton::clicked, this, &TeaDbGui::loadFile);
    connect(this->ui->btnQuery, &QPushButton::clicked, this, &TeaDbGui::getTop10Similar);
    tblModel->setHorizontalHeaderItem(0, new QStandardItem("ID"));
    tblModel->setHorizontalHeaderItem(1, new QStandardItem("Chinese name"));
    tblModel->setHorizontalHeaderItem(2, new QStandardItem("English name"));
    tblModel->setHorizontalHeaderItem(3, new QStandardItem("Species"));
    tblModel->setHorizontalHeaderItem(4, new QStandardItem("Group"));
    tblModel->setHorizontalHeaderItem(5, new QStandardItem("Country"));
    tblModel->setHorizontalHeaderItem(6, new QStandardItem("Area"));
    tblModel->setHorizontalHeaderItem(7, new QStandardItem("Similarity"));
    this->ui->tblTop10Similar->setModel(tblModel);
}

TeaDbGui::~TeaDbGui() {
    delete ui;
}
