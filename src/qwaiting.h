//
// Created by 张晟铖 on 2025/4/25.
//

#ifndef TEADB_QWAITING_H
#define TEADB_QWAITING_H

#include <QMovie>
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
    class QWaiting;
}
QT_END_NAMESPACE

class QWaiting : public QWidget {
    Q_OBJECT

public:
    explicit QWaiting(QWidget *parent = nullptr);
    ~QWaiting() override;

private:
    Ui::QWaiting *ui;
    QMovie *m_Move;
};


#endif//TEADB_QWAITING_H
