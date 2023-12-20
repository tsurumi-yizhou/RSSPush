//
// Created by yizhou on 20/12/2023.
//

#ifndef RSSPUSH_SERVER_HPP
#define RSSPUSH_SERVER_HPP
#include <dbng.hpp>
#include <iguana/reflection.hpp>
#include <sqlite.hpp>
#include <fv/fv.h>

namespace rss_push {

struct record_t {
    std::string username, platform, token;
    long id;
};
REGISTER_AUTO_KEY(record_t, id)
REFLECTION_WITH_NAME(record_t, "device", username, platform, token)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(record_t, username, platform, token)

Task<void> serve(const database_t& config) {
    ormpp::dbng<ormpp::sqlite> conn;
    if (!conn.connect("tokens.db")) {
        throw std::runtime_error("connect to database failed");
    }

    fv::HttpServer server{};
    server.SetHttpHandler("/register", [&] (fv::Request& request) -> Task<fv::Response> {
        try {
            if (request.Method != fv::MethodType::Post) {
                co_return fv::Response::FromNotFound();
            }
            nlohmann::json payload = request.Content;
            record_t record = payload;
            conn.insert(record);
            co_return fv::Response::FromText("Success");
        } catch (...) {
            co_return fv::Response::FromNotFound();
        }
    });
    fmt::print("Create server listen in {}:{}{}\n", "localhost", 6800, "/register");
    co_await server.Run(6800);
}

}

#endif //RSSPUSH_SERVER_HPP
