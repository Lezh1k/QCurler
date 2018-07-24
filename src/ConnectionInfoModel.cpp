#include <vector>

#include "ConnectionInfoModel.h"
#include "CurlWorker.h"
#include "Commons.h"

static std::vector<QString> lst_headers = {
  "URL", "Time", "Size", "Speed"
};

ConnectionInfoModel::ConnectionInfoModel(QObject *parent) : QAbstractTableModel(parent),
  lst_infos(CurlWorker::LstResources().size()) {

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
  if (role == Qt::DisplayRole) {
    switch (index.column()) {
    case 0:
      return lst_infos[index.row()].url;
    case 1:
      return lst_infos[index.row()].time_total;
    case 2:
      return QVariant::fromValue<long>(lst_infos[index.row()].download_size);
    case 3:
      return QVariant::fromValue<double>(lst_infos[index.row()].download_speed / 1024.0);
    default:
      return QVariant();
    }
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

void ConnectionInfoModel::info_received(internet_resource_info_t info) {
  if (info.ir) {
    lst_infos[info.ir->ix] = info;
  }
  this->endResetModel();
}
///////////////////////////////////////////////////////////
