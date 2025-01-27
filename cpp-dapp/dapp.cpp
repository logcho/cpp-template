#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "3rdparty/cpp-httplib/httplib.h"
#include "3rdparty/picojson/picojson.h"
#include "util.hpp"

std::string const ERC20 = "0x92C6bcA388E99d6B304f1Af3c3Cd749Ff0b591e2";
std::string const APPLICATION_ADDRESS = "0xab7528bb862fb57e8a2bcd567a2e929a0be56a5e";

void createReport(httplib::Client &cli, const std::string &payload) {
    auto report = std::string("{\"payload\":\"") + payload + std::string("\"}");
    auto r = cli.Post("/report", report, "application/json");   
    // Expect status 202
    std::cout << "Received report status " << r.value().status << std::endl;
}

void createNotice(httplib::Client &cli, const std::string &payload) {
    auto notice = std::string("{\"payload\":\"") + payload + std::string("\"}");
    auto r = cli.Post("/notice", notice, "application/json");    
    // Expect status 201
    std::cout << "Received notice status " << r.value().status << std::endl;
}

picojson::object parseERC20DepositToJson(const std::string& payload) {    
    picojson::object obj;
    obj["success"] = picojson::value(hexToBool(slice(payload, 0, 1)));
    obj["token"] = picojson::value(slice(payload, 1, 21));
    obj["sender"] = picojson::value(slice(payload, 21, 41));
    obj["amount"] = picojson::value(slice(payload, 41, 73));
    return obj;
}

std::string handle_advance(httplib::Client &cli, picojson::value data)
{
    std::string user = data.get("metadata").get("msg_sender").to_str();
    std::string payload = data.get("payload").to_str();
    std::cout << std::setw(20) << std::setfill('-') << "" << std::endl;
    std::cout << "User: " << user << std::endl;
    std::cout << "Payload: " << payload << std::endl;
    std::cout << "Converted Payload: " << hexToString(payload) << std::endl;
    std::cout << std::setw(20) << std::setfill('-') << "" << std::endl;
    
    return "accept";
}

std::string handle_inspect(httplib::Client &cli, picojson::value data)
{   
    std::string payload = data.get("payload").to_str();
    std::cout << std::setw(20) << std::setfill('-') << "" << std::endl;
    std::cout << "Payload: " << payload << std::endl;
    std::cout << "Converted Payload: " << hexToString(payload) << std::endl;
    std::cout << std::setw(20) << std::setfill('-') << "" << std::endl;

    createReport(cli, data.get("payload").to_str());

    return "accept";
}

int main(int argc, char **argv)
{
    std::map<std::string, decltype(&handle_advance)> handlers = {
        {std::string("advance_state"), &handle_advance},
        {std::string("inspect_state"), &handle_inspect},
    };
    httplib::Client cli(getenv("ROLLUP_HTTP_SERVER_URL"));
    cli.set_read_timeout(20, 0);
    std::string status("accept");
    std::string rollup_address;
    while (true)
    {
        std::cout << "Sending finish" << std::endl;
        auto finish = std::string("{\"status\":\"") + status + std::string("\"}");
        auto r = cli.Post("/finish", finish, "application/json");
        std::cout << "Received finish status " << r.value().status << std::endl;
        if (r.value().status == 202)
        {
            std::cout << "No pending rollup request, trying again" << std::endl;
        }
        else
        {
            picojson::value rollup_request;
            picojson::parse(rollup_request, r.value().body);
            picojson::value metadata = rollup_request.get("data").get("metadata");
            auto request_type = rollup_request.get("request_type").get<std::string>();
            auto handler = handlers.find(request_type)->second;
            auto data = rollup_request.get("data");
            status = (*handler)(cli, data);
        }
    }
    return 0;
}
