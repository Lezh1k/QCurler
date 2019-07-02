#include <vector>
#include <QColor>

#include "ConnectionInfoModel.h"
#include "CurlWorker.h"
#include "Commons.h"

static std::vector<QString> lst_headers = {
  "URL", "Time", "Speed"
};

ConnectionInfoModel::ConnectionInfoModel(QObject *parent) : QAbstractTableModel(parent) {
}

ConnectionInfoModel::~ConnectionInfoModel() {

}
///////////////////////////////////////////////////////////

int ConnectionInfoModel::rowCount(const QModelIndex &parent) const {
  UNUSED(parent);
  return static_cast<int>(m_lst_infos.size());
}
///////////////////////////////////////////////////////////

int ConnectionInfoModel::columnCount(const QModelIndex &parent) const {
  UNUSED(parent);
  return static_cast<int>(lst_headers.size());
}
///////////////////////////////////////////////////////////

QVariant ConnectionInfoModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DecorationRole) {
    if (index.column() != 0)
      return QVariant();
    QPixmap pm = QPixmap(m_lst_infos[index.row()].ir.img_path);
    if (pm.isNull())
      return QVariant();
    return pm.scaled(m_row_height, m_row_height, Qt::KeepAspectRatio);
  }

  if (role == Qt::DisplayRole) {
    if (index.column() == 0)
      return m_lst_infos[index.row()].ir.name;
    if (index.column() == 1)
      return m_lst_infos[index.row()].time_total;
    if (index.column() == 2)
      return m_lst_infos[index.row()].speed_info();
    return QVariant();
  }

  if (role == Qt::BackgroundColorRole) {    
    if (index.column() == 0)
      return QVariant();
    if (!m_lst_infos[index.row()].success)
      return QColor(0xff, 0, 0);

    double coeff = 255000.0 / m_lst_infos[index.row()].ir.timeout_ms;
    int r = static_cast<int>(coeff * m_lst_infos[index.row()].time_total);
    if (r > 0xff) r = 0xff;
    int g = 0xff - r;
    return QColor(r, g, 0);
  }

  if (role == Qt::TextAlignmentRole) {
    return Qt::AlignCenter;
  }

  return QVariant();
}

QVariant ConnectionInfoModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
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
///////////////////////////////////////////////////////

void ConnectionInfoModel::setRowHeight(int32_t rh) {
  m_row_height = rh;
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::setResourceProvider(IResourceProvider *prov) {
  m_prov = prov;
  resourcesUpdated();
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::resourcesUpdated() {
  m_lst_infos = std::vector<InternetResourceInfo>(m_prov->resources().size());
}
///////////////////////////////////////////////////////

void ConnectionInfoModel::infoReceived(InternetResourceInfo info) {
  if (info.ir.ix >= m_lst_infos.size())
    return;
  m_lst_infos[info.ir.ix] = info;
  this->endResetModel();
}
///////////////////////////////////////////////////////////
