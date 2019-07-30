#ifndef RESOURCEPROVIDERNETWORKSERVICEPOLICY_H
#define RESOURCEPROVIDERNETWORKSERVICEPOLICY_H

#include <vector>
#include "InternetResource.h"

class QNetworkAccessManager;
class QNetwokReply;
class QNetworkRequest;
class QNetworkReply;
class QJsonObject;

class ResourceProviderNetworkServicePolicy {
private:
  std::vector<InternetResource> m_lstResources;
  QNetworkAccessManager *m_network_manager;
  void updateListOfResourcesImpl();
  QString downloadImgToTemp(const QString &imgUrl, bool &success);

  //static methods
  static bool parseReply(const QJsonObject &obj, InternetResource &ir);
  static ResourceProviderNetworkServicePolicy& Instance(void);
  static QNetworkReply* getReply(QNetworkAccessManager* nam,
                                 QNetworkRequest& req);
  static void preHandleReply(const QNetworkReply *reply,
                             int &http_code,
                             int &err_code,
                             int &network_error);
public:
  ResourceProviderNetworkServicePolicy();
  ResourceProviderNetworkServicePolicy(ResourceProviderNetworkServicePolicy const &) = delete;
  void operator =(ResourceProviderNetworkServicePolicy const &) = delete;

protected:
  ~ResourceProviderNetworkServicePolicy(); //because we don't want vptr here.
  static std::vector<InternetResource> updateListOfResources();
};

#endif // RESOURCEPROVIDERNETWORKSERVICEPOLICY_H
