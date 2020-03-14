//
// Created by binybrion on 3/14/20.
//

#ifndef TEXTUREATLASCREATOR_NAMEFORM_H
#define TEXTUREATLASCREATOR_NAMEFORM_H

#include <QtWidgets/QDialog>

namespace Ui
{
    class NameForm;
}

namespace GUI
{
    /**
     *  Provides a dialog to enter a project name for the current project.
     */

    class NameForm : public QDialog
    {
        public:

            /**
             * Constructor that initializes all internal components.
             *
             * @param parent widget that owns this dialog
             */
            explicit NameForm(QWidget *parent = nullptr);

            /**
             * Get the name that the user chose, containing the line edit.
             *
             * @return chosen project name
             */
            QString getChosenName() const;

        private:

            Ui::NameForm *ui = nullptr;
    };
}

#endif //TEXTUREATLASCREATOR_NAMEFORM_H
