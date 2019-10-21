//
// Created by BinyBrion on 10/10/2019.
//

#ifndef TEXTUREATLASCREATOR_ADDNEWTAB_H
#define TEXTUREATLASCREATOR_ADDNEWTAB_H

#include <QtWidgets/QDialog>

namespace Ui
{
    class NewTabDialog;
}

namespace GUI
{
    namespace Dialogs
    {
        class AddNewTab : public QDialog
        {
            Q_OBJECT

            public:
                explicit AddNewTab(QWidget *parent = nullptr);
                void addNameExistingTab(QString tabName);
                void closeEvent(QCloseEvent *event) override;
                void removeNameExistingTab(QString tabName);

            signals:
                void newTabNameChosen(QString newTabName);

            private:

                Ui::NewTabDialog *ui = nullptr;
                std::vector<QString> existingTabNames;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ADDNEWTAB_H
