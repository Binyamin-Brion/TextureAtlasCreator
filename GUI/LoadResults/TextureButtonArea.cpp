//
// Created by BinyBrion on 2019-09-15.
//

#include "TextureButtonArea.h"

#include "TextureButton.h"
#include "TextureLogic/TextureBank.h"

#include <QGridLayout>

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

        TextureButtonArea::TextureButtonArea(QWidget *parent) : QWidget{parent}
        {
            // Minimum width is set so that there is no horizontal scrolling when the window is made full screen

            setMinimumSize(1860, 900);

            // Determine how many buttons can be fit, along with spacing, given the above minimum width

            maxColumnCount = minimumWidth() / TextureButton::buttonSizeLength;

            gridHorizontalSpacing = (minimumWidth() - (maxColumnCount * TextureButton::buttonSizeLength)) / maxColumnCount;

            gridLayout = new QGridLayout{this};

            gridLayout->setHorizontalSpacing(gridHorizontalSpacing);

            // By default there are 5 rows of maxColumnCount available slots for textures to be placed

            addTextureButtonPlaceHolders(5);
        }

        void TextureButtonArea::addTextureButton(const QString &textureLocation)
        {
            // Try loading the image first; if that operation fails, then there is no point continuing to create
            // a texture pushbutton for that image

            textureBank->storeNewTexture(textureLocation, {});

            // Swap the place holder widget with the newly created plcae holder; see the description at the top of
            // this file for more information.

            textureButtons.push_back(new TextureButton{textureLocation, this});

            gridLayout->removeWidget(textureButtonPlaceHolders.front());

            gridLayout->addWidget(textureButtons.back(), currentRow, currentColumn, Qt::AlignLeft | Qt::AlignTop);

            // The below erase call will not all the destructor as the vector storing the placeholders stores raw pointers

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
             *  is added onto the next row
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

            connect(textureButtons.back(), SIGNAL(buttonClicked(const QString&)), this, SLOT(textureButtonClicked(const QString&)));
        }

        void TextureButtonArea::setTextureBankReference(TextureLogic::TextureBank *textureBank)
        {
            if(textureBank != nullptr)
            {
                return;
            }

            this->textureBank = textureBank;
        }

        void TextureButtonArea::textureButtonClicked(const QString &textureLocation)
        {
            textureBank->textureButtonPressed(textureLocation, {});
        }

        void TextureButtonArea::addTextureButtonPlaceHolders(int addRows)
        {
            // Starting at the last row with place holders, add more of the place holders. Newly added buttons will
            // be placed in the place holders created here

            for(int i = currentRow; i <= addRows; ++i)
            {
                for(int j = 0; j < maxColumnCount; ++j)
                {
                    textureButtonPlaceHolders.push_back(new QWidget);

                    gridLayout->addWidget(textureButtonPlaceHolders.back(), i, j, Qt::AlignTop | Qt::AlignLeft);
                }
            }

            maxRowCount += addRows;
        }
    }
}