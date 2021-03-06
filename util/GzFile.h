#ifndef EVOL_GZFILE_H
#define EVOL_GZFILE_H

#include <zlib.h>
#include <filesystem>

#include "util/Assert.h"

namespace util {

/**
 * @brief GZ read-only file wrapper utility.
 */
class GzFile {
 public:
  GzFile() = default;

  explicit GzFile(std::filesystem::path const& path);

  ~GzFile() noexcept;

  [[nodiscard]] gzFile native_handle() noexcept;

 private:
  gzFile file_ = nullptr;
};

}  // namespace util

#endif  // EVOL_GZFILE_H
