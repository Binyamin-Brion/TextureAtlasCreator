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
        /**
         * Provides a pop-up dialog to specify the required information to add a new tab to any TabWidget. This dialog
         * is used to simply add a new tab, and nothing else; in other words, to be used to specify a name for a new tab and nothing more.
         * Note that not every TabWidget will necessarily have this dialog if it does not have an option to add a new tab,
         */

        class AddNewTab : public QDialog
        {
                Q_OBJECT

            signals:

                /**
                 *  Emitted when the user chose a valid name for a tab.
                 *
                 * @param newTabName name the new tab should have
                 */
                void newTabNameChosen(QString newTabName);

            public:

                /**
                 * Initializes this object with the parent passed in.
                 *
                 * @param parent widget that has ownership over this object.
                 */
                explicit AddNewTab(QWidget *parent = nullptr);

                /**
                 * Called if a tab is created outside of this dialog, and the name used for that tab has to be registered
                 * with this class to ensure that name remains unique.
                 *
                 * Tabs can be created outside of this dialog during a new project when a default tab is created or
                 * an existing project with tabs is loaded.
                 *
                 * @param tabName name of the tab added outside of this dialog
                 */
                void addNameExistingTab(QString tabName);

                /**
                 * Called if a tab is deleted. When that happens, that tab name is now free to use for another tab.
                 * This dialog must be notifited that name is now free to use.
                 *
                 * @param tabName name of tab that was deleted
                 */
                void removeNameExistingTab(QString tabName);

            private slots:

                /**
                 * Checks if the tab name entered is valid or not. If the tab name is not valid, the line edit
                 * where the name is being entered will turn a shade of red.
                 *
                 * @param tabName to check if it is valid
                 */
                void checkTabNameValid(const QString &tabName);

                /**
                 *  Tells the rest of the program that a new tab name has been chosen. Afterwards, make sure the dialog
                 *  is set to the default state.
                 */
                void newTabNameConfirmed();

            private:

                /**
                  * Resets this dialog to the default state so that no nothing of what was previously entered is shown.
                  */
                void resetDialog();

                Ui::NewTabDialog *ui = nullptr;
                std::vector<QString> existingTabNames;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ADDNEWTAB_H
