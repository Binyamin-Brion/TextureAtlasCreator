//
// Created by BinyBrion on 2020-03-09.
//

#include <QtTest/qtestcase.h>
#include <QtCore/QCoreApplication>
#include "TestProjectLoader.h"
#include "../EnableTests.h"
#include "../ProjectLoader/ProjectParser.h"

#ifdef ENABLE_PROJECT_LOADER_TESTS
    static Tests::ProjectLoader::TestProjectLoader testProjectLoader;
#endif

namespace Tests
{
    namespace ProjectLoader
    {
        // Beginning of private slots

        void TestProjectLoader::testParseProjectFile()
        {
            QString testAssetDirectory = getTestAssetDirectory();

            ::ProjectLoader::ProjectParser projectParser;

            projectParser.parseFile(testAssetDirectory + "/test.tac");

            // Texture Button Areas

            QVERIFY(projectParser.getTextureButtonAreas().size() == 1);

            ::ProjectLoader::TextureButtonAreaLoader textureButtonAreaLoader = projectParser.getTextureButtonAreas().front();

            QVERIFY(textureButtonAreaLoader.areaName == "Default");
            QVERIFY(textureButtonAreaLoader.textures.size() == 1);

            QVERIFY(textureButtonAreaLoader.textures[0].textureLocation == "/bark.jpg");
            QVERIFY(textureButtonAreaLoader.textures[0].intersectionWidth == 2);
            QVERIFY(textureButtonAreaLoader.textures[0].selectionWidth == 5);

            // Texture Atlases

            QVERIFY(projectParser.getTextureAtlases().size() == 1);
            ::ProjectLoader::TextureAtlasLoader textureAtlasLoader = projectParser.getTextureAtlases().front();

            QVERIFY(textureAtlasLoader.atlasName == "Default");
            QVERIFY((textureAtlasLoader.atlasSize == QSize{1920, 1080}));
            QVERIFY(textureAtlasLoader.format == "RGB32");
            QVERIFY(textureAtlasLoader.textures.size() == 1);
            QVERIFY(textureAtlasLoader.textures[0].textureLocation == "/bark.jpg");
            QVERIFY((textureAtlasLoader.textures[0].position == QPoint{60, 48}));
        }

        void TestProjectLoader::testParseProjectFile2()
        {
            QString testAssetDirectory = getTestAssetDirectory();

            ::ProjectLoader::ProjectParser projectParser;

            projectParser.parseFile(testAssetDirectory + "/test2.tac");

            QVERIFY(projectParser.getTextureButtonAreas().size() == 2);

            ::ProjectLoader::TextureButtonAreaLoader textureButtonAreaLoader = projectParser.getTextureButtonAreas().front();

            // Texture Button Areas

            QVERIFY(textureButtonAreaLoader.areaName == "Default");
            QVERIFY(textureButtonAreaLoader.textures.size() == 2);
            QVERIFY(textureButtonAreaLoader.textures[0].textureLocation == "/bark.jpg");
            QVERIFY(textureButtonAreaLoader.textures[0].intersectionWidth == 2);
            QVERIFY(textureButtonAreaLoader.textures[0].selectionWidth == 5);
            QVERIFY(textureButtonAreaLoader.textures[1].textureLocation == "/grassFlora.png");
            QVERIFY(textureButtonAreaLoader.textures[1].intersectionWidth == 2);
            QVERIFY(textureButtonAreaLoader.textures[1].selectionWidth == 5);

            textureButtonAreaLoader = projectParser.getTextureButtonAreas()[1];

            QVERIFY(textureButtonAreaLoader.areaName == "te");
            QVERIFY(textureButtonAreaLoader.textures.size() == 2);
            QVERIFY(textureButtonAreaLoader.textures[0].textureLocation == "/bark.jpg");
            QVERIFY(textureButtonAreaLoader.textures[0].intersectionWidth == 2);
            QVERIFY(textureButtonAreaLoader.textures[0].selectionWidth == 5);
            QVERIFY(textureButtonAreaLoader.textures[1].textureLocation == "/dirt.jpg");
            QVERIFY(textureButtonAreaLoader.textures[1].intersectionWidth == 2);
            QVERIFY(textureButtonAreaLoader.textures[1].selectionWidth == 5);

            // Texture Atlases

            QVERIFY(projectParser.getTextureAtlases().size() == 2);
            ::ProjectLoader::TextureAtlasLoader textureAtlasLoader = projectParser.getTextureAtlases().front();

            QVERIFY(textureAtlasLoader.atlasName == "Default");
            QVERIFY((textureAtlasLoader.atlasSize == QSize{1920, 1080}));
            QVERIFY(textureAtlasLoader.format == "RGB32");
            QVERIFY(textureAtlasLoader.textures.size() == 2);
            QVERIFY(textureAtlasLoader.textures[0].textureLocation == "/bark.jpg");
            QVERIFY((textureAtlasLoader.textures[0].position == QPoint{108, 0}));
            QVERIFY(textureAtlasLoader.textures[1].textureLocation == "/dirt.jpg");
            QVERIFY((textureAtlasLoader.textures[1].position == QPoint{764, 48}));

            textureAtlasLoader = projectParser.getTextureAtlases()[1];

            QVERIFY(textureAtlasLoader.atlasName == "test");
            QVERIFY((textureAtlasLoader.atlasSize == QSize{850, 240}));
            QVERIFY(textureAtlasLoader.format == "ARGB32");
            QVERIFY(textureAtlasLoader.textures.size() == 1);
            QVERIFY(textureAtlasLoader.textures[0].textureLocation == "/grassFlora.png");
            QVERIFY((textureAtlasLoader.textures[0].position == QPoint{6, 8}));
        }

        // Beginning of private functions

        QString TestProjectLoader::getTestAssetDirectory()
        {
            QDir executableDirectory = QDir::current();

            if(!executableDirectory.cdUp())
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "\n\nFailure to get to test asset folder.\n\n");
            }

            if(!executableDirectory.cd("Assets/Test"))
            {
                Q_ASSERT_X(false, __PRETTY_FUNCTION__, "\n\nFailure to get to test asset folder.\n\n");
            }

            return executableDirectory.path();
        }
    }
}