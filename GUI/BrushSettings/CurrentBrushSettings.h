//
// Created by BinyBrion on 10/2/2019.
//

#ifndef TEXTUREATLASCREATOR_CURRENTBRUSHSETTINGS_H
#define TEXTUREATLASCREATOR_CURRENTBRUSHSETTINGS_H

#include <QtWidgets/QWidget>
#include <TextureLogic/Zoom.h>

class QColorDialog;

namespace PaintFunctions
{
    class Brush;
}

namespace Ui
{
    class BrushSettings;
}

namespace GUI
{
    namespace BrushSettings
    {
        /**
         * Shows the current state of the brush (colour and size).
         * Additionally, it provides the user with the means to change the above two properties, which will change
         * the brush to match the properties specified. The updated properties will be reflected in this widget.
         */

        class CurrentBrushSettings : public QWidget
        {
                Q_OBJECT

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit CurrentBrushSettings(QWidget *parent = nullptr);

                /**
                 * Changes the brush displayed to what is passed in. This causes the colour of the brush and the size
                 * GUI components of this widget to change. This is called when a different render area is opened,
                 * such as specular render area.
                 *
                 * @param brush to display as the current brush
                 */
                void setBrushReference(const PaintFunctions::Brush &brush);

            public slots:

                /**
                 * Called when the selected texture that is being painted on has changed size.
                 *
                 * @param size of the new texture that is being painted on
                 * @param brushSize size of the brush used for painting
                 */
                void updateSelectedTextureSize(QSize size, QSize brushSize);

                /**
                 * Called when the zoom of the specular display area changes.
                 *
                 * @param newZoom updated zoom value of the specular display area
                 */
                void zoomChangedDisplayArea(::TextureLogic::Zoom newZoom);

                /**
                 * Called when the zoom of the PaintArea changes.
                 *
                 * @param newZoom updated zoom value of the PaintArea
                 */
                void zoomChangedPaintArea(::TextureLogic::Zoom newZoom);

            private slots:

                /**
                 *  Allows user to select a new colour for the current brush.
                 */
                void handleColourButtonPressed();

                /**
                 * Called when the user toggles the radio button controlling if the paint operations are to be applied to
                 * the specular texture. Updates the brush to the previous colour used for the mode that the paint operations
                 * are now in.
                 *
                 * @param value true if the user is in specular painting mode
                 */
                void switchDrawingMode(bool value);

                /**
                 * Sets the brush to the colour passed in.
                 *
                 * @param selectedColour colour of the brush after the user chose a different colour
                 */
                void updateBrushColour(QColor selectedColour);

                /**
                 * Sets the brush to the size specified in the currentBrushWidthLineEdit variable.
                 */
                void updateBrushSize();

            private:

                /**
                 * Helper function to aid in visually setting the colour chosen for the brush.
                 *
                 * @param colour to be displayed visually
                 * @return setStyleSheet string that can be used to set the colour of a widget
                 */
                QString createStyleSheet_CurrentBrushColour(QColor colour) const;

                Ui::BrushSettings *ui = nullptr;

                // Shown when the user wants to select a new colour for the brush
                QColorDialog *colourDialog = nullptr;

                PaintFunctions::Brush *brush = nullptr;

                // Keeps track of the size of the selected texture so it is known if a brush size is valid
                QSize selectedTextureSize;

                // Keep track of the zoom of RenderArea so the brush size can be queried from the Brush
                TextureLogic::Zoom currentZoom;

                // Keep track of what the colour of the current paint mode is so that when switching back to the mode,
                // the user does not need to respecify the colour
                QColor previousDiffuseColour;
                QColor previousSpecularDrawingColour;
        };
    }
}

#endif //TEXTUREATLASCREATOR_CURRENTBRUSHSETTINGS_H
