//
// Created by BinyBrion on 2019-09-16.
//

#ifndef TEXTUREATLASCREATOR_ATLASTABWIDGET_H
#define TEXTUREATLASCREATOR_ATLASTABWIDGET_H

#include <QtWidgets/QTabWidget>
#include "AccessRestriction/PassKey.h"
#include "Atlas/TextureAtlas.h"

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Atlas
    {
        class ScrollArea;

        class AtlasTabWidget : public QTabWidget
        {
            public:
                explicit AtlasTabWidget(QWidget *parent = nullptr);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>);

            private:
                std::vector<std::pair<ScrollArea*, QString>> currentTabs;
                TextureLogic::TextureBank *textureBank = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABWIDGET_H
