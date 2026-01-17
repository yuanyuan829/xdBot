#include "gdr2.hpp"
#include <fstream>

namespace xdbot {

gdr::Result<> saveGDR2(XDBotReplay& replay, const std::filesystem::path& path) {
    auto res = replay.exportData();
    if (res.isErr())
        return gdr::Err(res.unwrapErr());

    auto data = std::move(res).unwrap();

    std::ofstream f(path, std::ios::binary);
    if (!f)
        return gdr::Err("Failed to open file for writing");

    f.write(reinterpret_cast<const char*>(data.data()), data.size());
    if (!f)
        return gdr::Err("Failed to write data to file");

    f.close();
    return gdr::Ok();
}

gdr::Result<XDBotReplay> loadGDR2(const std::filesystem::path& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f)
        return gdr::Err<XDBotReplay>("Failed to open file for reading");

    f.seekg(0, std::ios::end);
    size_t fileSize = f.tellg();
    f.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(fileSize);
    f.read(reinterpret_cast<char*>(data.data()), fileSize);
    f.close();

    return loadGDR2(data);
}

gdr::Result<XDBotReplay> loadGDR2(std::span<uint8_t> data) {
    if (data.size() < 3)
        return gdr::Err<XDBotReplay>("Data is too small to be a valid replay");

    gdr::Result<XDBotReplay> res = gdr::Err<XDBotReplay>("");
    if (std::equal(data.begin(), data.begin() + 3, "GDR"))
        res = XDBotReplay::importData(data);
    else
        res = gdr::convert<XDBotReplay, gdr::Input<>>(data);

    return res;
}

}

