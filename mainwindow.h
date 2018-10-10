#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bignumber.h>
#include <string>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void displayNeedUpdate();

public slots:
    void updateDisplay();
    void on_btnNumber1_clicked();
    void on_btnNumber2_clicked();
    void on_btnNumber3_clicked();
    void on_btnNumber4_clicked();
    void on_btnNumber5_clicked();
    void on_btnNumber6_clicked();
    void on_btnNumber7_clicked();
    void on_btnNumber8_clicked();
    void on_btnNumber9_clicked();
    void on_btnNumber0_clicked();

    void on_btnSign_clicked();

    void on_btnDecimal_clicked();

    void on_btnClear_clicked();

    void on_btnDelete_clicked();

    void on_btnDivide_clicked();

    void on_btnEqualTo_clicked();

private slots:
    void on_btnMultiply_clicked();

    void on_btnSubstract_clicked();

    void on_btnAdd_clicked();

    void on_btnAllClear_clicked();

private:
    Ui::MainWindow *ui;
    QString inputString;
    QString leftVal;
    QString rightVal;
    QString equationString;
    char currentOperation;
};

#endif // MAINWINDOW_H
