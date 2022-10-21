#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->min->setEnabled(false);
  ui->max->setEnabled(false);

  connect(ui->zero, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->one, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->two, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->three, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->four, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->five, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->six, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->seven, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->eight, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->nine, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->minus, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->plus, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->division, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->multiplication, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->point, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->mod, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->pow, SIGNAL(clicked()), this, SLOT(digits()));

  connect(ui->sin, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->cos, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->tan, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->acos, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->asin, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->atan, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->ln, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->log, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->x, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->e, SIGNAL(clicked()), this, SLOT(digits()));

  connect(ui->left_parenthesis, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->right_parenthesis, SIGNAL(clicked()), this, SLOT(digits()));
  connect(ui->deletion, SIGNAL(clicked()), this, SLOT(deletion()));
  connect(ui->result, SIGNAL(clicked()), this, SLOT(result()));

  connect(ui->make_graph, SIGNAL(toggled(bool)), this, SLOT(enableButtons()));
  connect(ui->substitute, SIGNAL(toggled(bool)), this, SLOT(enableButtons()));
  connect(ui->min, SIGNAL(valueChanged(int)), this, SLOT(printGraph()));
  connect(ui->max, SIGNAL(valueChanged(int)), this, SLOT(printGraph()));
  connect(ui->print, SIGNAL(clicked()), this, SLOT(printGraph()));
  connect(ui->result_label, SIGNAL(textEdited(QString)), this,
          SLOT(deleteError()));

  QPushButton *result = this->ui->result;
  result->setShortcut(Qt::Key_Return);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::digits() {
  QPushButton *button = (QPushButton *)sender();
  QString result_label = ui->result_label->text();
  if (result_label.contains("Incorrect")) ui->result_label->setText("");

  if (result_label.length() <= 255) {
    ui->result_label->setText(result_label + button->text());
  }
}

void MainWindow::deletion() {
  QString result_label = ui->result_label->text();
  result_label.resize(result_label.size() - 1);
  ui->result_label->setText(result_label);
}

void MainWindow::result() {
  QString result_label = ui->result_label->text();

  if (result_label.contains("x") && ui->substitute->isChecked()) {
    result_label.replace("x", ui->x_value->text());
  }
  if (result_label.contains("e") || result_label.contains("E")) {
    result_label.replace("e", " *10^");
    result_label.replace("E", " *10^");
  }
  std::string input_string = result_label.toStdString();
  try {
    s21::CalcController controller(input_string);
    double result = controller.getResult();
    QString str = QString::number(result, 'g', 10);
    ui->result_label->setText(str);
  } catch (...) {
    ui->result_label->setText("Incorrect expression.");
  }
}

void MainWindow::enableButtons() {
  QRadioButton *button = (QRadioButton *)sender();
  if (button == ui->make_graph) {
    ui->print->setEnabled(true);
    ui->max->setEnabled(true);
    ui->min->setEnabled(true);
    ui->x_value->setEnabled(false);
  } else {
    ui->print->setEnabled(false);
    ui->x_value->setEnabled(true);
    ui->max->setEnabled(false);
    ui->min->setEnabled(false);
  }
}

void MainWindow::printGraph() {
  s21::GraphController graph_controller(ui->min->value(), ui->max->value(),
                                        ui->result_label->text().toStdString());
  ui->min->setMaximum(ui->max->value() - 1);
  try {
    int graph_result = graph_controller.makeGraph();
    if (graph_result == 1) {
      ui->result_label->setText("Expression does not contain x.");
    } else if (graph_result != 2) {
      auto vectors_pair = graph_controller.getVectors();
      QVector<double> vector_x =
          QVector<double>(vectors_pair.first.begin(), vectors_pair.first.end());
      QVector<double> vector_y = QVector<double>(vectors_pair.second.begin(),
                                                 vectors_pair.second.end());
      ui->graph->addGraph();
      ui->graph->graph(0)->data()->clear();
      ui->graph->graph(0)->addData(vector_x, vector_y);
      ui->graph->xAxis->setRange(ui->min->value(), ui->max->value());
      auto min_y = std::min_element(vectors_pair.second.begin(),
                                    vectors_pair.second.end());
      auto max_y = std::max_element(vectors_pair.second.begin(),
                                    vectors_pair.second.end());
      ui->graph->yAxis->setRange(*min_y, *max_y);
      ui->graph->replot();
    }
  } catch (...) {
    ui->result_label->setText("Incorrect expression.");
  }
}

void MainWindow::deleteError() {
  if (ui->result_label->text().contains("Incorrect") ||
      ui->result_label->text().contains("Expression")) {
    ui->result_label->setText("");
  }
}
