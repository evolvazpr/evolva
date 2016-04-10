/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *log_label_;
    QGraphicsView *graphicsView;
    QLabel *label_stats_;
    QPushButton *pushButton;
    QTextBrowser *stats_textWindow;
    QTextBrowser *log_textWindow;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1600, 900);
        log_label_ = new QLabel(Dialog);
        log_label_->setObjectName(QStringLiteral("log_label_"));
        log_label_->setGeometry(QRect(1420, 100, 141, 31));
        QFont font;
        font.setPointSize(14);
        log_label_->setFont(font);
        graphicsView = new QGraphicsView(Dialog);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(330, 140, 931, 661));
        label_stats_ = new QLabel(Dialog);
        label_stats_->setObjectName(QStringLiteral("label_stats_"));
        label_stats_->setGeometry(QRect(140, 110, 71, 21));
        label_stats_->setFont(font);
        pushButton = new QPushButton(Dialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(1320, 800, 221, 91));
        QFont font1;
        font1.setPointSize(22);
        pushButton->setFont(font1);
        stats_textWindow = new QTextBrowser(Dialog);
        stats_textWindow->setObjectName(QStringLiteral("stats_textWindow"));
        stats_textWindow->setGeometry(QRect(30, 140, 256, 661));
        log_textWindow = new QTextBrowser(Dialog);
        log_textWindow->setObjectName(QStringLiteral("log_textWindow"));
        log_textWindow->setGeometry(QRect(1310, 140, 256, 641));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        log_label_->setText(QApplication::translate("Dialog", "Log", 0));
        label_stats_->setText(QApplication::translate("Dialog", "Stats", 0));
        pushButton->setText(QApplication::translate("Dialog", "Kolejna tura", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
