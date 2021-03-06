#include "GzFile.h"

namespace util {

GzFile::GzFile(std::filesystem::path const& path) {
  file_ = gzopen(path.c_str(), "rb");
  IPS_VERIFY_S(nullptr != file_, "Failed to open file " << path);
}

GzFile::~GzFile() noexcept {
  if (file_ != nullptr) {
    gzclose(file_);
  }
}

gzFile GzFile::native_handle() noexcept { return file_; }

}  // namespace util