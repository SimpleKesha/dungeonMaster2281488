#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("Практика: Битовое множество");
    resize(900, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Заголовок
    QLabel *titleLabel = new QLabel("BitSet - Битовое множество (Вариант 7)");
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    mainLayout->addWidget(titleLabel);
    
    // Панель ввода
    QGroupBox *inputGroup = new QGroupBox("Операции с множествами");
    QHBoxLayout *inputLayout = new QHBoxLayout();
    
    QLabel *elemLabel = new QLabel("Элемент:");
    elementInput = new QLineEdit();
    elementInput->setMaximumWidth(80);
    elementInput->setPlaceholderText("0-99");
    
    QLabel *setLabel = new QLabel("Множество:");
    setSelector = new QComboBox();
    setSelector->addItem("A");
    setSelector->addItem("B");
    
    btnAdd = new QPushButton("Добавить");
    btnRemove = new QPushButton("Удалить");
    btnContains = new QPushButton("Содержит?");
    
    inputLayout->addWidget(elemLabel);
    inputLayout->addWidget(elementInput);
    inputLayout->addWidget(setLabel);
    inputLayout->addWidget(setSelector);
    inputLayout->addWidget(btnAdd);
    inputLayout->addWidget(btnRemove);
    inputLayout->addWidget(btnContains);
    inputLayout->addStretch();
    
    inputGroup->setLayout(inputLayout);
    mainLayout->addWidget(inputGroup);
    
    // Дисплеи множеств
    QHBoxLayout *setsLayout = new QHBoxLayout();
    
    // Множество A
    QGroupBox *setAGroup = new QGroupBox("Множество A");
    QVBoxLayout *setALayout = new QVBoxLayout();
    setADisplay = new QTextEdit();
    setADisplay->setReadOnly(true);
    setADisplay->setMaximumHeight(150);
    sizeALabel = new QLabel("Размер: 0");
    btnClearA = new QPushButton("Очистить A");
    setALayout->addWidget(setADisplay);
    setALayout->addWidget(sizeALabel);
    setALayout->addWidget(btnClearA);
    setAGroup->setLayout(setALayout);
    setsLayout->addWidget(setAGroup);
    
    // Множество B
    QGroupBox *setBGroup = new QGroupBox("Множество B");
    QVBoxLayout *setBLayout = new QVBoxLayout();
    setBDisplay = new QTextEdit();
    setBDisplay->setReadOnly(true);
    setBDisplay->setMaximumHeight(150);
    sizeBLabel = new QLabel("Размер: 0");
    btnClearB = new QPushButton("Очистить B");
    setBLayout->addWidget(setBDisplay);
    setBLayout->addWidget(sizeBLabel);
    setBLayout->addWidget(btnClearB);
    setBGroup->setLayout(setBLayout);
    setsLayout->addWidget(setBGroup);
    
    mainLayout->addLayout(setsLayout);
    
    // Операции со множествами
    QGroupBox *opsGroup = new QGroupBox("Операции над множествами");
    QHBoxLayout *opsLayout = new QHBoxLayout();
    
    btnUnite = new QPushButton("A ∪ B");
    btnIntersect = new QPushButton("A ∩ B");
    btnDifference = new QPushButton("A \\ B");
    btnCheckEmpty = new QPushButton("Пусто?");
    
    opsLayout->addWidget(btnUnite);
    opsLayout->addWidget(btnIntersect);
    opsLayout->addWidget(btnDifference);
    opsLayout->addWidget(btnCheckEmpty);
    opsLayout->addStretch();
    
    opsGroup->setLayout(opsLayout);
    mainLayout->addWidget(opsGroup);
    
    // Лог
    QGroupBox *logGroup = new QGroupBox("Лог операций");
    QVBoxLayout *logLayout = new QVBoxLayout();
    logOutput = new QTextEdit();
    logOutput->setReadOnly(true);
    logOutput->setMaximumHeight(200);
    logLayout->addWidget(logOutput);
    logGroup->setLayout(logLayout);
    mainLayout->addWidget(logGroup);
    
    // Сигналы
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(onAddClicked()));
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(onRemoveClicked()));
    connect(btnClearA, SIGNAL(clicked()), this, SLOT(onClearAClicked()));
    connect(btnClearB, SIGNAL(clicked()), this, SLOT(onClearBClicked()));
    connect(btnUnite, SIGNAL(clicked()), this, SLOT(onUniteClicked()));
    connect(btnIntersect, SIGNAL(clicked()), this, SLOT(onIntersectClicked()));
    connect(btnDifference, SIGNAL(clicked()), this, SLOT(onDifferenceClicked()));
    connect(btnContains, SIGNAL(clicked()), this, SLOT(onContainsClicked()));
    connect(btnCheckEmpty, SIGNAL(clicked()), this, SLOT(onCheckEmptyClicked()));
}

