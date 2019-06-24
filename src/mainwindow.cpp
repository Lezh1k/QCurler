#include <QThread>
#include <QPushButton>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConnectionInfoModel.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_curl_worker_running(false),
  m_cw(nullptr),
  m_model(nullptr)
{
  ui->setupUi(this);  
  m_model = new ConnectionInfoModel(this);
  ui->tv_statistics->setModel(m_model);

  for (int c = 0; c < ui->tv_statistics->horizontalHeader()->count(); ++c) {
    ui->tv_statistics->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
  }

  for (int c = 0; c < ui->tv_statistics->verticalHeader()->count(); ++c) {
    ui->tv_statistics->verticalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
  }

  start_new_curl_worker();
  QTimer *m_repaintTimer = new QTimer(this);
  connect(m_repaintTimer, &QTimer::timeout, [=]() {
    m_model->setRowHeight(ui->tv_statistics->rowHeight(0));
    update();
  });
  m_repaintTimer->start(1000);


}

MainWindow::~MainWindow() {
  if (m_model) delete m_model;
  if (m_cw) m_cw->Stop();
  delete ui;
}
///////////////////////////////////////////////////////////

void MainWindow::start_new_curl_worker() {
  QThread *th = new QThread;
  m_cw = new CurlWorker;

  connect(th, &QThread::started, m_cw, &CurlWorker::Start);  
  connect(m_cw, &CurlWorker::info_received, m_model, &ConnectionInfoModel::info_received);
  connect(m_cw, &CurlWorker::Stopped, th, &QThread::quit);
  connect(th, &QThread::finished, m_cw, &CurlWorker::deleteLater);
  connect(th, &QThread::finished, th, &QThread::deleteLater);

  m_cw->moveToThread(th);
  th->start();
  m_curl_worker_running = true;  
}
///////////////////////////////////////////////////////////

void MainWindow::stop_curl_worker() {
  m_cw->Stop();
  m_curl_worker_running = false;
}
///////////////////////////////////////////////////////////
