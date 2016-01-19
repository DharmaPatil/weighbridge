/********************************************************************************
** Form generated from reading UI file 'statuswidgeteK5964.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef STATUSWIDGETEK5964_H
#define STATUSWIDGETEK5964_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_StatusWidgetClass
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit11;
    QLineEdit *lineEdit12;
    QLineEdit *lineEdit14;
    QLineEdit *lineEdit15;
    QLineEdit *lineEdit13;
    QLineEdit *lineEdit16;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *lineEdit0;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEdit21;
    QLineEdit *lineEdit22;
    QLineEdit *lineEdit23;
    QLineEdit *lineEdit24;
    QLineEdit *lineEdit25;
    QLineEdit *lineEdit26;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QwtPlot *qwtPlot;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_9;

    void setupUi(QWidget *StatusWidgetClass)
    {
        if (StatusWidgetClass->objectName().isEmpty())
            StatusWidgetClass->setObjectName(QString::fromUtf8("StatusWidgetClass"));
        StatusWidgetClass->setWindowModality(Qt::NonModal);
        StatusWidgetClass->resize(782, 239);
        StatusWidgetClass->setAcceptDrops(false);
        StatusWidgetClass->setAutoFillBackground(false);
        layoutWidget = new QWidget(StatusWidgetClass);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 778, 237));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(250, 120));
        groupBox->setFlat(false);
        horizontalLayout_4 = new QHBoxLayout(groupBox);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        lineEdit11 = new QLineEdit(groupBox);
        lineEdit11->setObjectName(QString::fromUtf8("lineEdit11"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit11->sizePolicy().hasHeightForWidth());
        lineEdit11->setSizePolicy(sizePolicy1);
        lineEdit11->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(lineEdit11);

        lineEdit12 = new QLineEdit(groupBox);
        lineEdit12->setObjectName(QString::fromUtf8("lineEdit12"));

        horizontalLayout->addWidget(lineEdit12);

        lineEdit14 = new QLineEdit(groupBox);
        lineEdit14->setObjectName(QString::fromUtf8("lineEdit14"));

        horizontalLayout->addWidget(lineEdit14);

        lineEdit15 = new QLineEdit(groupBox);
        lineEdit15->setObjectName(QString::fromUtf8("lineEdit15"));

        horizontalLayout->addWidget(lineEdit15);

        lineEdit13 = new QLineEdit(groupBox);
        lineEdit13->setObjectName(QString::fromUtf8("lineEdit13"));

        horizontalLayout->addWidget(lineEdit13);

        lineEdit16 = new QLineEdit(groupBox);
        lineEdit16->setObjectName(QString::fromUtf8("lineEdit16"));

        horizontalLayout->addWidget(lineEdit16);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        lineEdit0 = new QLineEdit(groupBox);
        lineEdit0->setObjectName(QString::fromUtf8("lineEdit0"));
        lineEdit0->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEdit0->sizePolicy().hasHeightForWidth());
        lineEdit0->setSizePolicy(sizePolicy2);
        lineEdit0->setMinimumSize(QSize(0, 20));

        horizontalLayout_3->addWidget(lineEdit0);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        lineEdit21 = new QLineEdit(groupBox);
        lineEdit21->setObjectName(QString::fromUtf8("lineEdit21"));

        horizontalLayout_2->addWidget(lineEdit21);

        lineEdit22 = new QLineEdit(groupBox);
        lineEdit22->setObjectName(QString::fromUtf8("lineEdit22"));

        horizontalLayout_2->addWidget(lineEdit22);

        lineEdit23 = new QLineEdit(groupBox);
        lineEdit23->setObjectName(QString::fromUtf8("lineEdit23"));

        horizontalLayout_2->addWidget(lineEdit23);

        lineEdit24 = new QLineEdit(groupBox);
        lineEdit24->setObjectName(QString::fromUtf8("lineEdit24"));

        horizontalLayout_2->addWidget(lineEdit24);

        lineEdit25 = new QLineEdit(groupBox);
        lineEdit25->setObjectName(QString::fromUtf8("lineEdit25"));

        horizontalLayout_2->addWidget(lineEdit25);

        lineEdit26 = new QLineEdit(groupBox);
        lineEdit26->setObjectName(QString::fromUtf8("lineEdit26"));

        horizontalLayout_2->addWidget(lineEdit26);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_4->addLayout(verticalLayout);


        horizontalLayout_5->addWidget(groupBox);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        qwtPlot = new QwtPlot(layoutWidget);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        sizePolicy1.setHeightForWidth(qwtPlot->sizePolicy().hasHeightForWidth());
        qwtPlot->setSizePolicy(sizePolicy1);
        qwtPlot->setMinimumSize(QSize(500, 120));

        horizontalLayout_6->addWidget(qwtPlot);


        horizontalLayout_5->addLayout(horizontalLayout_6);

        horizontalSpacer_7 = new QSpacerItem(38, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);


        retranslateUi(StatusWidgetClass);

        QMetaObject::connectSlotsByName(StatusWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *StatusWidgetClass)
    {
        StatusWidgetClass->setWindowTitle(QApplication::translate("StatusWidgetClass", "StatusWidget", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StatusWidgetClass", "\344\274\240\346\204\237\345\231\250\351\233\266\347\202\271", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StatusWidgetClass: public Ui_StatusWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // STATUSWIDGETEK5964_H
