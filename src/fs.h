//
// Created by cpasjuste on 05/04/23.
//

#ifndef U_GAMEPAD_FS_H
#define U_GAMEPAD_FS_H

#include <string>
#include <cstdint>
#include <vector>
#include <functional>
#include "utility.h"

#ifdef LINUX
#define FLASH_SECTOR_SIZE 4096
#endif
#define FLASH_TARGET_OFFSET_CACHE ((1024 * 1024) * 0)   // 1MB flash cache (raw)
#define FLASH_TARGET_OFFSET_FATFS ((1024 * 1024) * 1)   // 1MB fatfs flash ("flash:")

namespace uGamePad {
    class Fs {
    public:
        struct DeviceInfo {
            uint64_t totalBytes;
            uint64_t usedBytes;
        };

        enum UsbMode {
            Host,
            Msc
        };

        class File final {
        public:
            enum OpenMode {
                Read = 1 << 0,
                Write = 1 << 1
            };

            enum Flags {
                Directory = 1
            };

            struct Info {
                std::string name;
                int flags;
                uint32_t size;
            };

            File() = default;

            explicit File(const std::string &path, int mode = OpenMode::Read) {
                m_path = path;
                m_name = Utility::baseName(m_path);
                open(path, mode);
            }

            File(const File &) = delete;

            File(File &&other) noexcept { *this = std::move(other); }

            ~File() { close(); }

            File &operator=(const File &) = delete;

            File &operator=(File &&other) noexcept {
                if (this != &other) {
                    close();
                    std::swap(m_path, other.m_path);
                    std::swap(p_fh, other.p_fh);
                    std::swap(buf, other.buf);
                    std::swap(buf_len, other.buf_len);
                }
                return *this;
            }

            bool open(const std::string &file, int mode = OpenMode::Read);

            int32_t read(uint32_t offset, uint32_t length, char *buffer) const;

            int32_t write(uint32_t offset, uint32_t length, const char *buffer) const;

            void close();

            [[nodiscard]] uint32_t getLength() const;

            [[nodiscard]] bool isOpen() const { return buf != nullptr || p_fh != nullptr; }

            [[nodiscard]] std::string getPath() const { return m_path; }

            [[nodiscard]] std::string getName() const { return m_name; }

        private:
            std::string m_path;
            std::string m_name;
            void *p_fh = nullptr;
            const uint8_t *buf = nullptr;
            uint32_t buf_len{};
        };

        Fs();

        virtual ~Fs() = default;

        virtual bool format();

        virtual bool isAvailable() { return m_available; };

        virtual std::string getRootDirectory() { return "flash:/"; };

        virtual std::string getDeviceDirectory() { return getRootDirectory() + "devices"; };

        virtual void createDirectory(const std::string &path);

        static bool fileExists(const std::string &path);

        static bool directoryExists(const std::string &path);

        static std::vector<File::Info> getDir(
            const std::string &path, std::function<bool(const File::Info &)> filter = nullptr);

        virtual void sync() {
        };

        virtual void setUsbMode(UsbMode mode) { m_usb_mode = mode; };

        virtual UsbMode getUsbMode() { return m_usb_mode; };

        virtual DeviceInfo getDeviceInfo() { return {0, 0}; };

    protected:
        bool m_available = false;
        UsbMode m_usb_mode = UsbMode::Host;

    private:
        class FatFs {
        public:
            static void *open_file(const std::string &path, int mode);

            static int32_t read_file(void *fh, uint32_t offset, uint32_t length, char *buffer);

            static int32_t write_file(void *fh, uint32_t offset, uint32_t length, const char *buffer);

            static int32_t close_file(void *fh);

            static uint32_t get_file_length(void *fh);

            static void list_files(const std::string &path, const std::function<void(File::Info &)> &callback);

            static bool file_exists(const std::string &path);

            static bool remove_file(const std::string &path);
        };
    };
}

#endif //U_GAMEPAD_FS_H
