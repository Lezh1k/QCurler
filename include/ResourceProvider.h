#ifndef RESOURCEPROVIDER_H
#define RESOURCEPROVIDER_H

#include <vector>
#include "InternetResource.h"

template <class UpdatePolicy> class ResourceProvider : public UpdatePolicy {
private:
  std::vector<InternetResource> m_resources;

public:
  ResourceProvider() = default;
  ~ResourceProvider() = default;

  bool update() {
    std::vector<InternetResource> newLst = UpdatePolicy::updateListOfResources();
    if (newLst == m_resources)
      return false;
    m_resources = std::move(newLst);
    return true;
  }

  const std::vector<InternetResource>& resources() const {return m_resources;}
};

#endif // RESOURCEPROVIDER_H
