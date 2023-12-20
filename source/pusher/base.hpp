//
// Created by yizhou on 20/12/2023.
//

#ifndef RSSPUSH_PUSHERBASE_HPP
#define RSSPUSH_PUSHERBASE_HPP
#include <fv/fv.h>
#include <config.hpp>

namespace rss_push {

struct notification_t {
    std::string title, body, image, topic;
};

void from_json(const nlohmann::json& json, notification_t& notification) {
    notification.title = json["title"];
    notification.body = json["body"];
    notification.image = json["image"];
    notification.topic = json["topic"];
}

void to_json(nlohmann::json& json, const notification_t& notification) {
    json["title"] = notification.title;
    json["body"] = notification.body;
    if (!json["image"].empty()) {
        json["image"] = notification.image;
    }
    if (!json["topic"].empty()) {
        json["topic"] = notification.topic;
    }
}

struct receipt_t {
    std::string access_token, token_type;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(receipt_t, access_token, token_type)

class pusher_base: public std::enable_shared_from_this<pusher_base> {
protected:
    std::string access_token;
public:
    virtual Task<void> refresh () {};
    virtual Task<void> notify(const notification_t&, const std::string&) {};
};

}

#endif //RSSPUSH_PUSHERBASE_HPP
