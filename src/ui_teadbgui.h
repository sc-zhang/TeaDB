/********************************************************************************
** Form generated from reading UI file 'teadbgui.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEADBGUI_H
#define UI_TEADBGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TeaDbGui
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTableView *tblTop10Similar;
    QHBoxLayout *horizontalLayout;
    QLineEdit *txtQueryFile;
    QPushButton *btnSelect;
    QPushButton *btnQuery;

    void setupUi(QWidget *TeaDbGui)
    {
        if (TeaDbGui->objectName().isEmpty())
            TeaDbGui->setObjectName("TeaDbGui");
        TeaDbGui->resize(800, 600);
        gridLayout = new QGridLayout(TeaDbGui);
        gridLayout->setObjectName("gridLayout");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        tblTop10Similar = new QTableView(TeaDbGui);
        tblTop10Similar->setObjectName("tblTop10Similar");
        tblTop10Similar->setMaximumSize(QSize(1205, 823));
        tblTop10Similar->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        verticalLayout->addWidget(tblTop10Similar);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        txtQueryFile = new QLineEdit(TeaDbGui);
        txtQueryFile->setObjectName("txtQueryFile");

        horizontalLayout->addWidget(txtQueryFile);

        btnSelect = new QPushButton(TeaDbGui);
        btnSelect->setObjectName("btnSelect");

        horizontalLayout->addWidget(btnSelect);

        btnQuery = new QPushButton(TeaDbGui);
        btnQuery->setObjectName("btnQuery");

        horizontalLayout->addWidget(btnQuery);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(TeaDbGui);

        QMetaObject::connectSlotsByName(TeaDbGui);
    } // setupUi

    void retranslateUi(QWidget *TeaDbGui)
    {
        TeaDbGui->setWindowTitle(QCoreApplication::translate("TeaDbGui", "TeaDb", nullptr));
        btnSelect->setText(QCoreApplication::translate("TeaDbGui", "Select", nullptr));
        btnQuery->setText(QCoreApplication::translate("TeaDbGui", "Query", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TeaDbGui: public Ui_TeaDbGui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEADBGUI_H
