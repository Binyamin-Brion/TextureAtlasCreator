//
// Created by BinyBrion on 2019-09-15.
//

#ifndef TEXTUREATLASCREATOR_LOADEDTEXTURES_H
#define TEXTUREATLASCREATOR_LOADEDTEXTURES_H

#include <QtWidgets/QTabWidget>

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Dialogs
    {
        class ChooseTexture;
    }

    namespace LoadResults
    {
        class ScrollArea;

        class LoadedTextures : public QTabWidget
        {
            Q_OBJECT

            public:
                explicit LoadedTextures(QWidget *parent = nullptr);
                void setTextureBankReference(const TextureLogic::TextureBank *textureBank);

            public slots:
                void showLoadTextureDialog();

            private slots:
                void openTexture(QString textureLocation);

            private:
                std::vector<std::pair<ScrollArea*, QString>> currentTabs;
                const TextureLogic::TextureBank *textureBank = nullptr;
                Dialogs::ChooseTexture *chooseTexture = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_LOADEDTEXTURES_H
