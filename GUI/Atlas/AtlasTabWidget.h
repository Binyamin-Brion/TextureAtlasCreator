//
// Created by BinyBrion on 2019-09-16.
//

#ifndef TEXTUREATLASCREATOR_ATLASTABWIDGET_H
#define TEXTUREATLASCREATOR_ATLASTABWIDGET_H

#include <QtWidgets/QTabWidget>
#include "HelperUtilities/PassKey.h"
#include "Atlas/TextureAtlas.h"
#include "Atlas/AtlasInformationBundle.h"

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Dialogs
    {
        class AddNewTab;
        class AddNewAtlasTab;
        class ResizeAtlas;
    }

    namespace Atlas
    {
        class AtlasTabOptionsMenu;
        class ScrollArea;

        /**
         *  Holds all of the ScrollAreas (that hold an AtlasWidget, which holds a TextureAtlas).
         *  It is a central place for managing all of the ScrollAreas, in the sense that if an action
         *  is requested for a texture atlas, it is applied to the current texture atlas being shown,
         *  which is the current tab being shown.
         */

        class AtlasTabWidget : public QTabWidget
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when the currently viewed atlas has had its dimensions change.
                 */
                void atlasDimensionChanged(::Atlas::AtlasInformationBundle);

                /**
                 * Emitted when a tab holding a different atlas is clicked, showing a new atlas.
                 * Statistics displayed about the currently displayed atlas changes and this has to be reflected in the GUI.
                 */
                void currentAtlasInformationChanged(::Atlas::AtlasInformationBundle);

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object
                 */
                explicit AtlasTabWidget(QWidget *parent = nullptr);

                /**
                 * Adds a texture to the atlas currently being displayed.
                 *
                 * @param texture to add to the current atlas
                 * @param tabName the tab holding the atlas to which to add the texture
                 * @param position the position of the texture within the atlas (QSize == {-1, -1} if no position is known)
                 */
                void addTextureToAtlas(const TextureLogic::Texture &texture, QString atlasName, QPoint position);

                /**
                 *  Goes through every atlas and closes it. Any unsaved changes will be lost, hence a warning will appear
                 *  if there are modified atlases that are not saved.
                 */
                void closeAllTabs();

                /**
                 * Get the format of the current atlas being displayed.
                 *
                 * @return format of the current atlas
                 */
                QImage::Format getCurrentAtlasFormat() const;

                /**
                 * Gets the information about the currently viewed texture atlas.
                 *
                 * @return information about the currently viewed atlas
                 */
                ::Atlas::AtlasInformationBundle getCurrentAtlasInformation() const;

                /**
                 * Gets the name of current tab being shown.
                 *
                 * @return name of tab displaying the currently viewed atlas
                 */
                QString getCurrentAtlasName() const;

                /**
                 * Checks if there are any unsaved changes in the atlases.
                 *
                 * @return true if there are unsaved changes
                 */
                bool getUnsavedAtlases() const;

                /**
                 * Removes the passed in textures from ALL texture atlases, regardless if an atlas is currently
                 * being displayed.
                 *
                 * @param texture to remove from all atlases
                 */
                void removeTexture(const TextureLogic::Texture *texture);

                /**
                 *  Saves all of the atlases to the specified location.
                 *
                 *  @param saveLocation location that the atlases should be saved to
                 */
                void saveAtlases(const QString &saveLocation) const;

                /**
                 * Checks the new intersection width for ALL textures atlases to see if it is valid (no intersection with other textures,
                 * regardless if an atlas is currently being displayed.
                 *
                 * @param texture whose intersection width has been resized
                 * @return true if the operation was NOT successful
                 */
                bool setIntersectionWidth(TextureLogic::Texture *texture);

                /**
                 * Sets the reference to the texture bank.
                 *
                 * @param textureBank reference where all textures that have been loaded are stored for the program
                 */
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);

                /**
                 *  Call when a new texture has been loaded from the file system. When this happens, references
                 *  to textures within textures may be invalidated; calling this function fixes that potential issue.
                 */
                void updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>);

            public slots:

                /**
                 *  Adds a new tab containing an empty atlas to this tab widget.
                 *
                 * @param tabName the name of the tab containing the new empty atlas
                 * @param atlasSize the size of the new atlas
                 * @param atlasFormat the format the atlas, and thus the textures it contains, should have
                 */
                void addAtlasWidget(const QString &tabName, QSize atlasSize, QImage::Format atlasFormat);

                /**
                 *  Call this function when an atlas should be exported. This will start the sequence of exporting
                 *  the currently viewed atlas.
                 */
                void exportTexture();

            private slots:

                /**
                 *  Handles when a different tab holding a different tab is selected.
                 *  When this happens, the statistics about the current atlas, such as percentage used, has to be changed.
                 *
                 * @param index the position of the current tab
                 */
                void currentTabChanged(int index);

                /**
                 *  Removes and deletes a tab along with the atlas inside. After this operation, the atlas within
                 *  the tab is lost permanently unless it was saved. The textures that were used in the atlas however,
                 *  remain in the program and can continue to be used.
                 */
                void deleteCurrentTab();

                /**
                 *  Moves the current tab to the left in the list of open tabs.
                 *  Internal note: See AtlasTabWidget.cpp
                 */
                void moveTabLeft();

                /**
                 *  Moves the current tab to the right in the list of open tabs.
                 *  Internal note: See AtlasTabWidget.cpp
                 */
                void moveTabRight();

                /**
                 * Renames the currently viewed tab. The new tab name chosen must be unique- a different tab cannot
                 * have the same name as what was chosen for the current tab.
                 *
                 * @param newTabName string that the currently viewed tab should have as its name
                 */
                void newTabNameChosen(const QString &newTabName);

                /**
                 * Repaints the current texture selected in the atlas displayed. This is required when the selected texture
                 * is painted in the CurrentTexture tab. Modifications to the texture are made in the program memory, but
                 * Qt is not aware of these changes. Thus notifying Qt is required in ordered to display these changes.
                 */
                void repaintSelectedTexture();

                /**
                 *  Shows the dialog to create a new texture atlas in a new tab.
                 */
                void showAddNewAtlasTab();

                /**
                 * Shows the context menu when the user right clicks the current atlas.
                 *
                 * @param pos the position of the cursor when the right-click event occurred
                 */
                void showContextMenu(const QPoint &pos);

                /**
                 *  Shows the dialog to rename the currently displayed atlas tab.
                 */
                void showRenameTabDialog();

                /**
                 * Shows the dialog to specify a new size for the atlas, and then requests the atlas to change its size
                 * if the user entered valid input.
                 */
                void showResizeAtlasDialog();

            private:

                // See internal note for the variable currentTabs in AtlasTabWidget.cpp
                std::vector<std::pair<ScrollArea*, QString>> currentTabs;

                // Needed to pass to atlas created so that they can notify the textureBank of when a texture is selected
                TextureLogic::TextureBank *textureBank = nullptr;

                // These variables are instantiated and have the lifetime of this object,
                // but are only shown when the appropriate events occur
                AtlasTabOptionsMenu *atlasTabOptionsMenu = nullptr;
                Dialogs::AddNewTab *renameTab = nullptr;
                Dialogs::AddNewAtlasTab *addNewAtlasTab = nullptr;
                Dialogs::ResizeAtlas *resizeAtlas = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABWIDGET_H
