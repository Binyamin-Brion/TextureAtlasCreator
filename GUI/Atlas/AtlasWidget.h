//
// Created by BinyBrion on 2019-09-17.
//

#ifndef TEXTUREATLASCREATOR_ATLASWIDGET_H
#define TEXTUREATLASCREATOR_ATLASWIDGET_H

#include <QtWidgets/QWidget>
#include <memory>

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
        class AtlasWidget : public QWidget
        {
            Q_OBJECT

            public:
                explicit AtlasWidget(QSize atlasSize, QWidget *parent = nullptr);
                void keyPressEvent(QKeyEvent *event) override;
                void mouseMoveEvent(QMouseEvent *event) override;
                void mousePressEvent(QMouseEvent *event) override;
                void mouseReleaseEvent(QMouseEvent *event) override;
                void moveMouseTo(int x, int y);
                void paintEvent(QPaintEvent *event) override;
                void resizeEvent(QResizeEvent *event) override;
                void setViewPort(QSize viewPort);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(const std::vector<TextureLogic::Texture> &textures);
                void translateViewPort(int dx, int dy);
                ~AtlasWidget();

            public slots:
                void textureButtonPressed(const TextureLogic::Texture &texture);

            private:
                bool moveCursorToViewPort(int &mouseX, int &mouseY);

                std::unique_ptr<::Atlas::TextureAtlas> textureAtlas;
                QSize size;
                QSize viewPort;
                QPoint viewPortOffset;

                QPoint previousMouseCoords;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASWIDGET_H
