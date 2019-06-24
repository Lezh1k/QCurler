#include <QThread>
#include <QPushButton>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConnectionInfoModel.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_curl_worker_running(false),
  m_cw(NULL),
  m_model(NULL)
{
  ui->setupUi(this);
  //connect(ui->btn_start_stop, &QPushButton::released, this, &MainWindow::btn_start_stop_released);
  m_model = new ConnectionInfoModel(this);
  ui->tv_statistics->setModel(m_model);

  for (int c = 0; c < ui->tv_statistics->horizontalHeader()->count(); ++c) {
    ui->tv_statistics->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
  }

  for (int c = 0; c < ui->tv_statistics->verticalHeader()->count(); ++c) {
    ui->tv_statistics->verticalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
  }

  start_new_curl_worker();
  QTimer *m_repaintTimer = new QTimer();
  connect(m_repaintTimer, &QTimer::timeout, [=]() {
      update();
  });
  m_repaintTimer->start(1000);
}

MainWindow::~MainWindow() {
  if (m_model) delete m_model;
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
  //ui->btn_start_stop->setText("Stop");
}
///////////////////////////////////////////////////////////

void MainWindow::stop_curl_worker() {
  m_cw->Stop();
  m_curl_worker_running = false;
  //ui->btn_start_stop->setText("Start");
}
///////////////////////////////////////////////////////////

void MainWindow::btn_start_stop_released() {
  if (m_curl_worker_running) {
    stop_curl_worker();
    return;
  }
  start_new_curl_worker();
}
///////////////////////////////////////////////////////////
