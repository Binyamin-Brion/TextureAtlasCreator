//
// Created by BinyBrion on 10/15/2019.
//

#include "Zoom.h"

namespace TextureLogic
{
    Zoom zoomIn(Zoom currentZoom)
    {
        switch(currentZoom)
        {
            case TextureLogic::Zoom::Out25:
                currentZoom = TextureLogic::Zoom::Out50;
                break;

            case TextureLogic::Zoom::Out50:
                currentZoom = TextureLogic::Zoom::Normal;
                break;

            case TextureLogic::Zoom::Normal:
                currentZoom = TextureLogic::Zoom::In200;
                break;

            case TextureLogic::Zoom::In200:
                currentZoom = TextureLogic::Zoom::In400;
                break;

            case TextureLogic::Zoom::In400:
                break;

            case TextureLogic::Zoom::ALWAYS_AT_EMD:
                break;
        }

        return currentZoom;
    }

    Zoom zoomOut(Zoom currentZoom)
    {
        switch (currentZoom)
        {
            case TextureLogic::Zoom::Out50:
                currentZoom = TextureLogic::Zoom::Out25;
                break;

            case TextureLogic::Zoom::Normal:
                currentZoom = TextureLogic::Zoom::Out50;
                break;

            case TextureLogic::Zoom::In200:
                currentZoom = TextureLogic::Zoom::Normal;
                break;

            case TextureLogic::Zoom::In400:
                currentZoom = TextureLogic::Zoom::In200;
                break;

            case TextureLogic::Zoom::Out25:
                break;

            case TextureLogic::Zoom::ALWAYS_AT_EMD:
                break;
        }

        return currentZoom;
    }
}