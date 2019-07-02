#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CurlWorker.h"
#include "IResourceProvider.h"

namespace Ui {
  class MainWindow;
}

class ConnectionInfoModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

private:
  Ui::MainWindow *ui;

  CurlWorker *m_cw;
  ConnectionInfoModel *m_model;
  IResourceProvider *m_resourcesProv;
  QTimer *m_repaintTimer;

  void startNewCurlWorker();
};

#endif // MAINWINDOW_H
