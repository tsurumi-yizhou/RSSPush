//
// Created by yihtseu on 2023/12/19.
//

#ifndef RSSPUSH_CONFIG_HPP
#define RSSPUSH_CONFIG_HPP

#include <nlohmann/json.hpp>

namespace rss_push {

struct feed_t {
    std::string link;
    std::uint64_t updated;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(feed_t, link, updated)

struct database_t {
    std::string host, file;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(database_t, host, file)

struct fcm_config_t {
    std::string project_id, private_key_id, private_key, client_email, client_id, scope;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(fcm_config_t, project_id, private_key_id, private_key, client_email, client_id, scope)

struct hms_config_t {
    std::string client_secret, client_id, project_id;

};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(hms_config_t, client_id, client_secret, project_id)

struct apn_config_t {
    std::string issuer_key, key_id, private_key;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(apn_config_t, issuer_key, key_id, private_key)

struct config_t {
    std::vector<feed_t> feeds;
    database_t database;
    fcm_config_t fcm;
    hms_config_t hms;
    apn_config_t apn;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(config_t, feeds, database, fcm, hms, apn)

}

#endif