#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QString>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <algorithm>
#include "Commons.h"
#include "ResourceProviderNetworkServicePolicy.h"

#include <QDebug>
//!todo init this from some config.
static QString url_management = "http://localhost:8000";

ResourceProviderNetworkServicePolicy&
ResourceProviderNetworkServicePolicy::Instance() {
  static ResourceProviderNetworkServicePolicy intance_;
  return intance_;
}
///////////////////////////////////////////////////////

QNetworkReply*
ResourceProviderNetworkServicePolicy::getReply(QNetworkAccessManager* nam,
                                               QNetworkRequest& req) {
  req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                   QNetworkRequest::AlwaysNetwork);
  if (nam->networkAccessible() != QNetworkAccessManager::Accessible) {
    nam->setNetworkAccessible(QNetworkAccessManager::Accessible);
  }
  QNetworkReply* reply = nam->get(req);
  reply->ignoreSslErrors();


  QTimer* timer = new QTimer;
  timer->setInterval(10000);
  timer->setSingleShot(true);
  timer->start();

  QObject::connect(timer, &QTimer::timeout, [reply]() {
    if (reply) reply->close();
  });
  QObject::connect(reply, &QNetworkReply::finished, timer, &QTimer::stop);
  QObject::connect(reply, &QNetworkReply::finished, timer, &QTimer::deleteLater);
  QObject::connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
  QObject::connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
                   reply, &QNetworkReply::close);
  return reply;
}
///////////////////////////////////////////////////////

void
ResourceProviderNetworkServicePolicy::preHandleReply(const QNetworkReply* reply,
                                                     int& http_code,
                                                     int& err_code,
                                                     int& network_error) {
  bool parsed = false;
  http_code =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(&parsed);
  err_code = 0; //todo remove magic numbers
  network_error = 0;
  if (reply->error() != QNetworkReply::NoError) {
    network_error = reply->error();
    err_code = 1; //todo remove magic numbers
  }
}
///////////////////////////////////////////////////////

void
ResourceProviderNetworkServicePolicy::updateListOfResourcesImpl() {
  const QString urlStr(QString("%1/api/resources").arg(url_management));
  QUrl urlUpdate(urlStr);
  QNetworkRequest request(urlUpdate);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  QNetworkReply *reply = getReply(m_network_manager, request);
  reply->ignoreSslErrors();
  QObject::connect(reply, &QNetworkReply::finished,
                   [reply, this]() {
    if (reply == nullptr)
      return;

    int http_code, err_code, network_error;
    preHandleReply(reply, http_code, err_code, network_error);

    if (err_code || network_error)
      return; //!todo log!

    QByteArray arr = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    if (doc.isNull() || doc.isEmpty() || !doc.isArray()) {
      //!todo log at least
      return;
    }

    QJsonArray jsonArr = doc.array();
    std::vector<InternetResource> lstResources;
    for (auto it : jsonArr) {
      if (!it.isObject())
        continue;

      QJsonObject obj = it.toObject();
      InternetResource ir;
      if (!parseReply(obj, ir))
        continue;

      ir.ix = lstResources.size();
      lstResources.push_back(ir);
    } //for it : jsonArr
    m_lstResources = lstResources;
  }); //finished, connect.
}
///////////////////////////////////////////////////////

QString
ResourceProviderNetworkServicePolicy::downloadImgToTemp(const QString &imgUrlStr,
                                                        bool &success) {
  success = false;
  QString tmpPathDir = QCoreApplication::instance()->applicationDirPath();
  QStringList lstTempLocations = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
  if (!lstTempLocations.empty())
    tmpPathDir = lstTempLocations[0];

  success = true;
  QString fileName = imgUrlStr;
  fileName.replace("\\", "_").replace("/", "_").replace(":", "_");
  fileName = QString("img_%1.pix").arg(fileName);
  QString fullFilePath = tmpPathDir + QDir::separator() + fileName;

  success = true;
  if (QFile::exists(fullFilePath)) {
    return fullFilePath;
  }

  QUrl imgUrl(imgUrlStr);
  QNetworkRequest request(imgUrl);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
  QNetworkReply *reply = getReply(m_network_manager, request);

  QObject::connect(reply, &QNetworkReply::finished, [reply, fullFilePath]() {
    if (reply == nullptr)
      return;

    int http_code, err_code, network_error;
    preHandleReply(reply, http_code, err_code, network_error);

    if (err_code || network_error)
      return; //!todo log!

    QByteArray arr = reply->readAll();
    QFile f(fullFilePath);
    if (!f.open(QFile::WriteOnly)) {
      //todo log
      return;
    }
    Defer fcl(std::bind([&f](){f.close();}));
    f.write(arr);
  });

  return fullFilePath;
}
///////////////////////////////////////////////////////

bool
ResourceProviderNetworkServicePolicy::parseReply(const QJsonObject &obj,
                                                 InternetResource &ir) {
  if (obj.find("url") == obj.end())
    return false;
  ir.url = obj["url"].toString();
  if (obj.find("skip_hostname_verification") != obj.end())
    ir.skip_hostname_verification = obj["skip_hostname_verification"].toInt(0) != 0;
  if (obj.find("skip_peer_verification") != obj.end())
    ir.skip_peer_verification = obj["skip_peer_verification"].toInt(0) != 0;
  if (obj.find("timeout") != obj.end())
    ir.timeout_ms = static_cast<uint32_t>(obj["timeout"].toInt(3000));
  if (obj.find("title") != obj.end())
    ir.name = obj["title"].toString();

  do {
    if (obj.find("picture") == obj.end())
      break;
    bool d = false;
    QString path = Instance().downloadImgToTemp(obj["picture"].toString(), d);
    if (!d)
      break;
    ir.img_path = path;
  } while (0);
  return true;
}
///////////////////////////////////////////////////////

ResourceProviderNetworkServicePolicy::ResourceProviderNetworkServicePolicy() :
  m_network_manager(nullptr) {
  //!todo init from config etc
  m_network_manager = new QNetworkAccessManager;
}

ResourceProviderNetworkServicePolicy::~ResourceProviderNetworkServicePolicy() {
  if (m_network_manager)
    delete m_network_manager;
}
///////////////////////////////////////////////////////

std::vector<InternetResource> ResourceProviderNetworkServicePolicy::updateListOfResources() {
  Instance().updateListOfResourcesImpl();
  return Instance().m_lstResources; //copy of vector
}
///////////////////////////////////////////////////////
