#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon(":/Calculator.png"));
    inputString = "";
    leftVal = "";
    rightVal = "";
    currentOperation = '\0';
    ui->setupUi(this);
    connect(this,&MainWindow::displayNeedUpdate,this,&MainWindow::updateDisplay);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDisplay()
{
    if(inputString.length() > 15)
        ui->numberBoard->setAlignment(Qt::AlignLeading);
    ui->numberBoard->setText(this->inputString);
}

void MainWindow::on_btnNumber1_clicked()
{
    inputString += "1";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber2_clicked()
{
    inputString += "2";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber3_clicked()
{
    inputString += "3";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber4_clicked()
{
    inputString += "4";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber5_clicked()
{
    inputString += "5";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber6_clicked()
{
    inputString += "6";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber7_clicked()
{
    inputString += "7";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber8_clicked()
{
    inputString += "8";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber9_clicked()
{
    inputString += "9";
    emit displayNeedUpdate();
}
void MainWindow::on_btnNumber0_clicked()
{
    inputString += "0";
    emit displayNeedUpdate();
}

void MainWindow::on_btnSign_clicked()
{
    if(inputString[0] != '-')
        inputString = "-" + inputString;
    else if(inputString[0] == '-') //remove negative sign
        inputString = inputString.remove(0,1);
    emit displayNeedUpdate();
}

void MainWindow::on_btnDecimal_clicked()
{
    if(!inputString.contains("."))
    {
        if(inputString=="")
            inputString += "0";
        inputString += ".";
    }
    emit displayNeedUpdate();
}

void MainWindow::on_btnClear_clicked()
{
    inputString = "";
    emit displayNeedUpdate();
}

void MainWindow::on_btnDelete_clicked()
{
    if(inputString != "")
    {
        inputString.remove(inputString.size() -1, 1);
        emit displayNeedUpdate();
    }
}


void MainWindow::on_btnDivide_clicked()
{
    if(ui->numberBoard->text() != "")
    {
        leftVal = ui->numberBoard->text();
        inputString = "";
        currentOperation = '/';
        emit displayNeedUpdate();
        equationString = leftVal +" " +currentOperation;
        ui->equation->setText(equationString);
    }
}

void MainWindow::on_btnMultiply_clicked()
{
    if(ui->numberBoard->text() != "")
    {
        leftVal = ui->numberBoard->text();
        inputString = "";
        currentOperation = '*';
        emit displayNeedUpdate();
        equationString = leftVal +" " +currentOperation;
        ui->equation->setText(equationString);
    }
}

void MainWindow::on_btnSubstract_clicked()
{
    if(ui->numberBoard->text() != "")
    {
        leftVal = ui->numberBoard->text();
        inputString = "";
        currentOperation = '-';
        emit displayNeedUpdate();
        equationString = leftVal +" " +currentOperation;
        ui->equation->setText(equationString);
    }
}

void MainWindow::on_btnAdd_clicked()
{
    if(ui->numberBoard->text() != "")
    {
        leftVal = ui->numberBoard->text();
        inputString = "";
        currentOperation = '+';
        emit displayNeedUpdate();
        equationString = leftVal +" " +currentOperation;
        ui->equation->setText(equationString);
    }
}

void MainWindow::on_btnEqualTo_clicked()
{
    if(inputString != "")
    {
        rightVal = inputString;
        BigNumber _rightVal(rightVal.toStdString());
        BigNumber _leftVal(leftVal.toStdString());
        string result = BigNumber::exectue(_leftVal,_rightVal,currentOperation).toString();
        inputString = QString::fromStdString(result);
//        qDebug() << inputString;
        emit displayNeedUpdate();
        equationString = ui->equation->text() +" "+ rightVal + " =";
        ui->equation->setText(equationString);

        currentOperation='\0';
        inputString="";
        equationString="";
    }

}

void MainWindow::on_btnAllClear_clicked()
{
    inputString = "";
    emit displayNeedUpdate();
    equationString = "";
    ui->equation->setText(equationString);

}
