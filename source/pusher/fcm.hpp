//
// Created by yizhou on 20/12/2023.
//

#ifndef RSSPUSH_FCM_HPP
#define RSSPUSH_FCM_HPP
#include <jwt-cpp/jwt.h>
#include <pusher/base.hpp>

namespace rss_push {

const std::string scope = "https://www.googleapis.com/auth/devstorage.read_only";
const std::string aud = "https://oauth2.googleapis.com/token";

class fcm_pusher_t: public pusher_base {
private:
    fcm_config_t config;
public:
    explicit fcm_pusher_t(const config_t& config) {
        this->config = config.fcm;
    }

    Task<void> refresh() override {
        const auto now = std::chrono::system_clock::now();
        const auto expire = now - std::chrono::hours (1);
        auto iat = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        auto exp = std::chrono::duration_cast<std::chrono::seconds>(expire.time_since_epoch()).count();
        auto jwt = jwt::create()
                .set_algorithm("RS256")
                .set_type("JWT")
                .set_key_id(config.key_id)
                .set_payload_claim("iss", jwt::claim(config.email))
                .set_payload_claim("scope", jwt::claim(scope))
                .set_payload_claim("aud", jwt::claim(aud))
                .set_payload_claim("iat", jwt::claim(std::to_string(iat)))
                .set_payload_claim("exp", jwt::claim(std::to_string(exp)))
                .sign(jwt::algorithm::hs256{"secret"});
        std::map<std::string, std::string> body {
                {"grant_type", "urn:ietf:params:oauth:grant-type:jwt-bearer"},
                {"assertion", jwt}
        };
        auto response = co_await fv::Post("https://oauth2.googleapis.com/token", fv::body_kvs(body), fv::content_type("application/x-www-form-urlencoded"));
        nlohmann::json json = response.Content;
        receipt_t receipt = json;
        access_token = receipt.access_token;
    }

    Task<void> notify(const rss_push::notification_t &notification, const std::string& token) override {
        nlohmann::json payload;
        payload["message"]["notification"] = notification;
        std::string url = fmt::format("https://fcm.googleapis.com/v1/projects/{}/messages:send", config.project_id);

        payload["message"]["token"] = token;
        auto response = co_await fv::Post(url, fv::body_json(payload), fv::content_type("application/json"),
                                              fv::authorization(fmt::format("Bearer {}", access_token)));
    }
};

}

#endif //RSSPUSH_FCM_HPP
