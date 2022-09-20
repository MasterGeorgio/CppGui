#include "parsetext.h"

#define MIN(a, b) (a < b) ? a : b

ParseText::ParseText()
{
    text = "";
    preText = "";
    pos = 0;
}


bool ParseText::changeSpecSymbol(QString istr)
{
    qint32 length = MIN(MIN(text.length(), istr.length()), pos);
    preText = "";
    for (qint32 i = 0; i < length; i++)
    {
        if (text[i] != istr[i])
        {
            pos = i;
            break;
        }
    }
    // #@RUB@
    const QChar *arStr = istr.constData();
    // курсор на начало символа, с которого будет осуществлять поиск метку #@
    qint32 ppos = 0, lastp = -1;
    // есть изменения текста?
    bool chg = false;
    for (qint32 fnd = istr.indexOf("#@", pos); // поиск первого примера в тексте
         fnd != -1 && lastp != pos; // -1 - означает, что в строке нет (больше) примеров
         fnd = istr.indexOf("#@", pos)
         )
    {
        preText.insert(preText.length(), &arStr[ppos], fnd); // копируем текст до примера
        lastp = pos;
        pos = fnd;
        int r = istr.indexOf("@", fnd + 2);//если есть равенство, то пример завершен
        int space = istr.indexOf(' ', fnd);
        // проверяем, что равно для текущего примера, а не следующего
        if ((r < space || space == -1) && r != -1)
        {
            QString exampl = "";
            exampl.insert(0, &arStr[fnd + 2], r - fnd - 2);
            QString rez = m_specSymbol[exampl];
            preText += rez;
            pos = r + 1;
            chg = true;
        }
        preText.insert(preText.length(), &arStr[pos], istr.length() - pos); // дописываем оставшийся текст
    }
    return chg;
}

bool ParseText::change(QString istr)
{
    qint32 length = MIN(MIN(text.length(), istr.length()), pos);
    preText = "";
    for (qint32 i = 0; i < length; i++)
    {
        if (text[i] != istr[i])
        {
            pos = i;
            break;
        }
    }
    // #@RUB@
    const QChar *arStr = istr.constData();
    qint32 ppos = 0, lastp = -1; // курсор на начало символа, с которого будет осуществлять поиск метку #@
    bool chg = false; // есть изменения текста?
    for (qint32 fnd = istr.indexOf("#@", pos); // поиск первого примера в тексте
         fnd != -1 && lastp != pos; // -1 - означает, что в строке нет (больше) примеров
         fnd = istr.indexOf("#@", pos)
         )
    {
        preText.insert(preText.length(), &arStr[ppos], fnd); // копируем текст до примера
        lastp = pos;
        pos = fnd;
        int r = istr.indexOf('=', fnd);//если есть равенство, то пример завершен
        int space = istr.indexOf(' ', fnd);
        if ((r < space || space == -1) && r != -1) // проверяем, что равно для текущего примера, а не следующего
        {
            QString exampl = "";
            exampl.insert(0, &arStr[fnd + 2], r - fnd - 1);
            QString rez = calc(exampl);
            preText += rez;
            pos = r + 1;
            chg = true;
        }
        preText.insert(preText.length(), &arStr[pos], istr.length() - pos); // дописываем оставшийся текст
    }
    return chg;
}

QString ParseText::calc(QString example)
{
    QString str = example;
    int length = str.length() - 1; // без раввно
    QString operations = "+-*/";
    int amount_oper = 0;
    qint32 posoper[10] = {0}; // пусть программа может обработать 10
    //арифметических операций в примере
    struct LogiExampl{
        qint32 a, b; // аргументы
        unsigned char idOpera; //номер операции
        qint32 *la, *lb; // указатель на результат
        LogiExampl(){la = nullptr, lb = la;}
    };
    LogiExampl logicRes[10];//на десять операций
    for (quint8 i = 1; i < 9; i++)
    {
        logicRes[i].la = &logicRes[i-1].b;
        logicRes[i].lb = &logicRes[i+1].a;
    }
    logicRes[0].lb = &logicRes[1].a;
    logicRes[9].la = &logicRes[8].b;
    QString number = "";
    bool muldiv = false; // наличие умножения и(или) деления
    for (quint8 i = 0; i < length; i++)
    {
        if (operations.indexOf(example[i]) != -1)
        {
            posoper[amount_oper] = i;
            logicRes[amount_oper].a = number.toInt();
            // запоминаем код арифмитической операции
            logicRes[amount_oper].idOpera = operations.indexOf(example[i]);
            if (logicRes[amount_oper].idOpera > 1)
                muldiv = true;

            number.clear();

            // проверяем, что есть предыдущая операция
            if (!(amount_oper - 1) & 0x80000000)
                logicRes[amount_oper - 1].b = logicRes[amount_oper].a;
            amount_oper++;
        } else number += example[i];
    }
    logicRes[amount_oper - 1].b = number.toInt();
    qint32 rez = 0;

    // сначало умножение и деление, если они есть
    if (muldiv)
    {
        for (quint8 i = 0; i < amount_oper; i++)
        {
            if (logicRes[i].idOpera > 1)
            {
                if (logicRes[i].idOpera == 2)
                    rez = logicRes[i].a * logicRes[i].b;
                if (logicRes[i].idOpera == 3)
                    rez = logicRes[i].a / logicRes[i].b;
                // чтобы не делать повторно, когда перейдем к сложению/вычитанию
                logicRes[i].idOpera = 0xFF;

                if (logicRes[i].la)
                    *logicRes[i].la = rez;
                if (logicRes[i].lb)
                    *logicRes[i].lb = rez;
                for (quint8 j = i - 1; j != 0xFF; j--)
                {
                    logicRes[j].b = rez;
                    logicRes[j].lb = logicRes[i].lb;
                }
            }
        }
    }
    //а затем сложение и вычитание
    for (quint8 i = 0; i < amount_oper; i++)
    {
        if (logicRes[i].idOpera == 0)
            rez = logicRes[i].a + logicRes[i].b;
        if (logicRes[i].idOpera == 1)
            rez = logicRes[i].a - logicRes[i].b;
        logicRes[i].idOpera = 0xFF;
        if (logicRes[i].la)
            *logicRes[i].la = rez;
        if (logicRes[i].lb)
            *logicRes[i].lb = rez;
    }
    // добавляем к строке результат
    return str + QString::number(rez);
}

QString ParseText::getText()
{
    text = preText;
    return text;
}
