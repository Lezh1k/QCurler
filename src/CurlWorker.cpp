#include "CurlWorker.h"
#include <QThread>
#include <assert.h>
#include "Commons.h"

CurlWorker::CurlWorker(IResourceProvider *prov) : m_provider(prov) {  
}
///////////////////////////////////////////////////////

CurlWorker::~CurlWorker() {  
}
///////////////////////////////////////////////////////

static size_t write_cb(void *ptr,
                      size_t size,
                      size_t nmemb,
                      void *data) {
  (void) ptr;
  (void) data;
  return size * nmemb;
}

int CurlWorker::multiRequest(const std::vector<InternetResource>& lst_resources) {
  CURLM *cm = nullptr;
  CURLMsg *msg = nullptr;
  int32_t qmsg;
  int32_t running_handlers = 0;
  int32_t max_fd = 0;
  int32_t res;
  CURLMcode curl_ec;
  fd_set fds_read, fds_write, fds_exc;
  struct timeval t;
  long timeout;
  uint32_t resources_count = static_cast<uint32_t>(lst_resources.size());

  cm = curl_multi_init();
  if (cm == nullptr) {
    fprintf(stderr, "curl_multi_init() failed!\n");
    return -1;
  }

  std::vector<CURL*> lst_h;
  lst_h.reserve(lst_resources.size());

  for (auto tir : lst_resources) {
    CURL* h = addInternetResourceToCURLM(tir, cm);
    if (h == nullptr) continue;

    CURLMcode mres = curl_multi_add_handle(cm, h);
    if (mres != CURLM_OK) {
      curl_easy_cleanup(h);
      continue;
    }
    lst_h.push_back(h);
  }

  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, lst_h.size());

  while(resources_count) {
    curl_multi_perform(cm, &running_handlers);

    if (running_handlers) {
      FD_ZERO(&fds_read);
      FD_ZERO(&fds_write);
      FD_ZERO(&fds_exc);
      curl_ec = curl_multi_fdset(cm, &fds_read, &fds_write, &fds_exc, &max_fd);

      if (curl_ec != CURLM_OK) {
        break;
      }

      if(curl_multi_timeout(cm, &timeout)) {
        break;
      }

      if(timeout < 0)
        timeout = 1000;

      if (max_fd == -1) {
        if (timeout) {
          QThread::currentThread()->usleep(timeout*1000);
        }
      } else {
        t.tv_sec = timeout/1000;
        t.tv_usec = (timeout%1000)*1000;
        res = select(max_fd+1, &fds_read, &fds_write, &fds_exc, &t);

        if (res < 0) {
          break;
        }
      } //if max_fd!=-1
    } //if running_handlers

    while((msg = curl_multi_info_read(cm, &qmsg))) {
      if (msg->msg != CURLMSG_DONE) {
        fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
        continue;
      }

      emit_internet_resource_info(msg);
      curl_multi_remove_handle(cm, msg->easy_handle);
      --resources_count;
    } //while (msg!=NULL)
  } //while(resources_count)

  std::for_each(lst_h.begin(), lst_h.end(), [&](CURL* h) {curl_easy_cleanup(h);});
  curl_multi_cleanup(cm);
  return 0;
}
///////////////////////////////////////////////////////

CURL* CurlWorker::addInternetResourceToCURLM(const InternetResource &ir, CURLM *cm) {
  CURL* hCurl = curl_easy_init();
  if (!hCurl) {
    fprintf(stderr, "curl_easy_init() failed");
    return nullptr;
  }

  curl_easy_setopt(hCurl, CURLOPT_URL, ir.url);
  curl_easy_setopt(hCurl, CURLOPT_TIMEOUT_MS, ir.timeout_ms);
  curl_easy_setopt(hCurl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(hCurl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
  curl_easy_setopt(hCurl, CURLOPT_WRITEFUNCTION, write_cb);

  if (ir.ua)
    curl_easy_setopt(hCurl, CURLOPT_USERAGENT, ir.ua);

#ifdef VERBOSE
  curl_easy_setopt(hCurl, CURLOPT_VERBOSE, 1L);
#endif

  if (ir.skip_peer_verification)
    curl_easy_setopt(hCurl, CURLOPT_SSL_VERIFYPEER, 0L);
  if (ir.skip_hostname_verification)
    curl_easy_setopt(hCurl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(hCurl, CURLOPT_PRIVATE, reinterpret_cast<void*>(ir.ix));
  return hCurl;
}
///////////////////////////////////////////////////////////

void CurlWorker::emit_internet_resource_info(const CURLMsg *msg) {
  CURL *hCurl;
  CURLcode res;
  InternetResourceInfo info;  
  uintptr_t ix; //HACK!

  info.success = msg->data.result == CURLE_OK;
  if (!info.success) {
    info.err_msg = curl_easy_strerror(msg->data.result);
  }

  hCurl = msg->easy_handle;
  res = curl_easy_getinfo(hCurl, CURLINFO_PRIVATE, &ix);

  if (res != CURLE_OK || ix < 0 || ix >= m_provider->resources().size())
    return; //do nothing

  info.ir = m_provider->resources()[ix]; //still dangerous
  /* check for total download time */
  res = curl_easy_getinfo(hCurl, CURLINFO_TOTAL_TIME, &info.time_total);
  if(CURLE_OK != res) {
    //todo log
  }

  /* check for average download speed */
  res = curl_easy_getinfo(hCurl, CURLINFO_SPEED_DOWNLOAD_T, &info.download_speed);
  if((CURLE_OK != res) || (info.download_speed<=0)) {
    //todo log
  }

  emit infoReceived(info);
}
///////////////////////////////////////////////////////

void CurlWorker::Start() {  
  m_isRunning = true;
  uint32_t counter = 0;
  while (m_isRunning) {
    multiRequest(m_provider->resources());
    if (counter++ != 10)
      continue;
    m_provider->update();
    emit resourcesUpdated();
    QThread::currentThread()->usleep(200*1000);
    counter = 0;
  }
}
///////////////////////////////////////////////////////

void CurlWorker::Stop() {
  m_isRunning = false;
  emit stopped();
}
///////////////////////////////////////////////////////
