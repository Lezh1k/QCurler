#ifndef CURLER_H
#define CURLER_H

#include <QObject>
#include <curl/curl.h>
#include <curl/multi.h>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <mutex>
#include "IResourceProvider.h"

class CurlWorker : public QObject {
  Q_OBJECT

private:
  volatile bool m_isRunning;  
  IResourceProvider *m_provider;  

  CurlWorker(const CurlWorker&); //prohibited
  CurlWorker& operator=(const CurlWorker&); //prohibited

  int multiRequest(const std::vector<InternetResource> &lst);
  CURL *addInternetResourceToCURLM(const InternetResource &ir, CURLM *cm);
  void emit_internet_resource_info(const CURLMsg *msg);

public:
  CurlWorker(IResourceProvider *prov);
  virtual ~CurlWorker();
  void Stop();

signals:
  void stopped();
  void infoReceived(InternetResourceInfo info);
  void resourcesUpdated();

public slots:  
  void Start();
};

#endif // CURLER_H
