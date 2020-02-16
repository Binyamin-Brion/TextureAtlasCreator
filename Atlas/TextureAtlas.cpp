//
// Created by BinyBrion on 2019-09-14.
//

#include "TextureAtlas.h"

#include "TextureLogic/TextureBank.h"
#include "Atlas/SelectedTexture.h"
#include "GUI/TextureHelperFunctions/TextureFormats.h"

#include <QPainter>
#include "GUI/Atlas/AtlasWidget.h"
#include <fstream>

namespace Atlas
{
    TextureAtlas::TextureAtlas(QImage::Format atlasFormat) : atlasFormat{atlasFormat}
    {
        selectedTexture = new SelectedTexture;

        selectedExistingTexture = new SelectedTexture;

        currentZoom = TextureLogic::Zoom::Normal;

        currentZoomIndex = ::TextureLogic::GetZoomIndex(currentZoom);
    }

    bool TextureAtlas::checkIntersection()
    {
        // A texture can only intersect with other textures if it's moving, in which case a texture must have
        // been selected (does not matter which one, as long as there is one selected)
        if(!selectedTexture->isOpen() && !selectedExistingTexture->isOpen())
        {
            return false;
        }

        intersectionOccured = false;

        for(auto &i : textureDrawingPositions)
        {
            if(selectedTexture->isOpen())
            {
                intersectionOccured |= i.surroundingBorder[currentZoomIndex].checkIntersection(selectedTexture->getSurroundingBorderForDrawing()[currentZoomIndex]);
            }
            else
            {
                intersectionOccured |= i.surroundingBorder[currentZoomIndex].checkIntersection(selectedExistingTexture->getSurroundingBorderForDrawing()[currentZoomIndex]);
            }
        }

        return intersectionOccured;
    }

    void TextureAtlas::draw(QPainter &painter)
    {
        // Note the order of drawing: the selected texture should be drawn last so that it is always drawn,
        // even when over another texture

        for(const auto &i : textureDrawingPositions)
        {
            painter.drawImage(i.drawingPosition, i.texture->getImage(currentZoom));

            i.surroundingBorder[currentZoomIndex].draw(painter);

           // i.texture->drawBorder(painter, currentZoom);
        }

        if(selectedTexture->isOpen())
        {
            painter.drawImage(selectedTexture->getDrawingCoordinates(), selectedTexture->getImageForDrawing().getImage(currentZoom));

            selectedTexture->getSurroundingBorderForDrawing()[currentZoomIndex].draw(painter);
        }
        if(selectedExistingTexture->isOpen())
        {
            painter.drawImage(selectedExistingTexture->getDrawingCoordinates(), selectedExistingTexture->getImageForDrawing().getImage(currentZoom));

            selectedExistingTexture->getSurroundingBorderForDrawing()[currentZoomIndex].draw(painter);
        }
    }

    QImage::Format TextureAtlas::getAtlasFormat() const
    {
        return atlasFormat;
    }

    QSize TextureAtlas::getAtlasSize() const
    {
        // The result of this function is intended to be used if a texture is selected, as the result is used to determine
        // if a texture can further moved in a direction and when to reset cursor (call to textureSelected has to be done first)

        return atlasSize;
    }

    unsigned int TextureAtlas::getNumberTextures()
    {
        return textureDrawingPositions.size() + selectedExistingTexture->isOpen();
    }

    unsigned int TextureAtlas::getPercentageAtlasUsed() const
    {
        unsigned int pixelsCovered = 0;

        for(const auto &i : textureDrawingPositions)
        {
            const QImage& referredToImage = i.texture->getImage(TextureLogic::Zoom::Normal);

            pixelsCovered += (referredToImage.width() * referredToImage.height());
        }

        if(selectedExistingTexture->isOpen())
        {
            const QImage& referredToImage = selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal);

            pixelsCovered += (referredToImage.width() * referredToImage.height());
        }

