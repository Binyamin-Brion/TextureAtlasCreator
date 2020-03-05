//
// Created by BinyBrion on 2019-09-12.
//

#ifndef TEXTUREATLAS_ZOOM_H
#define TEXTUREATLAS_ZOOM_H

#include <QtCore/QString>

// RIP compile times if this header file is modified

namespace TextureLogic
{
    /**
     * Specifies the amount to zoom into an image by, using a factor.
     * For example, zoom in to be twice as close to the image is a factor of 200%.
     *
     * Each enum value holds its index from the start of the enum in the highest 4 bits,
     * and the actual factor value in the lower 12 bits.
     *
     * Doing this allows additional zoom values to be added without changing the code in the
     * appropriate texture classes, as additional zoom values will have the next available index,
     * which the texture classes will loop over.
     *
     * Make sure to add one to the ALWAYS_AT_END value if a new zoom is added (and add new enum value
     * ti AllZoomValues!). Add the new enum value to the end of the enum to reduce work of changing the index values
     * of the other enums. Also, if more than 16 different zoom options are given, and 12 bits cannot encode the
     * zoom factor value, reconsider how many zoom options are being given. Zoom factors of power of 2 should be
     * used as that is what OpenGL mipmapping uses.
     */

    enum class Zoom : unsigned int
    {
        In400 = 0x00000190, // Index 0, Value 400
        In200 = 0x100000C8, // Index 1, Value 200
        Normal = 0x20000064,   // Index 2, Value 100
        Out50 = 0x30000032, // Index 3, Value 50
        Out25 = 0x40000019, // Index 4, Value 25
        ALWAYS_AT_EMD = 5 // Number of preceding enums
    };

    /*
     * Provides easy access to iterate over enum values. Make sure that if a new enum values is added, it is
     * added to this array. Keep the enums listed here in the same order as they are declared for consistency.
     */
    static const Zoom AllZoomValues[] = {Zoom::In400, Zoom::In200, Zoom::Normal, Zoom::Out50, Zoom::Out25};

    /**
     *  Returns the number of enum entries on Zoom
     *
     * @return number of zoom elements in Zoom
     */
    constexpr unsigned int NumberZoomElements()
    {
        return static_cast<unsigned int>(Zoom::ALWAYS_AT_EMD);
    }

    /**
     * Gets the index of the passed in zoom in the Zoom enum definition.
     *
     * @param zoom that needs its index calculated
     * @return index of zoom value in Zoom
     */
    constexpr unsigned int GetZoomIndex(Zoom zoom)
    {
        // Get highest 4 bits holding the index
        return static_cast<unsigned int>(zoom) >> 28u;
    }

    /**
     * Gets the zoom value of the passed in zoom as a number.
     *
     * @param zoom that needs its number equivalent calculated
     * @return number representing the zoom value
     */
    constexpr float GetZoomValue(Zoom zoom)
    {
        // Clear everything but the lower 12 bits that hold the zoom value
        return static_cast<float>((static_cast<unsigned int>(zoom) & 0xFFFu) / 100.0f);
    }

    /**
     * Calculates the next zoom from the current user if the user is zooming in. If the most zoomed in value is passed,
     * then the return result is the same as what was passed in.
     *
     * @param currentZoom from which the next zoom should be found
     * @return next zoom if the user zooms in
     */
    Zoom zoomIn(Zoom currentZoom);

    /**
     * Calculates the next zoom from the current user if the user is zooming out. If the most zoomed out value is passed,
     * then the return result is the same as what was passed in.
     *
     * @param currentZoom from which the next zoom should be found
     * @return next zoom if the user zooms out
     */
    Zoom zoomOut(Zoom currentZoom);

    /**
     *  Helper struct to logically group together a zoom and its string representation.
     */
    struct ZoomPair
    {
        Zoom zoom;
        QString zoomStringRepresentation;
    };

    /**
     * Get all of the possible zooms along with their respective string representation.
     *
     * @return vector zooms and string representation
     */
    std::vector<ZoomPair> getZoomPairs();
}

#endif //TEXTUREATLAS_ZOOM_H
