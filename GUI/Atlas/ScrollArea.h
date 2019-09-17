//
// Created by BinyBrion on 2019-09-17.
//

#ifndef TEXTUREATLASCREATOR_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SCROLLAREA_H

#include <QtWidgets/QScrollArea>

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace Atlas
    {
        class AtlasWidget;

        class ScrollArea : public QScrollArea
        {
            Q_OBJECT

            public:
                ScrollArea(QSize atlasSize, QWidget *parent = nullptr);
                void updateTextureReferences(const std::vector<TextureLogic::Texture> &textures);

            public slots:
                void textureButtonPressed(const TextureLogic::Texture &texture);

            signals:
                void passTextureButtonInfo(const TextureLogic::Texture &texture);

            private:
                AtlasWidget *atlasWidget = nullptr;
        };
    }
}


#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