void MainWindow::updateDisplays()
{
    setADisplay->setText(setA.toString());
    setBDisplay->setText(setB.toString());
    sizeALabel->setText(QString("Размер: %1").arg(setA.size()));
    sizeBLabel->setText(QString("Размер: %1").arg(setB.size()));
}

void MainWindow::appendLog(const QString &text)
{
    logOutput->append(text);
}

void MainWindow::onAddClicked()
{
    bool ok;
    int elem = elementInput->text().toInt(&ok);
    
    if (!ok || elem < 0 || elem >= 100) {
        QMessageBox::warning(this, "Ошибка", "Введите число от 0 до 99");
        return;
    }
    
    if (setSelector->currentText() == "A") {
        setA.add(elem);
        appendLog(QString("Добавлен %1 в множество A").arg(elem));
    } else {
        setB.add(elem);
        appendLog(QString("Добавлен %1 в множество B").arg(elem));
    }
    
    elementInput->clear();
    updateDisplays();
}

void MainWindow::onRemoveClicked()
{
    bool ok;
    int elem = elementInput->text().toInt(&ok);
    
    if (!ok || elem < 0 || elem >= 100) {
        QMessageBox::warning(this, "Ошибка", "Введите число от 0 до 99");
        return;
    }
    
    if (setSelector->currentText() == "A") {
        setA.remove(elem);
        appendLog(QString("Удален %1 из множества A").arg(elem));
    } else {
        setB.remove(elem);
        appendLog(QString("Удален %1 из множества B").arg(elem));
    }
    
    elementInput->clear();
    updateDisplays();
}

void MainWindow::onClearAClicked()
{
    setA = BitSet();
    appendLog("Множество A очищено");
    updateDisplays();
}

void MainWindow::onClearBClicked()
{
    setB = BitSet();
    appendLog("Множество B очищено");
    updateDisplays();
}

void MainWindow::onUniteClicked()
{
    BitSet result = setA.unite(setB);
    appendLog(QString("A ∪ B = %1").arg(result.toString()));
    QMessageBox::information(this, "Результат", 
                            QString("A ∪ B = %1\nРазмер: %2").arg(result.toString()).arg(result.size()));
}

void MainWindow::onIntersectClicked()
{
    BitSet result = setA.intersect(setB);
    appendLog(QString("A ∩ B = %1").arg(result.toString()));
    QMessageBox::information(this, "Результат", 
                            QString("A ∩ B = %1\nРазмер: %2").arg(result.toString()).arg(result.size()));
}

void MainWindow::onDifferenceClicked()
{
    BitSet result = setA.difference(setB);
    appendLog(QString("A \\ B = %1").arg(result.toString()));
    QMessageBox::information(this, "Результат", 
                            QString("A \\ B = %1\nРазмер: %2").arg(result.toString()).arg(result.size()));
}

void MainWindow::onContainsClicked()
{
    bool ok;
    int elem = elementInput->text().toInt(&ok);
    
    if (!ok || elem < 0 || elem >= 100) {
        QMessageBox::warning(this, "Ошибка", "Введите число от 0 до 99");
        return;
    }
    
    QString setName = setSelector->currentText();
    bool contains = (setName == "A") ? setA.contains(elem) : setB.contains(elem);
    
    QString msg = contains ? "ДА, содержит" : "НЕТ, не содержит";
    appendLog(QString("Множество %1 содержит %2: %3").arg(setName).arg(elem).arg(msg));
    QMessageBox::information(this, "Проверка", 
                            QString("Множество %1 содержит %2: %3").arg(setName).arg(elem).arg(msg));
}

void MainWindow::onCheckEmptyClicked()
{
    QString msgA = setA.isEmpty() ? "Пусто" : "Не пусто";
    QString msgB = setB.isEmpty() ? "Пусто" : "Не пусто";
    
    appendLog(QString("A: %1 | B: %2").arg(msgA).arg(msgB));
    QMessageBox::information(this, "Проверка", 
                            QString("Множество A: %1\nМножество B: %2").arg(msgA).arg(msgB));
}
