//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_MAINWINDOW_H
#define TEXTUREATLASCREATOR_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "Atlas/AtlasTabWidget.h"
#include <memory>

namespace TextureLogic
{
    class TextureBank;
}

namespace Ui
{
    class MainWindow;
}

namespace GUI
{
    class NameForm;

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
             * Asks the user to save if there are any unsaved changes before closing the program.
             *
             * @param event unused
             */
            void closeEvent(QCloseEvent *event) override;

            /**
             * Sets the dark theme for this program. Call once at program start up.
             */
            void setTheme();

            /**
             * See internal note at MainWindow.cpp.
             */
            ~MainWindow();

        private slots:

            /**
             *  Initialize the actions in the Atlas submenu with the appropriate connections.
             */
            void initializeAtlasActionsConnections();

            /**
             * Initialize the actions in the File submenu with the appropriate connections.
             */
            void initializeFileConnections();

            /**
             *  Initialize keyboard shortcuts with the appropriate connections.
             */
            void initializeKeyboardShortcutsConnections();

            /**
             * Initialize the actions in the 'Loaded Texture' submenu with the appropriate connections.
             */
            void initializeTextureButtonAreaActionsConnections();

            /**
             *  Initialize the connections between the UI widgets.
             */
            void initializeWidgetConnections();

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

            /**
             *  Updates widgets that display information about the current texture atlas.
             *
             * @param atlasInformationBundle information used to update the widgets showing texture statistics
             */
            void showPercentageUsed(::Atlas::AtlasInformationBundle atlasInformationBundle);

        private:

            /**
             * Closes the currently opened project. Any unsaved changes will be ask to be saved first.
             */
            void closeProject();

            Ui::MainWindow *ui = nullptr;
            NameForm *nameFormUi = nullptr;
            std::unique_ptr<::TextureLogic::TextureBank> textureBank;
            QString previousSaveLocation;
            QString projectName;
            const QString projectExtension = ".tac";
    };
}

#endif //TEXTUREATLASCREATOR_MAINWINDOW_H
