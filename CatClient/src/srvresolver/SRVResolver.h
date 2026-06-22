#pragma once

#include <string>
#include <stdexcept>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
struct SRVRecord {
    std::string host;
    uint16_t    port;
};
SRVRecord query_srv(const std::string & name);