//
// Created by binybrion on 3/28/20.
//

#include <QtCore/QDir>
#include <QtTest/QtTest>
#include "../../Atlas/SelectedTexture.h"
#include "../../TextureLogic/Texture.h"
#include "TestSelectedTexture.h"
#include "../EnableTests.h"

#ifdef ENABLE_SELECTED_TEXTURE_TESTS
    static Tests::TextureLogic::TestSelectedTexture testSelectedTexture;
#endif

namespace Tests
{
    namespace TextureLogic
    {
        /*
         *  Note: Only the functionality that is the most complex is tested.
         */

        void TestSelectedTexture::testMove()
        {
            // Remember that the texture being used for this test has dimensions of 200 x 200.

            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::SelectedTexture selectedTexture;

            selectedTexture.setTexture(texture);

            QSize boundaries{400, 400};

            // Try to move the selected texture offscreen to the left. Position is capped at (0, 0).
            selectedTexture.move(-50, -50, boundaries);

            QVERIFY((selectedTexture.getDrawingCoordinates() == QPointF{0, 0}));

            // Try to move the selected texture offscreen to the right. Position is capped to
            // (max boundary width - half texture width, max boundary height - half texture height).
            selectedTexture.move(500, 500, boundaries);

            QVERIFY((selectedTexture.getDrawingCoordinates() == QPointF{200, 200}));

            // A valid position.
            selectedTexture.move(200, 200, boundaries);

            QVERIFY((selectedTexture.getDrawingCoordinates() == QPointF{100, 100}));
        }

        // Beginning of private functions

        QString TestSelectedTexture::getTestAssetFolder() const
        {
            QDir testAssetFolder = QDir::currentPath();

            if(!testAssetFolder.cdUp())
            {
                throw std::runtime_error{"Unable to reach the test asset folder (could not cdUp from current path)!"};
            }

            if(!testAssetFolder.cd("Tests/TextureAtlas/TestAssets"))
            {
                throw std::runtime_error{"Unable to reach the test asset folder (could not cd into the test asset folder)!"};
            }

            return testAssetFolder.path();
        }
    }
}