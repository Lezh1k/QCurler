#include "CurlWorker.h"
#include <QThread>
#include <assert.h>

static std::vector<internet_resource_t> lst_resources = {
  internet_resource_t("https://www.vk.com", 3000, false, false, 0),
  internet_resource_t("https://www.facebook.com", 3000, false, false, 1),
  internet_resource_t("https://www.instagram.com", 3000, false, false, 2),
  internet_resource_t("https://www.diesel.elcat.kg", 3000, true, true, 3),
  internet_resource_t("https://www.youtube.com", 3000, false, false, 4),
  internet_resource_t("https://www.google.com", 3000, false, false, 5),
};

CurlWorker::CurlWorker() {
}
///////////////////////////////////////////////////////

CurlWorker::~CurlWorker() {  
}
///////////////////////////////////////////////////////

int CurlWorker::multi_request(std::vector<internet_resource_t>& lst) {
  CURLM *cm;
  CURLMsg *msg;
  int32_t qmsg;
  int32_t running_handlers = -1;
  int32_t max_fd = 0;
  int32_t res;
  CURLMcode curl_ec;
  fd_set fds_read, fds_write, fds_exc;

  struct timeval t;
  long timeo;

  cm = curl_multi_init();
  if (cm == nullptr) {
    fprintf(stderr, "curl_multi_init() failed!\n");
    return -1;
  }

  for (std::vector<internet_resource_t>::iterator tir = lst.begin(); tir != lst.end(); ++tir)
    add_ir_to_multi(tir, cm);

  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, lst.size());

  while(m_isRunning) {
    curl_multi_perform(cm, &running_handlers);

    if (running_handlers) {
      FD_ZERO(&fds_read);
      FD_ZERO(&fds_write);
      FD_ZERO(&fds_exc);
      curl_ec = curl_multi_fdset(cm, &fds_read, &fds_write, &fds_exc, &max_fd);

      if (curl_ec != CURLM_OK) {
        return -1; //todo handle this
      }

      if(curl_multi_timeout(cm, &timeo)) {
        return -2; //todo handle
      }

      if(timeo == -1)
        timeo = 1000;

      if (max_fd == -1) {
        if (timeo) {
          QThread::currentThread()->usleep(timeo*1000);
        }
      } else {
        t.tv_sec = timeo/1000;
        t.tv_usec = (timeo%1000)*1000;
        res = select(max_fd+1, &fds_read, &fds_write, &fds_exc, &t);

        if (res < 0) {
          return -3;
        }
      } //if max_fd!=-1
    } //if running_handlers

    while((msg = curl_multi_info_read(cm, &qmsg))) {
      if (msg->msg != CURLMSG_DONE) {
        fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
        continue;
      }

      //      print_result_info(msg);
      emit_internet_resource_info(msg);
      curl_multi_remove_handle(cm, msg->easy_handle);
      curl_multi_add_handle(cm, msg->easy_handle); //make loop infinite
    } //while (msg!=NULL)
  } //while(is_running)


  printf("curl_multi_cleanup(cm)\n");

  for (auto tir = lst.begin(); tir != lst.end(); ++tir) {
    if (!tir->hCurl) continue;
    curl_easy_cleanup(tir->hCurl);
  }

  curl_multi_cleanup(cm);
  return 0;
}

int CurlWorker::add_ir_to_multi(std::vector<internet_resource_t>::iterator ir, CURLM *cm) {
  ir->hCurl = curl_easy_init();
  if (!ir->hCurl) {
    fprintf(stderr, "curl_easy_init() failed");
    return 1;
  }

  curl_easy_setopt(ir->hCurl, CURLOPT_URL, ir->url);
  curl_easy_setopt(ir->hCurl, CURLOPT_TIMEOUT_MS, ir->timeout_ms);
  curl_easy_setopt(ir->hCurl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(ir->hCurl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);

//  curl_easy_setopt(ir->hCurl, CURLOPT_NOBODY, 1L); //hide output of curl_easy_perform();

  if (ir->ua)
    curl_easy_setopt(ir->hCurl, CURLOPT_USERAGENT, ir->ua);

#ifdef VERBOSE
  curl_easy_setopt(ir->hCurl, CURLOPT_VERBOSE, 1L);
#endif

  if (ir->skip_peer_verification)
    curl_easy_setopt(ir->hCurl, CURLOPT_SSL_VERIFYPEER, 0L);
  if (ir->skip_hostname_verification)
    curl_easy_setopt(ir->hCurl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(ir->hCurl, CURLOPT_PRIVATE, ir);

  CURLMcode mres = curl_multi_add_handle(cm, ir->hCurl);
  if (mres != CURLM_OK) {
    return 1;
  }
  return 0;
}
///////////////////////////////////////////////////////////

void CurlWorker::emit_internet_resource_info(CURLMsg *msg) {
  CURL *hCurl;
  CURLcode res;
  internet_resource_info_t info;

  do {
    if (msg->data.result != CURLE_OK) {
      info.err_msg = curl_easy_strerror(msg->data.result);
    }

    info.success = true;
    hCurl = msg->easy_handle;
    res = curl_easy_getinfo(hCurl, CURLINFO_PRIVATE, (void*)&info.ir);
    if (res == CURLE_OK) {
      info.url = info.ir->url;
    }

    res = curl_easy_getinfo(hCurl, CURLINFO_SIZE_DOWNLOAD_T, &info.download_size);
    if (res != CURLE_OK || info.download_size <= 0) {
      //todo log
    }

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
  } while (0);
  emit info_received(info);
}
///////////////////////////////////////////////////////

const std::vector<internet_resource_t> &CurlWorker::LstResources() {
  return lst_resources;
}
///////////////////////////////////////////////////////////

void CurlWorker::Start() {  
  m_isRunning = true;
  multi_request(lst_resources);
}
///////////////////////////////////////////////////////

void CurlWorker::Stop() {
  m_isRunning = false;
  emit Stopped();
}
///////////////////////////////////////////////////////
