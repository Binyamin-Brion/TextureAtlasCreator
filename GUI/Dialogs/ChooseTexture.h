//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_CHOOSETEXTURE_H
#define TEXTUREATLASCREATOR_CHOOSETEXTURE_H

#include "ui_chooseTexture.h"

namespace GUI
{
    namespace Dialogs
    {
        /**
         * Provides a pop-up dialog to specify the required information to load a new texture.
         */

        class ChooseTexture : public QDialog
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when the user confirms the selection of a texture from the file system.
                 *
                 *  The parameters are:
                 *   1. Name of the tab holding the texture button area to place the texture button associated with the chosen texture
                 *   2. Location to the texture chosen on the file system
                 *   3. Intersection width for the texture chosen
                 *   4. Selection width for the texture chosen
                 *   5. true if loading a project. False in this case.
                 */
                void textureChosen(QString, QString, unsigned int, unsigned int, bool);

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit ChooseTexture(QWidget *parent = nullptr);

                /**
                 *  Add to the list of tabs that the texture can be loaded into.
                 *  Note that each texture is displayed in a tab in the loaded texture sections.
                 *
                 * @param tabName name of the tab that a texture can be loaded into
                 */
                void addTab(const QString &tabName);

                /**
                 * Handles the dialog being closed
                 *
                 * @param event unused
                 */
                void closeEvent(QCloseEvent *event) override;

                void removeTab(const QString &tabName);

            private slots:

                /**
                 *  Opens a dialog to allow the user to pick a image from the file system, and updates the dialog as required.
                 */
                void fileLocationButtonClicked();

                /**
                 *  Checks to make sure that the intersection width written by the user is valid and updates the dialog as required.
                 */
                void intersectionWidthTextChanged(const QString&);

                /**
                 *  Checks to make sure that the selection width written by the user is valid and updates the dialog as required.
                 */
                void selectionWidthTextChanged(const QString&);

            private:

                /**
                 * Handles a QLineEdit in which the user can modify either the intersection or the selection width.
                 * Determines if the input is a valid number.
                 *
                 * @param lineEdit
                 * @param dimension
                 */
                void borderWidthSpecified(QLineEdit *lineEdit, int &dimension);

                /**
                 *  Checks that all of the information required to create a texture atlas is valid.
                 */
                void checkValidData();

                /**
                * Resets this dialog to the default state so that no nothing of what was previously entered is shown.
                */
                void resetDialog();

                /**
                 * Controls whether a message displayed telling the user that they have to enter valid location
                 * has a line through it or not; if there is a line through it, that means the user has complied with
                 * the requirements stated by the message.
                 *
                 * @param value true if the message should have a line through it
                 */
                void setStrikeOutErrorLabel(bool value);

                // Holds the requested intersection border width for the loaded texture
                int intersectionBorderWidth;

                // Holds the requested selection border width for the loaded texture
                int selectionBorderWidth;

                // Pointer to the UI form
                Ui::TextureChooser *ui = nullptr;

                // Keeps track if the user selected a path to a texture
                bool validTexturePathSelected = false;

                std::vector<QString> tabNames;
        };
    }
}

#endif //TEXTUREATLASCREATOR_CHOOSETEXTURE_H
