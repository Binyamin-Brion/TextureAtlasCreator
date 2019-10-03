//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H
#define TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H

#include <QtWidgets/QScrollArea>

namespace GUI
{
    namespace TextureInformation
    {
        class SelectedTextureInformation;

        class TextureInfoScrollArea : public QScrollArea
        {
            public:
                explicit TextureInfoScrollArea(QWidget *parent = nullptr);
                void setCentralWidget();

            private:
                SelectedTextureInformation *selectedTextureInformation = nullptr;

        };
    }
}

#endif //TEXTUREATLASCREATOR_TEXTUREINFOSCROLLAREA_H
