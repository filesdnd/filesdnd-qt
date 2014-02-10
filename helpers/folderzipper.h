/**************************************************************************************
**
** Copyright (C) 2014 Files Drag & Drop
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**************************************************************************************/

#ifndef FOLDERCOMPRESSOR_H
#define FOLDERCOMPRESSOR_H

#include <QFile>
#include <QObject>
#include <QDir>

/**
 * @class FolderCompressor
 *
 * Compress and decompress folders
 */
class FolderZipper
{
public:
    /**
     * A recursive function that scans all files inside the source folder
     * and serializes all files in a row of file names and compressed
     * binary data in a single file
     */
    static bool compressFolder(QString sourceFolder, QString destinationFile);

    /**
     * A function that deserializes data from the compressed file and
     * creates any needed subfolders before saving the file
     */
    static bool decompressFolder(QString sourceFile, QString destinationFolder);

private:
    /// File used
    static QFile file;
    /// File stream
    static QDataStream dataStream;

    /**
     * Recursive function that compress folder into a single file
     *
     * @param sourceFolder Source folder
     * @param prefex Prefix of the file
     * @return True if it suceed, false otherwise
     */
    static bool compress(QString sourceFolder, QString prefex = "");
};

#endif // FOLDERCOMPRESSOR_H
