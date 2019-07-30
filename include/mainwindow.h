#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CurlWorker.h"
#include "ResourceProvider.h"

#include "ResourceProviderHardcodePolicy.h"
#include "ResourceProviderNetworkServicePolicy.h"

namespace Ui {
  class MainWindow;
}

class ConnectionInfoModel;

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  Ui::MainWindow *ui;

  CurlWorker *m_cw;
  ConnectionInfoModel *m_model;
//  ResourceProvider<ResourceProviderHardcodePolicy> m_resourcesProv;
  ResourceProvider<ResourceProviderNetworkServicePolicy> m_resourcesProv;
  QTimer *m_repaintTimer;
  QTimer *m_updateResourcesTimer;

  void startNewCurlWorker();

public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();

private slots:
  void repaintTimerTimeout();
  void updateResourcesTimerTimeout();
};

#endif // MAINWINDOW_H
