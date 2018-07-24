#ifndef CONNECTIONINFOMODEL_H
#define CONNECTIONINFOMODEL_H

#include <QAbstractTableModel>
#include "CurlWorker.h"

class ConnectionInfoModel : public QAbstractTableModel {
  Q_OBJECT
public:

  ConnectionInfoModel(QObject *parent);
  virtual ~ConnectionInfoModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const ;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
  std::vector<internet_resource_info_t> lst_infos;

public slots:
  void info_received(internet_resource_info_t info);
};

#endif // CONNECTIONINFOMODEL_H
