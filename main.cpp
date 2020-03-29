#include "TextureLogic/TextureBank.h"
#include "GUI/MainWindow.h"
#include <QApplication>
#include <QtTest/qtestcase.h>
#include <QtCore/QTextStream>
#include <Tests/TestSuite.h>
#include "HelperUtilities/Assert.h"
#include "Tests/EnableTests.h"

int main(int argc, char *argv[]) {

#ifdef ENABLE_TESTS
    QApplication app{argc, argv};

    auto suite = Tests::TestSuite::suite();

    for(const auto i : suite)
    {
        QTest::qExec(i, argc, argv);
    }

    return 0;

#else
    QApplication app{argc, argv};

    GUI::MainWindow mainWindow;

    mainWindow.show();

    mainWindow.setTheme();

    return app.exec();

#endif

}