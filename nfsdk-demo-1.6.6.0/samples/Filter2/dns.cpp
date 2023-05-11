#include "stdafx.h"
#include <string>
#include <regex>

//#define DNS_DOMAIN_OFFSET 12
//
// BEGIN: PARSE DNS QUERY DOMAIN
// Parse domain payload, each sub-string of domain separated by dots is preceeded by a length field.
// So: www is 0x03, 0x77, 0x77, 0x77, etc
// The end is null terminated, so: 0x00
//
// The full domain name may not exceed the length of 253 characters in its textual representation. 
// In the internal binary representation of the DNS the maximum length requires 255 octets of storage, 
// as it also stores the length of the name.
//
// Alternative: https://github.com/mfontanini/libtins/blob/master/src/dns.cpp
//
std::string parse_dns_query_domain(const char* buf, int len) {
    std::string domain;
    int i = 12;
    while (i < len - 5 && *(buf + i) != 0) {
        int j = (int)*(buf + i);
        i++;
        while (j != 0 && i < len) {
            domain += *(buf + i);
            j--;
            i++;
        }
        if (j == 0 && i < len - 5) {
            if (*(buf + i + 1) != 0)
                domain += ".";
        }
    }
    return domain;
}
//
// END: PARSE DNS QUERY DOMAIN
//
