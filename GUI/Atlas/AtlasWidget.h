//
// Created by BinyBrion on 2019-09-17.
//

#ifndef TEXTUREATLASCREATOR_ATLASWIDGET_H
#define TEXTUREATLASCREATOR_ATLASWIDGET_H

#include <QtWidgets/QWidget>
#include <memory>

#include "Atlas/AtlasInformationBundle.h"

namespace Atlas
{
    class TextureAtlas;
}

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace Atlas
    {
        /**
         *  The Widget onto which the texture atlas is painted, and widget that stores the texture atlas
         *  itself. It forwards all necessary arguments to the texture atlas.
         */

        class AtlasWidget : public QWidget
        {
                Q_OBJECT

            signals:
                void currentAtlasInformationChanged(::Atlas::AtlasInformationBundle);

            public:

                /**
                 * Constructor that initializes all required internal variables, and initializes this object with
                 *  the parent passed in.
                 *
                 * @param atlasSize the size of the atlas to be created
                 * @param atlasFormat the format of the atlas to be created
                 * @param parent parent widget that has ownership over this object.
                 */
                explicit AtlasWidget(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent = nullptr);

                /**
                 * Determines if a given texture cannot be displayed entirely within the viewport.
                 *
                 * @param textureSize size of texture to check if it fits in the viewport entirely
                 * @return true if the texture can NOT be viewed entirely within the viewport
                 */
                bool checkTextureNotWithinViewPort(QSize textureSize);

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
                 *  Forward the request to the texture atlas to export itself.
                 */
                void exportTexture();

                /**
                 *  Handles key events.
                 *  Note: the scroll area holding this widget accepts key events, rather than this widget. As a result,
                 *        key events are forwarded to this widget from the scroll area after the scroll area is done
                 *        executing its required tasks in the event of a key event.
                 *
                 * @param event the key that was pressed
                 */
                void keyPressed(QKeyEvent *event);

                /**
                 * Handles mouse clicks.
                 *
                 * @param event the mouse button that was clicked
                 */
                void mouseMoveEvent(QMouseEvent *event) override;

                /**
                 * Handles mouse movement.
                 *
                 * @param event the mouse position as a result if the cursor moving
                 */
                void mousePressEvent(QMouseEvent *event) override;

                /**
                 * Handles mouse button releases.
                 *
                 * @param event the mouse button that was released
                 */
                void mouseReleaseEvent(QMouseEvent *event) override;

                /**
                 * Moves the mouse to a certain point relative to this widget.
                 *
                 * @param x the point where the cursor should be placed along the x-dimension
                 * @param y the point where the cursor should be placed along the y-dimension
                 */
                void moveMouseTo(int x, int y);

                /**
                 * Handles requests to paint the texture atlas onto widget, effectively showing the user the atlas onscreen.
                 *
                 * @param event unused
                 */
                void paintEvent(QPaintEvent *event) override;

                /**
                 * Forwards the request to remove a texture to the texture atlas.
                 *
                 * @param texture to remove from the atlas
                 */
                void removeTexture(const TextureLogic::Texture *texture);

                /**
                 * Resizes this widget by the given factor. This effectively changes the dimensions of the atlas that
                 * this object contains. It is used when the user changes the zoom.
                 *
                 * @param factor the ratio between the previous size and the new size
                 */
                void resizeAtlasFactor(float factor);

                /**
                 * Resizes this widget which also resizes the texture atlas that this object holds. This takes in an
                 * absolute dimension value, unlike resizeAtlasFactor(), and is only called by the Qt framework; it is
                 * not intended to be called directly.
                 *
                 * @param event
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
                 * Updates the viewport as a result of the user resizing the scroll area through which this widget
                 * is viewed.
                 *
                 * @param viewPort
                 */
                void setViewPort(QSize viewPort);

                /**
                 * Sets the x-offset of the viewport. This value changes whenever the horizontal scroll bar of the
                 * scroll area that contains this widget is moved.
                 *
                 * @param value the distance the horizontal scroll bar is shifted to the right. This value comes automatically
                 *        from the signal the horizontal scroll bar emits when it is moved
                 */
                void setViewPortOffsetX(int value);

                /**
                 * Sets the y-offset of the viewport. This value changes whenever the vertical scroll bar of the
                 * scroll area that contains this widget is moved.
                 *
                 * @param value the distance the vertical scroll bar is shifted to the right. This value comes automatically
                 *        from the signal the vertical scroll bar emits when it is moved
                 */
                void setViewPortOffsetY(int value);

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
                 * Tells the texture atlas to zoom in, and display the visual changes that occur as a result.
                 */
                void zoomIn();

                /**
                 * Tells the texture atlas to zoom out, and display the visual changes that occur as a result.
                 */
                void zoomOut();

            public slots:

                /**
                 * Called when the user selects a texture to be placed in the texture atlas.
                 * If the user is trying to add a texture to the atlas that was already added, a message is given to
                 * the user the operation is aborted.
                 *
                 * @param texture to be added to the atlas
                 */
                void textureButtonPressed(const TextureLogic::Texture &texture);

            private:

                /**
                 * Ensures that when the user is dragging a texture, they cannot move the cursor outside of the viewport,
                 * effectively helping to lock the cursor to the part of the widget that is visible.
                 *
                 * Internal note: See AtlasWidget.cpp for more details.
                 *
                 * @param mouseX
                 * @param mouseY
                 * @return
                 */
                bool lockCursorToViewPort(int &mouseX, int &mouseY);

                // Note that once this widget is deleted, the texture atlas should be deleted as well as it is tied
                // to the lifetime of this object
                std::unique_ptr<::Atlas::TextureAtlas> textureAtlas;

                // Used to determine if the texture is being moved off of the widget
                QSize viewPort;
                QPoint viewPortOffset;

                // Used to revert cursor back to valid position if user moves cursor to invalid position
                QPoint previousMouseCoords;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASWIDGET_H
