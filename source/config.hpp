//
// Created by yihtseu on 2023/12/19.
//

#include <string>
#include <boost/property_tree/ptree.hpp>

struct config_t {

    std::string host;

    struct {
        std::string filename;
    } database;

    struct {
        std::string email, key_id;
    } fcm_config;

    struct {
        std::string client_secret, client_id, project_id;
    } hms_config;

    struct {
        std::string issuer_key, key_id;
    } apn_config;
};

config_t deserialize(const boost::property_tree::ptree& root) {
    config_t config;
    config.host = root.get<std::string>("host");

    config.database.filename = root.get<std::string>("database.filename");

    config.fcm_config.email = root.get<std::string>("fcm.email");
    config.fcm_config.key_id = root.get<std::string>("fcm.keyId");

    config.hms_config.client_id = root.get<std::string>("hms.clientId");
    config.hms_config.client_secret = root.get<std::string >("hms.clientSecret");
    config.hms_config.project_id = root.get<std::string>("hms.projectId");

    config.apn_config.issuer_key = root.get<std::string>("apn.issuerKey");
    config.apn_config.key_id = root.get<std::string>("apn.keyId");

    return config;
}

boost::property_tree::ptree enserialize(const config_t& config) {
    boost::property_tree::ptree root;
    root.put("host", config.host);

    root.put("database.filename", config.database.filename);

    root.put("fcm.email", config.fcm_config.email);
    root.put("fcm.keyId", config.fcm_config.key_id);

    root.put("hms.clientId", config.hms_config.client_id);
    root.put("hms.clientSecret", config.hms_config.client_secret);
    root.put("hms.projectId", config.hms_config.project_id);

    root.put("apn.keyId", config.apn_config.key_id);
    root.put("apn.issuerKey", config.apn_config.issuer_key);

    return root;
}