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

        private slots:

            /**
             *  Called when user requests to create a new project. This will close the previous project.
             */
            void newProject();

            /**
             * Called when user requests to open a new project. This will close the previous project.
             */
            void openProject();

            /**
             * Called when the user wants to save project to a new location.
             */
            void saveAsProject();

            /**
             * Called when the user wants to save to the previously saved-to destination. If the user has not saved this
             * project once before, the saveAsProject() function will be called.
             */
            void saveProject();

            void showPercentageUsed(::Atlas::AtlasInformationBundle atlasInformationBundle);

        private:

            /**
             * Closes the currently opened project. Any unsaved changes will be ask to be saved first.
             */
            void closeProject();

            /**
             * Get a path to the texture being loaded from a project file. Abstracts difference between behaviour
             * when debugging and running release-mode program.
             *
             * @param loadedPath to the texture as specified in the file
             * @return texture location to be used when loading a project
             */
            QString createPathToTexture(QString loadedPath);

            Ui::MainWindow *ui = nullptr;
            std::unique_ptr<::TextureLogic::TextureBank> textureBank;
            QString previousSaveLocation;
            const QString projectExtension = ".tac";
    };
}

#endif //TEXTUREATLASCREATOR_MAINWINDOW_H
