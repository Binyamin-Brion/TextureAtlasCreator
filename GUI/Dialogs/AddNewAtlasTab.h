//
// Created by BinyBrion on 10/11/2019.
//

#ifndef TEXTUREATLASCREATOR_ADDNEWATLASTAB_H
#define TEXTUREATLASCREATOR_ADDNEWATLASTAB_H

#include <QtWidgets/QDialog>
#include "GUI/TextureHelperFunctions/TextureFormats.h"

class QLineEdit;

namespace Ui
{
    class NewTextureAtlasDialog;
}

namespace GUI
{
    namespace Dialogs
    {
        /*
         * Provides a pop-up dialog to specify the required information to create a new tab in the AtlasTabWidget,
         * as well as the information to create a new TextureAtlas as a result of adding a new tab to that widget
         */

        class AddNewAtlasTab : public QDialog
        {
            Q_OBJECT

            public:
                explicit AddNewAtlasTab(QWidget *parent = nullptr);
                void addExistingTabName(QString tabName);

            signals:
                void newAtlasInformationSpecified(QString, int, int, QImage::Format);

            private:
                void atlasDimensionSpecified(QLineEdit *lineEdit, int &dimension);
                void checkAllInformationValid();
                void resetDialog();

                Ui::NewTextureAtlasDialog *ui = nullptr;

                std::vector<QString> existingTabNames;

                int requestedHeight = -1;
                int requestedWidth = -1;
                std::vector<::GUI::TextureHelperFunctions::InternalFormatPair> internalFormatPairs;
        };
    }
}

#endif //TEXTUREATLASCREATOR_ADDNEWATLASTAB_H
