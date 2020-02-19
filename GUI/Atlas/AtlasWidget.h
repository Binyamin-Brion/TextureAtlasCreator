//
// Created by BinyBrion on 2019-09-17.
//

#ifndef TEXTUREATLASCREATOR_ATLASWIDGET_H
#define TEXTUREATLASCREATOR_ATLASWIDGET_H

#include <QtWidgets/QWidget>
#include <memory>

#include "Atlas/AtlasInformationBundle.h"

namespace Atlas
{
    class TextureAtlas;
}

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace Atlas
    {
        /**
         *  The Widget onto which the texture atlas is painted, and widget that stores the texture atlas
         *  itself. It forwards all necessary arguments to the texture atlas.
         */

        class AtlasWidget : public QWidget
        {
            Q_OBJECT

            public:
                explicit AtlasWidget(QSize atlasSize, QImage::Format atlasFormat, QWidget *parent = nullptr);
                QImage::Format getAtlasFormat() const;
                ::Atlas::AtlasInformationBundle getAtlasInformation() const;
                void enterEvent(QEvent *event) override;
                void exportTexture();
                void keyPressed(QKeyEvent *event);
                void mouseMoveEvent(QMouseEvent *event) override;
                void mousePressEvent(QMouseEvent *event) override;
                void mouseReleaseEvent(QMouseEvent *event) override;
                void moveMouseTo(int x, int y);
                void paintEvent(QPaintEvent *event) override;
                void removeTexture(const TextureLogic::Texture *texture);
                void resizeAtlasFactor(float factor);
                void resizeEvent(QResizeEvent *event) override;
                bool newIntersectionBorderWidthValid(TextureLogic::Texture *texture);
                void setViewPort(QSize viewPort);
                void setViewPortOffsetX(int value);
                void setViewPortOffsetY(int value);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences();
                void wheelEvent(QWheelEvent *event) override;
                void zoomIn();
                void zoomOut();
                ~AtlasWidget();

            signals:
                void currentAtlasInformationChanged(::Atlas::AtlasInformationBundle);

            public slots:
                void textureButtonPressed(const TextureLogic::Texture &texture);

            private:
                bool moveCursorToViewPort(int &mouseX, int &mouseY);

                std::unique_ptr<::Atlas::TextureAtlas> textureAtlas;

                // Used to determine if the texture is being moved off of the widget
                QSize viewPort;
                QPoint viewPortOffset;

                QPoint previousMouseCoords;

                bool controlKeyDown = false;

                QSizeF atlasDisplaySize;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASWIDGET_H
