//
// Created by BinyBrion on 2019-09-17.
//

#ifndef TEXTUREATLASCREATOR_ATLAS_SCROLLAREA_H
#define TEXTUREATLASCREATOR_ATLAS_SCROLLAREA_H

#include <QtWidgets/QScrollArea>

#include "Atlas/AtlasInformationBundle.h"

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
            Q_OBJECT

            public:
                ScrollArea(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent = nullptr);
                void addTexture(const TextureLogic::Texture &texture);
                void enterEvent(QEvent *event) override;
                void exportTexture();
                QImage::Format getAtlasFormat() const;
                ::Atlas::AtlasInformationBundle getAtlasInformation() const;
                void keyPressEvent(QKeyEvent *event) override;
                void keyReleaseEvent(QKeyEvent *event) override;
                void leaveEvent(QEvent *event) override;
                void scrollContentsBy(int dx, int dy) override;
                void removeTexture(const TextureLogic::Texture *texture);
                void repaintSelectedTexture();
                void resizeEvent(QResizeEvent *event) override;
                bool setIntersectionWidth(TextureLogic::Texture *texture);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(const std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures);
                void wheelEvent(QWheelEvent *event) override;

            signals:
                void currentAtlasInformationChanged(::Atlas::AtlasInformationBundle);

            private slots:
                void zoomIn();
                void zoomOut();

            private:
                AtlasWidget *atlasWidget = nullptr;

                bool controlKeyDown = false;
        };
    }
}


#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
