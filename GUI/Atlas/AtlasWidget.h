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
                void leaveEvent(QEvent *event) override;
                void mouseMoveEvent(QMouseEvent *event) override;
                void paintEvent(QPaintEvent *event) override;
                void resizeEvent(QResizeEvent *event) override;
                void updateTextureReferences(const std::vector<TextureLogic::Texture> &textures);
                ~AtlasWidget();

            public slots:
                void textureButtonPressed(const TextureLogic::Texture &texture);

            private:
                std::unique_ptr<::Atlas::TextureAtlas> textureAtlas;
                QSize size;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASWIDGET_H
