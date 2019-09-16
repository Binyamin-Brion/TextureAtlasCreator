//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QShortcut>
#include "MainWindow.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow}
    {
        ui->setupUi(this);

        new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), ui->tabWidget_2, SLOT(showLoadTextureDialog()));
    }

}