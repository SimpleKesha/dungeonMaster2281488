#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include "bitset.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onClearAClicked();
    void onClearBClicked();
    void onUniteClicked();
    void onIntersectClicked();
    void onDifferenceClicked();
    void onContainsClicked();
    void onCheckEmptyClicked();

private:
    void setupUI();
    void updateDisplays();
    void appendLog(const QString &text);
    
    BitSet setA;
    BitSet setB;
    
    QLineEdit *elementInput;
    QComboBox *setSelector;
    
    QTextEdit *logOutput;
    QTextEdit *setADisplay;
    QTextEdit *setBDisplay;
    
    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnClearA;
    QPushButton *btnClearB;
    QPushButton *btnUnite;
    QPushButton *btnIntersect;
    QPushButton *btnDifference;
    QPushButton *btnContains;
    QPushButton *btnCheckEmpty;
    
    QLabel *sizeALabel;
    QLabel *sizeBLabel;
};

#endif // MAINWINDOW_H
