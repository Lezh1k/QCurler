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

  void setRowHeight(int32_t rh) { m_row_height = rh;}

private:
  std::vector<internet_resource_info> m_lst_infos;
  int32_t m_row_height;

public slots:
  void info_received(internet_resource_info info);
};

#endif // CONNECTIONINFOMODEL_H
