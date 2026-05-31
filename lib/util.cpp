//
// Created by cbenesch on 5/31/26.
//
#include <string>
#include <cstring>

#include "util.h"

std::string handleError(int errnum,const std::string& errorMessage) {
  if (errnum) {
    char errbuf[256];
    if (strerror_r(errnum, errbuf, sizeof(errbuf)-1) != nullptr) {
      size_t len = strlen(errbuf);
      if (len) {
        return {errbuf, len};
      }
    }
  }
  return errorMessage;
}