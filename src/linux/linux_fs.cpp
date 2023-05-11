//
// Created by cpasjuste on 05/04/23.
//
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "main.h"
#include "linux_fs.h"

using namespace uGamePad;

LinuxFs::LinuxFs() : Fs() {
    // create devices directory if needed
    mkdir(LinuxFs::getDeviceDirectory().c_str(), 0755);
    m_available = true;
}

bool LinuxFs::writeFile(const std::string &path, const std::vector<uint8_t> &data) {
    std::ofstream file(path, std::ios::binary | std::ios::out);
    if (!file.good()) {
        printf("LinuxFs::writeFile: could not open file (%s)\r\n", path.c_str());
        file.close();
        return false;
    }

    if (!file.write((char *) data.data(), (std::streamsize) data.size())) {
        printf("LinuxFs::writeFile: could not write file (%s)\r\n", path.c_str());
        file.close();
        return false;
    }

    file.close();
    return true;
}

std::vector<uint8_t> LinuxFs::readFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.good()) {
        printf("LinuxFs::readFile: could not open file (%s)\r\n", path.c_str());
        file.close();
        return {};
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (!file.read((char *) buffer.data(), size)) {
        printf("LinuxFs::readFile: could not read file (%s)\r\n", path.c_str());
    }

    file.close();
    return buffer;
}
