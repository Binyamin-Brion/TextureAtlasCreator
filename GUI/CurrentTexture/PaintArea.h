//
// Created by BinyBrion on 2019-09-26.
//

#ifndef TEXTUREATLASCREATOR_PAINTAREA_H
#define TEXTUREATLASCREATOR_PAINTAREA_H

#include <QtWidgets/QWidget>
#include "CurrentTextureImage.h"
#include "TextureLogic/Zoom.h"
#include "PaintFunctions/Brush.h"
#include "PaintFunctions/PaintedArea.h"
#include <stack>

namespace TextureLogic
{
    class Texture;
}

namespace PaintFunctions
{
    class PaintHistoryCommand;
}

namespace GUI
{
    namespace CurrentTexture
    {
        /**
         *  Provides an area to modify the currently selected texture. Any changes made to a texture are applied
         *  directly to the texture, and unless undone, remain. The changes are visible automatically in the texture
         *  atlas, though there is a chance for a small delay to occur before the changes are visible in the atlas.
         *
         *  There are two render areas for a given texture- one to edit the texture as seen in the atlas, and another
         *   to edit the specular properties of the dimension.
         */

        class PaintArea : public QWidget
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

            public:

                /**
                  * Initializes this object with the parent passed in.
                  *
                  * @param currentTextureImage whether the current render area is used for specular editing or to edit the texture as is
                  * @param parent widget that has ownership over this object.
                  */
                explicit PaintArea(CurrentTextureImage currentTextureImage, QWidget *parent = nullptr);

                /**
                 * Get the brush that is currently being used for painting.
                 *
                 * @return brush being used to paint the selected texture
                 */
                PaintFunctions::Brush& getBrush();

                /**
                 * Get the format of the current image that can be painted.
                 *
                 * @return format of the selected image. Note that if no image is selected, then an Qt::Invalid_Format is returned
                 */
                QImage::Format getCurrentTextureFormat();

                /**
                 * Get the current zoom of the area where the selected texture can be painted.
                 *
                 * @return zoom of the paint area
                 */
                TextureLogic::Zoom getZoom() const;

                /**
                 * Handles mouse input.
                 *
                 * @param event the position of the cursor when the event was generated
                 */
                void mouseMoveEvent(QMouseEvent *event) override;

                /**
                 * Handles mouse clicks.
                 *
                 * @param event the mouse button that was clicked
                 */
                void mousePressEvent(QMouseEvent *event) override;

                /**
                 * Handles mouse button releases.
                 *
                 * @param event the mouse button that was released
                 */
                void mouseReleaseEvent(QMouseEvent *event) override;

                /**
                 * Handles requests to paint the selected texture, effectively showing the selected texture after
                 * it was painted.
                 *
                 * @param event unused
                 */
                void paintEvent(QPaintEvent *event) override;

                /**
                 * Set the texture that can be painted.
                 *
                 * @param texture to be painted
                 */
                void setTexture(TextureLogic::Texture *texture);

                /**
                 * Update the drawing position of the texture in this object, if there is one, to account for the change in zoom.
                 * Note: zoom is in done in steps, ie from 100% to 200%, not from 100% to 400%. This negates the need to
                 *       pass in the new zoom value in to this function.
                 */
                void zoomIn();

                /**
                 * Update the drawing position of the texture in this object, if there is one, to account for the change in zoom.
                 * Note: zoom is in done in steps, ie from 100% to 200%, not from 100% to 400%. This negates the need to
                 *       pass in the new zoom value in to this function.
                 */
                void zoomOut();

            private slots:

                /**
                 *  Called when the user requests to undo a paint operation made.
                 */
                void undoPaintOperation();

            private:

                /**
                 * Gets the image that is currently being displayed in this object- either the SelectedTexture or the
                 * Specular Texture for the texture that is selected.
                 *
                 * @param zoom the current zoom of this object
                 * @return reference to the image that this object is displaying at the passed in zoom level
                 */
                const QImage& getReferredToImage(TextureLogic::Zoom zoom) const;

                /**
                 * Gets the most recent paint history for the image currently being displayed. If the SelectedTexture is
                 * being displayed then the SelectedTexture paint history is returned; otherwise the Specular paint history.
                 *
                 * @param zoom the current zoom of this object
                 * @return most recent paint operation performed on the texture being displayed by this object
                 */
                PaintFunctions::PaintHistoryCommand* getReferredToImageHistory(TextureLogic::Zoom zoom) const;

                /**
                 * Paints the texture with the current brush. It is called to paint the image being shown- either a new
                 * paint operation or to undo a previous operation.
                 *
                 * @param zoom the current zoom of this object
                 * @param mousePosition the position of the mouse when the user requested to make a paint operation
                 * @param applyImage the image of the brush
                 * @param targetImage the image to apply the brush to
                 * @param undoOperation true if the call to this function is to undo a paint operation
                 */
                void paintTexture(TextureLogic::Zoom zoom, QPoint mousePosition, const QImage &applyImage, QImage &targetImage, bool undoOperation);

                /**
                 *  Stores the paint history for the image being displayed.
                 */
                void storePaintHistory();

                // Reference to the texture being displayed that can be modified
                TextureLogic::Texture *texture = nullptr;

                // Stores what type of texture is being displayed, the SelectedTexture or the Specular version of the SelectedTexture
                CurrentTextureImage currentTextureImage;
                TextureLogic::Zoom currentZoom;

                // Keep track of the brush for each render area as each render area has its own different brush
                PaintFunctions::Brush brush;
                // Keep track of where brush operations were applied between mouse click, mouse movement and mouse release
                std::array<std::stack<PaintFunctions::PaintedArea>, TextureLogic::NumberZoomElements()> paintSequence;

                // Keep track of where the cursor moved to so that the brush can be painted around that position
                QPoint previousMousePosition;
                // The brush is only applied if the cursor is dragged down while the left mouse button is down
                bool leftMouseButtonDown = false;

                // The format that the texture being displayed. Used as a convenience variable.
                QImage::Format textureFormat;
        };
    }
}

#endif //TEXTUREATLASCREATOR_PAINTAREA_H
