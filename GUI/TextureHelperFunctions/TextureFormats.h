//
// Created by BinyBrion on 10/11/2019.
//

#ifndef TEXTUREATLASCREATOR_TEXTUREFORMATS_H
#define TEXTUREATLASCREATOR_TEXTUREFORMATS_H

#include <QString>
#include <QtGui/qimage.h>

namespace GUI
{
    namespace TextureHelperFunctions
    {
        /// The function definition of "internalFormatPairRepresentations" explains the purpose of this struct.
        /// All of other functions here are helper functions related to this struct.
        struct InternalFormatPair
        {
            QImage::Format format;
            QString formatStringRepresentation;
        };

        /**
         * Converts the string representation of the passed in format to the literal Qt enum equivalent.
         *
         * @param imageFormat string representation of format to convert
         * @return equivalent Qt enum representation of passed in format
         */
        QImage::Format convertToFormat(QString imageFormat);

        /**
         *  Converts the Qt enum into an equivalent string representation.
         *
         *  @param imageFormat enum to convert to a string
         *  @return string representation of passed in format
         */
        QString convertToString(QImage::Format imageFormat);

        QString getImageFormat(QString textureLocation);

        QString getImageName(QString textureLocation);

        /**
         * Returns a list of Qt enum formats along with their string representation.
         *
         * @return see function description
         */
        std::vector<InternalFormatPair> internalFormatPairRepresentations();

        /**
         * Returns a list of possible image extensions for an image supported by Qt.
         *
         * @return see function description
         */
        std::vector<QString> listImageExtensions();

        /**
         * Finds the index into the list returned by internalFormatPairRepresentations() for the given format.
         *
         * @param format that the index should refer to
         * @param ignoreInvalidFormat if to assume the list being searched contains the invalid_format enum
         * @return index of the format
         */
        unsigned int indexFormat(QImage::Format format, bool ignoreInvalidFormat = false);

        /**
         * Overload of indexFormat(). Performs the same logic but with a string instead of a QImage::Format.
         *
         * @param formatStringRepresentation see indexFormat()
         * @param ignoreInvalidFormat see indexFormat()
         * @return see indexFormat()
         */
        unsigned int indexFormatString(const QString &formatStringRepresentation, bool ignoreInvalidFormat = false);
    }
}


#endif //TEXTUREATLASCREATOR_TEXTUREFORMATS_H
