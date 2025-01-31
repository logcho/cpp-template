#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string slice(const std::string& hexInput, size_t start, size_t end) {
    if (hexInput.substr(0, 2) != "0x") {
        throw std::invalid_argument("Invalid hex input: Missing 0x prefix");
    }
    return "0x" + hexInput.substr(start * 2 + 2, (end - start) * 2);
}

bool hexToBool(const std::string& hexInput) {
    return hexInput == "0x01";
}

std::string boolToHex(bool value) {
    return value ? "0x01" : "0x00";
}

std::string stringToHex(const std::string& input){
    std::ostringstream hexStream;
    hexStream << "0x";
    for(unsigned char c : input){
        hexStream << std::hex <<  std::setw(2) << std::setfill('0') << (int)c;
    }
    return hexStream.str();
}

std::string hexToString(const std::string& hexInput){
    if(hexInput.substr(0, 2) != "0x"){
        throw std::invalid_argument("Invalid hex input: Missing 0x prefix");
    }
    std::string result;
    for(size_t i = 2; i < hexInput.size(); i += 2){
        std::string byteString = hexInput.substr(i, 2);
        char byte = static_cast<char>(std::stoul(byteString, nullptr, 16));
        result.push_back(byte);
    }
    return result;
}

std::string intToHex(int number){
    std::ostringstream hexStream;
    hexStream << "0x";
    hexStream << std::hex << number;
    return hexStream.str();

}

int hexToInt(const std::string& hexInput){
    if(hexInput.substr(0, 2) != "0x"){
        throw std::invalid_argument("Invalid hex input: Missing 0x prefix");
    }
    return std::stoi(hexInput.substr(2), nullptr, 16);
}

std::string vectorToHexUint8(const std::vector<uint8_t>& bytes){
    std::ostringstream hexStream;
    hexStream << "0x";
    for(auto byte : bytes){
        hexStream << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    return hexStream.str();
}

std::vector<uint8_t> hexToVectorUint8(const std::string& hexInput){
    if(hexInput.substr(0, 2) != "0x"){
        throw std::invalid_argument("Invalid hex input: Missing 0x prefix");
    }
    std::vector<uint8_t> result;
    for(size_t i = 2; i < hexInput.size(); i += 2){
        std::string byteString = hexInput.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
        result.push_back(byte);
    }
    return result;
}

std::string vectorToHexUint16(const std::vector<uint16_t>& values){
    std::ostringstream hexStream;
    hexStream << "0x";
    for(auto value : values){
        hexStream << std::hex << std::setw(4) << std::setfill('0') << (int)value;
    }
    return hexStream.str();
}

std::vector<uint16_t> hexToVectorUint16(const std::string& hexInput){
    if(hexInput.substr(0, 2) != "0x"){
        throw std::invalid_argument("Invalid hex input: Missing 0x prefix");
    }
    std::vector<uint16_t> result;
    for(size_t i = 2; i < hexInput.size(); i += 4){
        std::string byteString = hexInput.substr(i, 4);
        uint16_t value = static_cast<uint16_t>(std::stoul(byteString, nullptr, 16));
        result.push_back(value);
    }
    return result;
}

std::string vectorToHexUint32(const std::vector<uint32_t>& values){
    std::ostringstream hexStream;
    hexStream << "0x";
    for(auto value : values){
        hexStream << std::hex << std::setw(8) << std::setfill('0') << int(value);
    }
    return hexStream.str();
}

std::vector<uint32_t> hexToVectorUint32(const std::string& hexInput){
    if(hexInput.substr(0, 2) != "0x"){
        throw std::invalid_argument("Invalid hex input: Missing 0x prefix");
    }
    std::vector<uint32_t> result;
    for(size_t i = 2; i < hexInput.size(); i += 8){
        std::string byteString = hexInput.substr(i, 8);
        uint32_t value  = static_cast<uint32_t>(std::stoul(byteString, nullptr, 16));
        result.push_back(value);
    }
    return result;
}



#endif // UTIL_H