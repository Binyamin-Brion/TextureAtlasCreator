//
// Created by BinyBrion on 2020-03-16.
//

#ifndef TEXTUREATLASCREATOR_ASSERT_H
#define TEXTUREATLASCREATOR_ASSERT_H

/*
 * This file is used to change assertions to message boxes in release mode. The reason is that ideally no assertions
 * will be visible to the user in the release mode. Though note that not all assertions can be moved to a message box-
 * some are fatal all the time. One such example is indexFormat() in GUI/TextureHelperFunctions/HelperFunctions.cpp.
 *
 * Note that Test cases still use the manual assert as if tests are run, then a user knowing what they are doing is
 * using the program. In that case they should see the assert in debug mode.
 */

#ifdef QT_DEBUG
    #define ASSERT(condition, location, message) Q_ASSERT_X(condition, location, message)
#else
    #include <QMessageBox>

    #define ASSERT(condition, location, message)                                        \
    QMessageBox::critical(this, "Fatal Internal Error",                                 \
    "Error: " + QString{message} + "\n Error Location at: " + QString{location},        \
    QMessageBox::Ok);                                                                   \
    return;
#endif


#ifdef QT_DEBUG
    #define ASSERT_SPECIFY_PARENT(parent, condition, location, message)  Q_ASSERT_X(condition, location, message)
#else
    #include <QMessageBox>

    #define ASSERT_SPECIFY_PARENT(parent, condition, location, message)                         \
    QMessageBox::critical(parent, "Fatal Internal Error",                                       \
    "Error: " + QString{message} + "\n Error Location at: " + QString{location},                \
    QMessageBox::Ok);
#endif

#endif //TEXTUREATLASCREATOR_ASSERT_H
