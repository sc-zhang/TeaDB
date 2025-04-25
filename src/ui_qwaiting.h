/********************************************************************************
** Form generated from reading UI file 'qwaiting.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QWAITING_H
#define UI_QWAITING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QWaiting
{
public:
    QLabel *lblAnime;
    QLabel *label;

    void setupUi(QWidget *QWaiting)
    {
        if (QWaiting->objectName().isEmpty())
            QWaiting->setObjectName("QWaiting");
        QWaiting->setWindowModality(Qt::NonModal);
        QWaiting->resize(149, 182);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QWaiting->sizePolicy().hasHeightForWidth());
        QWaiting->setSizePolicy(sizePolicy);
        lblAnime = new QLabel(QWaiting);
        lblAnime->setObjectName("lblAnime");
        lblAnime->setGeometry(QRect(10, 10, 128, 128));
        label = new QLabel(QWaiting);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 150, 81, 16));

        retranslateUi(QWaiting);

        QMetaObject::connectSlotsByName(QWaiting);
    } // setupUi

    void retranslateUi(QWidget *QWaiting)
    {
        QWaiting->setWindowTitle(QCoreApplication::translate("QWaiting", "QWaiting", nullptr));
        lblAnime->setText(QString());
        label->setText(QCoreApplication::translate("QWaiting", "Processing...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QWaiting: public Ui_QWaiting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QWAITING_H
