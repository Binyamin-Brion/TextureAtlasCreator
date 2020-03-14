//
// Created by BinyBrion on 2019-09-15.
//

#include <QtWidgets/QHBoxLayout>
#include "ScrollArea.h"

#include "TextureButtonArea.h"

namespace GUI
{
    namespace LoadResults
    {
        // Beginning of public functions

        ScrollArea::ScrollArea(QWidget *parent)
                    :
                        QScrollArea{parent}
        {
            textureButtonArea = new TextureButtonArea{this};

            setWidget(textureButtonArea);
        }

        void ScrollArea::addTextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, bool loadingProject)
        {
            textureButtonArea->addTextureButton(textureLocation, intersectionBorderWidth, selectionBorderWidth, loadingProject);
        }

        void ScrollArea::deleteTextureButtons(bool closingProgram)
        {
            textureButtonArea->deleteTextureButtons(closingProgram);
        }

        const TextureButtonArea* ScrollArea::getTextureArea() const
        {
            return textureButtonArea;
        }

        bool ScrollArea::getUnsavedChanges() const
        {
            return textureButtonArea->getUnsavedChanges();
        }

        void ScrollArea::paintTextureButton(const ::TextureLogic::Texture *texture)
        {
            textureButtonArea->paintTextureButton(texture);
        }

        void ScrollArea::saveLoadedTextures(const QString &textureButtonAreaName, const QString &saveLocation) const
        {
            textureButtonArea->saveLoadedTextures(textureButtonAreaName, saveLocation);
        }

        void ScrollArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            textureButtonArea->setTextureBankReference(textureBank);
        }

        void ScrollArea::updateTextureButtonsLocation(const QString &previousLocation, const QString &newLocation)
        {
            textureButtonArea->updateTextureButtonsLocation(previousLocation, newLocation);
        }
    }
}