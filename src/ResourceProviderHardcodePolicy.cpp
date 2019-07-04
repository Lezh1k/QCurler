#include "ResourceProviderHardcodePolicy.h"

#define def_timeout 5000
static std::vector<InternetResource> all = {
  InternetResource("https://www.vk.com", ":/ico/images/vk.png", "vk.com", def_timeout, false, false, 0),
  InternetResource("https://www.facebook.com", ":/ico/images/facebook.png", "facebook.com", def_timeout, false, false, 1),
  InternetResource("https://www.instagram.com", ":/ico/images/instagram.png", "instagram.com", def_timeout, false, false, 2),
  InternetResource("https://www.diesel.elcat.kg", ":/ico/images/diesel.png", "diesel.elcat.kg", def_timeout, true, true, 3),
  InternetResource("https://www.youtube.com", ":/ico/images/youtube.png", "youtube.com", def_timeout, false, false, 4),
  InternetResource("https://www.google.com", ":/ico/images/google.png", "google.com", def_timeout, false, false, 5)
};

ResourceProviderHardcodePolicy::ResourceProviderHardcodePolicy() {
}

ResourceProviderHardcodePolicy::~ResourceProviderHardcodePolicy() {
}

std::vector<InternetResource> ResourceProviderHardcodePolicy::updateListOfResources() {
  return all;
  /*uncomment for tests*/
  //  static int count = 1;
  //  m_resources = std::vector<InternetResource>(all.begin(), all.begin() + count);
  //  if (++count == static_cast<int>(all.size()))
  //    count = 1;
}
