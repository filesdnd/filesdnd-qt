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

#include "fonthelper.h"

#include <QFontDatabase>
#include <QStringList>

FontHelper::FontHelper()
{
}

QFont FontHelper::fontFromFile(const QString &fontFile) {
    QFont font;
    int fontId;
    QStringList loadedFontFamilies;

    fontId = QFontDatabase::addApplicationFont(fontFile);
    loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    if(!loadedFontFamilies.empty()) {
        font = loadedFontFamilies.at(0);
    }

    font.setStyleStrategy(QFont::PreferAntialias);

    return font;
}
