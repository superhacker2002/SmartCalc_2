#ifndef SMARTCALC_MAINWINDOW_H
#define SMARTCALC_MAINWINDOW_H

#include <QMainWindow>

#include "../controller/controller.h"
#include "QtWidgets/qpushbutton.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  QVector<QPushButton> buttons_;
  void connectButtons();

 private slots:
  void digits();
  void deletion();
  void result();
  void enableButtons();
  void printGraph();
  void deleteError();
};
#endif  // SMARTCALC_MAINWINDOW_H
