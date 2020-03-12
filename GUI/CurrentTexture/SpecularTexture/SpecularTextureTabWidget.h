//
// Created by binybrion on 3/12/20.
//

#ifndef TEXTUREATLASCREATOR_SPECULARTEXTURETABWIDGET_H
#define TEXTUREATLASCREATOR_SPECULARTEXTURETABWIDGET_H

#include <QtWidgets/QTabWidget>

namespace TextureLogic
{
    class Texture;
    class TextureBank;
}

namespace GUI
{
    namespace CurrentTexture
    {
        namespace SpecularTexture
        {
            class ScrollArea;

            class SpecularTextureTabWidget : public QTabWidget
            {
                    Q_OBJECT

                public:
                    explicit SpecularTextureTabWidget(QWidget *parent = nullptr);
                    void setSpecularTexture(const ::TextureLogic::Texture *texture);
                    void setTextureBankReference(const ::TextureLogic::TextureBank *textureBank);
                    void updateTextureReference();

                public slots:
                    void repaintSpecularTexture();

                private:
                    ScrollArea *displayScrollArea = nullptr;
                    int currentTextureIndex;
                    unsigned int formatIndex;
                    const TextureLogic::TextureBank *textureBank = nullptr;
            };
        }
    }
}

#endif //TEXTUREATLASCREATOR_SPECULARTEXTURETABWIDGET_H
