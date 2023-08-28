#include "stdafx.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "escriptengine.h"
#include "myspreadsheet.h"

using namespace escript;

// Это пример, как работать с EScript, поэтому просто забьём пустую таблицу 5*3
// В реальности всё можно через модель, но здесь это ни к чему

#define COLUMN_COUNT 5
#define ROW_COUNT 3

// скрипт
const std::u32string macro1 =
UR"(c1 = spreadsheet.cell(0, 0);
c1.text = "Hello";
s = c1.text;
spreadsheet.cell(0, 1).text = "+";
spreadsheet.cell(0, 2).text = "World";
spreadsheet.cell(0, 3).text = "=";
spreadsheet.cell(0, 4).text = s + spreadsheet.cell(0, 2).text;
spreadsheet.cell(2, 2).style.color = "#FFFF00";
sColor = spreadsheet.cell(2, 2).style.color.toString();
spreadsheet.cell(2, 1).text = "желтый:";
spreadsheet.cell(2, 2).text = sColor;
)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::pushButtonClicked);
    buildTableView();
    ui->plainTextEdit->setPlainText(QString::fromStdU32String(macro1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::pushButtonClicked()
{
    std::u32string code1 = ui->plainTextEdit->toPlainText().toStdU32String();
    MySpreadSheet spreadsheet(ui->tableWidget);
    EScript engine;
    engine.attachObject(&spreadsheet, U"spreadsheet");
    engine.eval(code1);
    engine.detachObject(&spreadsheet);
}

void MainWindow::buildTableView()
{
    auto table = ui->tableWidget;
    table->setColumnCount(COLUMN_COUNT);
    table->setRowCount(ROW_COUNT);
    QStringList labels;
    labels << "A" << "B" << "C" << "D" << "E";
    table->setHorizontalHeaderLabels(labels);
}

