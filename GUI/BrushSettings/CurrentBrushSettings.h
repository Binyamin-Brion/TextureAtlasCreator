//
// Created by BinyBrion on 10/2/2019.
//

#ifndef TEXTUREATLASCREATOR_CURRENTBRUSHSETTINGS_H
#define TEXTUREATLASCREATOR_CURRENTBRUSHSETTINGS_H

#include <QtWidgets/QWidget>
#include <TextureLogic/Zoom.h>

class QLabel;
class QHBoxLayout;
class QPushButton;
class QLineEdit;
class QColorDialog;

namespace PaintFunctions
{
    class Brush;
}

namespace GUI
{
    namespace BrushSettings
    {
        class CurrentBrushSettings : public QWidget
        {
            Q_OBJECT

            public:
                explicit CurrentBrushSettings(QWidget *parent = nullptr);

            public slots:
                void showDifferentBrush(const PaintFunctions::Brush &brush);
                void updateSelectedTextureSize(QSize size, QSize brushSize);
                void zoomChanged(TextureLogic::Zoom newZoom);

            private slots:
                void handleColourButtonPressed();

            private:
                QString createStyleSheet_CurrentBrushColour(QColor colour) const;

                QHBoxLayout *layout = nullptr;

                QLabel *currentBrushColourLabel = nullptr;
                QPushButton *currentBrushColourButton = nullptr;

                QLabel *currentBrushWidthLabel = nullptr;
                QLineEdit *currentBrushWidthLineEdit = nullptr;

                QColorDialog *colourDialog = nullptr;
                bool colourSelected = false;

                PaintFunctions::Brush *brush = nullptr;
                QSize selectedTextureSize;

                TextureLogic::Zoom currentZoom;
        };
    }
}

#endif //TEXTUREATLASCREATOR_CURRENTBRUSHSETTINGS_H
