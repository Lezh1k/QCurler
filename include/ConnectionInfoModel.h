#ifndef CONNECTIONINFOMODEL_H
#define CONNECTIONINFOMODEL_H

#include <QAbstractTableModel>
#include <QFont>
#include "CurlWorker.h"
#include "IResourceProvider.h"

class ConnectionInfoModel : public QAbstractTableModel {
  Q_OBJECT
public:

  ConnectionInfoModel(QObject *parent);
  virtual ~ConnectionInfoModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const ;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  void setRowHeight(int32_t rh);
  void setResourceProvider(IResourceProvider *prov);

private:
  std::vector<InternetResourceInfo> m_lst_infos;
  int32_t m_row_height;
  IResourceProvider *m_prov;

public slots:
  void resourcesUpdated();
  void infoReceived(InternetResourceInfo info);
};

#endif // CONNECTIONINFOMODEL_H
