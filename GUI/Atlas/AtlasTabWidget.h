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
                QImage::Format getCurrentAtlasFormat() const;
                void removeTexture(const TextureLogic::Texture *texture);
                bool setIntersectionWidth(TextureLogic::Texture *texture);
                void setTextureBankReference(TextureLogic::TextureBank *textureBank);
                void updateTextureReferences(AccessRestriction::PassKey<TextureLogic::TextureBank>);

            signals:
                void currentAtlasInformationChanged(QString, unsigned int, unsigned int, unsigned int);

            private slots:
                void repaintSelectedTexture();
                void showAddNewAtlasTab();
                void showContextMenu(const QPoint &pos);

            private:
                void addAtlasWidget(const QString &tabName, QSize atlasSize, QImage::Format atlasFormat);

                std::vector<std::pair<ScrollArea*, QString>> currentTabs;
                TextureLogic::TextureBank *textureBank = nullptr;
                AtlasTabOptionsMenu *atlasTabOptionsMenu = nullptr;
                Dialogs::AddNewAtlasTab *addNewAtlasTab = nullptr;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ATLASTABWIDGET_H
