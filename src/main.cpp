#include <QApplication>
#include <curl/curl.h>
#include "mainwindow.h"
#include "CurlWorker.h"
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (res != CURLE_OK) {
    return -1;
  }

  qRegisterMetaType<internet_resource_info>("internet_resource_info");

  QPixmap pm = QPixmap(":/ico/images/vk.png");

  MainWindow w;
  w.show();
  int mr = a.exec();
  curl_global_cleanup();
  return mr;
}
