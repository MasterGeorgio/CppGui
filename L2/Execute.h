#ifndef MathComputation_H
#define MathComputation_H

#include <QApplication>
#include <QtWidgets>

#include "parsetext.h"

class Execute: public QWidget {
    Q_OBJECT

public:
    Execute();

    ~Execute();

private:
    ParseText *parseText;

    QPlainTextEdit *plainTextEdit;

    QTableWidget *table;

private:
    /**
     * @fn     QWidget * createFirstPage()
     * @brief  Создание 1 страницы задачи
     * @return виджет 1 страницы
     */
    QWidget * createFirstPage();
    /**
     * @fn     QWidget * createSecondPage()
     * @brief  Создание 2 страницы задачи
     * @return виджет 2 страницы
     */
    QWidget * createSecondPage();
    /**
     * @fn     QWidget * createThirdPage()
     * @brief  Создание 3 страницы задачи
     * @return виджет 3 страницы
     */
    QWidget * createThirdPage();
};

#endif // MathComputation
