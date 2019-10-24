//
// Created by BinyBrion on 2019-09-16.
//

#ifndef TEXTUREATLASCREATOR_ATLASTABWIDGET_H
#define TEXTUREATLASCREATOR_ATLASTABWIDGET_H

#include <QtWidgets/QTabWidget>
#include "AccessRestriction/PassKey.h"
#include "Atlas/TextureAtlas.h"
#include "Atlas/AtlasInformationBundle.h"

namespace TextureLogic
{
    class TextureBank;
}

namespace GUI
{
    namespace Dialogs
    {
        class AddNewTab;
        class AddNewAtlasTab;
    }

    namespace Atlas
    {
        class AtlasTabOptionsMenu;
        class ScrollArea;

        /*
         *  Holds all of the ScrollArea (that hold an AtlasWidget, which holds a TextureAtlas).
         *  It is a central place for managing all of the ScrollArea, in the sense that if an action
         *  is requested for a texture atlas, it is applied to the current texture atlas being shown,
         *  which is the current tab being shown.
         */

        class AtlasTabWidget : public QTabWidget
        {
            Q_OBJECT

            public:
                explicit AtlasTabWidget(QWidget *parent = nullptr);
                void addTextureToCurrentAtlas(const TextureLogic::Texture &texture);
                QImage::Format getCurrentAtlasFormat() const;
                void removeTexture(const TextureLogic::Texture *texture);
                bool setIntersectionWidth(TextureLogic::Texture *texture);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>);

            public slots:
                void exportTexture();

            signals:
                void currentAtlasInformationChanged(::Atlas::AtlasInformationBundle);

            private slots:
                void moveTabLeft();
                void moveTabRight();
                void repaintSelectedTexture();
                void showAddNewAtlasTab();
                void showContextMenu(const QPoint &pos);
                void showRenameTabDialog();

            private:
                void addAtlasWidget(const QString &tabName, QSize atlasSize, QImage::Format atlasFormat);

                std::vector<std::pair<ScrollArea*, QString>> currentTabs;
                TextureLogic::TextureBank *textureBank = nullptr;
                AtlasTabOptionsMenu *atlasTabOptionsMenu = nullptr;
                Dialogs::AddNewTab *renameTab = nullptr;
                Dialogs::AddNewAtlasTab *addNewAtlasTab = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABWIDGET_H
