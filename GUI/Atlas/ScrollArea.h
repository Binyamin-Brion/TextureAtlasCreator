//
// Created by BinyBrion on 2019-09-17.
//

#ifndef TEXTUREATLASCREATOR_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SCROLLAREA_H

#include <QtWidgets/QScrollArea>

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace Atlas
    {
        class AtlasWidget;

        class ScrollArea : public QScrollArea
        {
            public:
                ScrollArea(QSize atlasSize, QWidget *parent = nullptr);
                void addTexture(const TextureLogic::Texture &texture);
                void scrollContentsBy(int dx, int dy);
                void removeTexture(const TextureLogic::Texture *texture);
                void repaintSelectedTexture();
                void resizeEvent(QResizeEvent *event) override;
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(const std::vector<TextureLogic::Texture> &textures);

            private:
                AtlasWidget *atlasWidget = nullptr;
        };
    }
}


#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
