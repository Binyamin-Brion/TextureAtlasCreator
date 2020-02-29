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
    /**
     * The main window for this program. Holds all of the other widgets and their resources.
     * Also sets up the connections between those widgets.
     */

    class MainWindow : public QMainWindow
    {
            Q_OBJECT

        public:

            /**
             * Initializes this object with the parent passed in.
             *
             * @param parent widget that has ownership over this object.
             */
            explicit MainWindow(QWidget *parent = nullptr);

            /**
             * See internal note at MainWindow.cpp.
             */
            ~MainWindow();
        private:
            Ui::MainWindow *ui = nullptr;
            std::unique_ptr<::TextureLogic::TextureBank> textureBank;
    };
}

#endif //TEXTUREATLASCREATOR_MAINWINDOW_H
