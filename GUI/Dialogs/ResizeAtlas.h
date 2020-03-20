//
// Created by BinyBrion on 2020-03-20.
//

#ifndef TEXTUREATLASCREATOR_RESIZEATLAS_H
#define TEXTUREATLASCREATOR_RESIZEATLAS_H

#include <QtWidgets/QDialog>

class QLineEdit;

namespace Ui
{
    class ResizeAtlasDialog;
}

namespace GUI
{
    namespace Dialogs
    {
        /**
         * Provides a dialog that allows the user to resize the currently viewed atlas.
         */

        class ResizeAtlas : public QDialog
        {
                Q_OBJECT

            public:

                /**
                 * Initializes the object with the parent passed in and internal connections used by this object.
                 *
                 * @param parent
                 */
                explicit ResizeAtlas(QWidget *parent = nullptr);

                /**
                 * Returns the size entered by the user. The returned size only has meaning if the user pressed the 'Ok" button
                 * which is only available after valid information has been entered. The information the user entered is
                 * reset every time this dialog is shown.
                 *
                 * @return size entered by the user into the dialog
                 */
                QSize getRequestedSize() const;

                /**
                 * Resets the dialog to a known state.
                 *
                 * @param event unused
                 */
                void showEvent(QShowEvent *event) override;

            private slots:

                /**
                 *  Checks if the texture atlas height is valid or not. If the entered dimension is not valid, the line
                 *  edit where the dimension is being entered will turn a shade of red.
                 */
                void handleAtlasHeightSpecified();

                /**
                 *  Checks if the texture atlas height is valid or not. If the entered dimension is not valid, the line
                 *  edit where the dimension is being entered will turn a shade of red.
                 */
                void handleAtlasWidthSpecified();

                /**
                 * Resets this dialog to the default state so that no nothing of what was previously entered is shown.
                 */
                void resetDialog();

            private:
                void atlasDimensionSpecified(QLineEdit *lineEdit, int &dimension);

                Ui::ResizeAtlasDialog *ui = nullptr;

                // Keep track of the new texture atlas dimensions
                int requestedHeight = -1;
                int requestedWidth = -1;
        };
    }
}

#endif //TEXTUREATLASCREATOR_RESIZEATLAS_H
