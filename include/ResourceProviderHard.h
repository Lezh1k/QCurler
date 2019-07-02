#ifndef RESOURCEPROVIDERHARD_H
#define RESOURCEPROVIDERHARD_H

#include "IResourceProvider.h"

class ResourceProviderHard : public IResourceProvider {
public:
  ResourceProviderHard() ;
  virtual ~ResourceProviderHard() { /*nothing to do*/ }  
  // IResourceProvider interface
public:
  void update();
};

#endif // RESOURCEPROVIDERHARD_H
