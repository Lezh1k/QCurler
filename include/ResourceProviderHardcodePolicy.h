#ifndef RESOURCEPROVIDERHARDCODEPOLICY_H
#define RESOURCEPROVIDERHARDCODEPOLICY_H

#include <vector>
#include "InternetResource.h"

class ResourceProviderHardcodePolicy {
public:
  ResourceProviderHardcodePolicy();

protected:
  ~ResourceProviderHardcodePolicy();
  static std::vector<InternetResource> updateListOfResources();
};

#endif // RESOURCEPROVIDERHARDCODEPOLICY_H
