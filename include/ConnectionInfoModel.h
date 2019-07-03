#ifndef CONNECTIONINFOMODEL_H
#define CONNECTIONINFOMODEL_H

#include <QAbstractTableModel>
#include <QFont>
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

  void setImageSize(int32_t rh);
  void resourceListUpdated(size_t size);

private:
  std::vector<InternetResourceInfo> m_lstIRI;
  int32_t m_imageSize;

public slots:
  void infoReceived(InternetResourceInfo info);
};

#endif // CONNECTIONINFOMODEL_H
