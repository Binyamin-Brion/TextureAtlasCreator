//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H
#define TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H

#include <QtWidgets/QWidget>

class QGridLayout;

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace LoadResults
    {
        class OptionsMenu;
        class TextureButton;

        /**
         * The widget embedded in the scroll area held within the LoadTextures class.
         * It stores all the texture buttons (see the TextureButton class) for the current
         * tab in the LoadedTexture widget.
         */

        class TextureButtonArea : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Called when the user requests to move this tab to the left when it is clicked. Forwards signal from the OptionsMenu.
                 */
                void moveTabLeftTriggered();

                /**
                 * Called when the user requests to move this tab to the right when it is clicked. Forwards signal from the OptionsMenu.
                 */
                void moveTabRightTriggered();

                /**
                 * Called when the user requests to rename this tab. Forwards signal from the OptionsMenu.
                 */
                void renameTabRequest();

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object
                 */
                explicit TextureButtonArea(QWidget *parent = nullptr);

                /**
                 * Add a new texture button to this texture button area. If a button representing the same texture
                 * already exists, a notification is given to the user and this function is aborted.
                 *
                 * @param textureLocation location of the texture this button represents
                 * @param intersectionBorderWidth specified intersection width in the texture loading dialog
                 * @param selectionBorderWidth specified selection width in the texture loading dialog
                 */
                void addTextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);

                /**
                 *  This does not directly delete texture buttons; when this area is deleted all of the buttons are automatically deleted.
                 *  What this function will do is delete all of the textures in the texture bank that the buttons in this object represent.
                 *
                 * @param whether or not the signal is being emitted as a result of the current project closing
                 */
                void deleteTextureButtons(bool closingProgram);

                /**
                 * Checks if there are any unsaved changes in the atlas.
                 *
                 * @return true if there are unsaved changes
                 */
                bool getUnsavedChanges() const;

                /**
                 * Handles cursor input.
                 *
                 * @param event unused
                 */
                void mousePressEvent(QMouseEvent *event) override;

                void paintTextureButton(const ::TextureLogic::Texture *texture);

                /**
                 * Saves the location of the loaded textures to the specified location.
                 *
                 * @param textureButtonAreaName name of the texture button area holding the texture buttons
                 * @param saveLocation location to save the texture locations to
                 */
                void saveLoadedTextures(const QString &textureButtonAreaName, const QString &saveLocation) const;

                /**
                 * Set the reference to the texture bank, so that the texture bank can be notified of loading new textures
                 * or marking a texture as selected.
                 *
                 * @param textureBank reference to the object that stores all textures
                 */
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

                /**
                 * Forwards request to all texture button areas that represent the modified texture to now represent
                 * the saved modified texture.
                 *
                 * @param previousLocation location on the file system that the texture buttons had before the modified texture was saved
                 * @param newLocation new location on the file system of the saved modified texture
                 */
                void updateTextureButtonsLocation(const QString &previousLocation, const QString &newLocation);

            private slots:

                /**
                 *  Deletes the texture button that the cursor is currently located on. A warning is given to the user
                 *  before this operation is done. If the cursor is not located on a button, the context menu will not
                 *  provide an option to delete a button.
                 */
                void deleteTextureButton();

                /**
                 * Shows the context menu when the user right clicks the current atlas.
                 *
                 * @param pos the position of the cursor when the right-click event occurred
                 */
                void showContextMenu(const QPoint &pos);

                /**
                 * Handles a texture button being clicked.
                 *
                 * @param textureLocation texture that the clicked button represents
                 * @param intersectionBorderWidth of the texture the clicked button represents
                 * @param selectionBorderWidth of the texture the clicked button represents
                 */
                void textureButtonClicked(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);

            private:

                /**
                 * Reserve additional space to add more texture buttons. Used as part of logic to ensure texture buttons
                 * are placed in desired locations/
                 *
                 * @param addRows number of new rows to add
                 */
                void addTextureButtonPlaceHolders(int addRows);

                /**
                 * Create the default layout for the texture buttons to be placed.
                 */
                void createLayout();

                /**
                 * Add a texture button to the next available location in the layout.
                 *
                 * @param button to add to this button area
                 */
                void placeTextureButton(TextureButton *button);

                std::vector<TextureButton*> textureButtons;
                std::vector<QWidget*> textureButtonPlaceHolders; // See TextureButtonArea.cpp for explanation of this
                TextureLogic::TextureBank *textureBank = nullptr;

                OptionsMenu *optionsMenu = nullptr;

                // Keep track of the next position to place a new button
                int currentRow = 0;
                int currentColumn = 0;

                // Keep track of when to reserve more space for additional buttons
                int maxRowCount = 0;
                int maxColumnCount; // Calculated at run time

                QGridLayout *gridLayout = nullptr;

                int gridHorizontalSpacing; // Calculated at run time

                // Keeps track of what button the cursor is located on at the time of cursor receiving a right-click
                // while it is over this button area
                int cursorOverButtonIndex = -1;

                mutable bool unsavedChanges = false;
        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTONAREA_H
