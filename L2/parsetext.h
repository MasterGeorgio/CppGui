#ifndef PARSETEXT_H
#define PARSETEXT_H

#include <QString>
#include <QMap>

class ParseText
{
public:
    ParseText();
    bool changeSpecSymbol(QString);
    bool change(QString);
    QString calc(QString);
    QString getText();
private:
    //Последний текст
    QString text;
    //Текст с правкой примера
    QString preText;
    //позиция указателя поиска
    qint32 pos;

    QMap<QString, QString> m_specSymbol = {
        {"RUB", QString("₽")},
        {"EURO", QString("€")},
        {"PROMILE", QString("‰")},
        {"COPY", QString("©")},
        {"RORO", QString("®")}
    };
};


#endif // PARSETEXT_H
