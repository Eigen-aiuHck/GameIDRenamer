#pragma once
#include <cstdint>
#include <filesystem>
#include <optional>
#include <vector>

class Fvector {
private:
protected:
  std::filesystem::path targetPath;
  bool isPathExist = false;

public:
  Fvector();
  Fvector(std::filesystem::path const &);

  bool setTargetPath(std::filesystem::path const &);
  std::string getTargetPath();
  bool getIsPathExist();

  std::vector<uint8_t> ifstreamVector();
  std::vector<uint8_t> ifstreamVector(std::vector<uint8_t> &);
  bool ofstreamVector(std::vector<uint8_t> const &);
};

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

Fvector::Fvector(){};
Fvector::Fvector(std::filesystem::path const &path) { setTargetPath(path); };

bool Fvector::setTargetPath(std::filesystem::path const &path) {
  if (!path.empty()) {
    targetPath = path;
    isPathExist = std::filesystem::is_regular_file(path);
    return true;
  }
  return false;
}

std::string Fvector::getTargetPath() {
  return targetPath.generic_string<char>();
}

bool Fvector::getIsPathExist() { return isPathExist; }

std::vector<uint8_t> Fvector::ifstreamVector() {
  std::vector<uint8_t> buf_retVal;
  return ifstreamVector(buf_retVal);
}

std::vector<uint8_t> Fvector::ifstreamVector(std::vector<uint8_t> &retVal) {
  if (isPathExist || !targetPath.empty()) {
    std::ifstream file(targetPath, std::ios_base::ate | std::ios_base::binary);
    if (!file.is_open()) {
      std::cout << "Failed to open file!! :: " << targetPath.generic_string()
                << std::endl;
      return {};
    }

    size_t fileSize = file.tellg();
    retVal.resize(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char *>(retVal.data()), fileSize);

    file.close();
    return retVal;
  }
  return {};
}

bool Fvector::ofstreamVector(std::vector<uint8_t> const &exportVal) {
  if (!targetPath.empty()) {
    std::ofstream file(targetPath, std::ios_base::binary);
    if (!file.is_open()) {
      std::cout << "Failed to open file!! :: " << targetPath.generic_string()
                << std::endl;
      return {};
    }

    size_t exportSize = exportVal.size();

    file.clear();
    file.write(reinterpret_cast<char const *>(exportVal.data()), exportSize);

    file.close();
    return true;
  }
  return false;
}