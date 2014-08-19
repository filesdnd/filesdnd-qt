#ifndef FONTHELPER_H
#define FONTHELPER_H

#include <QString>
#include <QFont>

class FontHelper
{
public:
    FontHelper();

    static QFont fontFromFile(const QString &fontFile);
};

#endif // FONTHELPER_H
