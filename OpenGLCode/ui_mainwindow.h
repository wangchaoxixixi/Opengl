/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSlider *brightness_Slider;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QSlider *contrast_Slider;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QSlider *saturation_Slider;
    QHBoxLayout *horizontalLayout;
    QCheckBox *isSepia_checkBox;
    QSlider *sepia_Slider;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLabel *zoom_label;
    QCheckBox *sphere_checkBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1040, 682);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMinimumSize(QSize(0, 0));
        scrollArea->setMaximumSize(QSize(16777215, 16777215));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 819, 605));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_2->addWidget(scrollArea, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1040, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(200, 625));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout_6 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(dockWidgetContents_2);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        brightness_Slider = new QSlider(dockWidgetContents_2);
        brightness_Slider->setObjectName(QStringLiteral("brightness_Slider"));
        brightness_Slider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(brightness_Slider);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(dockWidgetContents_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        contrast_Slider = new QSlider(dockWidgetContents_2);
        contrast_Slider->setObjectName(QStringLiteral("contrast_Slider"));
        contrast_Slider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(contrast_Slider);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_3 = new QLabel(dockWidgetContents_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_4->addWidget(label_3);

        saturation_Slider = new QSlider(dockWidgetContents_2);
        saturation_Slider->setObjectName(QStringLiteral("saturation_Slider"));
        saturation_Slider->setOrientation(Qt::Horizontal);

        verticalLayout_4->addWidget(saturation_Slider);


        verticalLayout_3->addLayout(verticalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        isSepia_checkBox = new QCheckBox(dockWidgetContents_2);
        isSepia_checkBox->setObjectName(QStringLiteral("isSepia_checkBox"));

        horizontalLayout->addWidget(isSepia_checkBox);

        sepia_Slider = new QSlider(dockWidgetContents_2);
        sepia_Slider->setObjectName(QStringLiteral("sepia_Slider"));
        sepia_Slider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(sepia_Slider);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_6->addLayout(verticalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_4 = new QLabel(dockWidgetContents_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        zoom_label = new QLabel(dockWidgetContents_2);
        zoom_label->setObjectName(QStringLiteral("zoom_label"));

        horizontalLayout_2->addWidget(zoom_label);


        verticalLayout_6->addLayout(horizontalLayout_2);

        sphere_checkBox = new QCheckBox(dockWidgetContents_2);
        sphere_checkBox->setObjectName(QStringLiteral("sphere_checkBox"));

        verticalLayout_6->addWidget(sphere_checkBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);

        dockWidget->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "\344\272\256\345\272\246\357\274\232", 0));
        label_2->setText(QApplication::translate("MainWindow", "\345\257\271\346\257\224\345\272\246\357\274\232", 0));
        label_3->setText(QApplication::translate("MainWindow", "\351\245\261\345\222\214\345\272\246\357\274\232", 0));
        isSepia_checkBox->setText(QApplication::translate("MainWindow", "\350\244\220\350\211\262", 0));
        label_4->setText(QApplication::translate("MainWindow", "zoom:", 0));
        zoom_label->setText(QString());
        sphere_checkBox->setText(QApplication::translate("MainWindow", "sphere", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
