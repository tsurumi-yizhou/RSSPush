//
// Created by yizhou on 20/12/2023.
//

#ifndef RSSPUSH_AGC_HPP
#define RSSPUSH_AGC_HPP

#include <pusher/base.hpp>

#ifdef DEBUG
#define VALIDATE true
#else
#define VALIDATE false
#endif

namespace rss_push {

class agc_pusher_t : public pusher_base {

private:
    hms_config_t config;

public:
    explicit agc_pusher_t(const config_t& config) {
        this->config = config.hms;
    };

    Task<void> refresh () override {
        std::map<std::string, std::string> body = {
                {"grant_type",    "client_credentials"},
                {"client_id",     config.client_id},
                {"client_secret", config.client_secret}
        };
        auto response = co_await fv::Post("https://oauth-login.cloud.huawei.com/oauth2/v3/token", fv::body_kvs(body),
                                          fv::content_type("application/x-www-form-urlencoded"));
        nlohmann::json json = response.Content;
        receipt_t receipt = json;
        access_token = receipt.access_token;
    }

    Task<void> notify (const rss_push::notification_t &notification, const std::string& token) override {
        nlohmann::json payload;
        payload["validate_only"] = VALIDATE;
        payload["message"]["notification"] = notification;
        std::string url = fmt::format("https://push-api.cloud.huawei.com/v1/{}/messages:send", config.client_id);

        payload["message"]["token"] = token;
        auto response = co_await fv::Post(url, fv::body_json(payload), fv::content_type("application/json"),
                                              fv::authorization(fmt::format("Bearer {}", access_token)));
    };
};

}

#endif //RSSPUSH_AGC_HPP
