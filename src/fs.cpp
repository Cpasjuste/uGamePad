//
// Created by cpasjuste on 09/03/24.
//

#include <cstring>
#include "ff.h"
#include "diskio.h"
#include "fs.h"

#ifdef PICO_BUILD
#include <hardware/flash.h>
#include "pico_flash.h"
#else
#include "linux_flash.h"
#endif

using namespace uGamePad;

static FATFS flash_fs;

Fs::Fs() {
    // mount flash fs
    printf("PicoFs: mounting flash fs...\r\n");
    const auto res = f_mount(&flash_fs, "flash:", 1);
    if (res != FR_OK) {
        printf("PicoFs: failed to mount flash filesystem! (%i)\r\n", res);
        if (res == FR_NO_FILESYSTEM) {
            printf("Io: no filesystem found on flash, formatting...\r\n");
            Fs::format();
        }
    } else {
        printf("PicoFs: mounted flash fs on \"flash:\" (%s)\r\n",
               io_flash_get_size_string().c_str());
    }

    m_available = true;
}

bool Fs::format() {
    constexpr MKFS_PARM opts{.fmt = FM_ANY | FM_SFD};
    constexpr uint32_t sector_size = FLASH_SECTOR_SIZE;
    const std::string path = "flash:";

    printf("Fs::format: formatting drive \"%s\"...\r\n", path.c_str());

    FRESULT res = f_mkfs(path.c_str(), &opts, flash_fs.win, sector_size);
    if (res != FR_OK) {
        printf("Fs::format: format failed! (%i)\r\n", res);
        return false;
    }

    res = f_mount(&flash_fs, path.c_str(), 1);
    if (res != FR_OK) {
        printf("Fs::format: failed to mount filesystem! (%i)\r\n", res);
        return false;
    }

    printf("Fs: mounted flash fs on \"flash:\" (%s)\r\n",
           io_flash_get_size_string().c_str());

    return true;
}


void Fs::createDirectory(const std::string &path) {
    // add "/"
    std::string newPath = path;
    if (newPath[newPath.size() - 1] != '/') {
        newPath = newPath + "/";
    }

    const auto temp = static_cast<char *>(calloc(1, strlen(newPath.c_str()) + 1));
    const char *p = newPath.c_str();

    while ((p = strchr(p, '/')) != nullptr) {
        if (p != newPath.c_str() && *(p - 1) == '/') {
            p++;
            continue;
        }
        memcpy(temp, newPath.c_str(), p - newPath.c_str());
        temp[p - newPath.c_str()] = '\0';
        p++;
        const auto fr = f_mkdir(temp);
        if (fr != FR_OK && fr != FR_EXIST) {
            break;
        }
    }

    free(temp);
    sync();
}

bool Fs::fileExists(const std::string &path) {
    return FatFs::file_exists(path);
}

bool Fs::directoryExists(const std::string &path) {
    FILINFO info;
    return f_stat(path.c_str(), &info) == FR_OK && (info.fattrib & AM_DIR);
}

std::vector<Fs::File::Info> Fs::getDir(const std::string &path, std::function<bool(const File::Info &)> filter) {
    std::vector<File::Info> ret;
    FatFs::list_files(path, [&ret, &filter](const File::Info &file) {
        if (!filter || filter(file))
            ret.push_back(file);
    });

    return ret;
}

bool Fs::File::open(const std::string &file, const int mode) {
    close();
    p_fh = FatFs::open_file(file, mode);
    return p_fh != nullptr;
}

int32_t Fs::File::read(const uint32_t offset, const uint32_t length, char *buffer) const {
    return FatFs::read_file(p_fh, offset, length, buffer);
}

int32_t Fs::File::write(const uint32_t offset, const uint32_t length, const char *buffer) const {
    return FatFs::write_file(p_fh, offset, length, buffer);
}

void Fs::File::close() {
    if (!p_fh) return;
    FatFs::close_file(p_fh);
    p_fh = nullptr;
}

uint32_t Fs::File::getLength() const {
    return FatFs::get_file_length(p_fh);
}

void *Fs::FatFs::open_file(const std::string &path, const int mode) {
    const auto f = new FIL();
    BYTE ff_mode = 0;

    if (mode & File::OpenMode::Read)
        ff_mode |= FA_READ;
    if (mode & File::OpenMode::Write)
        ff_mode |= FA_WRITE;
    if (mode == File::OpenMode::Write)
        ff_mode |= FA_CREATE_ALWAYS;

    const auto res = f_open(f, path.c_str(), ff_mode);
    if (res == FR_OK) {
        return f;
    }

    delete f;
    return nullptr;
}

