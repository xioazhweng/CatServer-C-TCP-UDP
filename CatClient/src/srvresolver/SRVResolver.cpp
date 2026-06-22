
#include "SRVResolver.h"

SRVRecord query_srv(const std::string & name) {
    unsigned char buf[4096];
    int len = res_query(name.c_str(), C_IN, T_SRV, buf, sizeof(buf));
    if (len < 0)
        throw std::runtime_error("SRV query failed: " + name);

    ns_msg msg;
    ns_initparse(buf, len, &msg);

    int count = ns_msg_count(msg, ns_s_an);
    if (count <= 0)
        throw std::runtime_error("No SRV records for: " + name);

    ns_rr rr;
    ns_parserr(&msg, ns_s_an, 0, &rr);

    const unsigned char * rd = ns_rr_rdata(rr);
    // SRV rdata: priority(2) weight(2) port(2) target(name)
    uint16_t port = ntohs(*reinterpret_cast<const uint16_t*>(rd + 4));

    char target[256];
    dn_expand(buf, buf + len, rd + 6, target, sizeof(target));

    return {target, port};
}
