//
// Created by binybrion on 3/29/20.
//

#include <QtCore/QDir>
#include <fstream>
#include <QtTest/qtestcase.h>
#include "../../ProjectLoader/ProjectParser.h"
#include "TestTextureButtonArea.h"
#include "../../GUI/LoadResults/TextureButtonArea.h"

static Tests::GUI::TestTextureButtonArea testLoadedTextures;

namespace Tests
{
    namespace GUI
    {
        void TestTextureButtonArea::testSaveLoadedTextures()
        {
            ::GUI::LoadResults::TextureButtonArea textureButtonArea{true};
            textureButtonArea.addTextureButton(getTestAssetFolder() + "/grassFlora.png", 5, 6, false);

            const QString SAVE_LOCATION = getTestAssetFolder() + "/testSave.tac";

            // Create a file to write to- QFile does create a new file for some reason in this test
            std::ofstream outfile(SAVE_LOCATION.toStdString());

            textureButtonArea.saveLoadedTextures("Default", SAVE_LOCATION);

            ::ProjectLoader::ProjectParser projectParser;

            // Read the saved project file using the same function that the project parser uses when opening
            // a project file in an actual run.
            std::vector<std::string> readContents = projectParser.readFile(SAVE_LOCATION);

            // Have the project parser parse the project file.
            for(const auto &i : readContents)
            {
                // An '=' signifies the end of a texture atlas. This statement is needed as it is not included
                // in the parseAtlas() function in the project parser (it is included in a different function
                // in the project parser that is called in a real run of the program).
                if(i.front() == '=')
                {
                    continue;
                }

                projectParser.parseButtonArea(QString::fromStdString(i));
            }

            // Check what the project parsed is what was expected.

            QVERIFY(projectParser.textureButtonAreaLoader.areaName == "Default");
            QVERIFY(projectParser.textureButtonAreaLoader.textures.size() == 1);
            QVERIFY(projectParser.textureButtonAreaLoader.textures[0].textureLocation == "/grassFlora.png");

            // Note: due to the way TextureButtonArea is coded, these values are hard coded to be written to the saved file.
            QVERIFY(projectParser.textureButtonAreaLoader.textures[0].intersectionWidth == 5);
            QVERIFY(projectParser.textureButtonAreaLoader.textures[0].selectionWidth == 6);

            // Delete the created files so that the next time this test run it does not fail / is affected because the
            // files that would be generated already exists

            QDir dir;
            QVERIFY(dir.remove(SAVE_LOCATION));
        }

        // Beginning of private functions

        QString TestTextureButtonArea::getTestAssetFolder()
        {
            QDir testAssetFolder = QDir::currentPath();

            if(!testAssetFolder.cdUp())
            {
                throw std::runtime_error{"Unable to reach the test asset folder (could not cdUp from current path)!"};
            }

            if(!testAssetFolder.cd("Tests/GUI/TestAssets"))
            {
                throw std::runtime_error{"Unable to reach the test asset folder (could not cd into the test asset folder)!"};
            }

            return testAssetFolder.path();
        }
    }
}
