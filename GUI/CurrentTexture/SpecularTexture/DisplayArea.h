//
// Created by binybrion on 3/12/20.
//

#ifndef TEXTUREATLASCREATOR_DISPLAYAREA_H
#define TEXTUREATLASCREATOR_DISPLAYAREA_H

#include <QtWidgets/QWidget>
#include "../TextureLogic/Zoom.h"

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            class DisplayArea : public QWidget
            {
                    Q_OBJECT

                public:
                    explicit DisplayArea(QWidget *parent = nullptr);
                    void paintEvent(QPaintEvent *event) override;
                    void setSpecularTexture(const ::TextureLogic::Texture *specularTexture);
                    void zoomIn();
                    void zoomOut();

                public slots:
                    void repaintSpecularTexture();

                private:
                    const ::TextureLogic::Texture *specularTexture = nullptr;
                    ::TextureLogic::Zoom currentZoom;
            };
        }
    }
}


#endif //TEXTUREATLASCREATOR_DISPLAYAREA_H
