#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CurlWorker.h"

namespace Ui {
  class MainWindow;
}

class ConnectionInfoModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

  bool m_curl_worker_running;
  CurlWorker *m_cw;
  ConnectionInfoModel *m_model;

  void start_new_curl_worker();
  void stop_curl_worker();
};

#endif // MAINWINDOW_H
