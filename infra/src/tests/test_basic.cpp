#include "infra/include/testing/LaunchFixture.h"

#include <filesystem>

TEST_F(LaunchFixture, base_test_no_backdoor) {
  infra::testing::LaunchConfig config;
  config.expected_result(infra::domain::UNSAT).config_path("base.json");
  for (const auto& path : LaunchFixture::cnfs_) {
    launch(config.input_path(path));
  }
}

TEST_F(LaunchFixture, base_test_backdoor) {
  infra::testing::LaunchConfig config;
  config.backdoor(true).config_path("base.json").expected_result(infra::domain::UNSAT);
  for (const auto& path : LaunchFixture::cnfs_) {
    launch(config.input_path(path));
  }
}
