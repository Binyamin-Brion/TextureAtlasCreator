//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SCROLLAREA_H


#include <QtWidgets/QScrollArea>

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace LoadResults
    {
        class TextureButtonArea;

        class ScrollArea : public QScrollArea
        {
            public:
                explicit ScrollArea(QWidget *parent = nullptr);
                void addTextureButton(const QString &textureLocation);
                void setTextureBankReference(const TextureLogic::TextureBank *textureBank);

            private:
                TextureButtonArea *textureButtonArea = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
