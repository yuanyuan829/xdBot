#include "../includes.hpp"
#include "gdr2.hpp"

int Macro::saveGDR2(std::string author, std::string desc, std::string path) {
    auto& g = Global::get();

    if (g.macro.inputs.empty()) return 31;

    std::string extension = ".gdr2";
    int iterations = 0;

    while (std::filesystem::exists(path + extension)) {
        iterations++;
        if (iterations > 1) {
            int length = 3 + std::to_string(iterations - 1).length();
            path.erase(path.length() - length, length);
        }
        path += fmt::format(" ({})", std::to_string(iterations));
    }

    path += extension;

    xdbot::XDBotReplay replay;
    replay.author = author;
    replay.description = desc;
    replay.duration = g.macro.inputs.back().frame / g.macro.framerate;
    replay.framerate = g.macro.framerate;
    replay.gameVersion = g.macro.gameVersion;

    replay.levelInfo.name = Macro::getLevelName();
    replay.levelInfo.id = Macro::getLevelId();

    for (const auto& inp : g.macro.inputs) {
        replay.inputs.push_back({static_cast<uint64_t>(inp.frame), static_cast<uint8_t>(inp.button), inp.player2, inp.down});
    }

    auto res = xdbot::saveGDR2(replay, path);
    if (res.isErr()) return 20;

    g.macro.author = author;
    g.macro.description = desc;

    return 0;
}

bool Macro::loadGDR2File(std::filesystem::path path) {
    auto res = xdbot::loadGDR2(path);
    if (res.isErr()) return false;

    auto replay = std::move(res).unwrap();

    std::string levelName = replay.levelInfo.name;
    uint32_t levelId = replay.levelInfo.id;

    Macro newMacro;
    newMacro.author = replay.author;
    newMacro.description = replay.description;
    newMacro.duration = replay.duration;
    newMacro.framerate = replay.framerate;
    newMacro.gameVersion = replay.gameVersion;

    for (const auto& inp : replay.inputs) {
        input xdInput(inp.frame, inp.button, inp.player2, inp.down);
        newMacro.inputs.push_back(xdInput);
    }

    Global::get().macro = newMacro;
    Macro::setLevelInfo(levelName, levelId);
    return true;
}
