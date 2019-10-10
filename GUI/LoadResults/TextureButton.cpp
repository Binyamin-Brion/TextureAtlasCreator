//
// Created by BinyBrion on 2019-09-15.
//

#include "TextureButton.h"

namespace GUI
{
    namespace LoadResults
    {
        TextureButton::TextureButton(const QString &textureLocation, QWidget *parent)
                        : QPushButton{parent}, textureLocation{textureLocation}
        {

            setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);

            setFixedSize(150, 150);

            if(!pixMap.load(textureLocation))
            {
                // TODO: Handle case if texture cannot be loaded

                Q_ASSERT(false);
            }

            buttonIcon.addPixmap(pixMap);

            setIcon(buttonIcon);
            setIconSize(QSize{140, 140});

            connect(this, &QPushButton::clicked, [this]()
            {
                emit buttonClicked(this->textureLocation);
            });
        }

        void TextureButton::enterEvent(QEvent *event)
        {
            emit cursorOverButton(this);
        }

        void TextureButton::leaveEvent(QEvent *event)
        {
            emit cursorNotOverButton(this);
        }

        const QString &TextureButton::getTextureLocation() const
        {
            return textureLocation;
        }
    }
}