#include "ResourceProviderHard.h"

ResourceProviderHard::ResourceProviderHard() {
  static const int def_timeout = 5000;
  m_resources = {
    InternetResource("https://www.vk.com", ":/ico/images/vk.png", "vk.com", def_timeout, false, false, 0),
    InternetResource("https://www.facebook.com", ":/ico/images/facebook.png", "facebook.com", def_timeout, false, false, 1),
    InternetResource("https://www.instagram.com", ":/ico/images/instagram.png", "instagram.com", def_timeout, false, false, 2),
    InternetResource("https://www.diesel.elcat.kg", ":/ico/images/diesel.png", "diesel.elcat.kg", def_timeout, true, true, 3),
    InternetResource("https://www.youtube.com", ":/ico/images/youtube.png", "youtube.com", def_timeout, false, false, 4),
    InternetResource("https://www.google.com", ":/ico/images/google.png", "google.com", def_timeout, false, false, 5)};
}

void ResourceProviderHard::update() {
  /*do nothing*/
}
