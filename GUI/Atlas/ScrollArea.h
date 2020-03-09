//
// Created by BinyBrion on 2019-09-17.
//

#ifndef TEXTUREATLASCREATOR_ATLAS_SCROLLAREA_H
#define TEXTUREATLASCREATOR_ATLAS_SCROLLAREA_H

#include <QtWidgets/QScrollArea>

#include "Atlas/AtlasInformationBundle.h"

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace Atlas
    {
        class AtlasWidget;

        /**
         *  Provides a scrollable window to view the Atlas widget. Since the atlas widget may be bigger than the
         *  size of the area reserved for it, a scroll view is required to view all of the atlas.
         *
         *  This class, beyond holding an atlas widget to provide with scrolling capabilities, simply forwards
         *  arguments to the atlas widget that are not events. Even if this is not explicitly stated in the function prototype documentation.
         */

        class ScrollArea : public QScrollArea
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when a tab holding a different atlas is clicked, showing a new atlas.
                 * Statistics displayed about the currently displayed atlas changes and this has to be reflected in the GUI.
                 */
                void currentAtlasInformationChanged(::Atlas::AtlasInformationBundle);

            public:

                /**
                 *  Constructor that initializes all required internal variables, and initializes this object with
                 *  the parent passed in.
                 *
                 *  @param atlasSize the size of the atlas to pass to the atlas widget
                 *  @param atlasFormat the format of the atlas to pass to the atlas widget
                 *  @param parent widget that has ownership over this object.
                 */
                ScrollArea(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent = nullptr);

                /**
                 * Add a texture to the current atlas. Forwards the argument to the atlas widget held by this object.
                 *
                 * @param texture to be added to the atlas
                 */
                void addTexture(const TextureLogic::Texture &texture);

                /**
                 * When the cursor enters this widget, it automatically grabs the focus of the keyboard so that
                 * zoom shortcuts are received and processed as expected
                 *
                 * @param event unused
                 */
                void enterEvent(QEvent *event) override;

                /**
                 *  Forward the command to export the atlas.
                 */
                void exportTexture();

                /**
                 * Get the format of the atlas held within this scroll area.
                 *
                 * @return format of the atlas currently being displayed
                 */
                QImage::Format getAtlasFormat() const;

                /**
                 * Gets the statistics of the atlas held within this scroll area.
                 *
                 * @return information about the atlas currently being displayed
                 */
                ::Atlas::AtlasInformationBundle getAtlasInformation() const;

                /**
                 * Checks if there are any unsaved changes in the atlas.
                 *
                 * @return true if there are unsaved changes
                 */
                bool getUnsavedChanges() const;

                /**
                 * Handles key presses made to this widget.
                 *
                 * @param event the key that was pressed
                 */
                void keyPressEvent(QKeyEvent *event) override;

                /**
                 * Handles key releases made to this widget.
                 *
                 * @param event the key that was released
                 */
                void keyReleaseEvent(QKeyEvent *event) override;

                /**
                 * When the cursor leaves this widget, it releases the focus of the keyboard so that other widgets
                 * are able to handle keyboard input directed to them.
                 *
                 * @param event unused
                 */
                void leaveEvent(QEvent *event) override;

                /**
                 * Forwards request to save the atlas to the widget holding the atlas widget.
                 *
                 * @param atlasName name of the atlas being saved
                 * @param saveLocation location to save the atlas to
                 */
                void saveAtlas(const QString &atlasName, const QString &saveLocation) const;

                /**
                 * Handles scroll events made to this widget. Ensures that if a scroll event is received, it is not
                 * processed if the control key is pressed, which indicates that the scroll event is intened to change
                 * zoom, not to change the area of the atlas being viewed.
                 *
                 * @param dx unused
                 * @param dy unused
                 */
                void scrollContentsBy(int dx, int dy) override;

                /**
                 * Removes the texture passed in from the atlas. Forwards argument to atlas widget.
                 *
                 * @param texture to remove from the currently viewed atlas
                 */
                void removeTexture(const TextureLogic::Texture *texture);

                /**
                 * Notifies the atlas widget to update its contents; ie show changes made to the texture atlas.
                 * Called when a texture is modified in the CurrentTexture portion of the program.
                 */
                void repaintSelectedTexture();

                /**
                 * Notifies the atlas widget that the area in which it is being viewed has changed.
                 * See AtlasWidget.cpp for why this function needs to be called.
                 *
                 * @param event contains the new dimensions of the viewport for this scroll area
                 */
                void resizeEvent(QResizeEvent *event) override;

                /**
                 * Forwards request to atlas widget to check if the new intersection width given to the passed in
                 * texture does not cause intersections with other textures.
                 *
                 * @param texture whose intersection width has been changed and is being checked if it is valid
                 * @return true if the operation was NOT successful
                 */
                bool newIntersectionBorderWidthValid(TextureLogic::Texture *texture);

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
                void updateTextureReferences();

                /**
                 * Handles wheel events from the user mouse. If the control key is down, then this function tells
                 * the atlas widget to zoom in or out on the atlas, depending on the direction of the scroll. Otherwise
                 * the area of the atlas that is viewed changes.
                 *
                 * @param event
                 */
                void wheelEvent(QWheelEvent *event) override;

            private slots:

                /**
                 *  Called when the atlas widget needs to zoom in on the atlas it is holding.
                 */
                void zoomIn();

                /**
                 *  Called when the atlas widget needs to zoom out on the atlas it is holding.
                 */
                void zoomOut();

            private:
                // Reference to the widget that this scroll area holds
                AtlasWidget *atlasWidget = nullptr;
                // Keeps track if the user is holding down the control key on their keyboard
                bool controlKeyDown = false;
        };
    }
}


#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
