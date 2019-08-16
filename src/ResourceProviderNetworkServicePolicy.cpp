#include <QString>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>
#include <QUrl>
#include "Commons.h"
#include "ResourceProviderNetworkServicePolicy.h"

ResourceProviderNetworkServicePolicy::ResourceProviderNetworkServicePolicy() :
  m_url_management("http://192.168.20.128:8000") {

  QString appDir = qApp->applicationDirPath();
  static const QString urlFile = "url.cfg";
  QString cfgPath = appDir + QDir::separator() + urlFile;
  QFile cfgF(cfgPath);
  do {
    if (!cfgF.open(QFile::ReadOnly))
      break;
    QString tmp = cfgF.readAll();
    QUrl turl;
    if (turl.isEmpty() || !turl.isValid())
      break;
    m_url_management = tmp;
  } while (0);
}

ResourceProviderNetworkServicePolicy::~ResourceProviderNetworkServicePolicy() {
}
///////////////////////////////////////////////////////

ResourceProviderNetworkServicePolicy&
ResourceProviderNetworkServicePolicy::Instance() {
  static ResourceProviderNetworkServicePolicy intance_;
  return intance_;
}
///////////////////////////////////////////////////////

size_t
ResourceProviderNetworkServicePolicy::updateResWriteFunc(void *ptr,
                                                         size_t size,
                                                         size_t nmemb,
                                                         std::string* data) {
  data->append(static_cast<char*>(ptr), size * nmemb);
  return size * nmemb;
}
///////////////////////////////////////////////////////

size_t
ResourceProviderNetworkServicePolicy::downloadImgWriteFunc(void *ptr,
                                                           size_t size,
                                                           size_t nmemb,
                                                           void *stream) {
  size_t written = std::fwrite(ptr, size, nmemb, static_cast<std::FILE*>(stream));
  return written;
}
///////////////////////////////////////////////////////

void
ResourceProviderNetworkServicePolicy::updateListOfResourcesImpl() {
  const QString urlStr(QString("%1/api/resources").arg(m_url_management));
  CURL *hCurl = curl_easy_init();
  if (!hCurl) {
    //!todo log
    return;
  }
  Defer curlCleanup(std::bind([&hCurl](){curl_easy_cleanup(hCurl);}));

  curl_easy_setopt(hCurl, CURLOPT_URL, urlStr.toStdString().c_str());
  curl_easy_setopt(hCurl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hCurl, CURLOPT_USERAGENT, "curl/7.42.0");
  curl_easy_setopt(hCurl, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hCurl, CURLOPT_TCP_KEEPALIVE, 1L);

  std::string strResponse;
  curl_easy_setopt(hCurl, CURLOPT_WRITEFUNCTION, updateResWriteFunc);
  curl_easy_setopt(hCurl, CURLOPT_WRITEDATA, &strResponse);

  long rc;
  curl_easy_getinfo(hCurl, CURLINFO_RESPONSE_CODE, &rc);
  curl_easy_perform(hCurl);

  if (rc != CURLE_OK) {
    //    qDebug() << rc;
    return;
  }

  QByteArray arr(strResponse.c_str(), strResponse.size());
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

  QString fileName = imgUrlStr;
  fileName.replace("\\", "_").replace("/", "_").replace(":", "_");
  fileName = QString("img_%1.pix").arg(fileName);
  QString fullFilePath = tmpPathDir + QDir::separator() + fileName;

  if (QFile::exists(fullFilePath)) {
    success = true;
    return fullFilePath;
  }

  CURL *hCurl;
  std::FILE *fImg;
  /* init the curl session */
  hCurl = curl_easy_init();
  if (!hCurl) {
    //!todo log
    success = false;
    return "";
  }
  Defer curlCleanup(std::bind([&hCurl](){curl_easy_cleanup(hCurl);}));

  fImg = std::fopen(fullFilePath.toStdString().c_str(), "wb");
  if (!fImg) {
    //!todo log
    success = false;
    return "";
  }
  Defer imgClose(std::bind([&fImg](){std::fclose(fImg);}));

  curl_easy_setopt(hCurl, CURLOPT_URL, imgUrlStr.toStdString().c_str());
  curl_easy_setopt(hCurl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hCurl, CURLOPT_USERAGENT, "curl/7.42.0");
  curl_easy_setopt(hCurl, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hCurl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(hCurl, CURLOPT_TCP_KEEPALIVE, 1L);

  curl_easy_setopt(hCurl, CURLOPT_WRITEFUNCTION, downloadImgWriteFunc);
  curl_easy_setopt(hCurl, CURLOPT_WRITEDATA, fImg);

  long rc;
  curl_easy_getinfo(hCurl, CURLINFO_RESPONSE_CODE, &rc);
  curl_easy_perform(hCurl);

  if (rc != CURLE_OK) {
    success = false;
    return "";
  }
  success = true;
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

std::vector<InternetResource>
ResourceProviderNetworkServicePolicy::updateListOfResources() {
  Instance().updateListOfResourcesImpl();
  return Instance().m_lstResources; //copy of vector
}
///////////////////////////////////////////////////////
