#include "Execute.h"


Execute::Execute() {

    parseText = new ParseText;
    plainTextEdit = new QPlainTextEdit;

    // Создать стек виджетов
    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(createFirstPage());
    stackedWidget->addWidget(createSecondPage());
    stackedWidget->addWidget(createThirdPage());

    // Создать список виджетов
    QListWidget *listDevice = new QListWidget();
    listDevice->addItem(tr("Спецсимволы"));
    listDevice->addItem(tr("Список"));
    listDevice->addItem(tr("Таблица"));

    // Соединить список виджетов со стеком
    connect(listDevice, QOverload<int>::of(&QListWidget::currentRowChanged),
            stackedWidget, &QStackedWidget::setCurrentIndex);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(listDevice);
    hbox->addWidget(stackedWidget);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(hbox);

    setLayout(vbox);
}


Execute::~Execute(){
    if (parseText) {
        delete parseText;
        parseText = nullptr;
    }
}

/**
 * @fn     QWidget * createFirstPage()
 * @brief  Создание 1 страницы задачи
 * @return виджет 1 страницы
 */
QWidget * Execute::createFirstPage() {

    QWidget *wdgPage = new QWidget;

    connect(plainTextEdit, &QPlainTextEdit::textChanged, [=](){
        // получаем текст виджета
        QString str = plainTextEdit->toPlainText();
        // заменяем текст примеры с результатами
        //if (parseText->change(str)) // были ли примеры
        if (parseText->changeSpecSymbol(str)) // были ли примеры
            plainTextEdit->setPlainText(parseText->getText());
    });

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(plainTextEdit);
    wdgPage->setLayout(vbox);

    return wdgPage;
}
/**
 * @fn     QWidget * createSecondPage()
 * @brief  Создание 2 страницы задачи
 * @return виджет 2 страницы
 */
QWidget * Execute::createSecondPage() {
    QWidget *wdgPage = new QWidget;

    QListWidget *m_Lang = new QListWidget;
    m_Lang->setDragEnabled(true);
    m_Lang->setDragDropMode(QAbstractItemView::DragDrop);
    m_Lang->setDefaultDropAction(Qt::MoveAction);

    m_Lang->addItem(new QListWidgetItem(QIcon("C++.jpg"),    "C++"));
    m_Lang->addItem(new QListWidgetItem(QIcon("Python.png"), "Python"));
    m_Lang->addItem(new QListWidgetItem(QIcon("Java.png"),   "Java"));
    m_Lang->addItem(new QListWidgetItem(QIcon("C#.jpg"),     "C#"));
    m_Lang->addItem(new QListWidgetItem(QIcon("PHP.png"),    "PHP"));
    m_Lang->addItem(new QListWidgetItem(QIcon("JS.jpg"),     "JS"));

    for (int i = 0; i < m_Lang->count(); i++)
        m_Lang->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);

    QPushButton *pbAddItem = new QPushButton("Добавить");
    connect(pbAddItem, &QPushButton::pressed, [=](){
        QListWidgetItem *item = new QListWidgetItem(QIcon("Folder.png"), "Новый язык");
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        m_Lang->addItem(item);
    });

    QPushButton *pbDelItem = new QPushButton("Удалить");
    connect(pbDelItem, &QPushButton::pressed, [=](){
        int i = m_Lang->currentRow();
        m_Lang->takeItem(i);
    });

    QCheckBox *cbViewMode = new QCheckBox("Отобразить иконки");
    connect(cbViewMode, &QCheckBox::pressed, [=](){
        if (!cbViewMode->isChecked())
            m_Lang->setViewMode(QListView::IconMode);
        else
            m_Lang->setViewMode(QListView::ListMode);
    });

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(m_Lang);
    vbox->addWidget(pbAddItem);
    vbox->addWidget(pbDelItem);
    vbox->addWidget(cbViewMode);
    wdgPage->setLayout(vbox);
    return wdgPage;
}
/**
 * @fn     QWidget * createThirdPage()
 * @brief  Создание 3 страницы задачи
 * @return виджет 3 страницы
 */
QWidget * Execute::createThirdPage() {
    QWidget *wdgPage = new QWidget;

    table = new QTableWidget(4, 3);
    table->setHorizontalHeaderLabels({"Имя компьютера", "IP адрес", "MAC"});
    table->horizontalHeader()->setStretchLastSection(true);

    table->setItem(0, 0, new QTableWidgetItem("PC 1"));
    table->setItem(0, 1, new QTableWidgetItem("172.30.100.58"));
    table->setItem(0, 2, new QTableWidgetItem("CC:DD:EE:AA:BB:FF"));

    table->setItem(1, 0, new QTableWidgetItem("PC 2"));
    table->setItem(1, 1, new QTableWidgetItem("172.30.100.59"));
    table->setItem(1, 2, new QTableWidgetItem("AA:BB:CC:DD:EE:FF"));

    table->setItem(2, 0, new QTableWidgetItem("PC 3"));
    table->setItem(2, 1, new QTableWidgetItem("172.30.100.60"));
    table->setItem(2, 2, new QTableWidgetItem("BB:CC:AA:DD:EE:FF"));

    QListWidget *m_Lang = new QListWidget;

    m_Lang->addItem(new QListWidgetItem(QIcon("C++.jpg"),    "C++"));

    QPushButton *pbColor = new QPushButton("Окрасить фон");
    connect(pbColor, &QPushButton::pressed, [=](){
        for(QTableWidgetItem *i: table->selectedItems())
            i->setBackground(QBrush(Qt::cyan));
    });

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(table);
    vbox->addWidget(pbColor);

    wdgPage->setLayout(vbox);
    return wdgPage;
}
