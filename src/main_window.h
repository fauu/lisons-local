#ifndef LISONS_LOCAL_MAIN_WINDOW_H
#define LISONS_LOCAL_MAIN_WINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

private:
  Ui::MainWindow *ui;
};


#endif //LISONS_LOCAL_MAIN_WINDOW_H
