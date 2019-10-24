//
// Created by BinyBrion on 2019-09-14.
//

#ifndef TEXTUREATLASCREATOR_TESTSURROUNDINGBORDER_H
#define TEXTUREATLASCREATOR_TESTSURROUNDINGBORDER_H

#include "Tests/TestSuite.h"

namespace Tests
{
    namespace SurroundingBorder
    {
        class TestSurroundingBorder : public TestSuite
        {
            Q_OBJECT

            private slots:

                /*
                 *  Following two functions make sure that the borders are setup correctly. They are split into
                 *  two tests (even though the parts that are being tested for are in done in function) to make it
                 *  more readable and to help narrow down an issue of an error occurs
                 */
                void testInitializeIntersectionBorder();
                void testInitializeSelectionBorder();

                /*
                 * The same as the above two tests, but after initializing the border they are then translated.
                 * Then it is checked that the borders are in the correct place.
                 */

                void testTranslateInitializationBorder();
                void testTranslateSelectionBorder();

                /*
                 *  Check that the intersection logic works as correctly:
                 */

                void testNoIntersection();

                // One border is moved a little bit so that the majority is intersecting, but not all of it
                void testPartialIntersection();

                // Only one border is intersecting (the right border)- everything else is not intersecting
                void testPartialIntersection2();

                // Two borders on top of one another
                void testEntireIntersection();
        };
    }
}

#endif //TEXTUREATLASCREATOR_TESTSURROUNDINGBORDER_H
