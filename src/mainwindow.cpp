#include <QThread>
#include <QPushButton>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConnectionInfoModel.h"
#include "ResourceProviderHard.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_cw(nullptr),
  m_model(nullptr),
  m_resourcesProv(nullptr),
  m_repaintTimer(nullptr)
{
  ui->setupUi(this);  
  m_resourcesProv = new ResourceProviderHard;
  m_model = new ConnectionInfoModel(this);
  m_model->setResourceProvider(m_resourcesProv);
  ui->tv_statistics->setModel(m_model);

  for (int c = 0; c < ui->tv_statistics->horizontalHeader()->count(); ++c) {
    ui->tv_statistics->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
  }

  for (int c = 0; c < ui->tv_statistics->verticalHeader()->count(); ++c) {
    ui->tv_statistics->verticalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
  }

  startNewCurlWorker();
  m_repaintTimer = new QTimer(this);
  connect(m_repaintTimer, &QTimer::timeout, [=]() {
    m_model->setRowHeight(ui->tv_statistics->rowHeight(0));
    update();
  });
  m_repaintTimer->start(1000);
}

MainWindow::~MainWindow() {
  if (m_repaintTimer) {
    m_repaintTimer->stop();
    m_repaintTimer->deleteLater();
  }

  if (m_model) delete m_model;
  if (m_cw) m_cw->Stop();
  delete ui;
}
///////////////////////////////////////////////////////////

void MainWindow::startNewCurlWorker() {
  QThread *th = new QThread;
  m_cw = new CurlWorker(m_resourcesProv);

  connect(th, &QThread::started, m_cw, &CurlWorker::Start);  
  connect(m_cw, &CurlWorker::infoReceived, m_model, &ConnectionInfoModel::infoReceived);
  connect(m_cw, &CurlWorker::resourcesUpdated, m_model, &ConnectionInfoModel::resourcesUpdated);
  connect(m_cw, &CurlWorker::stopped, th, &QThread::quit);
  connect(th, &QThread::finished, m_cw, &CurlWorker::deleteLater);
  connect(th, &QThread::finished, th, &QThread::deleteLater);

  m_cw->moveToThread(th);
  th->start();
}
///////////////////////////////////////////////////////////
