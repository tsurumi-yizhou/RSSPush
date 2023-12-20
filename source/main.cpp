#include <fstream>
#include <config.hpp>
#include <feed.hpp>
#include <client/server.hpp>
#include <pusher/fcm.hpp>
#include <pusher/agc.hpp>
#include <pusher/apn.hpp>

Task<void> push(rss_push::config_t config) {
    ormpp::dbng<ormpp::sqlite> conn;
    if(!conn.connect("tokens.db")) {
        throw std::runtime_error("connect to database error");
    }
    auto devices = conn.query<rss_push::record_t>();
    conn.disconnect();

    std::vector<rss_push::notification_t> notifications;
    for(const auto& feed: config.feeds) {
        std::time_t timestamp = feed.updated;
        auto list = co_await rss_push::get_items(feed.link, std::chrono::system_clock::from_time_t(timestamp));
        notifications.insert(notifications.end(), list.begin(), list.end());
    }

    std::map<std::string, std::shared_ptr<rss_push::pusher_base>> pushers;
    pushers.insert({"fcm", std::dynamic_pointer_cast<rss_push::pusher_base>(std::make_shared<rss_push::fcm_pusher_t>(config))});
    pushers.insert({"agc", std::dynamic_pointer_cast<rss_push::pusher_base>(std::make_shared<rss_push::agc_pusher_t>(config))});
    //pushers.insert({"apn", std::dynamic_pointer_cast<rss_push::pusher_base>(std::make_shared<rss_push::apn_pusher_t>(config))});

    for(auto[label, pusher]: pushers) {
        co_await pusher->refresh();
    }

    for(const auto& notification: notifications) {
        for(const auto& device: devices) {
            co_await pushers[device.platform]->notify(notification, device.token);
            fmt::print("push {} to {}\n", notification.title, device.username);
        }
    }
}

auto main(int argc, char* argv[]) -> int {

    std::ifstream file("config.json");
    nlohmann::json json;
    file >> json;
    file.close();
    rss_push::config_t config = json;

    fv::Tasks::Init();
    fv::Tasks::RunMainAsync(rss_push::serve, config.database);
    fv::Tasks::RunAsync(push, config);
    fv::Tasks::Run();

    return 0;
}