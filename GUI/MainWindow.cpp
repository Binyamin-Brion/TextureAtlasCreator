//
// Created by BinyBrion on 2019-09-15.
//

#include "MainWindow.h"

namespace GUI
{
    MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent}, ui{new Ui::MainWindow}
    {
        ui->setupUi(this);
    }
}