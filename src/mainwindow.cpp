#include <QThread>
#include <QPushButton>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ConnectionInfoModel.h"
#include "ResourceProviderHardcodePolicy.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_cw(nullptr),
  m_model(nullptr),
  m_repaintTimer(nullptr),
  m_updateResourcesTimer(nullptr)
{
  ui->setupUi(this);

  m_model = new ConnectionInfoModel(this);
  ui->tv_statistics->setModel(m_model);

  for (int c = 0; c < ui->tv_statistics->horizontalHeader()->count(); ++c) {
    ui->tv_statistics->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);
  }

  for (int c = 0; c < ui->tv_statistics->verticalHeader()->count(); ++c) {
    ui->tv_statistics->verticalHeader()->setSectionResizeMode(c, QHeaderView::Fixed);
  }

  startNewCurlWorker();

  m_repaintTimer = new QTimer;
  connect(m_repaintTimer, &QTimer::timeout, this, &MainWindow::repaintTimerTimeout);
  m_repaintTimer->start(1000);

  m_updateResourcesTimer = new QTimer;
  connect(m_updateResourcesTimer, &QTimer::timeout, this, &MainWindow::updateResourcesTimerTimeout);
  m_updateResourcesTimer->start(5*1000);
}

MainWindow::~MainWindow() {
  if (m_repaintTimer) {
    m_repaintTimer->stop();
    m_repaintTimer->deleteLater();
  }

  if (m_model) delete m_model;
  if (m_cw) m_cw->stop();
  delete ui;
}
///////////////////////////////////////////////////////
void MainWindow::repaintTimerTimeout() {

  //Update UI
  if (m_model->rowCount()) {
    int dh = ui->tv_statistics->height() - ui->tv_statistics->horizontalHeader()->height();
    dh /= m_model->rowCount();
    for (int i = 0; i < m_model->rowCount(); ++i)
      ui->tv_statistics->verticalHeader()->resizeSection(i, dh);

    int min = std::min(ui->tv_statistics->rowHeight(0), ui->tv_statistics->columnWidth(0));
    m_model->setImageSize(min);
  }

  update();
}
///////////////////////////////////////////////////////

void MainWindow::updateResourcesTimerTimeout() {
  if (!m_resourcesProv.update())
    return;
  m_model->resourceListUpdated(m_resourcesProv.resources().size());
  if (m_cw)
    m_cw->updateResourceList(m_resourcesProv.resources());
}
///////////////////////////////////////////////////////

void MainWindow::startNewCurlWorker() {
  QThread *th = new QThread;
  m_cw = new CurlWorker;

  connect(th, &QThread::started, m_cw, &CurlWorker::start);
  connect(m_cw, &CurlWorker::infoReceived, m_model, &ConnectionInfoModel::infoReceived);
  connect(m_cw, &CurlWorker::stopped, th, &QThread::quit);
  connect(th, &QThread::finished, m_cw, &CurlWorker::deleteLater);
  connect(th, &QThread::finished, th, &QThread::deleteLater);

  m_cw->moveToThread(th);
  th->start();
}
///////////////////////////////////////////////////////////
