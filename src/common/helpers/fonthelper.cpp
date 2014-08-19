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
