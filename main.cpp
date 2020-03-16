#include "TextureLogic/TextureBank.h"
#include "GUI/MainWindow.h"
#include <QApplication>
#include <QtTest/qtestcase.h>
#include <Tests/TestSuite.h>
#include <QtCore/QTextStream>
#include "Tests/EnableTests.h"

int main(int argc, char *argv[]) {

#ifdef ENABLE_TESTS

    auto suite = Tests::TestSuite::suite();

    for(const auto i : suite)
    {
        QTest::qExec(i, argc, argv);
    }

    return 0;

#else
    QApplication app{argc, argv};

    QDir dir = QDir::current();
    dir.cdUp();
    dir.cd("Assets");

    QFile f(dir.path() + "/darkTheme.txt");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream textStream(&f);
        qApp->setStyleSheet(textStream.readAll());
    }

    GUI::MainWindow mainWindow;

    mainWindow.show();

    return app.exec();

#endif

}