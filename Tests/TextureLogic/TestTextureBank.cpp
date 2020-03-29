//
// Created by binybrion on 3/28/20.
//

#include <QtCore/QDir>
#include <QtTest/QtTest>
#include "TestTextureBank.h"

#include "../TextureLogic/TextureBank.h"
#include "../EnableTests.h"

#ifdef ENABLE_TEXTURE_BANK_TESTS
    static Tests::TextureLogic::TestTextureBank testTextureBank;
#endif

namespace Tests
{
    namespace TextureLogic
    {
        /*
         *  Note: Not all of the texture bank's functionality was tested as other parts are heavily tied to GUI components.
         *        What is tested is the core of the texture bank's functionality.
         */

        void TestTextureBank::testLoadNewTexture()
        {
            ::TextureLogic::TextureBank textureBank{true};
            textureBank.loadNewTexture(getTestAssetFolder() + "/grassFlora.png", 5, 6);

            // Texture location should exist in the map holding the locations of all of the loaded textures.
            auto loadedTextureLocation = textureBank.originalTextureUploadLocation.find((getTestAssetFolder() + "/grassFlora.png").toStdString());
            QVERIFY(loadedTextureLocation != textureBank.originalTextureUploadLocation.end());

            // Ensure that the format of the loaded image is RGB32.
            QVERIFY(loadedTextureLocation->second == ::GUI::TextureHelperFunctions::indexFormat(QImage::Format_RGB32, true));

            // In the RGB32 vector in texture bank, ensure that the loaded texture has one loaded texture, and that loaded texture
            // is the test asset texture.
            QVERIFY(textureBank.textures[loadedTextureLocation->second].first.size() == 1);
            QVERIFY(textureBank.textures[loadedTextureLocation->second].first[0].textureLocation() == getTestAssetFolder() + "/grassFlora.png");
            QVERIFY(textureBank.textures[loadedTextureLocation->second].first[0].getIntersectionBorderWidth(::TextureLogic::Zoom::Normal) == 5);
            QVERIFY(textureBank.textures[loadedTextureLocation->second].first[0].getSelectedBorderWidth(::TextureLogic::Zoom::Normal) == 6);
        }

        void TestTextureBank::testRemoveTexture()
        {
            ::TextureLogic::TextureBank textureBank{true};
            textureBank.loadNewTexture(getTestAssetFolder() + "/grassFlora.png", 5, 6);

            auto loadedTextureLocation = textureBank.originalTextureUploadLocation.find((getTestAssetFolder() + "/grassFlora.png").toStdString());

            // Keep track of what the format index of the texture is, as after it is removed from the texture bank this information cannot be obtained from
            // the texture bank anymore.
            unsigned int formatIndex = loadedTextureLocation->second;

            textureBank.removeTexture(getTestAssetFolder() + "/grassFlora.png");

            // The texture bank should no longer keep track of the location of the loaded texture
            loadedTextureLocation = textureBank.originalTextureUploadLocation.find((getTestAssetFolder() + "/grassFlora.png").toStdString());
            QVERIFY(loadedTextureLocation == textureBank.originalTextureUploadLocation.end());

            // The below terminology used refers to the RGB32 format as that is what the format of the test asset texture
            // was when this test was written. It can be whatever the actual format is if a different texture is used.

            // The free spot vector of the RGB32 element should have one element.
            // That element should be the index of the loaded texture in the RGB32 vector.
            // Note that the texture itself in the loaded vector is not removed; it is simply marked as being allowed to be overwritten.
            QVERIFY(textureBank.textures[formatIndex].second.size() == 1);
            QVERIFY(textureBank.textures[formatIndex].second[0] == 0);
        }

        void TestTextureBank::testFindTexture()
        {
            ::TextureLogic::TextureBank textureBank{true};
            textureBank.loadNewTexture(getTestAssetFolder() + "/grassFlora.png", 5, 6);

            // After loading the texture, check if the texture bank could find the texture and ensure that the texture it did find
            // actually refers to the loaded texture.

            const ::TextureLogic::Texture *texture = textureBank.findTexture(getTestAssetFolder() + "/grassFlora.png");

            QVERIFY(texture->textureLocation() == getTestAssetFolder() + "/grassFlora.png");
            QVERIFY(texture->getIntersectionBorderWidth(::TextureLogic::Zoom::Normal) == 5);
            QVERIFY(texture->getSelectedBorderWidth(::TextureLogic::Zoom::Normal) == 6);
        }

        // Beginning of private functions

        QString TestTextureBank::getTestAssetFolder() const
        {
            QDir testAssetFolder = QDir::currentPath();

            if(!testAssetFolder.cdUp())
            {
                throw std::runtime_error{"Unable to reach the test asset folder (could not cdUp from current path)!"};
            }

            if(!testAssetFolder.cd("Tests/TextureLogic/TestAssets"))
            {
                throw std::runtime_error{"Unable to reach the test asset folder (could not cd into the test asset folder)!"};
            }

            return testAssetFolder.path();
        }
    }
}
