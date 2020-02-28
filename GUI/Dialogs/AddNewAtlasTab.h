//
// Created by BinyBrion on 10/11/2019.
//

#ifndef TEXTUREATLASCREATOR_ADDNEWATLASTAB_H
#define TEXTUREATLASCREATOR_ADDNEWATLASTAB_H

#include <QtWidgets/QDialog>
#include "GUI/TextureHelperFunctions/TextureFormats.h"

class QLineEdit;

namespace Ui
{
    class NewTextureAtlasDialog;
}

namespace GUI
{
    namespace Dialogs
    {
        /**
         * Provides a pop-up dialog to specify the required information to create a new tab in the AtlasTabWidget,
         * as well as the information to create a new TextureAtlas as a result of adding a new tab to that widget
         */

        class AddNewAtlasTab : public QDialog
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when all of the required information to create a new texture atlas has been specified.
                 */
                void newAtlasInformationSpecified(QString, QSize, QImage::Format);

            public:

                /**
                * Initializes this object with the parent passed in.
                *
                * @param parent widget that has ownership over this object.
                */
                explicit AddNewAtlasTab(QWidget *parent = nullptr);

                /**
                 * Called if a tab is created outside of this dialog, and the name used for that tab has to be registered
                 * with this class to ensure that name remains unique.
                 *
                 * Tabs can be created outside of this dialog during a new project when a default tab is created or
                 * an existing project with tabs is loaded.
                 *
                 * @param tabName name of the tab added outside of this dialog
                 */
                void addExistingTabName(QString tabName);

                /**
                 * Called if a tab is deleted. When that happens, that tab name is now free to use for another tab.
                 * This dialog must be notifited that name is now free to use.
                 *
                 * @param tabName name of tab that was deleted
                 */
                void removeNameExistingTab(QString tabName);

            private slots:

                /**
                 * Checks if the tab name entered is valid or not. If the tab name is not valid, the line edit
                 * where the name is being entered will turn a shade of red.
                 *
                 * @param tabName to check if it is valid
                 */
                void checkTabNameValid(const QString &tabName);

                /**
                 *  Checks if the texture atlas height is valid or not. If the entered dimension is not valid, the line
                 *  edit where the dimension is being entered will turn a shade of red.
                 */
                void handleAtlasHeightSpecified();

                /**
                 *  Checks if the texture atlas height is valid or not. If the entered dimension is not valid, the line
                 *  edit where the dimension is being entered will turn a shade of red.
                 */
                void handleAtlasWidthSpecified();

                /**
                 *  Tells the rest of the program to create a new texture atlas with the information specified in the
                 *  dialog. Afterwards, it makes sure the dialog is reset to the default state.
                 */
                void textureAtlasInformationConfirmed();

            private:

                /**
                 * Checks if the the dimensions for the new texture atlas are valid.
                 *
                 * @param lineEdit containing the dimension of the new texture atlas
                 * @param dimension variable used to store if a valid number is contained in the passed in QLineEdit
                 */
                void atlasDimensionSpecified(QLineEdit *lineEdit, int &dimension);

                /**
                 *  Checks that all of the information required to create a texture atlas is valid.
                 */
                void checkAllInformationValid();

                /**
                 * Resets this dialog to the default state so that no nothing of what was previously entered is shown.
                 */
                void resetDialog();

                // Pointer to the form for this dialog
                Ui::NewTextureAtlasDialog *ui = nullptr;

                std::vector<QString> existingTabNames;

                // Keep track of the new texture atlas dimensions
                int requestedHeight = -1;
                int requestedWidth = -1;

                // Convenience variable to display valid formats for a texture atlas
                std::vector<::GUI::TextureHelperFunctions::InternalFormatPair> internalFormatPairs;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ADDNEWATLASTAB_H
