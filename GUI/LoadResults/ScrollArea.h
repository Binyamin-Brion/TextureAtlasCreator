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

        /*
         *  Provides a scroll area to a TextureButtonArea. Since a TextureButtonArea can hold many buttons that may
         *  not fit in the given space allocated to it onscreen, a scrollable view is required to be able to view
         *  all of the buttons that TextureButtonArea holds.
         */

        class ScrollArea : public QScrollArea
        {
            public:
                explicit ScrollArea(QWidget *parent = nullptr);
                void addTextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth);
                void deleteTextureButtons();
                const TextureButtonArea* getTextureArea() const;
                void setTextureBankReference(const TextureLogic::TextureBank *textureBank);

            private:
                TextureButtonArea *textureButtonArea = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
