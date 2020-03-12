//
// Created by binybrion on 3/12/20.
//

#ifndef TEXTUREATLASCREATOR_SPECULAR_TEXTURE_SCROLLAREA_H
#define TEXTUREATLASCREATOR_SPECULAR_TEXTURE_SCROLLAREA_H

#include <QtWidgets/QScrollArea>

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
            class DisplayArea;

            class ScrollArea : public QScrollArea
            {
                    Q_OBJECT

                public:
                    explicit ScrollArea(QWidget *parent = nullptr);
                    void enterEvent(QEvent *event) override;
                    void keyPressEvent(QKeyEvent *event) override;
                    void keyReleaseEvent(QKeyEvent *event) override;
                    void leaveEvent(QEvent *event) override;
                    void repaintSelectedTexture();
                    void setSpecularTexture(const ::TextureLogic::Texture *texture);
                    void wheelEvent(QWheelEvent *event) override;

                private:
                    DisplayArea *displayArea = nullptr;
                    // Keeps track if the user is holding down the control key on their keyboard
                    bool controlKeyDown = false;
            };
        }
    }
}

#endif //TEXTUREATLASCREATOR_SCROLLAREA_H
