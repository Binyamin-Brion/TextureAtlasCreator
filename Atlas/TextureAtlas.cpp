//
// Created by BinyBrion on 2019-09-14.
//

#include "TextureAtlas.h"

#include "TextureLogic/TextureBank.h"
#include "Atlas/SelectedTexture.h"
#include "GUI/TextureHelperFunctions/HelperFunctions.h"

#include <QPainter>
#include "GUI/Atlas/AtlasWidget.h"
#include <fstream>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QMessageBox>
#include "HelperUtilities/Assert.h"

namespace Atlas
{
    TextureAtlas::TextureAtlas(QImage::Format atlasFormat, bool inTestingMode)
                    :
                        atlasFormat{atlasFormat},
                        inTestingMode{inTestingMode}
    {
        selectedTexture = new SelectedTexture;

        selectedExistingTexture = new SelectedTexture;

        currentZoom = TextureLogic::Zoom::Normal;

        currentZoomIndex = ::TextureLogic::GetZoomIndex(currentZoom);
    }

    void TextureAtlas::addTextureWithPosition(const TextureLogic::Texture &texture, QPoint position)
    {
        selectedTexture->setTexture(texture);
        selectedTexture->setDrawingCoordinates(position);

        addTexture(selectedTexture);
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
            // Check needed due to action taken in newIntersectionBorderWidthValid().
            if((selectedTexture->isOpen() && i.texture == &selectedTexture->getImageForDrawing()))
            {
                continue;
            }

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
        // When drawing the texture, only the texture and its border at the current zoom level is drawn. Since
        // the contents position and size changes with the zoom, only the contents that have been adjusted for
        // the current zoom can be drawn.

        // Note: the order of drawing: the selected texture should be drawn last so that it is always drawn,
        // even when over another texture
        for(const auto &i : textureDrawingPositions)
        {
            painter.drawImage(i.drawingPosition, i.texture->getImage(currentZoom));

            i.surroundingBorder[currentZoomIndex].draw(painter);
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

    QSize TextureAtlas::getNormalZoomAtlasSize() const
    {
        float zoomFactor = TextureLogic::GetZoomValue(TextureLogic::Zoom::Normal) / TextureLogic::GetZoomValue(currentZoom);

        return atlasSize * zoomFactor;
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
            const QImage& referredToImage = i.texture->getImage(currentZoom);

            pixelsCovered += (referredToImage.width() * referredToImage.height());
        }

        if(selectedExistingTexture->isOpen())
        {
            const QImage& referredToImage = selectedExistingTexture->getImageForDrawing().getImage(currentZoom);

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

    bool TextureAtlas::getUnsavedChanges() const
    {
        return unsavedChanges;
    }

    TextureLogic::Zoom TextureAtlas::getCurrentZoom() const
    {
        return currentZoom;
    }

    void TextureAtlas::exportImage(const QString &exportLocation, const QString &exportLocationSpecular) const
    {
        QString textureCoordinateSaveFileLocation = exportLocation;
        textureCoordinateSaveFileLocation.chop(textureCoordinateSaveFileLocation.size() - textureCoordinateSaveFileLocation.indexOf('.'));
        textureCoordinateSaveFileLocation += "_UV_Coordinates.txt";

        QFile textureCoordinateSaveFile{textureCoordinateSaveFileLocation};

        if(!textureCoordinateSaveFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            throw std::runtime_error{"Failed to open the specified save location for writing."};
        }

        QTextStream saveStream{&textureCoordinateSaveFile};

        // Exporting is done at the Normal zoom level, as that is true representation of the sizes that the user
        // has input into the program. To account for this without having to change the zoom onscreen, the ratio
        // between the current zoom and the normal zoom is calculated so it is known what the exported atlas should be.
        // Remember that when zoom changes, so does the atlas size and the drawing positions of the textures in the atlas,
        // which should not affect the final exported atlas size.
        float zoomFactor = TextureLogic::GetZoomValue(TextureLogic::Zoom::Normal) / TextureLogic::GetZoomValue(currentZoom);

        QImage diffuseTexture = QPixmap(atlasSize * zoomFactor).toImage();
        QImage specularTexture = QPixmap(atlasSize * zoomFactor).toImage();

        // The exported image should have the same format as what the atlas was specified to have when it was created
        diffuseTexture = diffuseTexture.convertToFormat(atlasFormat);
        specularTexture = specularTexture.convertToFormat(atlasFormat);

        for(const auto &i : textureDrawingPositions)
        {
            float xDrawingPosition = i.drawingPosition.x() * zoomFactor;

            float yDrawingPosition = i.drawingPosition.y() * zoomFactor;

            QString localTextureLocation = '/' + i.texture->textureName() + '.' + i.texture->textureFormat();

            float topLeftU_Coord = xDrawingPosition / atlasSize.width();
            float topLeftV_Cord =  1.f - (yDrawingPosition / atlasSize.height()); // Because in OpenGL, the top of a texture is when V = 1.

            float bottomRightU_Coord = (xDrawingPosition + i.texture->getImage(::TextureLogic::Zoom::Normal).width()) / atlasSize.width();
            float bottomRightV_Coord = (yDrawingPosition + i.texture->getImage(::TextureLogic::Zoom::Normal).height()) / atlasSize.height();

            saveStream << localTextureLocation << ": "
                       << topLeftU_Coord << " , " << bottomRightV_Coord << " | " // Bottom Left Corner
                       << topLeftU_Coord << " , " << topLeftV_Cord << " | "      // Top Left Corner
                       << bottomRightU_Coord << " , " << topLeftV_Cord << " | " // Top Right Corner
                       << bottomRightU_Coord << " , " << bottomRightV_Coord << '\n'; // Bottom Right Corner


            for(int x = 0; x < i.texture->getImage(TextureLogic::Zoom::Normal).size().width(); ++x)
            {
                for(int y = 0; y < i.texture->getImage(TextureLogic::Zoom::Normal).size().height(); ++y)
                {
                    // There is a bug related to Qt where if an atlas is made to be the size of a texture, or more generally
                    // there is very, very little empty space around textures next to the border of the atlas, an out of bounds
                    // setPixelColour call is done. It is unclear why this is the case.
                    // Since the size of the diffuseTexture == SpecularTexture, only one check against one image is done.
                    if((xDrawingPosition + x) >= diffuseTexture.width() || (yDrawingPosition + y) >= diffuseTexture.height())
                    {
                        throw std::runtime_error{
                                                 "There was an error writing the atlas to disk.\n"
                                                 "It is possible that the atlas dimensions along with the placement\n"
                                                 "of the textures within are causing this error.\n Try resizing the atlas to a larger size."};

                    }

                    diffuseTexture.setPixelColor(xDrawingPosition + x, yDrawingPosition + y, i.texture->getImage(::TextureLogic::Zoom::Normal).pixelColor(x, y));
                    specularTexture.setPixelColor(xDrawingPosition + x, yDrawingPosition + y, i.texture->getSpecularTexture(::TextureLogic::Zoom::Normal).pixelColor(x, y));
                }
            }
        }

        // The selected existing texture, if any existing texture is selected, is not held in the textureDrawingPositions variable.
        // In order for it to be drawn, it must be added to the exported atlas separately. Exporting an atlas only writes the textures
        // that were added to the atlas, and as such the selectedTexture is not considered for exporting
        if(selectedExistingTexture->isOpen())
        {
            float xDrawingPosition = selectedExistingTexture->getDrawingCoordinates().x() * zoomFactor;

            float yDrawingPosition = selectedExistingTexture->getDrawingCoordinates().y() * zoomFactor;

            QString localTextureLocation =  '/' + selectedExistingTexture->getImageForDrawing().textureName() + '.' + selectedExistingTexture->getImageForDrawing().textureFormat();

            float topLeftU_Coord = xDrawingPosition / atlasSize.width();
            float topLeftV_Cord =  1.f - (yDrawingPosition / atlasSize.height()); // Because in OpenGL, the top of a texture is when V = 1.

            float bottomRightU_Coord = (xDrawingPosition + selectedExistingTexture->getImageForDrawing().getImage(::TextureLogic::Zoom::Normal).width()) / atlasSize.width();
            float bottomRightV_Coord = (yDrawingPosition + selectedExistingTexture->getImageForDrawing().getImage(::TextureLogic::Zoom::Normal).height()) / atlasSize.height();

            saveStream << localTextureLocation << ": "
                       << topLeftU_Coord << " , " << bottomRightV_Coord << " | " // Bottom Left Corner
                       << topLeftU_Coord << " , " << topLeftV_Cord << " | "      // Top Left Corner
                       << bottomRightU_Coord << " , " << topLeftV_Cord << " | " // Top Right Corner
                       << bottomRightU_Coord << " , " << bottomRightV_Coord << '\n'; // Bottom Right Corner

            for(int x = 0; x < selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal).size().width(); ++x)
            {
                for(int y = 0; y <  selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal).size().height(); ++y)
                {
                    // Same reasoning as the exporting of a non-selected texture.
                    if((xDrawingPosition + x) >= diffuseTexture.width() || (yDrawingPosition + y) >= diffuseTexture.height())
                    {
                        throw std::runtime_error{
                                "There was an error writing the atlas to disk.\n"
                                "It is possible that the atlas dimensions along with the placement\n"
                                "of the textures within are causing this error.\n Try resizing the atlas to a larger size."};
                    }

                    diffuseTexture.setPixelColor(xDrawingPosition + x, yDrawingPosition + y,
                            selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal).pixelColor(x, y));

                    specularTexture.setPixelColor(xDrawingPosition + x, yDrawingPosition + y,
                            selectedExistingTexture->getImageForDrawing().getSpecularTexture(::TextureLogic::Zoom::Normal).pixelColor(x, y));
                }
            }
        }

        if(!(diffuseTexture.save(exportLocation) && specularTexture.save(exportLocationSpecular)))
        {
            throw std::runtime_error{"Failed to export the texture atlas.\n The atlases could not be saved!"};
        }
    }

