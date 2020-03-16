//
// Created by BinyBrion on 2019-09-15.
//

#include "TextureButtonArea.h"

#include "TextureButton.h"
#include "TextureLogic/TextureBank.h"
#include "OptionsMenu.h"
#include <QMouseEvent>

#include <QGridLayout>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTextStream>

namespace GUI
{
    namespace LoadResults
    {
        /*
         *  This widget uses a grid layout. There is a problem though: when buttons are added, they are not added
         *  at a fixed location beside the previously added button. For example, this does not happen:
         *
         *  [button] [button] [button]
         *
         *   * Notice the equal spaces between each button
         *
         *   What happens instead is that buttons are added with large spaces between them, and then as more buttons
         *   are added the space between gets smaller until a fixed space between them exists.
         *
         *   To deal with this issue, the grid layout is populated with empty widgets, where enough are added that
         *   there is a fixed space between them. Then with a texture button is added, the next calculated location
         *   widget is removed, and replaced with the texture button, like so:
         *
         *   [widget] [widget] [widget] ->  * A texture button is added * -> [button] [widget] [widget]
         */

        // Beginning of public functions

        TextureButtonArea::TextureButtonArea(QWidget *parent)
                            :
                                QWidget{parent}
        {
            // Minimum width is set so that there is no horizontal scrolling when the window is made full screen
            setMinimumSize(500, 900);

            // Determine how many buttons can be fit, along with spacing, given the above minimum width
            maxColumnCount = minimumWidth() / TextureButton::BUTTON_SIZE_LENGTH;

            createLayout();

            this->setContextMenuPolicy(Qt::CustomContextMenu);

            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

            // A button area cannot be deleted from a texture button area (through a context menu). The problem is that once the button area
            // is deleted through that action, the 'this' pointer is now invalid, but the control flow returns to the function that requested
            // the button area to be deleted. That function is a slot in 'this' (ie the calling texture button) object- but this object no longer exists.
            // Hence a seg fault would occur.
            optionsMenu = new OptionsMenu{true, false, false, this};

            connect(optionsMenu, &OptionsMenu::moveTabLeftTriggered, [this]() { emit moveTabLeftTriggered(); });

            connect(optionsMenu, &OptionsMenu::moveTabRightTriggered, [this]() { emit moveTabRightTriggered(); });

            connect(optionsMenu, &OptionsMenu::renameTabActionTriggered, [this]() { emit renameTabRequest(); });

            connect(optionsMenu, SIGNAL(deleteTextureButtonTriggered()), this, SLOT(deleteTextureButton()));
        }

        void TextureButtonArea::addTextureButton(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth, bool loadingProject)
        {

            if(!loadingProject && TextureButton::textureNameExists(textureLocation))
            {
                // When saving a project, the names of a texture are used in the saving process. Duplicate textures could mess this up.

                QMessageBox::warning(this, "Texture Already Loaded", "A texture with the name: " + TextureHelperFunctions::getImageName(textureLocation) + " already exists.\n"
                                                                                                                                                           "Rename the texture being loaded.");

                return;
            }

            // Try loading the image first; if the texture is already loaded, then this call has no effect
            textureBank->storeNewTexture(textureLocation, intersectionBorderWidth, selectionBorderWidth, {});

            try
            {
                // Keep this as a separate statement so if this constructor fails, the vecto holding the texture buttons is known to not be affected
                TextureButton *textureButton = new TextureButton{textureLocation, intersectionBorderWidth, selectionBorderWidth, this};

                // The newly created button will be swapped the place holder widget with the newly created place-holder; see the description at the top of
                // this file for more information.
                textureButtons.push_back(textureButton);
            }
            catch(std::runtime_error &e)
            {
                QMessageBox::critical(this, "Failed to load texture", e.what(), QMessageBox::Ok);

                return;
            }

            connect(textureButtons.back(), SIGNAL(buttonClicked(const QString&, unsigned int, unsigned int)), this, SLOT(textureButtonClicked(const QString&, unsigned int, unsigned int)));

            placeTextureButton(textureButtons.back());

            unsavedChanges = true;
        }

