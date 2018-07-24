#ifndef CURLER_H
#define CURLER_H

#include <QObject>
#include <curl/curl.h>
#include <curl/multi.h>

struct internet_resource_t {
  const char *url;
  const char *ua; //user agent
  CURL *hCurl;
  uint32_t timeout_ms;
  bool skip_peer_verification;
  bool skip_hostname_verification;
  uint8_t ix;
  uint8_t padding[1]; //on 64x machines. we assume, that sizeof(bool) == 1;

  internet_resource_t(const char *a_url, uint32_t a_timeout_ms,
                      bool a_skip_peer_verification, bool a_skip_hostname_verification,
                      uint8_t a_ix) :
    url(a_url),
    ua(nullptr),
    hCurl(nullptr),
    timeout_ms(a_timeout_ms),
    skip_peer_verification(a_skip_peer_verification),
    skip_hostname_verification(a_skip_hostname_verification),
    ix(a_ix){
  }
};
///////////////////////////////////////////////////////////

struct internet_resource_info_t {
  internet_resource_t *ir;
  const char *url;
  const char *err_msg;
  double time_total;
  long download_size;
  long download_speed;
  bool success;
  uint8_t padding[7];
  internet_resource_info_t() : ir(nullptr), url(nullptr),
    err_msg(nullptr), time_total(0.0), download_size(0l), download_speed(0l), success(false){}
};
///////////////////////////////////////////////////////////

class CurlWorker : public QObject {
  Q_OBJECT

private:
  bool m_isRunning;
  CurlWorker(const CurlWorker&); //prohibited
  CurlWorker& operator=(const CurlWorker&); //prohibited

  int multi_request(std::vector<internet_resource_t> &lst);
  int add_ir_to_multi(std::vector<internet_resource_t>::iterator ir, CURLM *cm);
  void emit_internet_resource_info(CURLMsg *msg);

public:
  CurlWorker();
  virtual ~CurlWorker();

  static const std::vector<internet_resource_t> &LstResources();
  void Stop();

signals:
  void Stopped();
  void info_received(internet_resource_info_t info);

public slots:
  void Start();
};

#endif // CURLER_H
