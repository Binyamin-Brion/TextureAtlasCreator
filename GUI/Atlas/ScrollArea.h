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
                ScrollArea(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent = nullptr);
                void addTexture(const TextureLogic::Texture &texture);
                void enterEvent(QEvent *event) override;
                void keyPressEvent(QKeyEvent *event) override;
                void keyReleaseEvent(QKeyEvent *event) override;
                void leaveEvent(QEvent *event) override;
                QImage::Format getAtlasFormat() const;
                void scrollContentsBy(int dx, int dy) override;
                void removeTexture(const TextureLogic::Texture *texture);
                void repaintSelectedTexture();
                void resizeEvent(QResizeEvent *event) override;
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(const std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures);
                void wheelEvent(QWheelEvent *event) override;

            private:
                AtlasWidget *atlasWidget = nullptr;

                bool controlKeyDown = false;
        };
    }
}


#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
