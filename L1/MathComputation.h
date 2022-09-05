#ifndef MathComputation_H
#define MathComputation_H

#include <QApplication>
#include <QtWidgets>

class MathComputation: public QWidget {
    Q_OBJECT

    // Страница
    enum PAGE_TASK{
        TASK1,
        TASK2,
        TASK3
    };
    // Поля виджета
    enum leDATA_SOURCE{
        koef_a,
        koef_b,
        koef_c,
        side_a,
        side_b,
        angle_between_ab
    };

public:
    MathComputation() {

        // Создать стек виджетов
        QStackedWidget *stackedWidget = new QStackedWidget;
        stackedWidget->addWidget(createFirstPage());
        stackedWidget->addWidget(createSecondPage());
        stackedWidget->addWidget(createThirdPage());

        // Создать список виджетов
        QListWidget *listDevice = new QListWidget();
        listDevice->addItem(tr("Решить квадр.ур."));
        listDevice->addItem(tr("Найти сторону"));
        listDevice->addItem(tr("Копирование"));

        // Соединить список виджетов со стеком
        connect(listDevice, QOverload<int>::of(&QListWidget::currentRowChanged),
                stackedWidget, &QStackedWidget::setCurrentIndex);

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(listDevice);
        hbox->addWidget(stackedWidget);

        m_teConsole = new QTextEdit();
        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addLayout(hbox);
        vbox->addWidget(m_teConsole);

        setLayout(vbox);
    }

    ~MathComputation(){

    }

private:
    // Поля виджета
    QMap<PAGE_TASK, QMap<leDATA_SOURCE, QLineEdit*>> m_leDataSource;

