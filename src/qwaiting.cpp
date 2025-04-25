//
// Created by 张晟铖 on 2025/4/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_QWaiting.h" resolved

#include "qwaiting.h"
#include "ui_qwaiting.h"

QWaiting::QWaiting(QWidget *parent) : QWidget(parent), ui(new Ui::QWaiting) {
    ui->setupUi(this);
    this->setWindowOpacity(0.8);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);//设置为对话框风格，并且去掉边框
    setWindowModality(Qt::WindowModal);                  //设置为模式对话框，同时在构造该对话框时要设置父窗口
    ui->lblAnime->setStyleSheet("background-color: transparent;");
    m_Move = new QMovie("resources/loading.gif");
    ui->lblAnime->setMovie(m_Move);
    ui->lblAnime->setScaledContents(true);
    m_Move->start();
}

QWaiting::~QWaiting() {
    m_Move->stop();
    delete ui;
}
