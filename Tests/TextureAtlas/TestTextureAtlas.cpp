//
// Created by binybrion on 3/28/20.
//

#include <QtCore/QDir>
#include <QtTest/QtTest>
#include "TestTextureAtlas.h"
#include "../../Atlas/TextureAtlas.h"
#include "../../Atlas//SelectedTexture.h"
#include "../EnableTests.h"
#include <ProjectLoader/ProjectParser.h>
#include <fstream>

#ifdef ENABLE_TEXTURE_ATLAS_TESTS
    static Tests::TextureAtlas::TestTextureAtlas testTextureAtlas;
#endif

namespace Tests
{
    namespace TextureAtlas
    {
        void TestTextureAtlas::testAddTexture()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            textureAtlas.setSelectedTexture(texture);

            // Check that the texture being added is a "selected texture".
            QVERIFY(textureAtlas.selectedTexture != nullptr);
            QVERIFY(textureAtlas.selectedTexture->isOpen());
            QVERIFY(textureAtlas.selectedTexture->selectedTexture == &texture);

            // While the mouse click does not do anything here, it is a part of the actions a user would do
            // when adding a texture.
            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);

            // Texture should only be added after the mouse is released.
            QVERIFY(textureAtlas.selectedTexture->isOpen());

            textureAtlas.mouseReleased(0, 0, Qt::MouseButton::LeftButton);

