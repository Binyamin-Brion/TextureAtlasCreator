//
// Created by BinyBrion on 10/3/2019.
//

#ifndef TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
#define TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H

#include <QtWidgets/QWidget>
#include <TextureLogic/Zoom.h>
#include "ui_selectedTextureInformation.h"
#include "GUI/TextureHelperFunctions/TextureFormats.h"

namespace TextureLogic
{
    class Texture;
}

namespace GUI
{
    namespace TextureInformation
    {
        /*
         *  Holds all of the widgets to show information about the currently selected texture,
         *  as well as perform operations on it.
         */

        class SelectedTextureInformation : public QWidget
        {
            Q_OBJECT

            public:
                explicit SelectedTextureInformation(QWidget *parent = nullptr);
                QImage::Format getSelectedTextureFormat() const;
                void setIntersectionWidthLineEdit(unsigned int previousBorderWidth);
                void selectedTextureModified();
                void setTexture(const TextureLogic::Texture *texture);

            signals:
                void newIntersectionBorderWidth(TextureLogic::Texture*, TextureLogic::Zoom , unsigned int);
                void newSelectionBorderWidth(TextureLogic::Texture*, TextureLogic::Zoom, unsigned int);
                void reuploadTexture(const QString&, const TextureLogic::Texture*, unsigned int, unsigned int);

            private:
                int checkValidBorderWidth(QLineEdit *lineEdit, int maxValue);
                float normalZoomFactorValue(TextureLogic::Zoom zoom) const;
                void resetDefaultLabels();
                void saveImage(const QString &newFileLocation);

                Ui::SelectedTextureInformation *ui = nullptr;
                TextureLogic::Texture *texture = nullptr;

                std::vector<GUI::TextureHelperFunctions::InternalFormatPair> internalFormatPairs;
                std::vector<QString> imageExtensions;

                QImage::Format selectedTextureFormat;

                std::vector<TextureLogic::ZoomPair> zoomPairs;
        };
    }
}

#endif //TEXTUREATLASCREATOR_SELECTEDTEXTUREINFORMATION_H
