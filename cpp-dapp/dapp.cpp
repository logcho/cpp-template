#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "3rdparty/cpp-httplib/httplib.h"
#include "3rdparty/picojson/picojson.h"
#include "util.h"
#include "uint256_t/uint256_t.h"

std::string const TEST_TOKEN = "0x92c6bca388e99d6b304f1af3c3cd749ff0b591e2";
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

picojson::object parseEtherDeposit(const std::string& payload) {    
    picojson::object obj;
    obj["sender"] = picojson::value(slice(payload, 0, 20));
    obj["value"] = picojson::value(slice(payload, 20, 52));
    return obj;
}

picojson::object parseERC20Deposit(const std::string& payload) {    
    picojson::object obj;
    obj["success"] = picojson::value(hexToBool(slice(payload, 0, 1)));
    obj["token"] = picojson::value(slice(payload, 1, 21));
    obj["sender"] = picojson::value(slice(payload, 21, 41));
    obj["amount"] = picojson::value(slice(payload, 41, 73));
    return obj;
}

picojson::object parseERC721Deposit(const std::string& payload) {    
    picojson::object obj;
    obj["token"] = picojson::value(slice(payload, 0, 20));
    obj["sender"] = picojson::value(slice(payload, 20, 40));
    obj["tokenId"] = picojson::value(slice(payload, 40, 72));
    return obj;
}

std::string handle_advance(httplib::Client &cli, picojson::value data)
{
    std::string address = data.get("metadata").get("msg_sender").to_str();
    std::string payload = data.get("payload").to_str();
    
    std::cout << std::setw(20) << std::setfill('-') << "" << std::endl;
    std::cout << "Address: " << address << std::endl;
    std::cout << "Payload: " << payload << std::endl;
    // std::cout << "Converted Payload: " << hexToString(payload) << std::endl;
    if(isEtherDeposit(address)){
        picojson::object deposit = parseEtherDeposit(payload);
        uint256_t value(deposit["value"].to_str().substr(2), 16);
        std::cout << "Deposit: " << "Ether" << std::endl;
        std::cout << "Sender: " << deposit["sender"].to_str() << std::endl;
        std::cout << "Value: " << value << std::endl;   
    }
    if(isERC20Deposit(address)){
        picojson::object deposit = parseERC20Deposit(payload);
        uint256_t amount(deposit["amount"].to_str().substr(2), 16);
        std::cout << "Deposit: " << "ERC20" << std::endl;
        std::cout << "Success: " << deposit["success"].to_str() << std::endl;
        std::cout << "Token: " << deposit["token"].to_str() << std::endl;
        std::cout << "Sender: " << deposit["sender"].to_str() << std::endl;
        std::cout << "Amount: " << amount << std::endl;
    }
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
