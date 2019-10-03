//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
#define TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H

#include <QtWidgets/QWidget>
#include "ui_selectedTextureInformation.h"

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace TextureInformation
    {
        class SelectedTextureInformation : public QWidget
        {
            public:
                explicit SelectedTextureInformation(QWidget *parent = nullptr);
                void setTexture(const TextureLogic::Texture *texture);

            private:
                void resetDefaultLabels();

                Ui::SelectedTextureInformation *ui = nullptr;
                TextureLogic::Texture *texture = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
