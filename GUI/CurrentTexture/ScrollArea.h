//
// Created by BinyBrion on 10/2/2019.
//

#ifndef TEXTUREATLASCREATOR_PAINT_AREA_SCROLLAREA_H
#define TEXTUREATLASCREATOR_PAINT_AREA_SCROLLAREA_H

#include <QtWidgets/QScrollArea>
#include "TextureLogic/Zoom.h"

namespace PaintFunctions
{
    class Brush;
}

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace CurrentTexture
    {
        class PaintArea;

        /**
         *  Provides a scrollable window to view the RenderArea. Since the RenderArea scales with the size of the
         *  texture selected in the texture atlas, the RenderArea may be bigger than the size of the area reserved for it.
         *
         *  This class, beyond holding an render area widget to provide with scrolling capabilities, simply forwards
         *  arguments to the render area. that are not events Even if this is not explicitly stated in the function prototype documentation.
         */

        class ScrollArea : public QScrollArea
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when the user is done with a paint operation and the text in the GUI information panel
                 * about the currently selected texture needs to be modified.
                 */
                void paintedSelectedTexture();

                /**
                 *  Emitted when the selected texture has been modified with a brush.
                 */
                void repaintSelectedTexture();

                /**
                 *  Emitted when the zoom changes. Emits the new zoom level.
                 */
                void zoomChanged(::TextureLogic::Zoom);

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit ScrollArea(QWidget *parent = nullptr);

                /**
                 *  Notifies this object that unsaved changes have been saved.
                 */
                void changesSaved();

                /**
                 * When the cursor enters this widget, it automatically grabs the focus of the keyboard so that
                 * zoom shortcuts are received and processed as expected
                 *
                 * @param event unused
                 */
                void enterEvent(QEvent *event) override;

                /**
                * Get the brush that is currently being used for painting.
                *
                * @return brush being used to paint the selected texture
                */
                PaintFunctions::Brush& getBrush() const;

                /**
                 * Get the format of the current image that can be painted.
                 *
                 * @return format of the selected image. Note that if no image is selected, then an Qt::Invalid_Format is returned
                 */
                QImage::Format getCurrentTextureFormat() const;

                /**
                 * Returns whether there are any unsaved changes made to the texture.
                 *
                 * @return true if there are unsaved changes
                 */
                bool getUnsavedChanges() const;

                /**
                 * Get the current zoom of the area where the selected texture can be painted.
                 *
                 * @return zoom of the paint area
                 */
                TextureLogic::Zoom getZoom() const;

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
                 * Set the texture that can be painted. Forwards argument to the render area.
                 *
                 * @param texture to be painted
                 */
                void setTexture(TextureLogic::Texture *texture);

                /**
                 * Handles wheel events from the user mouse. If the control key is down, then this function tells
                 * the atlas widget to zoom in or out on the atlas, depending on the direction of the scroll. Otherwise
                 * the area of the atlas that is viewed changes.
                 *
                 * @param event
                 */
                void wheelEvent(QWheelEvent *event) override;

            private:
                // Reference to the render area that this scroll area holds
                PaintArea *paintArea = nullptr;
                // Keeps track if the user is holding down the control key on their keyboard
                bool controlKeyDown = false;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
