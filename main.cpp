#include "TextureLogic/TextureBank.h"
#include "GUI/MainWindow.h"
#include <QApplication>
#include <QtTest/qtestcase.h>
#include <Tests/TestSuite.h>
#include <QtCore/QTextStream>

int main(int argc, char *argv[]) {

    auto suite = Tests::TestSuite::suite();

    for(const auto i : suite)
    {
        QTest::qExec(i, argc, argv);
    }

    QApplication app{argc, argv};

    QDir dir = QDir::current();
    dir.cdUp();

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
}