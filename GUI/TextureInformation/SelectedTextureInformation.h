//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
#define TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H

#include <QtWidgets/QWidget>
#include <TextureLogic/Zoom.h>
#include "ui_selectedTextureInformation.h"
#include "GUI/TextureHelperFunctions/TextureFormats.h"

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace TextureInformation
    {
        /**
         *  Holds all of the widgets to show information about the currently selected texture,
         *  as well as perform operations on it.
         */

        class SelectedTextureInformation : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when the user has selected a valid number for a new intersection width.
                 *  Note: this signal being emitted does not mean that the new intersection width is valid regarding
                 *        use within the texture atlases; rather the user entered a number without error.
                 */
                void newIntersectionBorderWidth(TextureLogic::Texture*, TextureLogic::Zoom , unsigned int);

                /**
                 *  Emitted when the user has selected a valid number for a new selection width.
                 *  Note: this signal does not have the same limitation as the intersection width; once this signal is
                 *        emitted, the value contained in the signal can be used immediately.
                 */
                void newSelectionBorderWidth(TextureLogic::Texture*, TextureLogic::Zoom, unsigned int);

                /**
                 *  Emitted when a modified texture was saved.
                 */
                void updateTextureButtonLocation(const QString, const QString);

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit SelectedTextureInformation(QWidget *parent = nullptr);

                /**
                 * Get the format of the currently selected format.
                 *
                 * @return format of the texture that is selected
                 */
                QImage::Format getSelectedTextureFormat() const;

                /**
                 * Update the text field holding the selected texture's intersection border width to the previous valid number.
                 * If the number entered by the user is valid, then this function will not be called.
                 *
                 * @param previousBorderWidth the most recent intersection width that the selected texture had
                 */
                void setIntersectionWidthLineEdit(unsigned int previousBorderWidth);

                /**
                 *  Called when the user modifies the selected texture in the paint area, resulting in the texture name
                 *  being modified.
                 */
                void selectedTextureModified();

                /**
                 * Set the texture that has been selected.
                 *
                 * @param texture reference to the currently selected texture
                 */
                void setTexture(const TextureLogic::Texture *texture);

            private slots:

                /**
                 *  Checks that the user entered a valid number for the intersection border width before emitting the
                 *  newIntersectionBorderWidth signal.
                 */
                void intersectionBorderWidthChanged();

                /**
                 *  Updates the text field showing the intersection border width for the current selected texture at
                 *  the selected zoom.
                 *
                 * @param index into the list of zoom options
                 */
                void intersectionZoomComboBoxChanged(int index);

                /**
                 *  Checks that the user entered a valid number for the selection border width before emitting the
                 *  newSelectionBorderWidth signal.
                 */
                void selectionBorderWidthChanged();

                /**
                 *  Updates the text field showing the selection border width for the current selected texture at
                 *  the selected zoom.
                 *
                 * @param index into the list of zoom options
                 */
                void selectionZoomComboBoxChanged(int index);

                /**
                 *  Checks that the new texture name is valid.
                 */
                void textureNameChanged();

            private:

                /**
                 * Checks if the text contained within the given line edit contains an actual number and is positive
                 * and small or lower than the max value.
                 *
                 * @param lineEdit that contains the text to check
                 * @param maxValue that the number in the text field can return
                 * @return integer representation of the text in the line edit. If the text is not a valid number, '-1' is returned
                 */
                int checkValidBorderWidth(QLineEdit *lineEdit, int maxValue);

                /**
                 * Returns the ratio between the normal zoom and the passed in zoom.
                 *
                 * @param zoom to get the ratio between
                 * @return ratio between normal zoom and the passed in zoom
                 */
                float normalZoomFactorValue(TextureLogic::Zoom zoom) const;

                /**
                 * Restores the area displaying the texture information to a default state. Used when a texture is deselected.
                 */
                void resetDefaultLabels();

                /**
                 * Saves a copy of the selected image to the disk at the given location.
                 *
                 * @param newFileLocation where to save the currently selected image to
                 */
                void saveImage(const QString &newFileLocation);

                Ui::SelectedTextureInformation *ui = nullptr;
                TextureLogic::Texture *texture = nullptr;

                // Helper variables used to display texture information and hold literal data structure equivalents
                // of the string versions used in the GUI.
                std::vector<GUI::TextureHelperFunctions::InternalFormatPair> internalFormatPairs;
                std::vector<QString> imageExtensions;
                std::vector<TextureLogic::ZoomPair> zoomPairs;

                QImage::Format selectedTextureFormat;

        };
    }
}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
