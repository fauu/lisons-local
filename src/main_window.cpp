#include "main_window.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
//  this->setCentralWidget(ui->textEdit);
}

MainWindow::~MainWindow()
{
  delete ui;
}
