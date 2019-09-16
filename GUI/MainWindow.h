//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_MAINWINDOW_H
#define TEXTUREATLASCREATOR_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

namespace GUI
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = nullptr);

        private:
            Ui::MainWindow *ui = nullptr;
    };
}

#endif //TEXTUREATLASCREATOR_MAINWINDOW_H