    // Вывод отладки
    QTextEdit   *m_teConsole;

private:
    /**
     * @fn     QWidget * createFirstPage()
     * @brief  Создание 1 страницы задачи
     * @return виджет 1 страницы
     */
    QWidget * createFirstPage() {
        QWidget *wdgPage = new QWidget;
        m_leDataSource[TASK1][koef_a] = new QLineEdit();
        m_leDataSource[TASK1][koef_b] = new QLineEdit();
        m_leDataSource[TASK1][koef_c] = new QLineEdit();

        QPushButton *pbResolve = new QPushButton("Вычислить");

        connect(pbResolve, &QPushButton::pressed, [=](){
            for(QLineEdit *le: m_leDataSource[TASK1]){
                if (le->text() == "") {
                    m_teConsole->append("Заполните поля");
                    return;
                }

                bool bOk;
                le->text().toDouble(&bOk);
                if (!bOk) {
                    m_teConsole->append("Некорректный ввод");
                    return;
                }
            }

            QPair<QString, QString> res = SolveKvUr(m_leDataSource[TASK1][koef_a]->text().toDouble(),
                                                    m_leDataSource[TASK1][koef_b]->text().toDouble(),
                                                    m_leDataSource[TASK1][koef_c]->text().toDouble());

            if (m_leDataSource[TASK1][koef_a]->text().toDouble() == 0)
                m_teConsole->append(tr("x = %1").arg(res.first));
            else {
                m_teConsole->append(tr("x1 = %1").arg(res.first));
                m_teConsole->append(tr("x2 = %1").arg(res.second));
            }
        });

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(new QLabel("Коэффициент а:"));
        vbox->addWidget(m_leDataSource[TASK1][koef_a]);
        vbox->addWidget(new QLabel("Коэффициент b:"));
        vbox->addWidget(m_leDataSource[TASK1][koef_b]);
        vbox->addWidget(new QLabel("Коэффициент c:"));
        vbox->addWidget(m_leDataSource[TASK1][koef_c]);
        vbox->addWidget(pbResolve);
        wdgPage->setLayout(vbox);

        return wdgPage;
    }
    /**
     * @fn     QWidget * createSecondPage()
     * @brief  Создание 2 страницы задачи
     * @return виджет 2 страницы
     */
    QWidget * createSecondPage() {
        QWidget *wdgPage = new QWidget;

        QMap<QString, QRadioButton*> ptePlain;
        ptePlain["rbDegrees"] = new QRadioButton("Градусы");
        ptePlain["rbRadians"] = new QRadioButton("Радианы");
        ptePlain["rbDegrees"]->setChecked(true);

        m_leDataSource[TASK2][side_a] = new QLineEdit();
        m_leDataSource[TASK2][side_b] = new QLineEdit();
        m_leDataSource[TASK2][angle_between_ab] = new QLineEdit();

        QPushButton *pbResolve = new QPushButton("Вычислить");

        connect(pbResolve, &QPushButton::pressed, [=](){
            for(QLineEdit *le: m_leDataSource[TASK2]){
                if (le->text() == "") {
                    m_teConsole->append("Заполните поля");
                    return;
                }

                bool bOk;
                le->text().toDouble(&bOk);
                if (!bOk) {
                    m_teConsole->append("Некорректный ввод");
                    return;
                }
            }

            double degrees = m_leDataSource[TASK2][angle_between_ab]->text().toDouble();

            if(ptePlain["rbDegrees"]->isChecked())
                degrees = qDegreesToRadians(degrees);

            m_teConsole->append(tr("Сторона равна = %1")
                                .arg(lengthSide(m_leDataSource[TASK2][side_a]->text().toDouble(),
                                                m_leDataSource[TASK2][side_b]->text().toDouble(),
                                                degrees)));
        });

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(new QLabel("Сторона а:"));
        vbox->addWidget(m_leDataSource[TASK2][side_a]);
        vbox->addWidget(new QLabel("Сторона b:"));
        vbox->addWidget(m_leDataSource[TASK2][side_b]);
        vbox->addWidget(new QLabel("Угол между а и b:"));
        vbox->addWidget(m_leDataSource[TASK2][angle_between_ab]);
        vbox->addWidget(pbResolve);

        wdgPage->setLayout(vbox);
        return wdgPage;
    }
    /**
     * @fn     QWidget * createThirdPage()
     * @brief  Создание 3 страницы задачи
     * @return виджет 3 страницы
     */
    QWidget * createThirdPage() {
        QWidget *wdgPage = new QWidget;

        enum eButtons{
            oneButton,
            twoButton,
            threeButton
        };

        QMap<QString, QPlainTextEdit*> ptePlain;
        ptePlain["plaintOne"] = new QPlainTextEdit;
        ptePlain["plaintTwo"] = new QPlainTextEdit;

        QMap<eButtons, QPushButton*> pbButtons;
        pbButtons[oneButton] = new QPushButton("Добавить");
        pbButtons[twoButton] = new QPushButton("Заменить");
        pbButtons[threeButton] = new QPushButton("Вставить HTML");

        // Добавить
        connect(pbButtons[oneButton], &QPushButton::pressed, [=]() {
            ptePlain["plaintTwo"]->appendPlainText(ptePlain["plaintOne"]->toPlainText());
        });
        // Заменить
        connect(pbButtons[twoButton], &QPushButton::pressed, [=]() {
            ptePlain["plaintTwo"]->setPlainText(ptePlain["plaintOne"]->toPlainText());
        });
        // Вставить HTML
        // <font color='red'>Hello</font>
        connect(pbButtons[threeButton], &QPushButton::pressed, [=]() {
            ptePlain["plaintTwo"]->appendHtml(ptePlain["plaintOne"]->toPlainText());
        });

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(pbButtons[oneButton]);
        vbox->addWidget(pbButtons[twoButton]);
        vbox->addWidget(pbButtons[threeButton]);

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(ptePlain["plaintOne"]);
        hbox->addLayout(vbox);
        hbox->addWidget(ptePlain["plaintTwo"]);

        wdgPage->setLayout(hbox);
        return wdgPage;
    }

protected:
    /**
     * @fn     QPair<QString, QString> SolveKvUr(double a, double b, double c)
     * @brief  Найти корни квадратного уравнения
     * @param  a - коэффициент a
     * @param  b - коэффициент b
     * @param  c - коэффициент c
     * @return Пара корней x1, x2. Если a==0, то только x1
     */
    QPair<QString, QString> SolveKvUr(double a, double b, double c) {
        QPair<QString, QString> strX;
        QString strIm = "";

        if (a == 0) {
            strX.first = QString::number(-c / b);
            return strX;
        }

        double D = b*b - 4 * a * c;

        if (D < 0) {
            strIm = "i";

            strX.first = QString(tr("(-%1 + %2%3) / (2 * %4)")
                                 .arg(b).arg(qSqrt(-D)).arg(strIm).arg(a));
            strX.second = QString(tr("(-%1 - %2%3) / (2 * %4)")
                                 .arg(b).arg(qSqrt(-D)).arg(strIm).arg(a));
        }
        else {
            strX.first = QString::number(((-1) * b + qSqrt(D))/(2 * a));
            strX.second = QString::number(((-1) * b - qSqrt(D))/(2 * a));
        }

        return strX;
    }

    /**
      @fn      double lengthSide(double b, double c, double alpha
     * @brief  Вычислить длину стороны треуг по 2 сторонам и углу между ними
     * @param  b - сторона 1
     * @param  c - сторона 2
     * @param  alpha - угол между ними
     * @return
     */
    double lengthSide(double b, double c, double alpha) {
        return qSqrt(qPow(b, 2) + qPow(c, 2) - 2 * b * c * qCos(alpha));
    }
};

#endif // MathComputation
