//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREBUTTON_H
#define TEXTUREATLASCREATOR_TEXTUREBUTTON_H

#include <QtWidgets/QPushButton>

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace LoadResults
    {
        /**
         *  Represents a push button that when pressed, requests the texture it represents
         *  to be loaded into the current texture atlas.
         */

        class TextureButton : public QPushButton
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when a texture button is clicked. See internal note in TextureButton.cpp
                 */
                void buttonClicked(const QString&, unsigned int, unsigned int);

            public:

                /**
                 * Initializes all of the required internal members.
                 *
                 * @param textureLocation location of the texture this button represents
                 * @param intersectionBorderWidth specified intersection width in the texture loading dialog
                 * @param selectionBorderWidth specified selection width in the texture loading dialog
                 * @param parent widget that has ownership over this object
                 */
                TextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, QWidget *parent = nullptr);


                /**
                 * Get the texture location used to initialized this object. Used to determine if a button representing
                 * the same texture is already present in a texture button area.
                 *
                 * @return location of texture this button is associated with
                 */
                const QString& getTextureLocation() const;

                /**
                 * Determines if the cursor is currently over this button.
                 *
                 * @param mousePos position of the cursor
                 * @return true if the cursor is over this button
                 */
                bool mouseOver(QPoint mousePos) const;

                /**
                 * Updates the location this texture button represents.
                 *
                 * @param textureLocation new location on the file system that this button represents
                 */
                void setTextureLocation(const QString &textureLocation);

                /**
                 *  Updates the icon of this button when the texture it represents is painted.
                 *
                 * @param texture reference to the texture this button represents
                 */
                void updateTextureButton(const ::TextureLogic::Texture *texture);

                /**
                 * Decrement the count of the number of texture buttons representing a texture. Used when deleting a button.
                 *
                 * @param textureLocation whose count of buttons representing it should be reduced by one.
                 */
                static void decrementTextureRepresentation(const QString &textureLocation);

                /**
                 * Checks if there are any textures loaded with the same name as what was passed in.
                 *
                 * @param textureLocation location from which to extract name of the texture that is being loaded
                 * @return true if there is a texture with the same name
                 */
                static bool textureNameExists(const QString &textureLocation);

                /**
                 *  Get the map holding how many texture buttons are representing a given texture.
                 *
                 * @return map counting how many texture buttons are representing a texture
                 */
                static const QMap<QString, int>& getTextureRepresentationCount();

                /// Variable to ensure all texture buttons are all of the same size. TextureButtonArea requires access
                /// to this variable, hence why it is public.
                static const int BUTTON_SIZE_LENGTH = 175;

                // Minimum number of texture representations possible for a texture
                static const int MINIMUM_TEXTURE_REPRESENTATION_COUNT = 1;

            private:

                // Used as part of process to upload button icon. It is a member variable instead of a local variable
                // as it is passed by reference to the buttonIcon, but it is not clear if a local variable would cause a dangling reference
                QPixmap pixMap;

                // Holds the image of the texture that the button will display
                QIcon buttonIcon;

                // The information used to load the image. See internal note at TextureButton.cpp
                QString textureLocation;
                unsigned int intersectionBorderWidth;
                unsigned int selectionBorderWidth;

                // Keeps track of how many texture buttons are representing a texture. See internal note TextureButton.cpp
                static QMap<QString, int> textureRepresentationCount;
        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREBUTTON_H