        void TextureButtonArea::deleteTextureButtons(bool closingProgram)
        {
            // When closing a project it is taken to be known that all textures will be deleted from the texture atlas. No warning is required as a result.
            if(!closingProgram)
            {
                // Before actually modifying the texture buttons, check if deleting the texture buttons would result in texture
                // being removed from the program. If such a texture(s) exist, ask the user if this function should continue.
                for(const auto &i : textureButtons)
                {
                    int currentTextureCount = TextureButton::getTextureRepresentationCount()[i->getTextureLocation()];

                    if(currentTextureCount == TextureButton::MINIMUM_TEXTURE_REPRESENTATION_COUNT)
                    {
                        int questionResponse = QMessageBox::question(this, tr("Confirmation Required"), "Deleting this texture area will results in texture being permanently deleted. "
                                                                                                        "All instances of this texture in ALL atlases will be removed. \n\n"
                                                                                                        "\nContinue, and apply this response to all other textures that would be deleted if this operation finishes?",
                                                                     QMessageBox::Yes | QMessageBox::Cancel);

                        if(questionResponse != QMessageBox::Yes)
                        {
                            return;
                        }
                    }
                }
            }

            for(auto &i : textureButtons)
            {
                // Only delete the texture if there is only one more texture button representing the texture
                if(TextureButton::getTextureRepresentationCount()[i->getTextureLocation()] == TextureButton::MINIMUM_TEXTURE_REPRESENTATION_COUNT)
                {
                    textureBank->removeTexture(i->getTextureLocation());
                }

                // Texture button that represents button no longer exists; update count accordingly
                TextureButton::decrementTextureRepresentation(i->getTextureLocation());
            }
        }

        bool TextureButtonArea::getUnsavedChanges() const
        {
            return unsavedChanges;
        }

        void TextureButtonArea::mousePressEvent(QMouseEvent *event)
        {
            // When a right-click occurs, before offering the option of deleting a texture button in the context menu,
            // first it must be seen if the cursor is actually over a button. If it is, only then offer an option to delete
            // a texture button.
            if(event->button() == Qt::RightButton)
            {
                cursorOverButtonIndex = -1;
                bool foundTexture = false;

                int index = 0;

                for(const auto &i : textureButtons)
                {
                    if(i->mouseOver(event->pos()))
                    {
                        cursorOverButtonIndex = index;

                        optionsMenu->showTextureButtonDeleteAction(true);

                        foundTexture = true;

                        break;
                    }

                    index += 1;
                }

                if(!foundTexture)
                {
                    optionsMenu->showTextureButtonDeleteAction(false);
                }
            }
        }

        void TextureButtonArea::paintTextureButton(const ::TextureLogic::Texture *texture)
        {
            for(auto &i : textureButtons)
            {
                if(i->getTextureLocation() == texture->textureLocation())
                {
                    i->updateTextureButton(texture);
                }
            }
        }

        void TextureButtonArea::saveLoadedTextures(const QString &textureButtonAreaName, const QString &saveLocation) const
        {
            QFile saveFile{saveLocation};

            if(!saveFile.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                throw std::runtime_error{"Failed to open the specified save location for writing."};
            }

            QTextStream saveStream{&saveFile};

            saveStream << "Texture Button Area: " << textureButtonAreaName << "\n\n";

            for(const auto &i : textureButtons)
            {
                // Save textures relative to the folder they are in so that the project folder can be moved around
                QString localTextureLocation = '/' + TextureHelperFunctions::getImageName(i->getTextureLocation()) + '.' + TextureHelperFunctions::getImageFormat(i->getTextureLocation());

                saveStream << localTextureLocation << " -> " << textureBank->getIntersectionWidth(i->getTextureLocation()) << " , "
                           << textureBank->getSelectionWidth(i->getTextureLocation()) << '\n';
            }

            saveStream << "\n=======================================\n\n";

            unsavedChanges = false;
        }

        void TextureButtonArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            // This function logically should only be called once
            if(this->textureBank == nullptr)
            {
                this->textureBank = textureBank;
            }
        }

        void TextureButtonArea::updateTextureButtonsLocation(const QString &previousLocation, const QString &newLocation)
        {
            for(auto &i : textureButtons)
            {
                if(i->getTextureLocation() == previousLocation)
                {
                    i->setTextureLocation(newLocation);
                }
            }
        }

        // Beginning of private slots

