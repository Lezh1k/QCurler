#include <vector>
#include <QColor>

#include "ConnectionInfoModel.h"
#include "CurlWorker.h"
#include "Commons.h"

static std::vector<QString> lst_headers = {
  "URL", "Time", "Speed"
};

ConnectionInfoModel::ConnectionInfoModel(QObject *parent) : QAbstractTableModel(parent),
  m_lst_infos(CurlWorker::LstResources().size()) {

}

ConnectionInfoModel::~ConnectionInfoModel() {

}
///////////////////////////////////////////////////////////

int ConnectionInfoModel::rowCount(const QModelIndex &parent) const {
  UNUSED(parent);
  return CurlWorker::LstResources().size();
}
///////////////////////////////////////////////////////////

int ConnectionInfoModel::columnCount(const QModelIndex &parent) const {
  UNUSED(parent);
  return (int)lst_headers.size();
}
///////////////////////////////////////////////////////////

QVariant ConnectionInfoModel::data(const QModelIndex &index, int role) const {

  if (role == Qt::DecorationRole) {
    if (index.column() != 0)
      return QVariant();

    if (!m_lst_infos[index.row()].ir)
      return QVariant();

    QPixmap pm = QPixmap(m_lst_infos[index.row()].ir->img_path);
    if (pm.isNull())
      return QVariant();
    return pm.scaled(m_row_height, m_row_height, Qt::KeepAspectRatio);
  }

  if (role == Qt::DisplayRole) {
//    QPixmap pm = QPixmap(lst_infos[index.row()].
    if (index.column() == 0)
      return m_lst_infos[index.row()].url;
    if (index.column() == 1)
      return m_lst_infos[index.row()].time_total;
    if (index.column() == 2)
      return QVariant::fromValue<QString>(QString("%1 kB/sec").arg(m_lst_infos[index.row()].download_speed / 1024.0));
    return QVariant();
  }

  if (role == Qt::BackgroundColorRole) {
    if (!m_lst_infos[index.row()].ir)
      return QVariant();
    if (index.column() == 0)
      return QVariant();

    double coeff = 255000.0 / m_lst_infos[index.row()].ir->timeout_ms;
    int r = (int)(coeff * m_lst_infos[index.row()].time_total);
    if (r > 0xff) r = 0xff;
    int g = 0xff - r;
    return QColor(r, g, 0);
  }

  if (role == Qt::TextAlignmentRole) {
    return Qt::AlignCenter;
  }

  return QVariant();
}

QVariant ConnectionInfoModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      return lst_headers[section];
    }
    if (orientation == Qt::Vertical) {
      return section+1;
    }
  }
  return QVariant();
}
///////////////////////////////////////////////////////////

void ConnectionInfoModel::info_received(internet_resource_info info) {
  if (info.ir) {
    m_lst_infos[info.ir->ix] = info;
  }
  this->endResetModel();
}
///////////////////////////////////////////////////////////
