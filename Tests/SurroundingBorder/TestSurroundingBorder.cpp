//
// Created by BinyBrion on 2019-09-14.
//

#include <QtTest/qtestcase.h>
#include "TestSurroundingBorder.h"

#include "Atlas/TextureBorder/SurroundingBorder.h"
#include "Tests/EnableTests.h"

#ifdef ENABLE_SURROUNDING_BORDER_TESTS
    static Tests::SurroundingBorder::TestSurroundingBorder testSurroundingBorder;
#endif

using namespace Atlas::TextureBorder;

namespace Tests
{
    namespace SurroundingBorder
    {
        // Note that the entire area is only for intersection tests- therefore it is NOT used in with the selection border

        void TestSurroundingBorder::testInitializeIntersectionBorder()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            int intersectionBorderWidth = surroundingBorder.intersectionBorderWidth;

            QVERIFY(intersectionBorderWidth == 5);

            QRect topBorder{0, -intersectionBorderWidth, 10, intersectionBorderWidth};
            QRect rightBorder{10, 0, intersectionBorderWidth, 10};
            QRect bottomBorder{0, 10, 10, intersectionBorderWidth};
            QRect leftBorder{-intersectionBorderWidth, 0, intersectionBorderWidth, 10};

            QRect topLeftBorder{-intersectionBorderWidth, -intersectionBorderWidth, intersectionBorderWidth, intersectionBorderWidth};
            QRect topRightBorder{10, -intersectionBorderWidth, intersectionBorderWidth, intersectionBorderWidth};
            QRect bottomRightBorder{10, 10, intersectionBorderWidth,intersectionBorderWidth};
            QRect bottomLeftBorder{-intersectionBorderWidth, 10, intersectionBorderWidth, intersectionBorderWidth};

