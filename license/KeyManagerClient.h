#pragma once

#include <filesystem>
#include <string>

namespace thimpos::license
{
struct VerifiedLicense
{
    std::string registrantName;
    std::string expiresAt;
};

class KeyManagerClient
{
  public:
    KeyManagerClient();
    VerifiedLicense verifyAtStartup() const;

  private:
    std::string apiUrl_;
    std::string product_;
    std::filesystem::path cachePath_;
};
}  // namespace thimpos::license