            // Check that the texture has been added to the atlas- it is no longer a "selected texture".
            QVERIFY(!textureAtlas.selectedTexture->isOpen());
            QVERIFY(textureAtlas.textureDrawingPositions.size() == 1);
            QVERIFY(textureAtlas.textureDrawingPositions[0].texture == &texture);
            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPointF{0.0f, 0.0f}));
        }

        void TestTextureAtlas::testAddDuplicateTexture()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            // Add the texture the first time.
            textureAtlas.setSelectedTexture(texture);

            QVERIFY(textureAtlas.selectedTexture != nullptr);
            QVERIFY(textureAtlas.selectedTexture->isOpen());
            QVERIFY(textureAtlas.selectedTexture->selectedTexture == &texture);

            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);

            QVERIFY(textureAtlas.selectedTexture->isOpen());

            textureAtlas.mouseReleased(0, 0, Qt::MouseButton::LeftButton);

            QVERIFY(!textureAtlas.selectedTexture->isOpen());

            // Try adding the texture a second time. Adding a duplicate texture results in an exception being thrown.
            try
            {
                textureAtlas.setSelectedTexture(texture);
            }
            catch(std::runtime_error &e)
            {
                goto success;
            }

            QVERIFY(false);

            success:;
        }

        void TestTextureAtlas::testAddIntersectingTextures()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};
            ::TextureLogic::Texture secondTexture{getTestAssetFolder() + "/topGrass.jpeg",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            // Add first texture

            textureAtlas.setSelectedTexture(texture);
            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);
            textureAtlas.mouseReleased(0, 0, Qt::MouseButton::LeftButton);

            // Try to add second texture

            textureAtlas.setSelectedTexture(secondTexture);
            QVERIFY(textureAtlas.selectedTexture->isOpen());

            textureAtlas.mouseMoved(0, 0);

            // After the selected texture has moved, an intersection should occur.
            QVERIFY(textureAtlas.intersectionOccured);

            textureAtlas.mouseReleased(0, 0, Qt::MouseButton::LeftButton);

            // With an intersection occuring, the selected texture should not be placed after the mouse is released.
            QVERIFY(textureAtlas.selectedTexture->isOpen());
        }

        void TestTextureAtlas::testAddTextureWithPosition()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            textureAtlas.addTextureWithPosition(texture, QPoint{100, 100});

            // Check that the texture was added to the atlas in the position as specified.
            QVERIFY(textureAtlas.textureDrawingPositions.size() == 1);
            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPoint{100, 100}));
        }

        void TestTextureAtlas::testRemoveNewSelectedTexture()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            textureAtlas.setSelectedTexture(texture);

            QVERIFY(textureAtlas.selectedTexture->isOpen());

            // Escape key is taken to mean quit adding a selected texture.
            textureAtlas.keyPressed(Qt::Key::Key_Escape);

            // Resulting in there not being a selected texture.
            QVERIFY(!textureAtlas.selectedTexture->isOpen());
        }

        void TestTextureAtlas::testMoveExistingTexture()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            // Add the selected texture to the atlas

            textureAtlas.setSelectedTexture(texture);

            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);
            textureAtlas.mouseReleased(0, 0, Qt::MouseButton::LeftButton);

            // Select the existing texture- it is now a "selected existing texture",
            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);

            QVERIFY(textureAtlas.selectedExistingTexture->isOpen());
            QVERIFY(textureAtlas.selectedExistingTexture->selectedTexture == &texture);

            // Move the texture to a new position. Cursor does not need to be released for the texture to be moved;
            // release the cursor would finalize the new texture position if it was valid.
            textureAtlas.mouseMoved(500, 500);

            // Verify new position of the texture. Note that since the movement is done with the cursor at the centre of the
            // texture, moving the cursor to a position of (500, 500) does not mean the drawing position is the same- the drawing
            // position is done using the top-left corner of the texture. The when selecting the texture above the cursor was implicitly
            // moved to the centre of the texture.
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{400.0f, 400.0f}));
        }

        void TestTextureAtlas::testMoveExistingTextureIntersection()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};
            ::TextureLogic::Texture secondTexture{getTestAssetFolder() + "/topGrass.jpeg",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            textureAtlas.addTextureWithPosition(texture, QPoint{0, 0});
            textureAtlas.addTextureWithPosition(secondTexture, QPoint{500, 500});

            // Try to place first texture on the second texture
            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);
            textureAtlas.mouseMoved(500, 500);

            QVERIFY(textureAtlas.intersectionOccured);

            // An intersection with a different texture is only important if the user wants to place a texture in
            // the chosen location as a final location. Hence the cursor needs to be released to cause a problem.
            textureAtlas.mouseReleased(500, 500, Qt::MouseButton::LeftButton);

            // Since an intersection occurred the new position if the moved texture is invalid; it is placed back to
            // its location before the move occurred.
            QVERIFY((textureAtlas.selectedExistingTexture->getDrawingCoordinates() == QPointF{0.0f, 0.0f}));
        }

        void TestTextureAtlas::testDeleteExistingTexture()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            // Add the texture to the atlas.

            textureAtlas.setSelectedTexture(texture);

            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);
            textureAtlas.mouseReleased(0, 0, Qt::MouseButton::LeftButton);

            // Select the texture- it is now a "selected existing texture".
            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);
            QVERIFY(textureAtlas.selectedExistingTexture->isOpen());
            QVERIFY(textureAtlas.textureDrawingPositions.empty());

            // Delete the existing texture- it no longer is a selected existing texture, nor does it exist in the
            // atlas drawing positions.
            textureAtlas.keyPressed(Qt::Key::Key_Delete);

            QVERIFY(!textureAtlas.selectedExistingTexture->isOpen());
            QVERIFY(textureAtlas.textureDrawingPositions.empty());
        }

        void TestTextureAtlas::testTextureMovementUndo()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            // Add the texture to the atlas

            textureAtlas.setSelectedTexture(texture);

            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);
            textureAtlas.mouseReleased(0, 0, Qt::MouseButton::LeftButton);

            // Move texture to new location
            textureAtlas.mouseClicked(0, 0, Qt::MouseButton::LeftButton);
            textureAtlas.mouseMoved(500, 500);
            textureAtlas.mouseReleased(500, 500, Qt::MouseButton::LeftButton);

            // See note in testMoveExistingTexture() for why drawing position is not (500, 500), ie same as the position
            // specified in the mouseMoved() function.
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{400.0f, 400.0f}));

            // Undo the texture movement. The new position should be the previous position of the texture.
            textureAtlas.undoTextureMovement();

            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{0.0f, 0.0f}));

            // Try undoing another movement- since there is no other movement, nothing should change.
            textureAtlas.undoTextureMovement();

            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{0.0f, 0.0f}));
        }

        void TestTextureAtlas::testZoom()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};
            ::TextureLogic::Texture secondTexture{getTestAssetFolder() + "/topGrass.jpeg",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1920});

            // Note: the atlas itself should change size. This is not tested as the changing of the atlas size
            //       is tied with a GUI component, which is not used here.

            // Add first texture

            textureAtlas.setSelectedTexture(texture);
            textureAtlas.mouseClicked(500, 500, Qt::MouseButton::LeftButton);
            textureAtlas.mouseMoved(500, 500);
            textureAtlas.mouseReleased(500, 500, Qt::MouseButton::LeftButton);

            // Add second texture

            textureAtlas.setSelectedTexture(secondTexture);
            textureAtlas.mouseClicked(1000, 1000, Qt::MouseButton::LeftButton);
            textureAtlas.mouseMoved(1000, 1000);
            textureAtlas.mouseReleased(1000, 1000, Qt::MouseButton::LeftButton);
            textureAtlas.mouseClicked(1000, 1000, Qt::MouseButton::LeftButton);

            QVERIFY(textureAtlas.textureDrawingPositions.size() == 1);
            QVERIFY(textureAtlas.selectedExistingTexture->isOpen());

            // > When zooming out- texture drawing positions are halved.
            // > When zooming in- texture drawing position are increased by a factor of two.
            // > The original coordinates are those the textures were placed at in the above function calls at the
            //   100% zoom level. These coordinates are either increased by a factor of two or multiplied. Again,
            //   see testMoveExistingTexture() to see why the original texture coordinates are not the same as those
            //   specified in the mouseMoved() functions.

            textureAtlas.zoomIn(); // Zoom Level: 200%

            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPointF{800, 800}));
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{1776, 1776}));

            textureAtlas.zoomIn(); // Zoom Level: 400%

            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPointF{1600, 1600}));
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{3552, 3552}));

            textureAtlas.zoomIn(); // Zoom Level: 400%- ensure that the max zoom is indeed 400% and nothing changes

            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPointF{1600, 1600}));
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{3552, 3552}));

            // Currently at zoom = 400%; to get back to 100% (Normal), have to zoom out twice.
            textureAtlas.zoomOut();
            textureAtlas.zoomOut();
            textureAtlas.zoomOut(); // 50%

            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPointF{200, 200}));
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{444, 444}));

            textureAtlas.zoomOut(); // 26%

            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPointF{100, 100}));
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{222, 222}));

            textureAtlas.zoomOut(); // 25% -ensure that the min zoom is indeed 25% and nothing changes

            QVERIFY((textureAtlas.textureDrawingPositions[0].drawingPosition == QPointF{100, 100}));
            QVERIFY((textureAtlas.selectedExistingTexture->drawingCoordinates == QPointF{222, 222}));
        }

        void TestTextureAtlas::testResizeAtlas()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1920});

            textureAtlas.addTextureWithPosition(texture, QPoint{500, 500});

            QVERIFY(textureAtlas.resizeAtlas(QSize{3840, 2160})); // Making atlas bigger should always succeed

            QVERIFY(!textureAtlas.resizeAtlas(QSize{200, 200})); // Making atlas smaller than the current position of any texture
                                                                         // should always fail

            QVERIFY(textureAtlas.resizeAtlas(QSize{1900, 1900})); // Making atlas smaller but not smaller than any position of any texture
                                                                           // should always succeed.
        }

        void TestTextureAtlas::testRemoveTexture()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1920});

            textureAtlas.addTextureWithPosition(texture, QPoint{500, 500});

            QVERIFY(textureAtlas.textureDrawingPositions.size() == 1);

            textureAtlas.removeTexture(&texture);

            QVERIFY(textureAtlas.textureDrawingPositions.empty());
        }

        void TestTextureAtlas::testExportAtlas()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            // Create an atlas with only one texture that is tightly packed- it contains one texture and no border.
            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{200, 200});

            textureAtlas.addTextureWithPosition(texture, QPoint{0, 0});

            // Export the atlas.
            QVERIFY(textureAtlas.exportImage(getTestAssetFolder() + "/ExpectedResults/exportTest.png", getTestAssetFolder() + "/ExpectedResults/exportTestSpecular.png"));

            // Open the exported atlas (along with its specular version) as well as the images representing the images
            // that are expected to be exported.
            QImage openExportTest{getTestAssetFolder() + "/ExpectedResults/exportTest.png"};
            QImage openExportSpecularTest{getTestAssetFolder() + "/ExpectedResults/exportTestSpecular.png"};

            QImage openExpectedExportTest{getTestAssetFolder() + "/ExpectedResults/expectedExport.png"};
            QImage openExpectedExportSpecularTest{getTestAssetFolder() + "/ExpectedResults/expectedExportSpecular.png"};

            // See if the exported images match what was expected.
            QVERIFY(openExportTest == openExpectedExportTest);
            QVERIFY(openExportSpecularTest == openExportSpecularTest);

            // Delete the created files to ensure that next run of this test does not fail / is affected because
            // the files already exist.

            QDir dir;
            QVERIFY(dir.remove(getTestAssetFolder() + "/ExpectedResults/exportTest.png"));
            QVERIFY(dir.remove(getTestAssetFolder() + "/ExpectedResults/exportTestSpecular.png"));

        }

        void TestTextureAtlas::testSaveAtlas()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};
            ::TextureLogic::Texture secondTexture{getTestAssetFolder() + "/topGrass.jpeg",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            // Randomly populate an atlas.
            textureAtlas.addTextureWithPosition(texture, QPoint{0, 0});
            textureAtlas.addTextureWithPosition(secondTexture, QPoint{1000, 1000});

            // Save the atlas to a file.
            const QString SAVE_LOCATION = getTestAssetFolder() + "/ExpectedResults/testSave.tac";

            // Create a file to write to- QFile does create a new file for some reason in this te
            std::ofstream outfile (SAVE_LOCATION.toStdString());;

            textureAtlas.saveAtlas("TestAtlas", SAVE_LOCATION);

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

                projectParser.parseAtlas(QString::fromStdString(i));
            }

            // Check what the project parsed is what was expected.

            QVERIFY(projectParser.textureAtlasLoader.atlasName == "TestAtlas");
            QVERIFY(projectParser.textureAtlasLoader.format == "RGB32");
            QVERIFY((projectParser.textureAtlasLoader.atlasSize == QSize{1920, 1080}));

            QVERIFY(projectParser.textureAtlasLoader.textures.size() == 2);

            QVERIFY((projectParser.textureAtlasLoader.textures[0].position == QPoint{0, 0}));
            QVERIFY(projectParser.textureAtlasLoader.textures[0].textureLocation == "/grassFlora.png");

            QVERIFY((projectParser.textureAtlasLoader.textures[1].position == QPoint{1000, 1000}));
            QVERIFY(projectParser.textureAtlasLoader.textures[1].textureLocation == "/topGrass.jpeg");

            // Delete the created files so that the next time this test run it does not fail / is affected because the
            // files that would be generated already exists.

            QDir dir;
            QVERIFY(dir.remove(getTestAssetFolder() + "/ExpectedResults/testSave.tac"));
            QVERIFY(dir.remove(getTestAssetFolder() + "/ExpectedResults/grassFlora.png"));
            QVERIFY(dir.remove(getTestAssetFolder() + "/ExpectedResults/grassFlora_Specular.png"));
            QVERIFY(dir.remove(getTestAssetFolder() + "/ExpectedResults/topGrass.jpeg"));
            QVERIFY(dir.remove(getTestAssetFolder() + "/ExpectedResults/topGrass_Specular.jpeg"));
        }

        void TestTextureAtlas::testNewIntersectionBorderWidthValid()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};
            ::TextureLogic::Texture secondTexture{getTestAssetFolder() + "/topGrass.jpeg",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{1920, 1080});

            textureAtlas.addTextureWithPosition(texture, QPoint{100, 100});
            textureAtlas.addTextureWithPosition(secondTexture, QPoint{600, 600});

            // Try making the intersection border width encompass another texture- this should fail.
            texture.setIntersectionBorderWidthInternal(1000, ::TextureLogic::Zoom::Normal);
            QVERIFY(!textureAtlas.newIntersectionBorderWidthValid(&texture));

            // Make the intersection border width smaller than what it was originally, which was valid.
            // Thus this new intersection border width should be valid.
            texture.setIntersectionBorderWidthInternal(1, ::TextureLogic::Zoom::Normal);
            QVERIFY(textureAtlas.newIntersectionBorderWidthValid(&texture));
        }

        void TestTextureAtlas::testGetPercentageAtlasUsed()
        {
            ::TextureLogic::Texture texture{getTestAssetFolder() + "/grassFlora.png",5, 6};

            ::Atlas::TextureAtlas textureAtlas{QImage::Format_RGB32, true};
            textureAtlas.setAtlasSize(QSize{800, 800});

            textureAtlas.addTextureWithPosition(texture, QPoint{100, 100});

            QVERIFY(textureAtlas.getPercentageAtlasUsed() == 6);

            // Note: since the atlas size cannot be changed because it is tied with a GUI component,
            //       that is not being tested, zooming the atlas will cause the percentage of the atlas
            //       to increase. This does not happen in the real program, but does test that the relative
            //       size of the textures do change as expected.
            textureAtlas.zoomIn();

            // Width and height of texture increases by a factor two- therefore percentage atlas used will increase
            // by a factor of 4. Note that at 100% the atlas uses 6.25% (rounded to 6), so times four is actually
            // 6.25 * 4 = 25.
            QVERIFY(textureAtlas.getPercentageAtlasUsed() == 25);

            textureAtlas.zoomOut(); // Back to 100% zoom (Normal)
            textureAtlas.zoomOut(); // To 50% zoom

            // Percentage atlas used will decrease by a factor of 4. Thus the original percentage atlas used
            // (6.25) is divided by four to get new result: 6.25 / 4 = 1 (when rounded to an integer).
            QVERIFY(textureAtlas.getPercentageAtlasUsed() == 1);
        }

        // Beginning of private function

        QString TestTextureAtlas::getTestAssetFolder() const
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