    void TextureAtlas::keyPressed(int keyID)
    {
        // Remove the selected existing texture by making the variable holding the selected texture empty.
        // Since the selected existing texture is not part of textureDrawingPositions when it was selected,
        // this operation effectively removes the texture from the atlas as it is not added back to textureDrawingPositions.
        if(keyID == Qt::Key_Delete && selectedExistingTexture->isOpen())
        {
            selectedExistingTexture->getImage();

            if(!inTestingMode)
            {
                textureBank->textureSelected(nullptr);
            }
        }
        else if(keyID == Qt::Key_Escape && selectedTexture->isOpen())
        {
            selectedTexture->getImage();
        }
    }

    void TextureAtlas::mouseClicked(int mouseX, int mouseY, int mouseButton)
    {
        leftMouseButtonDown = mouseButton == Qt::LeftButton;

        bool deSelectTexture = false;

        // Block in if-statement refers to determining if selectedExistingTexture should be unselected or moved around (actually moving texture around is done in mouseMoved() )
        if(selectedExistingTexture->isOpen())
        {
            // Check if cursor is over the selected existing texture, if there is one.
            if(mouseX >= selectedExistingTexture->getDrawingCoordinates().x() && mouseX <= selectedExistingTexture->getDrawingCoordinates().x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).width())
            {
                if(mouseY >= selectedExistingTexture->getDrawingCoordinates().y() && mouseY <= selectedExistingTexture->getDrawingCoordinates().y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).height())
                {
                    // If an existing texture is selected, and at the time of the cursor click the cursor is over the texture,
                    // then make a note that when the cursor is released, execute the logic that may result in the cursor being unselected.
                    ignoreMouseRelease = false;

                    // If the texture has been clicked, there is a chance it will be dragged (moved). However, it may be placed in an invalid
                    // location, which case it has to be returned to its position at the time of being clicked. This assignment keeps track of that position.
                    previousDrawingCoords = selectedExistingTexture->getDrawingCoordinates();

                    previousTexturePositionHistory = selectedExistingTexture->getDrawingCoordinates();

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

        if(!inTestingMode)
        {
            textureBank->textureSelected(nullptr);
        }

        // Block of code in if-statement refers to selecting an existing texture within the atlas
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

                // Determine if cursor position is over any existing texture
                if(mouseX >= i.drawingPosition.x() && mouseX <= i.drawingPosition.x() + currentTextureWidth)
                {
                    if(mouseY >= i.drawingPosition.y() && mouseY <= i.drawingPosition.y() + currentTextureHeight)
                    {
                        if(!inTestingMode)
                        {
                            // Restriction is in-place that only textures that fit entirely within the viewport can be selected.
                            // This ensures that checks on valid cursor positions with an open new selected texture remain valid.
                            if (atlasWidget->checkTextureNotWithinViewPort(i.texture->getImage(currentZoom).size()))
                            {
                                return;
                            }
                        }

                        // Put back the existing selected texture, if it exists, so that a new texture can be selected
                        if(selectedExistingTexture->isOpen())
                        {
                            selectedExistingTexture->setDrawSelectedSurroundingBorder(false);
                            addTexture(selectedExistingTexture);
                        }

                        // Initialize the selectedExistingTexture reference with information about the texture that was clicked;
                        // the reference is drawing the exact same texture in the same place, so the texture data and drawing position
                        // has to be identical
                        selectedExistingTexture->setTexture(*i.texture, i.index);

                        selectedExistingTexture->move(i.drawingPosition.x() + i.texture->getImage(currentZoom).width() / 2,
                                              i.drawingPosition.y() + i.texture->getImage(currentZoom).height() / 2,
                                              atlasSize);

                        // The texture that was clicked on was meant to be selected, so visually show it as selected.
                        selectedExistingTexture->setDrawSelectedSurroundingBorder(true);

                        // Update the variable that holds where existing selected texture should go if it is placed on
                        // another texture. This variable will change when the texture is placed on a new valid spot in the atlas.
                        previousDrawingCoords = i.drawingPosition;

                        // When the mouse is released is when the logic to place an existing selected texture is executed.
                        // However, then what will happen is that as soon as the texture is selected through a click, it
                        // will be unselected when the same mouse click is released. Thus for the click that determines
                        // to select a texture, ignore (only) the next mouse release so that the user can release the mouse
                        // without deselecting the texture.
                        ignoreMouseRelease = true;

                        deleteTextureDrawingPosition = true;

                        if(!inTestingMode)
                        {
                            textureBank->textureSelected(i.texture);
                        }

                        previousTexturePositionHistory = selectedExistingTexture->getDrawingCoordinates();
                    }
                }

                if(!deleteTextureDrawingPosition)
                {
                    deleteIndex += 1;
                }
            }

            // The selected existing texture is no longer part of the textureDrawingPositions and thus must be removed
            // from that variable in order for the texture to be drawn according to where it should be as the it is moved
            // as a selected existing texture. if it is not removed, the texture will be drawn in two places- where it was
            // when it was selected, and its new position as a selected existing texture.
            if(deleteTextureDrawingPosition)
            {
                textureDrawingPositions.erase(textureDrawingPositions.begin() + deleteIndex);
            }
        }
    }

    void TextureAtlas::mouseMoved(int mouseX, int mouseY)
    {
        // If the selected texture is not an existing texture, then the selected texture will always move with the cursor.
        // Otherwise, it will only move the left mouse button is pressed.

        if(selectedTexture->isOpen())
        {
            selectedTexture->move(mouseX, mouseY, atlasSize);

            checkIntersection();
        }
        else if(selectedExistingTexture->isOpen())
        {
            // If the left mouse is not down when an existing texture is selected, then the user is not trying to drag
            // the texture. In that case, no action has to be done if the mouse moves.
            if(!leftMouseButtonDown)
            {
                return;
            }

            // If it is the first time the mouse is being dragged after clicking the left mouse, then centre the cursor
            // over the centre of the image. This is to prevent the texture from jumping during the first event of the mouse moving,
            // Remember that when moving the texture, is it relative to the centre of the texture.
            // TODO: In some cases the texture that is selected jumps around when it is moved initially. Would be nice, though not required, if it was fixed.
            if(!ignoreMouseRelease)
            {
                QPointF drawingCoords = selectedExistingTexture->getDrawingCoordinates();
                auto& currentImage = selectedExistingTexture->getImageForDrawing().getImage(currentZoom);

                newMousePosition = QPoint(drawingCoords.x() + currentImage.width() / 2, drawingCoords.y() + currentImage.height() / 2);

                if(!inTestingMode)
                {
                    atlasWidget->moveMouseTo(newMousePosition.x(), newMousePosition.y());
                }

                // If the user has dragged the mouse, then don't automatically deselect the texture.
                // That would be annoying if that happened.
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
        // A new texture is only added to the atlas after the user release the left mouse button.
        // This prevents allows user to still drag the texture after clicking, which seems more intuitive.
        if(leftMouseButtonDown && selectedTexture->isOpen())
        {
            addTexture(selectedTexture);
        }

        // Deselecting an existing texture can only be done by releasing the same button used to select a texture- left mouse button.
        if(leftMouseButtonDown && selectedExistingTexture->isOpen())
        {
            if(intersectionOccured)
            {
                // If the texture was released over another texture, then it has to be moved back to its original location
                // before the drag occurred. Note that the previousDrawingCoords are set whenever a new existing texture is selected
                // and every time an existing selected texture is clicked.
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

                        if(!inTestingMode)
                        {
                            textureBank->textureSelected(nullptr);
                        }
                    }
                }
            }
            else if(selectedExistingTexture->getDrawingCoordinates() != previousTexturePositionHistory)
            {
                textureMovementHistory.push(TextureMovement{selectedExistingTexture->getTextureLocation(), previousTexturePositionHistory});
            }
        }

        if(mouseButton == Qt::LeftButton)
        {
            leftMouseButtonDown = false;
        }
    }

    bool TextureAtlas::newIntersectionBorderWidthValid(TextureLogic::Texture *texture)
    {
        unsigned int intersectionBorderWidth = texture->getIntersectionBorderWidth(TextureLogic::Zoom::Normal);

        bool foundTexture = false;

        // The passed texture whose intersection border width was changed should have been selected when the border width
        // was changed. Check to make sure this is the case, and that the currently selected texture is the same texture
        // whose intersection border width was changed.
        if(selectedExistingTexture->isOpen())
        {
            if(texture == &selectedExistingTexture->getImageForDrawing())
            {
                QPointF previousSelectedDrawingCoords = selectedExistingTexture->getDrawingCoordinates();

                selectedExistingTexture->setTexture(*texture, selectedExistingTexture->getTextureIndex());

                selectedExistingTexture->setDrawingCoordinates(previousSelectedDrawingCoords);

                foundTexture = true;
            }

            // There is a serious logic failed if the current selected texture is not the same texture whose intersection border width
            // was changed, as the option to change the border width is only present if a texture is selected and the check is done immediately.
            if(!foundTexture)
            {
                if(!inTestingMode)
                {
                    ASSERT_SPECIFY_PARENT(atlasWidget, false, __PRETTY_FUNCTION__,"Unable to find the selected texture passed into this function!");

                    return false;
                }
            }

            // The intersection width must not pass the borders of the atlas, as that logically does not make sense.
            // To ensure this, calculate if the intersectionBorderWidth on all four sides of the texture is greater than
            // the distance between the texture and the atlas border in the respective direction.
            if(selectedExistingTexture->getDrawingCoordinates().x() < intersectionBorderWidth)
            {
                return true;
            }

            if(selectedExistingTexture->getDrawingCoordinates().y() < intersectionBorderWidth)
            {
                return true;
            }

            // To calculate the empty area to the right and bottom of the texture, its bottom-right corner is needed.
            // Remember that the empty area is calculated with respect to the current atlas size, which changes with the zoom value.
            // This is done as the user is selecting a border width when there viewing at a certain zoom, so it makes sense
            // to keep all calculations at the same zoom level.
            QPointF bottomRightCorner = QPointF{selectedExistingTexture->getDrawingCoordinates().x() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).size().width(),
                                              selectedExistingTexture->getDrawingCoordinates().y() + selectedExistingTexture->getImageForDrawing().getImage(currentZoom).size().height()};

            unsigned int emptyAreaRight = std::max(atlasSize.width() - bottomRightCorner.x(), 0.0);
            unsigned int emptyAreaBottom = std::max(atlasSize.height() - bottomRightCorner.y(), 0.0);

            if(emptyAreaRight < intersectionBorderWidth || emptyAreaBottom < intersectionBorderWidth)
            {
                return true;
            }
        }

        // To ensure that the checking of an intersection is valid, a texture has to be a "selected texture". To avoid
        // moving the modified texture in the texture drawing position to the "selected texture" which could be hard,
        // that texture is temporarily also a "selected texture".
        ignoreCheckingExistingTexture = true;
        setSelectedTexture(*texture);
        ignoreCheckingExistingTexture = false;

        // Determine if the new intersection border width results in intersections with other textures in the atlas
        bool intersectionOccuered = checkIntersection();

        // "Remove" the temporarily created selected texture.
        selectedTexture->getImage();

        return !intersectionOccuered;
    }

    void TextureAtlas::removeTexture(const TextureLogic::Texture *texture)
    {
        auto textureLocation = std::find_if(textureDrawingPositions.begin(), textureDrawingPositions.end(), [texture](const TextureDrawingPosition &drawingPosition)
        {
            return drawingPosition.texture == texture;
        });

        // The checks to see if the selected textures are the texture that have to be deleted just an additional check
        // to ensure program correctness. The last branch- with the assert- should not be reached.
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

        unsavedChanges = true;
    }

    bool TextureAtlas::resizeAtlas(QSize newAtlasSize) const
    {
        selectedTexture->getImage();

        // The atlas widget will be resized in this function, which in turn will resize the atlas. However, the atlas will
        // be resized to the scaled new size to match what the size should be at the current zoom. The checks to see if
        // any textures will be affected are done at the Normal zoom level, as it is easier to think about. Thus the
        // size used to resize the atlas and the size used to check if the resize can be done are different, and so a
        // separate variable is required.
        QSize originalRequestedSize = newAtlasSize;

        float zoomFactor = TextureLogic::GetZoomValue(TextureLogic::Zoom::Normal) / TextureLogic::GetZoomValue(currentZoom);

        // Adjusted the atlas size to match the scaled size at the current zoom.
        originalRequestedSize /= zoomFactor;

        // Go through each texture, and check if the new size would result in textures not being shown in the new atlas.
        // This is a problem if the atlas size is being reduced.
        for(const auto &i : textureDrawingPositions)
        {
            float xDrawingPosition = i.drawingPosition.x() * zoomFactor;

            float yDrawingPosition = i.drawingPosition.y() * zoomFactor;

            QSize textureSize = i.texture->getImage(::TextureLogic::Zoom::Normal).size();

            if( (xDrawingPosition + textureSize.width() > newAtlasSize.width() ) || ( yDrawingPosition + textureSize.height() > newAtlasSize.height() ))
            {
                return false;
            }
        }

        if(selectedExistingTexture->isOpen())
        {
            float xDrawingPosition = selectedExistingTexture->getDrawingCoordinates().x() * zoomFactor;

            float yDrawingPosition = selectedExistingTexture->getDrawingCoordinates().y() * zoomFactor;

            QSize textureSize = selectedExistingTexture->getImageForDrawing().getImage(TextureLogic::Zoom::Normal).size();

            if( (xDrawingPosition + textureSize.width() > newAtlasSize.width() ) || ( yDrawingPosition + textureSize.height() > newAtlasSize.height() ))
            {
                return false;
            }
        }

        if(!inTestingMode)
        {
            // Resizing the atlas widget will in turn call the required texture atlas function to resize this atlas.
            atlasWidget->setMinimumSize(originalRequestedSize);
            atlasWidget->setMaximumSize(originalRequestedSize);
        }

        return true;
    }

    void TextureAtlas::saveAtlas(const QString &atlasName, const QString &saveLocation) const
    {
        QFile saveFile{saveLocation};

        if(!saveFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            throw std::runtime_error{"Failed to open the specified save location for writing."};
        }

        QTextStream saveStream{&saveFile};

        float zoomFactor = TextureLogic::GetZoomValue(TextureLogic::Zoom::Normal) / TextureLogic::GetZoomValue(currentZoom);

        saveStream << "Atlas Name: " << atlasName << '\n';
        saveStream << "Dimensions: " << atlasSize.width() * zoomFactor << " , " << atlasSize.height() * zoomFactor << '\n';

        try
        {
            saveStream << "Format: " << ::GUI::TextureHelperFunctions::convertToString(atlasFormat) << "\n\n";
        }
        catch(std::runtime_error &e)
        {
            if(!inTestingMode)
            {
                QMessageBox::critical(atlasWidget, "Fatal Internal Error", e.what() +
                                                                           QString{"\n\nNote for this error, you must edit the project file to specify the format of the atlas"} +
                                                                           atlasName, QMessageBox::Ok);
            }
            else
            {
                throw;
            }
        }

        for(const auto &i : textureDrawingPositions)
        {
            float xDrawingPosition = i.drawingPosition.x();

            float yDrawPosition = i.drawingPosition.y();

            xDrawingPosition *= zoomFactor;

            yDrawPosition *= zoomFactor;

            // Save the texture location as a relative path (so that if project folder is moved, loading a project still works)
            QString localTextureLocation = '/' + i.texture->textureName() + '.' + i.texture->textureFormat();

            saveStream << "Texture: " << localTextureLocation << " -> Position: " << xDrawingPosition << " , " << yDrawPosition << '\n';

            // But save an an actual copy of the texture in the folder that the user specified
            QString saveLocationCopy = saveLocation;
            saveLocationCopy.chop(saveLocationCopy.size() - saveLocationCopy.lastIndexOf('/'));

            localTextureLocation = saveLocationCopy + localTextureLocation;

            if(!QFile::exists(localTextureLocation))
            {
                i.texture->getImage(::TextureLogic::Zoom::Normal).save(localTextureLocation);
            }

            localTextureLocation.remove('.' + i.texture->textureFormat());
            localTextureLocation += "_Specular." + i.texture->textureFormat();

            std::string specularSave = localTextureLocation.toStdString();

            if(!QFile::exists(localTextureLocation))
            {
                i.texture->getSpecularTexture(::TextureLogic::Zoom::Normal).save(localTextureLocation);
            }
        }

        if(selectedExistingTexture->isOpen())
        {
            float xDrawingPosition = selectedExistingTexture->getDrawingCoordinates().x() * zoomFactor;

            float yDrawingPosition = selectedExistingTexture->getDrawingCoordinates().y() * zoomFactor;

            // Same location save logic as the textures that are not selected
            QString localTextureLocation =  '/' + selectedExistingTexture->getImageForDrawing().textureName() + '.' + selectedExistingTexture->getImageForDrawing().textureFormat();

            saveStream << "Texture: " << localTextureLocation  << " -> Position: " << xDrawingPosition << " , " << yDrawingPosition << '\n';

            QString saveLocationCopy = saveLocation;
            saveLocationCopy.chop(saveLocationCopy.size() - saveLocationCopy.lastIndexOf('/'));

            localTextureLocation = saveLocationCopy + localTextureLocation;

            if(!QFile::exists(localTextureLocation))
            {
                selectedExistingTexture->getImageForDrawing().getImage(::TextureLogic::Zoom::Normal).save(localTextureLocation);
            }

            localTextureLocation.remove('.' + selectedExistingTexture->getImageForDrawing().textureFormat());
            localTextureLocation += "_Specular." + selectedExistingTexture->getImageForDrawing().textureFormat();

            if(!QFile::exists(localTextureLocation))
            {
                selectedExistingTexture->getImageForDrawing().getSpecularTexture(::TextureLogic::Zoom::Normal).save(localTextureLocation);
            }
        }

        saveStream << "\n\n=======================================\n\n";

        unsavedChanges = false;
    }

    void TextureAtlas::setAtlasSize(QSize size)
    {
        // This function is called when the atlasWidget holding this texture atlas resizes, which happens when the
        // the zoom of the atlas changes
        atlasSize = size;
    }

    void TextureAtlas::setAtlasWidgetReference(GUI::Atlas::AtlasWidget *atlasWidget)
    {
        // This class needs to be able to call a function in atlasWidget. Tried passing a function pointer instead
        // but got compile time errors. It is not required but would increase encapsulation as only access to a specific
        // function is granted rather than the entire class. It is not mandatory though.
        // TODO: change reference to atlasWidget to function pointer pointing to function that needs to be called within atlasWidget

        this->atlasWidget = atlasWidget;
    }

    void TextureAtlas::setSelectedTexture(const TextureLogic::Texture &texture)
    {
        // In a texture atlas, there is only one of each texture. If that texture is already put in the atlas, then it is an error
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

        if(textureLoadedAlready && !ignoreCheckingExistingTexture)
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
        // checkIntersection() function. Since it is no longer selected, its selected border is no longer visible.
        selectedExistingTexture->getSurroundingBorder()[currentZoomIndex].setSelectedBorderVisible(false);
        addTexture(selectedExistingTexture);
    }

    void TextureAtlas::setTextureBankReference(TextureLogic::TextureBank *textureBank)
    {
        if(this->textureBank == nullptr)
        {
            this->textureBank = textureBank;

            textures = &textureBank->getTextures();
        }
    }

    void TextureAtlas::textureLoaded()
    {
        if(!inTestingMode)
        {

            // Texture has been added to the texture bank. Note that when adding a texture to the image, the old vector of textures
            // might be moved in memory due to reallocation of the vector. Therefore references to that vector have to be reset just in case.
            this->textures = &textureBank->getTextures();

            for (auto &i : textureDrawingPositions)
            {
                i.texture = &((*this->textures)[GUI::TextureHelperFunctions::indexFormat(atlasFormat,true)].first[i.index]);
            }

            if (selectedExistingTexture->isOpen())
            {
                selectedExistingTexture->setTextureReference(((*this->textures)[GUI::TextureHelperFunctions::indexFormat(atlasFormat,true)].first[selectedExistingTexture->getTextureIndex()]));
            }
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
            // Only return true if the left mouse button is down as where the cursor can be moved to should be
            // limited only if the texture is being dragged around. If true is always returned, then if a texture
            // is selected the cursor will not be able to move freely as the program will think the user is dragging
            // the texture and take measures to ensure that the "moving" texture cannot be placed past the atlas borders.
            return leftMouseButtonDown;
        }

        return false;
    }

    void TextureAtlas::undoTextureMovement()
    {
        if(textureMovementHistory.empty())
        {
            return;
        }

        for(auto &i : textureDrawingPositions)
        {
            if(i.texture->textureLocation() == textureMovementHistory.top().textureLocation)
            {
                i.drawingPosition = textureMovementHistory.top().previousLocation;
            }
        }

        if(selectedExistingTexture->isOpen() && selectedExistingTexture->getTextureLocation() == textureMovementHistory.top().textureLocation)
        {
            selectedExistingTexture->setDrawingCoordinates(textureMovementHistory.top().previousLocation);
        }

        textureMovementHistory.pop();
    }

    void TextureAtlas::zoomIn()
    {
        TextureLogic::Zoom oldZoom = currentZoom;

        currentZoom = TextureLogic::zoomIn(currentZoom);

        // Restriction is in-place that only textures that fit entirely within the viewport can be selected.
        // This ensures that checks on valid cursor positions with an open new selected texture remain valid.
        // Since the zoomIn increases the effective size of a selected texture, only a zoomIn can cause a texture
        // to no longer be displayed entirely within a viewport.
        if(selectedExistingTexture->isOpen())
        {
            if(!inTestingMode)
            {
                if (atlasWidget->checkTextureNotWithinViewPort(selectedExistingTexture->getImageForDrawing().getImage(currentZoom).size()))
                {
                    currentZoom = oldZoom;

                    return;
                }
            }
        }

        if(oldZoom != currentZoom)
        {
            // Zoom factors can only be zoomed in by one step increments, ie from 100% to 200%, not 100% to 400%.
            // Thus with the zoom options available, zooming in will always have a zoom factor relative to the previous
            // factor of two. Note that TextureLogic::zoomOut will ensure zoom past 400% is never obtained.
            updateTextureDrawingPositions(2.0f);

            currentZoomIndex = ::TextureLogic::GetZoomIndex(currentZoom);

            // Remember: selected textures (both existing and new) are not part of textureDrawingPositions; they have to be
            // updated separately
            updateSelectedTexturesZoom(currentZoom, 2.0f);

            if(!inTestingMode)
            {
                // Tell the widget holding the atlas to visually resize the atlas to match the new zoom value, which will
                // also logically resize the atlas in setAtlasSize()
                atlasWidget->resizeAtlasFactor(2.0f);
            }
        }
    }

    void TextureAtlas::zoomOut()
    {
        TextureLogic::Zoom oldZoom = currentZoom;

        currentZoom = TextureLogic::zoomOut(currentZoom);

        if(oldZoom != currentZoom)
        {
            // Zoom factors can only be zoomed in by one step increments, ie from 100% to 50%, not 100% to 25%.
            // Thus with the zoom options available, zooming in will always have a zoom factor relative to the previous
            // factor of two. Note that TextureLogic::zoomOut will ensure zoom past 25% is never obtained.
            updateTextureDrawingPositions(0.5f);

            currentZoomIndex = ::TextureLogic::GetZoomIndex(currentZoom);

            // Remember: selected textures (both existing and new) are not part of textureDrawingPositions; they have to be
            // updated separately
            updateSelectedTexturesZoom(currentZoom, 0.5f);

            if(!inTestingMode)
            {
                // Tell the widget holding the atlas to visually resize the atlas to match the new zoom value, which will
                // also logically resize the atlas in setAtlasSize()
                atlasWidget->resizeAtlasFactor(0.5f);
            }
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
             *    See textureLoaded()
             */
            textureDrawingPositions.emplace_back();

            textureDrawingPositions.back().drawingPosition = selectedTexture->getDrawingCoordinates();

            textureDrawingPositions.back().texture = &selectedTexture->getImage();

            textureDrawingPositions.back().surroundingBorder = selectedTexture->getSurroundingBorderForDrawing();

            if(!inTestingMode)
            {
                for (unsigned int i = 0; i < textures->size(); ++i)
                {
                    if ((*textures)[GUI::TextureHelperFunctions::indexFormat(atlasFormat,true)].first[i].textureLocation() ==
                        selectedTexture->getTextureLocation())
                    {
                        textureDrawingPositions.back().index = i;

                        break;
                    }
                }
            }

            // Serious issue if the selected texture is not known to exist in the texture bank; this means that within the bounds
            // of the program it no longer exists, yet the fact that it was in selected existing texture implies that it does exist
            if(textureDrawingPositions.back().index == -1)
            {
                if(!inTestingMode)
                {
                    ASSERT_SPECIFY_PARENT(atlasWidget, false, __PRETTY_FUNCTION__, "Selected texture has a location not found in texture bank!");
                }
            }
        }

        unsavedChanges = true;
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
            // For example: if a point is at 50, and the user zooms in, resulting in a zoom factor of 2, the new drawing point is 100
            QPointF oldDrawingCoordinates = i.drawingPosition;

            i.drawingPosition *= factor;

            // The surrounding border uses QRects internally, which only support translations for moving the QRects,
            // meaning the above factor multplication won't work with the texture borders
            QPointF translation = i.drawingPosition - oldDrawingCoordinates;

            for(auto &j: i.surroundingBorder)
            {
                j.translate(translation.x(), translation.y());
            }
        }
    }
}