/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_stats_;
    QLabel *log_label_;
    QHBoxLayout *horizontalLayout;
    QTextEdit *stats_textWindow;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QTextEdit *log_textWindow;
    QPushButton *pushButton;

    void setupUi(QDialog *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QStringLiteral("Gui"));
        Gui->resize(1024, 768);
        verticalLayout = new QVBoxLayout(Gui);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        graphicsView = new QGraphicsView(Gui);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setEnabled(true);
        graphicsView->setMinimumSize(QSize(100, 100));
        graphicsView->setSizeIncrement(QSize(1, 1));
        graphicsView->setBaseSize(QSize(1, 1));
        graphicsView->setSceneRect(QRectF(0, 0, 0, 0));

        verticalLayout->addWidget(graphicsView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_stats_ = new QLabel(Gui);
        label_stats_->setObjectName(QStringLiteral("label_stats_"));
        QFont font;
        font.setPointSize(14);
        label_stats_->setFont(font);

        horizontalLayout_2->addWidget(label_stats_);

        log_label_ = new QLabel(Gui);
        log_label_->setObjectName(QStringLiteral("log_label_"));
        log_label_->setFont(font);

        horizontalLayout_2->addWidget(log_label_, 0, Qt::AlignRight);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        stats_textWindow = new QTextEdit(Gui);
        stats_textWindow->setObjectName(QStringLiteral("stats_textWindow"));
        stats_textWindow->setTextInteractionFlags(Qt::NoTextInteraction);

        horizontalLayout->addWidget(stats_textWindow);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        log_textWindow = new QTextEdit(Gui);
        log_textWindow->setObjectName(QStringLiteral("log_textWindow"));
        log_textWindow->setTextInteractionFlags(Qt::NoTextInteraction);

        horizontalLayout->addWidget(log_textWindow);


        verticalLayout->addLayout(horizontalLayout);

        pushButton = new QPushButton(Gui);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QFont font1;
        font1.setPointSize(22);
        pushButton->setFont(font1);

        verticalLayout->addWidget(pushButton);

        pushButton->raise();
        graphicsView->raise();

        retranslateUi(Gui);

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QDialog *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "Evolva - a game of life", 0));
        label_stats_->setText(QApplication::translate("Gui", "Stats", 0));
        log_label_->setText(QApplication::translate("Gui", "Log", 0));
        pushButton->setText(QApplication::translate("Gui", "Kolejna tura", 0));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
