//
// Created by cpasjuste on 28/03/23.
//

#include "main.h"
#include "devices.h"
#include "tusb.h"

using namespace uGamePad;

#define MAX_REPORT  4

#if 0
#define LANGUAGE_ID 0x0409
#define BUF_COUNT   4

tusb_desc_device_t desc_device;

uint8_t buf_pool[BUF_COUNT][64];
uint8_t buf_owner[BUF_COUNT] = {0}; // device address that owns buffer

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
static void print_utf16(uint16_t *temp_buf, size_t buf_len);

void print_device_descriptor(tuh_xfer_t *xfer);

void parse_config_descriptor(uint8_t dev_addr, tusb_desc_configuration_t const *desc_cfg);

uint8_t *get_hid_buf(uint8_t daddr);

void free_hid_buf(uint8_t daddr);

//--------------------------------------------------------------------+
// Device Descriptor
//--------------------------------------------------------------------+

void print_device_descriptor(tuh_xfer_t *xfer) {
    if (XFER_RESULT_SUCCESS != xfer->result) {
        printf("Failed to get device descriptor\r\n");
        return;
    }

    uint8_t const daddr = xfer->daddr;

    printf("Device %u: ID %04x:%04x\r\n", daddr, desc_device.idVendor, desc_device.idProduct);
    printf("Device Descriptor:\r\n");
    printf("  bLength             %u\r\n", desc_device.bLength);
    printf("  bDescriptorType     %u\r\n", desc_device.bDescriptorType);
    printf("  bcdUSB              %04x\r\n", desc_device.bcdUSB);
    printf("  bDeviceClass        %u\r\n", desc_device.bDeviceClass);
    printf("  bDeviceSubClass     %u\r\n", desc_device.bDeviceSubClass);
    printf("  bDeviceProtocol     %u\r\n", desc_device.bDeviceProtocol);
    printf("  bMaxPacketSize0     %u\r\n", desc_device.bMaxPacketSize0);
    printf("  idVendor            0x%04x\r\n", desc_device.idVendor);
    printf("  idProduct           0x%04x\r\n", desc_device.idProduct);
    printf("  bcdDevice           %04x\r\n", desc_device.bcdDevice);

    // Get String descriptor using Sync API
    uint16_t temp_buf[128];

    printf("  iManufacturer       %u     ", desc_device.iManufacturer);
    if (XFER_RESULT_SUCCESS ==
        tuh_descriptor_get_manufacturer_string_sync(daddr, LANGUAGE_ID, temp_buf, sizeof(temp_buf))) {
        print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
    }
    printf("\r\n");

    printf("  iProduct            %u     ", desc_device.iProduct);
    if (XFER_RESULT_SUCCESS == tuh_descriptor_get_product_string_sync(daddr, LANGUAGE_ID, temp_buf, sizeof(temp_buf))) {
        print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
    }
    printf("\r\n");

    printf("  iSerialNumber       %u     ", desc_device.iSerialNumber);
    if (XFER_RESULT_SUCCESS == tuh_descriptor_get_serial_string_sync(daddr, LANGUAGE_ID, temp_buf, sizeof(temp_buf))) {
        print_utf16(temp_buf, TU_ARRAY_SIZE(temp_buf));
    }
    printf("\r\n");

    printf("  bNumConfigurations  %u\r\n", desc_device.bNumConfigurations);

    // Get configuration descriptor with sync API
    if (XFER_RESULT_SUCCESS == tuh_descriptor_get_configuration_sync(daddr, 0, temp_buf, sizeof(temp_buf))) {
        parse_config_descriptor(daddr, (tusb_desc_configuration_t *) temp_buf);
    }
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

// count total length of an interface
uint16_t count_interface_total_len(tusb_desc_interface_t const *desc_itf, uint8_t itf_count, uint16_t max_len);

void open_hid_interface(uint8_t daddr, tusb_desc_interface_t const *desc_itf, uint16_t max_len);

typedef struct {
    uint8_t daddr;

    uint8_t itf_num;
    uint8_t ep_in;
    uint8_t ep_out;
    bool mounted;           // Enumeration is complete

    uint8_t itf_protocol;   // None, Keyboard, Mouse
    uint8_t protocol_mode;  // Boot (0) or Report protocol (1)

    uint8_t report_desc_type;
    uint16_t report_desc_len;

    uint16_t epin_size;
    uint16_t epout_size;

    CFG_TUSB_MEM_ALIGN uint8_t epin_buf[CFG_TUH_HID_EPIN_BUFSIZE];
    CFG_TUSB_MEM_ALIGN uint8_t epout_buf[CFG_TUH_HID_EPOUT_BUFSIZE];
} hidh_interface_t;

static hidh_interface_t _hidh_itf[CFG_TUH_HID];

static hidh_interface_t *find_new_itf(void) {
    for (uint8_t i = 0; i < CFG_TUH_HID; i++) {
        if (_hidh_itf[i].daddr == 0) return &_hidh_itf[i];
    }
    return NULL;
}

bool open_hid_interface_custom(uint8_t rhport, uint8_t daddr, tusb_desc_interface_t const *desc_itf, uint16_t max_len) {
    (void) rhport;
    (void) max_len;

    //TU_VERIFY(TUSB_CLASS_HID == desc_itf->bInterfaceClass);
    printf("[%u] HID opening Interface %u\r\n", daddr, desc_itf->bInterfaceNumber);

    // len = interface + hid + n*endpoints
    uint16_t const drv_len = (uint16_t) (sizeof(tusb_desc_interface_t) + sizeof(tusb_hid_descriptor_hid_t) +
                                         desc_itf->bNumEndpoints * sizeof(tusb_desc_endpoint_t));
    TU_ASSERT(max_len >= drv_len);
    uint8_t const *p_desc = (uint8_t const *) desc_itf;

    //------------- HID descriptor -------------//
    p_desc = tu_desc_next(p_desc);
    tusb_hid_descriptor_hid_t const *desc_hid = (tusb_hid_descriptor_hid_t const *) p_desc;
    //TU_ASSERT(HID_DESC_TYPE_HID == desc_hid->bDescriptorType);

    hidh_interface_t *p_hid = find_new_itf();
    TU_ASSERT(p_hid); // not enough interface, try to increase CFG_TUH_HID
    p_hid->daddr = daddr;

    //------------- Endpoint Descriptors -------------//
    p_desc = tu_desc_next(p_desc);
    tusb_desc_endpoint_t const *desc_ep = (tusb_desc_endpoint_t const *) p_desc;

    for (int i = 0; i < desc_itf->bNumEndpoints; i++) {
        TU_ASSERT(TUSB_DESC_ENDPOINT == desc_ep->bDescriptorType);
        TU_ASSERT(tuh_edpt_open(daddr, desc_ep));

        if (tu_edpt_dir(desc_ep->bEndpointAddress) == TUSB_DIR_IN) {
            p_hid->ep_in = desc_ep->bEndpointAddress;
            p_hid->epin_size = tu_edpt_packet_size(desc_ep);
        } else {
            p_hid->ep_out = desc_ep->bEndpointAddress;
            p_hid->epout_size = tu_edpt_packet_size(desc_ep);
        }

        p_desc = tu_desc_next(p_desc);
        desc_ep = (tusb_desc_endpoint_t const *) p_desc;
    }

    p_hid->itf_num = desc_itf->bInterfaceNumber;

    // Assume bNumDescriptors = 1
    p_hid->report_desc_type = desc_hid->bReportType;
    p_hid->report_desc_len = tu_unaligned_read16(&desc_hid->wReportLength);

    // Per HID Specs: default is Report protocol, though we will force Boot protocol when set_config
    p_hid->protocol_mode = tuh_hid_get_protocol(daddr, HID_PROTOCOL_BOOT);
    if (HID_SUBCLASS_BOOT == desc_itf->bInterfaceSubClass) {
        p_hid->itf_protocol = desc_itf->bInterfaceProtocol;
    }

    return true;
}

// simple configuration parser to open and listen to HID Endpoint IN
void parse_config_descriptor(uint8_t dev_addr, tusb_desc_configuration_t const *desc_cfg) {
    uint8_t const *desc_end = ((uint8_t const *) desc_cfg) + tu_le16toh(desc_cfg->wTotalLength);
    uint8_t const *p_desc = tu_desc_next(desc_cfg);

    // parse each interfaces
    while (p_desc < desc_end) {
        uint8_t assoc_itf_count = 1;

        // Class will always starts with Interface Association (if any) and then Interface descriptor
        if (TUSB_DESC_INTERFACE_ASSOCIATION == tu_desc_type(p_desc)) {
            tusb_desc_interface_assoc_t const *desc_iad = (tusb_desc_interface_assoc_t const *) p_desc;
            assoc_itf_count = desc_iad->bInterfaceCount;

            p_desc = tu_desc_next(p_desc); // next to Interface
        }

        // must be interface from now
        if (TUSB_DESC_INTERFACE != tu_desc_type(p_desc)) return;
        tusb_desc_interface_t const *desc_itf = (tusb_desc_interface_t const *) p_desc;

        uint16_t const drv_len = count_interface_total_len(desc_itf, assoc_itf_count, (uint16_t) (desc_end - p_desc));

        // probably corrupted descriptor
        if (drv_len < sizeof(tusb_desc_interface_t)) return;

        // only open and listen to HID endpoint IN
        // TUSB_CLASS_VENDOR_SPECIFIC for xbox pad support
        if (desc_itf->bInterfaceClass == TUSB_CLASS_HID
            || desc_itf->bInterfaceClass == TUSB_CLASS_VENDOR_SPECIFIC) {
            //open_hid_interface(dev_addr, desc_itf, drv_len);
            //tuh_hid_mount_cb(dev_addr, 0, reinterpret_cast<const uint8_t *>(desc_itf), drv_len);
            //printf("hidh_open: %i\r\n", hidh_open(0, dev_addr, desc_itf, drv_len));
            open_hid_interface_custom(0, dev_addr, desc_itf, drv_len);
            hidh_set_config(dev_addr, 0);
        }

        // next Interface or IAD descriptor
        p_desc += drv_len;
    }
}

uint16_t count_interface_total_len(tusb_desc_interface_t const *desc_itf, uint8_t itf_count, uint16_t max_len) {
    uint8_t const *p_desc = (uint8_t const *) desc_itf;
    uint16_t len = 0;

    while (itf_count--) {
        // Next on interface desc
        len += tu_desc_len(desc_itf);
        p_desc = tu_desc_next(p_desc);

        while (len < max_len) {
            // return on IAD regardless of itf count
            if (tu_desc_type(p_desc) == TUSB_DESC_INTERFACE_ASSOCIATION) return len;

            if ((tu_desc_type(p_desc) == TUSB_DESC_INTERFACE) &&
                ((tusb_desc_interface_t const *) p_desc)->bAlternateSetting == 0) {
                break;
            }

            len += tu_desc_len(p_desc);
            p_desc = tu_desc_next(p_desc);
        }
    }

    return len;
}

//--------------------------------------------------------------------+
// HID Interface
//--------------------------------------------------------------------+

void hid_report_received(tuh_xfer_t *xfer);

void open_hid_interface(uint8_t daddr, tusb_desc_interface_t const *desc_itf, uint16_t max_len) {
    // len = interface + hid + n*endpoints
    uint16_t const drv_len = (uint16_t) (sizeof(tusb_desc_interface_t) + sizeof(tusb_hid_descriptor_hid_t) +
                                         desc_itf->bNumEndpoints * sizeof(tusb_desc_endpoint_t));

    // corrupted descriptor
    if (max_len < drv_len) return;

    uint8_t const *p_desc = (uint8_t const *) desc_itf;

    // HID descriptor
    p_desc = tu_desc_next(p_desc);
    tusb_hid_descriptor_hid_t const *desc_hid = (tusb_hid_descriptor_hid_t const *) p_desc;
    if (HID_DESC_TYPE_HID != desc_hid->bDescriptorType) return;

    // Endpoint descriptor
    p_desc = tu_desc_next(p_desc);
    tusb_desc_endpoint_t const *desc_ep = (tusb_desc_endpoint_t const *) p_desc;

    for (int i = 0; i < desc_itf->bNumEndpoints; i++) {
        if (TUSB_DESC_ENDPOINT != desc_ep->bDescriptorType) return;

        if (tu_edpt_dir(desc_ep->bEndpointAddress) == TUSB_DIR_IN) {
            // skip if failed to open endpoint
            if (!tuh_edpt_open(daddr, desc_ep)) return;

            uint8_t *buf = get_hid_buf(daddr);
            if (!buf) return; // out of memory

            tuh_xfer_t xfer =
                    {
                            .daddr       = daddr,
                            .ep_addr     = desc_ep->bEndpointAddress,
                            .buflen      = 64,
                            .buffer      = buf,
                            .complete_cb = hid_report_received,
                            .user_data   = (uintptr_t) buf, // since buffer is not available in callback, use user data to store the buffer
                    };

            // submit transfer for this EP
            tuh_edpt_xfer(&xfer);

            printf("Listen to [dev %u: ep %02x]\r\n", daddr, desc_ep->bEndpointAddress);
        }

        p_desc = tu_desc_next(p_desc);
        desc_ep = (tusb_desc_endpoint_t const *) p_desc;
    }
}

void hid_report_received(tuh_xfer_t *xfer) {
    // Note: not all field in xfer is available for use (i.e filled by tinyusb stack) in callback to save sram
    // For instance, xfer->buffer is NULL. We have used user_data to store buffer when submitted callback
    uint8_t *buf = (uint8_t *) xfer->user_data;

    if (xfer->result == XFER_RESULT_SUCCESS) {
        printf("[dev %u: ep %02x] HID Report:", xfer->daddr, xfer->ep_addr);
        for (uint32_t i = 0; i < xfer->actual_len; i++) {
            if (i % 16 == 0) printf("\r\n  ");
            printf("%02X ", buf[i]);
        }
        printf("\r\n");
    }

    // continue to submit transfer, with updated buffer
    // other field remain the same
    xfer->buflen = 64;
    xfer->buffer = buf;

    tuh_edpt_xfer(xfer);
}

//--------------------------------------------------------------------+
// Buffer helper
//--------------------------------------------------------------------+

// get an buffer from pool
uint8_t *get_hid_buf(uint8_t daddr) {
    for (size_t i = 0; i < BUF_COUNT; i++) {
        if (buf_owner[i] == 0) {
            buf_owner[i] = daddr;
            return buf_pool[i];
        }
    }

    // out of memory, increase BUF_COUNT
    return nullptr;
}

// free all buffer owned by device
void free_hid_buf(uint8_t daddr) {
    for (unsigned char &i: buf_owner) {
        if (i == daddr) i = 0;
    }
}

//--------------------------------------------------------------------+
// String Descriptor Helper
//--------------------------------------------------------------------+

static void _convert_utf16le_to_utf8(const uint16_t *utf16, size_t utf16_len, uint8_t *utf8, size_t utf8_len) {
    // TODO: Check for runover.
    (void) utf8_len;
    // Get the UTF-16 length out of the data itself.

    for (size_t i = 0; i < utf16_len; i++) {
        uint16_t chr = utf16[i];
        if (chr < 0x80) {
            *utf8++ = chr & 0xffu;
        } else if (chr < 0x800) {
            *utf8++ = (uint8_t) (0xC0 | (chr >> 6 & 0x1F));
            *utf8++ = (uint8_t) (0x80 | (chr >> 0 & 0x3F));
        } else {
            // TODO: Verify surrogate.
            *utf8++ = (uint8_t) (0xE0 | (chr >> 12 & 0x0F));
            *utf8++ = (uint8_t) (0x80 | (chr >> 6 & 0x3F));
            *utf8++ = (uint8_t) (0x80 | (chr >> 0 & 0x3F));
        }
        // TODO: Handle UTF-16 code points that take two entries.
    }
}

// Count how many bytes a utf-16-le encoded string will take in utf-8.
static int _count_utf8_bytes(const uint16_t *buf, size_t len) {
    size_t total_bytes = 0;
    for (size_t i = 0; i < len; i++) {
        uint16_t chr = buf[i];
        if (chr < 0x80) {
            total_bytes += 1;
        } else if (chr < 0x800) {
            total_bytes += 2;
        } else {
            total_bytes += 3;
        }
        // TODO: Handle UTF-16 code points that take two entries.
    }
    return (int) total_bytes;
}

static void print_utf16(uint16_t *temp_buf, size_t buf_len) {
    if ((temp_buf[0] & 0xff) == 0) return;  // empty
    size_t utf16_len = ((temp_buf[0] & 0xff) - 2) / sizeof(uint16_t);
    size_t utf8_len = (size_t) _count_utf8_bytes(temp_buf + 1, utf16_len);
    _convert_utf16le_to_utf8(temp_buf + 1, utf16_len, (uint8_t *) temp_buf, sizeof(uint16_t) * buf_len);
    ((uint8_t *) temp_buf)[utf8_len] = '\0';

    printf((char *) temp_buf);
}
#endif

extern "C" {
void tuh_mount_cb(uint8_t dev_addr) {
    printf("tuh_mount_cb: new device connected, address: %d\r\n", dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr) {
    printf("tuh_umount_cb: device disconnected, address: %d\r\n", dev_addr);
}
}

static struct {
    uint8_t report_count;
    tuh_hid_report_info_t report_info[MAX_REPORT];
} hid_info[CFG_TUH_HID];

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    printf("mount_cb: vid: %x, pid: %x, addr: %i, instance: %i, len: %i\r\n",
           vid, pid, dev_addr, instance, desc_len);

    // return if pad is not initialized yet (should not happen...)
    if (!getPlatform()->getPad()) return;

    // first try to find gamepad device configuration from (user) flash filesystem
#warning "TODO: port config"
#if 0
    const Device *device = getPlatform()->getConfig()->loadDevice(vid, pid);
    if (!device) {
        // now try to find the gamepad device from devices table
        device = get_device(vid, pid);
    }
#else
    const Device *device = get_device(vid, pid);
#endif
    // a know controller was plugged in (see devices.c)
    if (device) {
        if (device->vendor != getPlatform()->getPad()->getDevice()->vendor ||
            device->product != getPlatform()->getPad()->getDevice()->product) {
            getPlatform()->getPad()->setDevice(device, dev_addr, instance);
            // send init message if provided
            if (device->data->init.size > 0) {
                tuh_hid_set_report(dev_addr, instance, 5, HID_REPORT_TYPE_OUTPUT,
                                   &device->data->init.msg, device->data->init.size);
            }
        }
    } else {
        printf("mount_cb: unknown gamepad (%04x:%04x)\r\n", vid, pid);
    }

    // Parse data descriptor with built-in parser
    hid_info[instance].report_count = tuh_hid_parse_report_descriptor(
            hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("mount_cb: tuh_hid_receive_report failed\r\n");
    }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("umount_cb: device unmounted (address: %d, instance: %d)\r\n", dev_addr, instance);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    uint8_t const rpt_count = hid_info[instance].report_count;
    tuh_hid_report_info_t *rpt_info_arr = hid_info[instance].report_info;
    tuh_hid_report_info_t *rpt_info = nullptr;
    uint16_t vid, pid;

    auto pad = (PicoGamePad *) getPlatform()->getPad();
    if (!pad || pad->isUnknown()) return;

    tuh_vid_pid_get(dev_addr, &vid, &pid);
    if (pad->getDevice()->product != pid || pad->getDevice()->vendor != vid) {
        printf("tuh_hid_report_received_cb: skipping data, wrong vid or pid for %s...\r\n", pad->getDevice()->name);
        tuh_hid_receive_report(dev_addr, instance);
        return;
    }

    //printf("tuh_hid_report_received_cb: addr: %i, instance: %i, len: %i\r\n", dev_addr, instance, len);
    if (!pad->report(report, len)) {
        // try to handle generic pads
        if (rpt_count == 1 && rpt_info_arr[0].report_id == 0) {
            // simple data without data ID as 1st byte
            rpt_info = &rpt_info_arr[0];
        } else {
            // composite data, 1st byte is data ID, data starts from 2nd byte
            uint8_t const rpt_id = report[0];
            // find data id in the arrray
            for (uint8_t i = 0; i < rpt_count; i++) {
                if (rpt_id == rpt_info_arr[i].report_id) {
                    rpt_info = &rpt_info_arr[i];
                    break;
                }
            }
            report++;
            len--;
        }

        if (!rpt_info) {
            printf("tuh_hid_report_received_cb: couldn't find the data info for this data !\r\n");
            return;
        }

        //printf("usage %d, %d\n", rpt_info->usage_page, rpt_info->usage);
        if (rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP) {
            switch (rpt_info->usage) {
                case HID_USAGE_DESKTOP_MOUSE:
                case HID_USAGE_DESKTOP_KEYBOARD:
                    //TU_LOG1("HID receive mouse/keyboard data\n");
                    break;
                case HID_USAGE_DESKTOP_JOYSTICK: {
                    // TU_LOG1("HID receive joystick data\n");
                    struct JoyStickReport {
                        uint8_t axis[3];
                        uint8_t buttons;
                    };
                    auto *rep = reinterpret_cast<const JoyStickReport *>(report);
                    //printf("x %d y %d button %02x\r\n", rep->axis[0], rep->axis[1], rep->buttons);
                }
                    break;
                case HID_USAGE_DESKTOP_GAMEPAD: {
                    // TODO
                }
                    break;
                default:
                    break;
            }
        }
    }

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("tuh_hid_report_received_cb: cannot request to receive data\r\n");
    }
}