        void TextureButtonArea::deleteTextureButton()
        {
            // Only delete the texture if there is only one more texture button representing the texture
            if(TextureButton::getTextureRepresentationCount()[textureButtons[cursorOverButtonIndex]->getTextureLocation()] == TextureButton::MINIMUM_TEXTURE_REPRESENTATION_COUNT)
            {
                int questionResponse = QMessageBox::question(this, tr("Confirmation Required"), "Are you sure you wish to delete this texture button? \n"
                                                                                                "\n This will remove all instances of this texture in all atlases!",
                                                             QMessageBox::Yes | QMessageBox::Cancel);

                if(questionResponse != QMessageBox::Yes)
                {
                    return;
                }

                textureBank->removeTexture(textureButtons[cursorOverButtonIndex]->getTextureLocation());
            }

            // Texture button that represents button no longer exists; update count accordingly
            TextureButton::decrementTextureRepresentation(textureButtons[cursorOverButtonIndex]->getTextureLocation());

            // Logically makes sense to delete the button now rather than wait until this button area is deleted
            delete textureButtons[cursorOverButtonIndex];

            textureButtons.erase(textureButtons.begin() + cursorOverButtonIndex);

            // Remember that any texture button at an arbitrary place could be deleted. Because of this, it is easier to
            // just recreate the layout for the buttons.
            for(auto &i : textureButtonPlaceHolders)
            {
                // Don't wait until this button area is deleted for memory to be released
                delete i;
            }

            textureButtonPlaceHolders.clear();

            // Recreate the layout and add the other buttons that still remain into the layout
            createLayout();

            for(auto &i : textureButtons)
            {
                placeTextureButton(i);
            }

            unsavedChanges = true;
        }

        void TextureButtonArea::showContextMenu(const QPoint &pos)
        {
            optionsMenu->exec(mapToGlobal(pos));
        }

        void TextureButtonArea::textureButtonClicked(const QString &textureLocation, unsigned int intersectionBorderWidth, unsigned int selectionBorderWidth)
        {
            textureBank->textureButtonPressedButtonArea(textureLocation, intersectionBorderWidth, selectionBorderWidth, {});
        }

        // Beginning of private functions

        void TextureButtonArea::addTextureButtonPlaceHolders(int addRows)
        {
            // Starting at the last row with place holders, add more of the place holders. Newly added buttons will
            // be placed in the place holders created here

            for(int i = currentRow; i <= addRows; ++i)
            {
                for(int j = 0; j < maxColumnCount; ++j)
                {
                    textureButtonPlaceHolders.push_back(new QWidget{this});

                    gridLayout->addWidget(textureButtonPlaceHolders.back(), i, j, Qt::AlignTop | Qt::AlignLeft);
                }
            }

            maxRowCount += addRows;
        }

        void TextureButtonArea::createLayout()
        {
            delete gridLayout;

            currentRow = 0;

            currentColumn = 0;

            maxRowCount = 0;

            gridHorizontalSpacing = (minimumWidth() - (maxColumnCount * TextureButton::BUTTON_SIZE_LENGTH)) / maxColumnCount;

            gridLayout = new QGridLayout;

            setLayout(gridLayout);

            gridLayout->setHorizontalSpacing(gridHorizontalSpacing);

            // By default there are 5 rows of maxColumnCount available slots for textures to be placed
            addTextureButtonPlaceHolders(5);
        }

        void TextureButtonArea::placeTextureButton(TextureButton *button)
        {
            // This will in effect give space to add the button to
            textureButtonPlaceHolders.front()->hide();

            gridLayout->addWidget(button, currentRow, currentColumn, Qt::AlignLeft | Qt::AlignTop);

            // The below erase call will not call the destructor as the vector storing the placeholders stores raw pointers
            delete textureButtonPlaceHolders.front();
            textureButtonPlaceHolders.erase(textureButtonPlaceHolders.begin());

            currentColumn += 1;

            /*
             *  Whenever a button is added, two cases needed to be considered:
             *
             *  1. The max amount of place holders have been used- it is time to add more place holders and update
             *  the current row and column variable so that the next button is added at the next (newly added) place holder
             *
             *  2. The end of the row is reached. Update the current row and column variables so that the next button
             *  is added into the next row
             */

            if(currentColumn == maxColumnCount && (currentRow + 1) == maxRowCount)
            {
                addTextureButtonPlaceHolders(5);

                currentColumn = 0;

                currentRow += 1;
            }

            else if(currentColumn == maxColumnCount && currentColumn % maxColumnCount == 0)
            {
                currentColumn = 0;

                currentRow += 1;
            }
        }
    }
}