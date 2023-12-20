//
// Created by yizhou on 20/12/2023.
//

#ifndef RSSPUSH_APN_HPP
#define RSSPUSH_APN_HPP
#include <jwt-cpp/jwt.h>
#include <pusher/base.hpp>
#ifdef DEBUG
#define APN_SERVER "https://api.sandbox.push.apple.com:443"
#else
#define APN_SERVER "https://api.push.apple.com:443"
#endif

namespace rss_push {

class apn_pusher_t: public pusher_base {
private:
    apn_config_t config;

public:
    explicit apn_pusher_t(const config_t& config) {
        this->config = config.apn;
    }

    Task<void> refresh() override {
        const auto now = std::chrono::system_clock::now();
        auto iat = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        access_token = jwt::create()
                .set_algorithm("ES256")
                .set_key_id(config.key_id)
                .set_payload_claim("iss", jwt::claim(config.issuer_key))
                .set_payload_claim("iat", jwt::claim(std::to_string(iat)))
                .sign(jwt::algorithm::es256{config.private_key});
    }

    Task<void> notify(const rss_push::notification_t &notification, const std::string& token) override {
        nlohmann::json payload;
        payload["aps"]["alert"]["title"] = notification.title;
        payload["aps"]["alert"]["body"] = notification.body;
        payload["aps"]["alert"]["launch_image"] = notification.image;

        std::string url = fmt::format("{}/3/device/{}", APN_SERVER, token);
        auto response = co_await fv::Post(url, fv::body_json(payload), fv::content_type("application/json"), fv::authorization(fmt::format("bearer {}", access_token)), fv::header("apns-push-type", "alert"));

    }
};

}

#endif //RSSPUSH_APN_HPP
