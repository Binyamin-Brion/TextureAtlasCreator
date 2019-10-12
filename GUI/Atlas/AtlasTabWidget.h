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
    namespace Dialogs
    {
        class AddNewAtlasTab;
    }

    namespace Atlas
    {
        class AtlasTabOptionsMenu;
        class ScrollArea;

        class AtlasTabWidget : public QTabWidget
        {
            Q_OBJECT

            public:
                explicit AtlasTabWidget(QWidget *parent = nullptr);
                void addTextureToCurrentAtlas(const TextureLogic::Texture &texture);
                void removeTexture(const TextureLogic::Texture *texture);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>);

            private slots:
                void repaintSelectedTexture();
                void showAddNewAtlasTab();
                void showContextMenu(const QPoint &pos);

            private:
                void addAtlasWidget(const QString &tabName);

                std::vector<std::pair<ScrollArea*, QString>> currentTabs;
                TextureLogic::TextureBank *textureBank = nullptr;
                AtlasTabOptionsMenu *atlasTabOptionsMenu = nullptr;
                Dialogs::AddNewAtlasTab *addNewAtlasTab = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABWIDGET_H