        return 100.0f * pixelsCovered / (atlasSize.width() * atlasSize.height());
    }

    QSize TextureAtlas::getSelectedTextureSize() const
    {
        // Same idea for using pair and checking if a texture is selected as fn getAtlasSize

        if(selectedTexture->isOpen())
        {
            return QSize{selectedTexture->getImageForDrawing().getImage(currentZoom).width(), selectedTexture->getImageForDrawing().getImage(currentZoom).height()};
        }
        else if(selectedExistingTexture->isOpen())
        {
            return QSize{selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width(), selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height()};
        }

        return QSize{-1, -1};
    }

    bool TextureAtlas::exportImage(const QString &exportLocation) const
    {
        float zoomFactor = TextureLogic::GetZoomValue(TextureLogic::Zoom::Normal) / TextureLogic::GetZoomValue(currentZoom);

        QImage image = QPixmap(atlasSize * zoomFactor).toImage();

        image = image.convertToFormat(atlasFormat);

        for(const auto &i : textureDrawingPositions)
        {
            float xDrawingPosition = i.drawingPosition.x();

            float yDrawPosition = i.drawingPosition.y();

            xDrawingPosition /= TextureLogic::GetZoomValue(currentZoom);

            yDrawPosition /= TextureLogic::GetZoomValue(currentZoom);

            for(int x = 0; x < i.texture->getImage(TextureLogic::Zoom::Normal).size().width(); ++x)
            {
                for(int y = 0; y < i.texture->getImage(TextureLogic::Zoom::Normal).size().height(); ++y)
                {
                    image.setPixelColor(xDrawingPosition + x, yDrawPosition + y, i.texture->getImage(TextureLogic::Zoom::Normal).pixelColor(x, y));
                }
            }
        }

        if(selectedExistingTexture->isOpen())
        {
            for(int x = 0; x < selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal).size().width(); ++x)
            {
                for(int y = 0; y <  selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal).size().height(); ++y)
                {
                    image.setPixelColor(selectedExistingTexture->getDrawingCoordinates().x() + x, selectedExistingTexture->getDrawingCoordinates().y() + y, selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal).pixelColor(x, y));
                }
            }
        }

        return image.save(exportLocation);
    }

    void TextureAtlas::keyPressed(int keyID)
    {
        if(keyID == Qt::Key_Delete)
        {
            if(selectedExistingTexture->isOpen())
            {
                selectedExistingTexture->getImage();
                textureBank->textureSelected(nullptr);
            }
        }
    }

    void TextureAtlas::mouseClicked(int mouseX, int mouseY, int mouseButton)
    {
        leftMouseButtonDown = mouseButton == Qt::LeftButton;

        bool deSelectTexture = false;

        if(selectedExistingTexture->isOpen())
        {
            if(mouseX >= selectedExistingTexture->getDrawingCoordinates().x() && mouseX <= selectedExistingTexture->getDrawingCoordinates().x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width())
            {
                if(mouseY >= selectedExistingTexture->getDrawingCoordinates().y() && mouseY <= selectedExistingTexture->getDrawingCoordinates().y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height())
                {
                    // If an existing texture is selected, and at the time of the cursor click the cursor is over the texture,
                    // then make a note that when the cursor is released, execute the logic that may result in the cursor being unselected

                    ignoreMouseRelease = false;

                    // If the texture has been clicked, there is a chance it will be dragged (moved). However, it may be placed in an invalid
                    // location, which case it has to be returned to its position at the time of being clicked. This assignment keeps track of that position.

                    previousDrawingCoords = selectedExistingTexture->getDrawingCoordinates();

                    return;
                }
                else
                {
                    deSelectTexture = true;
                }
            }
            else
            {
                deSelectTexture = true;
            }
        }

        if(deSelectTexture)
        {
            // If an area that is not the texture is clicked, then deselect it visually by not drawing its selection borders,
            // and add the texture internally so that logically it is not longer selected (hence call to addTexture)
            // Note that because only one texture can be selected at a time, this effectively marks all the textures as unselected

            selectedExistingTexture->setDrawSelectedSurroundingBorder(false);

            addTexture(selectedExistingTexture);
        }

        textureBank->textureSelected(nullptr);

        if(!selectedTexture->isOpen())
        {
            // If a texture is selected, then it needs to removed from the texture drawing position,
            // as it now is being referenced by the selectedExistingTexture object. Otherwise the texture will be drawn twice
            // and as the selected texture is moved, the texture will be drawn twice in different locations

            bool deleteTextureDrawingPosition = false;

            unsigned int deleteIndex = 0;

            for(auto &i : textureDrawingPositions)
            {
                int currentTextureWidth = i.texture->getImage(currentZoom).width();

                int currentTextureHeight = i.texture->getImage(currentZoom).height();

                if(mouseX >= i.drawingPosition.x() && mouseX <= i.drawingPosition.x() + currentTextureWidth)
                {
                    if(mouseY >= i.drawingPosition.y() && mouseY <= i.drawingPosition.y() + currentTextureHeight)
                    {
                        // Put back the existing selected texture, if it exists, so that a new texture can be selected

                        if(selectedExistingTexture->isOpen())
                        {
                            addTexture(selectedExistingTexture);
                        }

                        // Initialize the selectedExistingTexture reference with information about the texture that was clicked;
                        // the reference is drawing the exact same texture in the same place, so the texture data and drawing position
                        // has to be identical

                        selectedExistingTexture->setTexture(*i.texture, i.index);

                        selectedExistingTexture->move(i.drawingPosition.x() + i.texture->getImage(currentZoom).width() / 2,
                                              i.drawingPosition.y() + i.texture->getImage(currentZoom).height() / 2,
                                              atlasSize);

                        // The texture that was clicked on was meant to be selected, so visually show it as selected

                        selectedExistingTexture->setDrawSelectedSurroundingBorder(true);

                        // Update the variable that holds where existing selected texture should go if it is placed on
                        // another texture. This variable will change when the texture is placed on a new valid spot in the atlas

                        previousDrawingCoords = i.drawingPosition;

                        // When the mouse is released is when the logic to place an existing selected texture is executed.
                        // However, then what will happen is that as soon as the texture is selected through a click, it
                        // will be unselected when the same mouse click is released. Thus for the click that determines
                        // to select a texture, ignore (only) the next mouse release so that the user can release the mouse
                        // without deselecting the texture

                        ignoreMouseRelease = true;

                        deleteTextureDrawingPosition = true;

                        textureBank->textureSelected(i.texture);
                    }
                }

                if(!deleteTextureDrawingPosition)
                {
                    deleteIndex += 1;
                }
            }

            if(deleteTextureDrawingPosition)
            {
                textureDrawingPositions.erase(textureDrawingPositions.begin() + deleteIndex);
            }
        }
    }

    void TextureAtlas::mouseMoved(int mouseX, int mouseY)
    {
        if(selectedTexture->isOpen())
        {
            selectedTexture->move(mouseX, mouseY, atlasSize);

            checkIntersection();
        }
        else if(selectedExistingTexture->isOpen())
        {
            // If the left mouse is not down when an existing texture is selected, then the user is not trying to drag
            // the texture. In that case, no action has to be done if the mouse moves

            if(!leftMouseButtonDown)
            {
                return;
            }

            // If it is the first time the mouse is being dragged after clicking the left mouse, then centre the cursor
            // over the centre of the image. This is to prevent the texture from jumping during the first event of the mouse moving,
            // Remember that when moving the texture, is it relative to the centre of the texture.
            // TODO: Occasionally there are jumps in the texture when moving it for the first time. Perhaps figure out why?

            if(!ignoreMouseRelease)
            {
                QPointF drawingCoords = selectedExistingTexture->getDrawingCoordinates();
                auto& currentImage = selectedExistingTexture->getImageForDrawing().getImage(currentZoom);

                newMousePosition = QPoint(drawingCoords.x() + currentImage.width() / 2, drawingCoords.y() + currentImage.height() / 2);

                atlasWidget->moveMouseTo(newMousePosition.x(), newMousePosition.y());

                // If the user has dragged the mouse, then don't automatically deselect the texture.
                // That would be annoying if that happened

                ignoreMouseRelease = true;

                // There is a small error in the program logic that causes the selected texture to jump the first time
                // it is moved after being selected. Returning early from the function (causing mouse movements to begin
                // from the centre of the texture due to the above code) fixes that issue.

                return;
            }

            selectedExistingTexture->move(mouseX, mouseY, atlasSize);

            checkIntersection();
        }
    }

    void TextureAtlas::mouseReleased(int mouseX, int mouseY, int mouseButton)
    {
        if(mouseButton == Qt::LeftButton)
        {
            leftMouseButtonDown = false;
        }

        if(selectedExistingTexture->isOpen())
        {
            if(intersectionOccured)
            {
                // If the texture was released over another texture, then it has to be moved back to its original location
                // before the drag occurred. Note that the previousDrawingCoords are set whenever a new existing texture is selected
                // and everytime an existing selected texture is clicked

                previousDrawingCoords.setX(previousDrawingCoords.x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width() / 2);
                previousDrawingCoords.setY(previousDrawingCoords.y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height() / 2);

                selectedExistingTexture->move(previousDrawingCoords.x(), previousDrawingCoords.y(), atlasSize);

                // This will always return false, but it will clear all textures of showing an intersection visually

                checkIntersection();
            }
            else if(!ignoreMouseRelease)
            {
                // Only deselect texture if the cursor is released over the selected texture. This behaviour is enabled as
                // it allows the user to drag the mouse away from over the texture and then release, preventing the texture
                // from being deselected. It's not required behaviour, but it seems more intuitive.

                if(mouseX >= selectedExistingTexture->getDrawingCoordinates().x() && mouseX <= selectedExistingTexture->getDrawingCoordinates().x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width())
                {
                    if(mouseY >= selectedExistingTexture->getDrawingCoordinates().y() && mouseY <= selectedExistingTexture->getDrawingCoordinates().y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height())
                    {
                        selectedExistingTexture->setDrawSelectedSurroundingBorder(false);

                        addTexture(selectedExistingTexture);

                        textureBank->textureSelected(nullptr);
                    }
                }
            }
        }

        if(selectedTexture->isOpen())
        {
            addTexture(selectedTexture);
        }
    }

    bool TextureAtlas::resizeIntersectionBorderWidth(TextureLogic::Texture *texture)
    {
        unsigned int intersectionBorderWidth = texture->getIntersectionBorderWidth(TextureLogic::Zoom::Normal);

        if(intersectionBorderWidth > atlasSize.width() || intersectionBorderWidth > atlasSize.height())
        {
            return true;
        }

        bool foundTexture = false;

        if(selectedExistingTexture->isOpen())
        {
            if(texture == &selectedExistingTexture->getImageForDrawing())
            {
                QPointF previousSelectedDrawingCoords = selectedExistingTexture->getDrawingCoordinates();

                selectedExistingTexture->setTexture(*texture, selectedExistingTexture->getTextureIndex());

                selectedExistingTexture->setDrawingCoordinates(previousSelectedDrawingCoords);

                foundTexture = true;
            }

            if(!foundTexture)
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Unable to find the selected texture passed into this function!");
            }
        }

        return checkIntersection();
    }

    void TextureAtlas::removeTexture(const TextureLogic::Texture *texture)
    {
        auto textureLocation = std::find_if(textureDrawingPositions.begin(), textureDrawingPositions.end(), [texture](const TextureDrawingPosition &drawingPosition)
        {
            return drawingPosition.texture == texture;
        });

        if(textureLocation != textureDrawingPositions.end())
        {
            textureDrawingPositions.erase(textureLocation);
        }
        else if(selectedTexture->isOpen())
        {
            if(selectedTexture->getTextureLocation() == texture->textureLocation())
            {
                selectedTexture->getImage();
            }
        }
        else if(selectedExistingTexture->isOpen())
        {
            if(selectedExistingTexture->getTextureLocation() == texture->textureLocation())
            {
                selectedExistingTexture->getImage();
            }
        }
    }

    void TextureAtlas::setAtlasSize(QSize size)
    {
        // This function is called when the atlasWidget holding this texture atlas resizes

        atlasSize = size;
    }

    void TextureAtlas::setAtlasWidgetReference(GUI::Atlas::AtlasWidget *atlasWidget)
    {
        // This class needs to be able to call a function in atlasWidget. Tried passing a function pointer instead
        // but got compile time errors. Should be revisited if author has time.
        // TODO: change reference to atlasWidget to function pointer pointing to function that needs to be called within atlasWidget

        this->atlasWidget = atlasWidget;
    }

    void TextureAtlas::setSelectedTexture(const TextureLogic::Texture &texture)
    {
        // In a texture atlas, there is only one of each atlas. If that texture is already put in the atlas, then it is an error
        // to place it again and the user must be notified

        bool textureLoadedAlready = false;

        for(const auto &i : textureDrawingPositions)
        {
            if(i.texture->textureName() == texture.textureName())
            {
                textureLoadedAlready = true;

                break;
            }
        }

        if(selectedExistingTexture->isOpen() && selectedExistingTexture->getImageForDrawing().textureName() == texture.textureName())
        {
            textureLoadedAlready = true;
        }

        if(textureLoadedAlready)
        {
            std::string errorMessage;

            errorMessage += "The texture: ";
            errorMessage += texture.textureLocation().toStdString() + " has already been loaded.\n\n";
            errorMessage += "A texture cannot be added to an atlas twice.\n";
            errorMessage += "The requested operation will be aborted.";

            throw std::runtime_error{errorMessage};
        }

        selectedTexture->setTexture(texture);

        // If there is an existing texture that is selected, deselect it. This will put the texture back into
        // textureDrawingPositions, allowing for that texture to be checked against the newly selected texture in the
        // checkIntersection() function

        addTexture(selectedExistingTexture);

        for(auto &i : textureDrawingPositions)
        {
            i.surroundingBorder[currentZoomIndex].setSelectedBorderVisible(false);
        }
    }

    void TextureAtlas::setTextureBankReference(TextureLogic::TextureBank *textureBank)
    {
        if(this->textureBank == nullptr)
        {
            this->textureBank = textureBank;

            textures = &textureBank->getTextures();
        }
    }

    void TextureAtlas::textureLoaded(const std::vector<std::pair<std::vector<TextureLogic::Texture>, std::vector<unsigned int>>> &textures)
    {
        // Texture has been added to the texture bank. Note that when adding a texture to the image, the old vector of textures
        // might be moved in memory due to reallocation of the vector. Therefore references to that vector have to be reset just in case.

        this->textures = &textures;

        for(auto &i : textureDrawingPositions)
        {
            i.texture = &((*this->textures)[GUI::TextureHelperFunctions::indexFormat(atlasFormat, true)].first[i.index]);
        }

        if(selectedExistingTexture->isOpen())
        {
            selectedExistingTexture->setTextureReference(((*this->textures)[GUI::TextureHelperFunctions::indexFormat(atlasFormat, true)].first[selectedExistingTexture->getTextureIndex()]));
        }
    }

    bool TextureAtlas::textureSelected() const
    {
        if(selectedTexture->isOpen())
        {
            return true;
        }
        else if(selectedExistingTexture->isOpen())
        {
            return leftMouseButtonDown;
        }
    }

    void TextureAtlas::zoomIn()
    {
        TextureLogic::Zoom oldZoom = currentZoom;

        currentZoom = TextureLogic::zoomIn(currentZoom);

        if(oldZoom != currentZoom)
        {
            updateTextureDrawingPositions(2.0f);
            currentZoomIndex = ::TextureLogic::GetZoomIndex(currentZoom);
            updateSelectedTexturesZoom(currentZoom, 2.0f);

            atlasWidget->resizeAtlasFactor(2.0f);
        }
    }

    void TextureAtlas::zoomOut()
    {
        TextureLogic::Zoom oldZoom = currentZoom;

        currentZoom = TextureLogic::zoomOut(currentZoom);

        if(oldZoom != currentZoom)
        {
            updateTextureDrawingPositions(0.5f);
            currentZoomIndex = ::TextureLogic::GetZoomIndex(currentZoom);
            updateSelectedTexturesZoom(currentZoom, 0.5f);

            atlasWidget->resizeAtlasFactor(0.5f);
        }
    }

    void TextureAtlas::addTexture(SelectedTexture *selectedTexture)
    {
        if(intersectionOccured)
        {
            return;
        }

        if(selectedTexture->isOpen())
        {
            /* When adding the selected texture to the texture atlas, four things need to be done:
             * 1. Copy the drawing position of where the selected texture is currently being drawn
             * 2. Get the reference to the selected texture QImage- ie the actual data to render
             * 3. Copy the surrounding border of the selected texture. This gets the state of the border: its position, and whether it is drawn
             * 4. The index of the QImage reference into the textures reference. This is needed when the textures reference is updated.
             *    See fn textureLoaded
             */

            textureDrawingPositions.emplace_back();

            textureDrawingPositions.back().drawingPosition = selectedTexture->getDrawingCoordinates();

            textureDrawingPositions.back().texture = &selectedTexture->getImage();

            textureDrawingPositions.back().surroundingBorder = selectedTexture->getSurroundingBorder();

            for(int i = 0; i < textures->size(); ++i)
            {
                if((*textures)[GUI::TextureHelperFunctions::indexFormat(atlasFormat, true)].first[i].textureLocation() == selectedTexture->getTextureLocation())
                {
                    textureDrawingPositions.back().index = i;

                    break;
                }
            }

            if(textureDrawingPositions.back().index == -1)
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "Selected texture has a location not found in texture bank!");
            }
        }
    }

    void TextureAtlas::updateSelectedTexturesZoom(TextureLogic::Zoom newZoom, float zoomFactor)
    {
        selectedTexture->setZoom(newZoom, zoomFactor);

        selectedExistingTexture->setZoom(newZoom, zoomFactor);
    }

    void TextureAtlas::updateTextureDrawingPositions(float factor)
    {
        for(auto &i : textureDrawingPositions)
        {
            QPointF oldDrawingCoordinates = i.drawingPosition;

            i.drawingPosition *= factor;

            QPointF translation = i.drawingPosition - oldDrawingCoordinates;

            for(auto &j: i.surroundingBorder)
            {
                j.translate(translation.x(), translation.y());
            }
        }
    }
}