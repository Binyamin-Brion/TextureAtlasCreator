#include "TextureLogic/TextureBank.h"
#include "GUI/MainWindow.h"
#include <QApplication>
#include <QtTest/qtestcase.h>
#include <Tests/TestSuite.h>

int main(int argc, char *argv[]) {

    auto suite = Tests::TestSuite::suite();

    for(const auto i : suite)
    {
        QTest::qExec(i, argc, argv);
    }

    QApplication app{argc, argv};

    GUI::MainWindow mainWindow;

    mainWindow.show();

    return app.exec();
}