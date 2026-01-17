#pragma once

#include <gdr_convert.hpp>
#include <filesystem>
#include <vector>

namespace xdbot {

struct XDBotReplay : gdr::Replay<XDBotReplay, gdr::Input<>> {
    XDBotReplay() = default;
};

gdr::Result<> saveGDR2(XDBotReplay& replay, const std::filesystem::path& path);
gdr::Result<XDBotReplay> loadGDR2(const std::filesystem::path& path);
gdr::Result<XDBotReplay> loadGDR2(std::span<uint8_t> data);

}
