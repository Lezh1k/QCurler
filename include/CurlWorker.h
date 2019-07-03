#ifndef CURLER_H
#define CURLER_H

#include <QObject>
#include <curl/curl.h>
#include <curl/multi.h>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <mutex>
#include "ResourceProvider.h"

class CurlWorker : public QObject {
  Q_OBJECT

private:
  volatile bool m_isRunning;  
  std::mutex m_mut;
  std::vector<InternetResource> m_lstResources;

  CurlWorker(const CurlWorker&); //prohibited
  CurlWorker& operator=(const CurlWorker&); //prohibited

  int multiRequest(const std::vector<InternetResource> &lst);
  CURL *addInternetResourceToCURLM(const InternetResource &ir);
  void emit_ir_info(const CURLMsg *msg);

  void run();

public:
  CurlWorker() = default;
  virtual ~CurlWorker() = default;
  void stop();
  void updateResourceList(const std::vector<InternetResource> &lst);

signals:
  void stopped();
  void infoReceived(InternetResourceInfo info);

public slots:  
  void start();
};

#endif // CURLER_H
