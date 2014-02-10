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

#include "folderzipper.h"

QFile FolderZipper::file;
QDataStream FolderZipper::dataStream;

bool FolderZipper::compressFolder(QString sourceFolder, QString destinationFile)
{
    QDir src(sourceFolder);
    if(!src.exists())
        return false;

    file.setFileName(destinationFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    dataStream.setDevice(&file);

    bool success = compress(sourceFolder);
    file.close();

    return success;
}

bool FolderZipper::compress(QString sourceFolder, QString prefex)
{
    QDir dir(sourceFolder);
    if(!dir.exists())
        return false;

    //1 - list all folders inside the current folder
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Hidden | QDir::Dirs);
    QFileInfoList foldersList = dir.entryInfoList();

    //2 - For each folder in list: call the same function with folders' paths
    for(int i = 0; i < foldersList.length(); i++)
    {
        QString folderName = foldersList.at(i).fileName();
        QString folderPath = dir.absolutePath()+"/"+folderName;
        QString newPrefex = prefex+"/"+folderName;

        compress(folderPath, newPrefex);
    }

    //3 - List all files inside the current folder
    dir.setFilter(QDir::NoDotAndDotDot | QDir::Hidden | QDir::Files);
    QFileInfoList filesList = dir.entryInfoList();
	
	// [FILESDND] For empty folders
	if(filesList.length() == 0) {
		dataStream << QString(prefex + "/");
		dataStream << " ";
	}

    //4- For each file in list: add file path and compressed binary data
    for(int i = 0; i < filesList.length(); i++)
    {
        QFile file(dir.absolutePath() + "/" + filesList.at(i).fileName());
        if(!file.open(QIODevice::ReadOnly))
            return false;

        dataStream << QString(prefex + "/" + filesList.at(i).fileName());
        dataStream << file.readAll();
//        dataStream << qCompress(file.readAll());

        file.close();
    }

    return true;
}

bool FolderZipper::decompressFolder(QString sourceFile, QString destinationFolder)
{
    //validation
    QFile src(sourceFile);
    if(!src.exists())
        return false;

    QDir dir;
    if(!dir.mkpath(destinationFolder))
        return false;

    file.setFileName(sourceFile);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    dataStream.setDevice(&file);

    while(!dataStream.atEnd())
    {
        QString fileName;
        QByteArray data;

        //extract file name and data in order
        dataStream >> fileName >> data;
		
		if(fileName.endsWith("/") == false) 
		{
			//create any needed folder
			QString subfolder;
            for(int i = fileName.length() - 1; i > 0; --i)
			{
				if((QString(fileName.at(i)) == QString("\\")) || (QString(fileName.at(i)) == QString("/")))
				{
					subfolder = fileName.left(i);
					dir.mkpath(destinationFolder+"/"+subfolder);
					break;
				}
			}

            QFile outFile(destinationFolder + "/" + fileName);
			if(!outFile.open(QIODevice::WriteOnly))
			{
				file.close();
				return false;
			}
//            outFile.write(qUncompress(data));
            outFile.write(data);
			outFile.close();
		}
		else
			dir.mkpath(destinationFolder + "/" + fileName);
    }

    file.close();
    return true;
}