            QRect entireArea{-intersectionBorderWidth, -intersectionBorderWidth,
                             2 * intersectionBorderWidth + 10, 2 * intersectionBorderWidth + 10};

            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Top)] == topBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Right)] == rightBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Bottom)] == bottomBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Left)] == leftBorder);

            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::TopLeft)] == topLeftBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::TopRight)] == topRightBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::BottomRight)] == bottomRightBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::BottomLeft)] == bottomLeftBorder);

            QVERIFY(surroundingBorder.entireTextureArea == entireArea);
        }

        void TestSurroundingBorder::testInitializeSelectionBorder()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            int selectionBorderWidth = surroundingBorder.selectedBorderWidth;

            QVERIFY(selectionBorderWidth == 2);

            QRect topBorder{0, -selectionBorderWidth, 10, selectionBorderWidth};
            QRect rightBorder{10, 0, selectionBorderWidth, 10};
            QRect bottomBorder{0, 10, 10, selectionBorderWidth};
            QRect leftBorder{-selectionBorderWidth, 0, selectionBorderWidth, 10};

            QRect topLeftBorder{-selectionBorderWidth, -selectionBorderWidth, selectionBorderWidth, selectionBorderWidth};
            QRect topRightBorder{10, -selectionBorderWidth, selectionBorderWidth, selectionBorderWidth};
            QRect bottomRightBorder{10, 10, selectionBorderWidth, selectionBorderWidth};
            QRect bottomLeftBorder{-selectionBorderWidth, 10, selectionBorderWidth, selectionBorderWidth};

            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Top)] == topBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Right)] == rightBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Bottom)] == bottomBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Left)] == leftBorder);

            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::TopLeft)] == topLeftBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::TopRight)] == topRightBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::BottomRight)] == bottomRightBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::BottomLeft)] == bottomLeftBorder);
        }

        void TestSurroundingBorder::testTranslateInitializationBorder()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            int intersectionBorderWidth = surroundingBorder.intersectionBorderWidth;

            QVERIFY(intersectionBorderWidth == 5);

            int translationAmount = 10;

            surroundingBorder.translate(translationAmount, translationAmount);

            QRect topBorder{translationAmount, -intersectionBorderWidth + translationAmount, 10, intersectionBorderWidth};
            QRect rightBorder{10 + translationAmount, translationAmount, intersectionBorderWidth, 10};
            QRect bottomBorder{translationAmount, 10 + translationAmount, 10, intersectionBorderWidth};
            QRect leftBorder{-intersectionBorderWidth + translationAmount, translationAmount, intersectionBorderWidth, 10};

            QRect topLeftBorder{-intersectionBorderWidth + translationAmount, -intersectionBorderWidth + translationAmount, intersectionBorderWidth, intersectionBorderWidth};
            QRect topRightBorder{10 + translationAmount, -intersectionBorderWidth + translationAmount, intersectionBorderWidth, intersectionBorderWidth};
            QRect bottomRightBorder{10 + translationAmount, 10 + translationAmount, intersectionBorderWidth,intersectionBorderWidth};
            QRect bottomLeftBorder{-intersectionBorderWidth + translationAmount, 10 + translationAmount, intersectionBorderWidth, intersectionBorderWidth};

            QRect entireArea{-intersectionBorderWidth + translationAmount, -intersectionBorderWidth + translationAmount,
                             2 * intersectionBorderWidth + 10, 2 * intersectionBorderWidth + 10};

            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Top)] == topBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Right)] == rightBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Bottom)] == bottomBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::Left)] == leftBorder);

            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::TopLeft)] == topLeftBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::TopRight)] == topRightBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::BottomRight)] == bottomRightBorder);
            QVERIFY(surroundingBorder.intersectionBorder[BorderValue(Border::BottomLeft)] == bottomLeftBorder);

            QVERIFY(surroundingBorder.entireTextureArea == entireArea);
        }

        void TestSurroundingBorder::testTranslateSelectionBorder()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            int selectedBorderWidth = surroundingBorder.selectedBorderWidth;

            QVERIFY(selectedBorderWidth == 2);

            int translationAmount = 10;

            surroundingBorder.translate(translationAmount, translationAmount);

            QRect topBorder{translationAmount, -selectedBorderWidth + translationAmount, 10, selectedBorderWidth};
            QRect rightBorder{10 + translationAmount, translationAmount, selectedBorderWidth, 10};
            QRect bottomBorder{translationAmount, 10 + translationAmount, 10, selectedBorderWidth};
            QRect leftBorder{-selectedBorderWidth + translationAmount, translationAmount, selectedBorderWidth, 10};

            QRect topLeftBorder{-selectedBorderWidth + translationAmount, -selectedBorderWidth + translationAmount, selectedBorderWidth, selectedBorderWidth};
            QRect topRightBorder{10 + translationAmount, -selectedBorderWidth + translationAmount, selectedBorderWidth, selectedBorderWidth};
            QRect bottomRightBorder{10 + translationAmount, 10 + translationAmount, selectedBorderWidth, selectedBorderWidth};
            QRect bottomLeftBorder{-selectedBorderWidth + translationAmount, 10 + translationAmount, selectedBorderWidth, selectedBorderWidth};


            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Top)] == topBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Right)] == rightBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Bottom)] == bottomBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::Left)] == leftBorder);

            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::TopLeft)] == topLeftBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::TopRight)] == topRightBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::BottomRight)] == bottomRightBorder);
            QVERIFY(surroundingBorder.selectedBorder[BorderValue(Border::BottomLeft)] == bottomLeftBorder);
        }

        // Note that intersection tests should give the same result both ways; ie, if A intersects B, then B intersects A

        void TestSurroundingBorder::testNoIntersection()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            ::Atlas::TextureBorder::SurroundingBorder otherSurroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            surroundingBorder.initialize(10, 10, 5, 2);

            surroundingBorder.translate(500, 500);

            QVERIFY(!surroundingBorder.checkIntersection(otherSurroundingBorder));
            QVERIFY(!otherSurroundingBorder.checkIntersection(surroundingBorder));
        }

        void TestSurroundingBorder::testPartialIntersection()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            ::Atlas::TextureBorder::SurroundingBorder otherSurroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            otherSurroundingBorder.initialize(10, 10, 5, 2);

            surroundingBorder.translate(5, 5);

            Q_ASSERT(surroundingBorder.checkIntersection(otherSurroundingBorder));
            Q_ASSERT(otherSurroundingBorder.checkIntersection(surroundingBorder));
        }

        void TestSurroundingBorder::testPartialIntersection2()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            ::Atlas::TextureBorder::SurroundingBorder otherSurroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            otherSurroundingBorder.initialize(10, 10, 5, 2);

            surroundingBorder.translate(15, 5);

            QVERIFY(surroundingBorder.checkIntersection(otherSurroundingBorder));
            QVERIFY(otherSurroundingBorder.checkIntersection(surroundingBorder));
        }

        void TestSurroundingBorder::testEntireIntersection()
        {
            ::Atlas::TextureBorder::SurroundingBorder surroundingBorder;

            ::Atlas::TextureBorder::SurroundingBorder otherSurroundingBorder;

            surroundingBorder.initialize(10, 10, 5, 2);

            otherSurroundingBorder.initialize(10, 10, 5, 2);

            QVERIFY(surroundingBorder.checkIntersection(otherSurroundingBorder));
            QVERIFY(otherSurroundingBorder.checkIntersection(surroundingBorder));
        }
    }
}