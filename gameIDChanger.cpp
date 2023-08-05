#include "./fvector_monolithic.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

struct TargetData {
  std::string path;
  uint32_t targetOffset;
  std::optional<uint32_t> virsionOffset;
};

// Path, target Path, Change ID, Change Virsion
int main(int arg_count, char *arg_pChar[]) {
  const TargetData targetDatas[4] = {{"/disc/header.bin", 0x00, 0x04},
                                     {"/sys/boot.bin", 0x00, 0x04},
                                     {"/ticket.bin", 0x01e0},
                                     {"/tmd.bin", 0x0190, 0x08}};
  constexpr int32_t filenumb = 4;
  constexpr int32_t gameIDLenth = 4;
  constexpr int32_t gameVirsionLenth = 2;
  Fvector fvector, fvector_bk;

  if (arg_count < 3) {
    std::cout << "app [target path] [export ID] {export Virsion}\n";
    return 1;
  }

  std::cout << "Open file...\n";
  for (auto targetData : targetDatas) {
    fvector.setTargetPath(std::string(arg_pChar[1]) + targetData.path);
    fvector_bk.setTargetPath(std::string(arg_pChar[1]) + targetData.path + ".bk");

    std::cout << fvector.getTargetPath() << "...: ";
    if (fvector.getIsPathExist()) {
      auto buffer = fvector.ifstreamVector();
      if(!fvector_bk.getIsPathExist()){
        fvector_bk.ofstreamVector(buffer);
      }

      // game ID
      for (auto i = 0; i < gameIDLenth; i++) {
        buffer[targetData.targetOffset + i] = arg_pChar[2][i];
      }
      // game Virsion
      if (targetData.virsionOffset.has_value() && (arg_count == 4)) {
        for (auto i = 0; i < gameVirsionLenth; i++) {
          buffer[targetData.targetOffset + targetData.virsionOffset.value() +
                 i] = arg_pChar[3][i];
        }
      }

      if (fvector.ofstreamVector(buffer)) {
        std::cout << "OK.\n";
      } else {
        std::cout << "NG.\n";
      }

    } else {
      std::cout << "do not have!!\n";
    }
  }

  return 0;
}