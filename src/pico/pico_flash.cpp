//
// Created by cpasjuste on 19/01/24.
//

#include <cstring>
#include <pico/binary_info.h>
#include <hardware/sync.h>
#include "fs.h"
#include "pico_flash.h"
#include "utility.h"

using namespace uGamePad;

static const uint32_t flash_offset = FLASH_TARGET_OFFSET_FATFS;
static const uint32_t flash_size = PICO_FLASH_SIZE_BYTES - FLASH_TARGET_OFFSET_FATFS;

bool uGamePad::io_flash_init() {
    bi_decl(bi_block_device(
            BINARY_INFO_MAKE_TAG('P', '2'),
            "flash",
            XIP_BASE + flash_offset,
            flash_size,
            nullptr,
            BINARY_INFO_BLOCK_DEV_FLAG_READ | BINARY_INFO_BLOCK_DEV_FLAG_WRITE | BINARY_INFO_BLOCK_DEV_FLAG_PT_NONE))

    return true;
}

uint64_t uGamePad::io_flash_get_size() {
    return flash_size;
}

void uGamePad::io_flash_get_size(uint16_t &block_size, uint32_t &num_blocks) {
    block_size = FLASH_SECTOR_SIZE;
    num_blocks = flash_size / FLASH_SECTOR_SIZE;
}

std::string uGamePad::io_flash_get_size_string() {
    return Utility::parseSize(flash_size);
}

int32_t uGamePad::io_flash_read(uint32_t sector, uint32_t offset, void *buffer, uint32_t size_bytes) {
    // TODO: XIP_NOCACHE_NOALLOC_BASE ?
    memcpy(buffer, (uint8_t *) XIP_BASE + flash_offset + sector * FLASH_SECTOR_SIZE + offset, size_bytes);
    return (int32_t) size_bytes;
}

int32_t uGamePad::io_flash_read_sector(uint32_t offset, void *buffer) {
    memcpy(buffer, (uint8_t *) XIP_BASE + offset, FLASH_SECTOR_SIZE);
    return (int32_t) FLASH_SECTOR_SIZE;
}

int32_t uGamePad::io_flash_write(uint32_t sector, uint32_t offset, const uint8_t *buffer, uint32_t size_bytes) {
    //printf("io_flash_write: sector: %lu, offset: %lu, size: %lu (flash offset: 0x%08lx)\r\n",
    //     sector, offset, size_bytes, flash_offset + sector * FLASH_SECTOR_SIZE);
    auto status = save_and_disable_interrupts();

    if (offset == 0) flash_range_erase(flash_offset + sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
    flash_range_program(flash_offset + sector * FLASH_SECTOR_SIZE + offset, buffer, size_bytes);

    restore_interrupts(status);

    return (int32_t) size_bytes;
}

void uGamePad::io_flash_write_sector(uint32_t offset, const uint8_t *buffer) {
    auto status = save_and_disable_interrupts();

    flash_range_erase(offset, FLASH_SECTOR_SIZE);
    flash_range_program(offset, buffer, FLASH_SECTOR_SIZE);

    restore_interrupts(status);
}

void uGamePad::io_flash_exit() {}