int32_t Fs::FatFs::read_file(void *fh, const uint32_t offset, const uint32_t length, char *buffer) {
    const auto f = static_cast<FIL *>(fh);
    FRESULT r = FR_OK;

    if (offset != f_tell(f)) r = f_lseek(f, offset);

    if (r == FR_OK) {
        unsigned int bytes_read;
        r = f_read(f, buffer, length, &bytes_read);
        if (r == FR_OK) {
            return static_cast<int32_t>(bytes_read);
        }
    }

    return -1;
}

int32_t Fs::FatFs::write_file(void *fh, const uint32_t offset, const uint32_t length, const char *buffer) {
    FRESULT r = FR_OK;
    const auto f = static_cast<FIL *>(fh);

    if (offset != f_tell(f))
        r = f_lseek(f, offset);

    if (r == FR_OK) {
        unsigned int bytes_written;
        r = f_write(f, buffer, length, &bytes_written);
        if (r == FR_OK) {
            return static_cast<int32_t>(bytes_written);
        }
    }

    return -1;
}

int32_t Fs::FatFs::close_file(void *fh) {
    const auto r = f_close(static_cast<FIL *>(fh));
    delete static_cast<FIL *>(fh);
    return r == FR_OK ? 0 : -1;
}

uint32_t Fs::FatFs::get_file_length(void *fh) {
    return f_size(static_cast<FIL *>(fh));
}

void Fs::FatFs::list_files(const std::string &path, const std::function<void(File::Info &)> &callback) {
    FILINFO ent;
    DIR dir;

    if (f_opendir(&dir, path.c_str()) != FR_OK)
        return;

    while (f_readdir(&dir, &ent) == FR_OK && ent.fname[0]) {
        File::Info info{
            .name = ent.fname,
            .flags = 0,
            .size = ent.fsize
        };

        if (ent.fattrib & AM_DIR)
            info.flags |= File::Flags::Directory;

        callback(info);
    }

    f_closedir(&dir);
}

bool Fs::FatFs::file_exists(const std::string &path) {
    FILINFO info;
    return f_stat(path.c_str(), &info) == FR_OK && !(info.fattrib & AM_DIR);
}

bool Fs::FatFs::remove_file(const std::string &path) {
    return f_unlink(path.c_str()) == FR_OK;
}


///
/// FatFs low level io functions
///

DSTATUS disk_initialize(BYTE drv) {
    //printf("disk_initialize: %i\r\n", drv);

    // flash always available...
    io_flash_init();
    return RES_OK;
}

DSTATUS disk_status(BYTE drv) {
    //printf("disk_status: %i\r\n", drv);

    // flash always available...
    return RES_OK;
}

DRESULT disk_read(BYTE drv, BYTE *buff, LBA_t sector, UINT count) {
    //printf("disk_read: %i\r\n", drv);
    return io_flash_read(sector, 0, buff, FLASH_SECTOR_SIZE * count)
           == int32_t(FLASH_SECTOR_SIZE * count)
               ? RES_OK
               : RES_ERROR;
}

DRESULT disk_write(BYTE drv, const BYTE *buff, LBA_t sector, UINT count) {
#if 0
    printf("disk_write: %i, address: 0x%08lx\r\n", drv,
           XIP_BASE + FLASH_TARGET_OFFSET_FATFS + ( sector * FLASH_SECTOR_SIZE));
#endif
    return io_flash_write(sector, 0, buff, FLASH_SECTOR_SIZE * count)
           == int32_t(FLASH_SECTOR_SIZE * count)
               ? RES_OK
               : RES_ERROR;
}

DRESULT disk_ioctl(BYTE drv, BYTE cmd, void *buff) {
    //printf("disk_ioctl: drv: %i, cmd: %i\r\n", drv, cmd);
    uint16_t block_size;
    uint32_t num_blocks;

    switch (cmd) {
        case CTRL_SYNC:
            return RES_OK;
        case GET_SECTOR_COUNT:
            io_flash_get_size(block_size, num_blocks);
            *static_cast<LBA_t *>(buff) = num_blocks;
            return RES_OK;
        case GET_SECTOR_SIZE:
            *static_cast<WORD *>(buff) = FLASH_SECTOR_SIZE;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *static_cast<DWORD *>(buff) = 1;
            return RES_OK;
        default:
            break;
    }

    return RES_PARERR;
}
