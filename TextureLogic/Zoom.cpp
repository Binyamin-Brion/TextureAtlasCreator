//
// Created by BinyBrion on 10/15/2019.
//

#include "Zoom.h"

namespace TextureLogic
{
    Zoom zoomIn(Zoom currentZoom)
    {
        // Calculate the next zoom if zooming in, if there is one.

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

            // No more zoom in values possible.
            case TextureLogic::Zoom::In400:
                break;

            case TextureLogic::Zoom::ALWAYS_AT_EMD:
                break;
        }

        return currentZoom;
    }

    Zoom zoomOut(Zoom currentZoom)
    {
        // Calculate the next zoom if zooming out, if there is one.

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

            // No more zoom out values possible.
            case TextureLogic::Zoom::Out25:
                break;

            case TextureLogic::Zoom::ALWAYS_AT_EMD:
                break;
        }

        return currentZoom;
    }

    std::vector<ZoomPair> getZoomPairs()
    {
        // Convenience function for use with a GUI.

        std::vector<ZoomPair> zoomPairs;

        zoomPairs.push_back(ZoomPair{Zoom::In400, "400%"});
        zoomPairs.push_back(ZoomPair{Zoom::In200, "200%"});
        zoomPairs.push_back(ZoomPair{Zoom::Normal, "100%"});
        zoomPairs.push_back(ZoomPair{Zoom::Out50, "50%"});
        zoomPairs.push_back(ZoomPair{Zoom::Out25, "25%"});

        return zoomPairs;
    }
}