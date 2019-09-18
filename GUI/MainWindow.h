//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_MAINWINDOW_H
#define TEXTUREATLASCREATOR_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <memory>

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = nullptr);
            ~MainWindow();
        private:
            Ui::MainWindow *ui = nullptr;
            std::unique_ptr<::TextureLogic::TextureBank> textureBank;
    };
}

#endif //TEXTUREATLASCREATOR_MAINWINDOW_H
