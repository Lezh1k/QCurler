#ifndef INTERNETRESOURCE_H
#define INTERNETRESOURCE_H

#include <stdint.h>
#include <QString>
#include <curl/curl.h>

struct InternetResource {
  QString url;
  QString img_path;
  QString name;
  QString ua; //user agent
  uint32_t timeout_ms;
  bool skip_peer_verification;
  bool skip_hostname_verification;
  uint8_t ix;

  InternetResource() :
    url(""),
    img_path(""),
    name(""),
    ua(""),
    timeout_ms(3000),
    skip_peer_verification(false),
    skip_hostname_verification(false),
    ix(0xff){}

  InternetResource(const char *a_url,
                   const char *a_img_path,
                   const char *a_name,
                   uint32_t a_timeout_ms,
                   bool a_skip_peer_verification,
                   bool a_skip_hostname_verification,
                   uint8_t a_ix) :
    url(a_url),
    img_path(a_img_path),
    name(a_name),
    ua(""),
    timeout_ms(a_timeout_ms),
    skip_peer_verification(a_skip_peer_verification),
    skip_hostname_verification(a_skip_hostname_verification),
    ix(a_ix){
  }

  bool operator==(const InternetResource& arg) {
    if (this == &arg) return true;
    return this->url == arg.url;
  }
};

inline bool operator==(const InternetResource& lhs, const InternetResource& rhs){
  if (&lhs == &rhs) return true;
  return lhs.url == rhs.url && lhs.img_path == rhs.img_path;
}

inline bool operator!=(const InternetResource& lhs,
                       const InternetResource& rhs){ return !(lhs == rhs); }
///////////////////////////////////////////////////////////

struct InternetResourceInfo {
  InternetResource ir;
  const char *err_msg;
  double time_total;
  curl_off_t download_speed;
  bool success;
  InternetResourceInfo() :
    err_msg(nullptr),
    time_total(0.0),
    download_speed(0),
    success(false){}

  QString speed_info(void) const {
    if (success)
      return QString("%1 kB/sec").arg(download_speed / 1024.0);
    return QString(err_msg);
  }
};
///////////////////////////////////////////////////////////

#endif // INTERNETRESOURCE_H
