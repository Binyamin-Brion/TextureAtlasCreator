//
// Created by BinyBrion on 2019-09-15.
//

#include "TextureButton.h"
#include <QMap>

namespace GUI
{
    namespace LoadResults
    {
        /*
         *  Things to know:
         *
         *  1.  The variables intersectionBorderWidth and selectionBorderWidth are required in case the texture is being loaded
         *      into the texture bank for the first time. (Textures are lazily loaded- they are only actually loaded into program
         *      the first time a texture button associated with that texture is clicked, ie only where it is actually needed).
         *      Until this happens, these border widths have to be stored somewhere so that they can be used to load the texture properly.
         *
         *  2.  The signal for when this button is clicked has to include the values of the internal variables used by this object as
         *      the source of the signal (the 'this' pointer) is not known to the texture button area that handles the signal call.
         *      As a result the textureButtonArea will not know from which button to get information from, so it has to be included
         *      in the signal itself. The 'this' pointer is not used simply because it could be more dangerous to pass around.
         *
         *  3.  When deleting a texture button, the texture associated with that button is deleted if there will no longer be any
         *      buttons representing that texture. The reason is that without any textures, that texture cannot be added and so is
         *      assumed to be not needed anymore. However, to allow a texture button to be deleted without deleting that texture
         *      (if there are more than one buttons representing that texture), it is necessary to keep track of when the last
         *      button representing tha texture is deleted. Hence the reason for textureRepresentationCount.
         */

        const int TextureButton::MINIMUM_TEXTURE_REPRESENTATION_COUNT;
        QMap<QString, int> TextureButton::textureRepresentationCount;

        // Beginning of public functions

        TextureButton::TextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, QWidget *parent)
                        :
                            QPushButton{parent},
                            textureLocation{textureLocation},
                            intersectionBorderWidth{intersectionBorderWidth},
                            selectionBorderWidth{selectionBorderWidth}
        {
            // For the layout of buttons to work, the buttons must be of constant size
            setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);

            setFixedSize(150, 150);

            // Display a scaled down version of the texture as this button icon

            if(!pixMap.load(textureLocation))
            {
                // TODO: Handle case if texture cannot be loaded

                Q_ASSERT(false);
            }

            buttonIcon.addPixmap(pixMap);

            setIcon(buttonIcon);

            // Leave some border of the button visible around the icon so it still looks like a button
            setIconSize(QSize{140, 140});

            connect(this, &QPushButton::clicked, [this]()
            {
                emit buttonClicked(this->textureLocation, this->intersectionBorderWidth, this->selectionBorderWidth);
            });

            if(!textureRepresentationCount.contains(textureLocation))
            {
                textureRepresentationCount.insert(textureLocation, MINIMUM_TEXTURE_REPRESENTATION_COUNT);
            }
            else
            {
                textureRepresentationCount[textureLocation] += 1;
            }
        }

        const QString &TextureButton::getTextureLocation() const
        {
            return textureLocation;
        }

        bool TextureButton::mouseOver(QPoint mousePos) const
        {
            return pos().x() <= mousePos.x() && mousePos.x() <= pos().x() + width() &&
                   pos().y() <= mousePos.y() && mousePos.y() <= pos().y() + height();

        }

        const QMap<QString, int> &TextureButton::getTextureRepresentationCount()
        {
            return textureRepresentationCount;
        }

        void TextureButton::decrementTextureRepresentation(const QString &textureLocation)
        {
            if(textureRepresentationCount[textureLocation] > MINIMUM_TEXTURE_REPRESENTATION_COUNT)
            {
                textureRepresentationCount[textureLocation] -= 1;
            }
            else
            {
                textureRepresentationCount.remove(textureLocation);
            }
        }
    }
}