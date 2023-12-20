//
// Created by yizhou on 20/12/2023.
//

#ifndef RSSPUSH_FEED_HPP
#define RSSPUSH_FEED_HPP
#include <chrono>
#include <tinyxml2.h>
#include <pusher/base.hpp>

namespace rss_push {

Task<std::vector<notification_t>> get_items(const std::string& link, std::chrono::system_clock::time_point tp) {
    tinyxml2::XMLDocument xml;
    std::vector<notification_t> notifications;
    auto source = co_await fv::Get(link);
    xml.Parse(source.Content.c_str(), source.Content.length());

    auto rss = xml.RootElement();
    auto channel = rss->FirstChildElement("channel");
    auto item = channel->FirstChildElement("item");

    while (item) {

        std::string pubDate = item->FirstChildElement("pubDate")->GetText();
        std::istringstream dateStr(pubDate);
        std::tm tm;
        dateStr >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S %Z");

        if (std::chrono::system_clock::from_time_t(std::mktime(&tm)) >= tp) {
            notification_t notification;
            notification.title = item->FirstChildElement("title")->GetText();
            notification.body = item->FirstChildElement("description")->GetText();
            if (item->FirstChildElement("category") != nullptr) {
                notification.topic = item->FirstChildElement("category")->GetText();
            }
            if (item->FirstChildElement("image") != nullptr) {
                notification.image = item->FirstChildElement("image")->FirstChildElement("url")->GetText();
            }
            notifications.push_back(notification);
        }

        item = item->NextSiblingElement();
    }

    co_return notifications;
}

}

#endif //RSSPUSH_FEED_HPP
