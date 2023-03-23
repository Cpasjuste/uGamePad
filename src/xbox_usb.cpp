/* Copyright (C) 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */

#include "xbox_usb.h"
#include "game_controller.h"
#include "led.h"

// To enable serial debugging see "settings.h"
//#define DEBUG_USB_HOST 1
//#define EXTRADEBUG // Uncomment to get even more debugging data
//#define PRINTREPORT // Uncomment to print the report send by the Xbox 360 Controller

XboxUsb::XboxUsb(USBHost *p) :
        pUsb(p), // pointer to USB class instance - mandatory
        bAddress(0), // device address - mandatory
        bPollEnable(false) { // don't start polling before dongle is connected
    for (uint8_t i = 0; i < XBOX_MAX_ENDPOINTS; i++) {
        epInfo[i].epAddr = 0;
        epInfo[i].maxPktSize = (i) ? 0 : 8;
        epInfo[i].epAttribs = 0;
        epInfo[i].bmSndToggle = 0;
        epInfo[i].bmRcvToggle = 0;
        epInfo[i].bmNakPower = (i) ? USB_NAK_NOWAIT : USB_NAK_MAX_POWER;
    }

    if (pUsb) // register in USB subsystem
        pUsb->RegisterDeviceClass(this); //set devConfig[] entry
}

uint32_t XboxUsb::Init(uint32_t parent, uint32_t port, uint32_t lowspeed) {
    uint8_t buf[sizeof(USB_DEVICE_DESCRIPTOR)];
    auto udd = reinterpret_cast<USB_DEVICE_DESCRIPTOR *>(buf);
    uint8_t rcode;
    UsbDeviceDefinition *p;
    EpInfo *old_ep_ptr;
    GameControllerDesc *controller;
    uint16_t PID;
    uint16_t VID;

    // get memory address of USB device address pool
    AddressPool &addrPool = pUsb->GetAddressPool();
#ifdef EXTRADEBUG
    Notify(PSTR("\r\nXBOXUSB Init"), 0x80);
#endif
    // check if address has already been assigned to an instance
    if (bAddress) {
#ifdef DEBUG_USB_HOST
        Notify(PSTR("\r\nAddress in use"), 0x80);
#endif
        return USB_ERROR_CLASS_INSTANCE_ALREADY_IN_USE;
    }

    // Get pointer to pseudo device with address 0 assigned
    p = addrPool.GetUsbDevicePtr(0);

    if (!p) {
#ifdef DEBUG_USB_HOST
        Notify(PSTR("\r\nAddress not found"), 0x80);
#endif
        return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;
    }

    if (!p->epinfo) {
#ifdef DEBUG_USB_HOST
        Notify(PSTR("\r\nepinfo is null"), 0x80);
#endif
        return USB_ERROR_EPINFO_IS_NULL;
    }

    // Save old pointer to EP_RECORD of address 0
    old_ep_ptr = p->epinfo;

    // Temporary assign new pointer to epInfo to p->epinfo in order to avoid toggle inconsistence
    p->epinfo = epInfo;

    p->lowspeed = lowspeed;

    // Get device descriptor
    rcode = pUsb->getDevDescr(0, 0, sizeof(USB_DEVICE_DESCRIPTOR),
                              (uint8_t *) buf); // Get device descriptor - addr, ep, nbytes, data
    // Restore p->epinfo
    p->epinfo = old_ep_ptr;

    if (rcode)
        goto FailGetDevDescr;

    VID = udd->idVendor;
    PID = udd->idProduct;

    controller = GameController::getController(PID, VID);
    if (controller != nullptr && controller->xtype == XTYPE_XBOX360) {
        Serial1.printf("new device detected: %s\n", controller->name);
        Led::Blink(2, Led::BLINK_RATE_SUCCESS);
    } else {
        Serial1.printf("unknown device detected: vid: %x, pid: %x\n", VID, PID);
        Led::Blink(2, Led::BLINK_RATE_ERROR);
        goto FailUnknownDevice;
    }

    // Allocate new address according to device class
    bAddress = addrPool.AllocAddress(parent, false, port);

    if (!bAddress)
        return USB_ERROR_OUT_OF_ADDRESS_SPACE_IN_POOL;

    // Extract Max Packet Size from device descriptor
    epInfo[0].maxPktSize = udd->bMaxPacketSize0;

    // Assign new address to the device
    rcode = pUsb->setAddr(0, 0, bAddress);
    if (rcode) {
        p->lowspeed = false;
        addrPool.FreeAddress(bAddress);
        bAddress = 0;
#ifdef DEBUG_USB_HOST
        Notify(PSTR("\r\nsetAddr: "), 0x80);
        D_PrintHex<uint8_t>(rcode, 0x80);
#endif
        return rcode;
    }
#ifdef EXTRADEBUG
    Notify(PSTR("\r\nAddr: "), 0x80);
    D_PrintHex<uint8_t > (bAddress, 0x80);
#endif
    //delay(300); // Spec says you should wait at least 200ms

    p->lowspeed = false;

    //get pointer to assigned address record
    p = addrPool.GetUsbDevicePtr(bAddress);
    if (!p)
        return USB_ERROR_ADDRESS_NOT_FOUND_IN_POOL;

    p->lowspeed = lowspeed;

    // Assign epInfo to epinfo pointer - only EP0 is known
    rcode = pUsb->setEpInfoEntry(bAddress, 1, epInfo);
    if (rcode)
        goto FailSetDevTblEntry;

    /* The application will work in reduced host mode, so we can save program and data
       memory space. After verifying the VID we will use known values for the
       configuration values for device, interface, endpoints and HID for the XBOX360 Controllers */

    /* Initialize data structures for endpoints of device */
    epInfo[XBOX_INPUT_PIPE].epAddr = 0x01; // XBOX 360 report endpoint
    epInfo[XBOX_INPUT_PIPE].epAttribs = USB_TRANSFER_TYPE_INTERRUPT;
    epInfo[XBOX_INPUT_PIPE].bmNakPower = USB_NAK_NOWAIT; // Only poll once for interrupt endpoints
    epInfo[XBOX_INPUT_PIPE].maxPktSize = EP_MAXPKTSIZE;
    epInfo[XBOX_INPUT_PIPE].bmSndToggle = 0;
    epInfo[XBOX_INPUT_PIPE].bmRcvToggle = 0;
    epInfo[XBOX_OUTPUT_PIPE].epAddr = 0x02; // XBOX 360 output endpoint
    epInfo[XBOX_OUTPUT_PIPE].epAttribs = USB_TRANSFER_TYPE_INTERRUPT;
    epInfo[XBOX_OUTPUT_PIPE].bmNakPower = USB_NAK_NOWAIT; // Only poll once for interrupt endpoints
    epInfo[XBOX_OUTPUT_PIPE].maxPktSize = EP_MAXPKTSIZE;
    epInfo[XBOX_OUTPUT_PIPE].bmSndToggle = 0;
    epInfo[XBOX_OUTPUT_PIPE].bmRcvToggle = 0;

    rcode = pUsb->setEpInfoEntry(bAddress, 3, epInfo);
    if (rcode)
        goto FailSetDevTblEntry;

    delay(200); // Give time for address change

    rcode = pUsb->setConf(bAddress, epInfo[XBOX_CONTROL_PIPE].epAddr, 1);
    if (rcode)
        goto FailSetConfDescr;

#ifdef DEBUG_USB_HOST
    Notify(PSTR("\r\nXbox 360 Controller Connected\r\n"), 0x80);
#endif
    onInit();
    Xbox360Connected = true;
    bPollEnable = true;
    return 0; // Successful configuration

    /* Diagnostic messages */
    FailGetDevDescr:
#ifdef DEBUG_USB_HOST
    NotifyFailGetDevDescr();
    goto Fail;
#endif

    FailSetDevTblEntry:
#ifdef DEBUG_USB_HOST
    NotifyFailSetDevTblEntry();
    goto Fail;
#endif

    FailSetConfDescr:
#ifdef DEBUG_USB_HOST
    NotifyFailSetConfDescr();
#endif
    goto Fail;

    FailUnknownDevice:
#ifdef DEBUG_USB_HOST
    NotifyFailUnknownDevice(VID, PID);
#endif
    rcode = USB_DEV_CONFIG_ERROR_DEVICE_NOT_SUPPORTED;

    Fail:
#ifdef DEBUG_USB_HOST
    Notify(PSTR("\r\nXbox 360 Init Failed, error code: "), 0x80);
    NotifyFail(rcode);
#endif
    Release();
    return rcode;
}

/* Performs a cleanup after failed Init() attempt */
uint32_t XboxUsb::Release() {
    Xbox360Connected = false;
    pUsb->GetAddressPool().FreeAddress(bAddress);
    bAddress = 0;
    bPollEnable = false;
    return 0;
}

uint32_t XboxUsb::Poll() {
    if (!bPollEnable)
        return 0;
    uint16_t BUFFER_SIZE = EP_MAXPKTSIZE;
    pUsb->inTransfer((uint32_t) bAddress, epInfo[XBOX_INPUT_PIPE].epAddr, &BUFFER_SIZE, readBuf); // input on endpoint 1
    readReport();
#ifdef PRINTREPORT
    printReport(); // Uncomment "#define PRINTREPORT" to print the report send by the Xbox 360 Controller
#endif
    return 0;
}

void XboxUsb::readReport() {
    if (readBuf[0] != 0x00 ||
        readBuf[1] != 0x14) { // Check if it's the correct report - the controller also sends different status reports
        return;
    }

    ButtonState = (uint32_t) (readBuf[5] | ((uint16_t) readBuf[4] << 8) | ((uint32_t) readBuf[3] << 16) |
                              ((uint32_t) readBuf[2] << 24));

    hatValue[LeftHatX] = (int16_t) (((uint16_t) readBuf[7] << 8) | readBuf[6]);
    hatValue[LeftHatY] = (int16_t) (((uint16_t) readBuf[9] << 8) | readBuf[8]);
    hatValue[RightHatX] = (int16_t) (((uint16_t) readBuf[11] << 8) | readBuf[10]);
    hatValue[RightHatY] = (int16_t) (((uint16_t) readBuf[13] << 8) | readBuf[12]);

    //Notify(PSTR("\r\nButtonState"), 0x80);
    //PrintHex<uint32_t>(ButtonState, 0x80);

    if (ButtonState != OldButtonState) {
        ButtonClickState = (ButtonState >> 16) & ((~OldButtonState)
                >> 16); // Update click state variable, but don't include the two trigger buttons L2 and R2
        if (((uint8_t) OldButtonState) == 0 &&
            ((uint8_t) ButtonState) != 0) // The L2 and R2 buttons are special as they are analog buttons
            R2Clicked = true;
        if ((uint8_t) (OldButtonState >> 8) == 0 && (uint8_t) (ButtonState >> 8) != 0)
            L2Clicked = true;
        OldButtonState = ButtonState;
    }
}

void XboxUsb::printReport() { //Uncomment "#define PRINTREPORT" to print the report send by the Xbox 360 Controller
#ifdef PRINTREPORT
    if (readBuf == NULL)
        return;
    for (uint8_t i = 0; i < XBOX_REPORT_BUFFER_SIZE; i++) {
        D_PrintHex<uint8_t>(readBuf[i], 0x80);
        Notify(PSTR(" "), 0x80);
    }
    Notify(PSTR("\r\n"), 0x80);
#endif
}

uint8_t XboxUsb::getButtonPress(ButtonEnum b) {
    if (b == L2) // These are analog buttons
        return (uint8_t) (ButtonState >> 8);
    else if (b == R2)
        return (uint8_t) ButtonState;
    return (bool) (ButtonState & ((uint32_t) pgm_read_word(&XBOX_BUTTONS[(uint8_t) b]) << 16));
}

bool XboxUsb::getButtonClick(ButtonEnum b) {
    if (b == L2) {
        if (L2Clicked) {
            L2Clicked = false;
            return true;
        }
        return false;
    } else if (b == R2) {
        if (R2Clicked) {
            R2Clicked = false;
            return true;
        }
        return false;
    }
    uint16_t button = pgm_read_word(&XBOX_BUTTONS[(uint8_t) b]);
    bool click = (ButtonClickState & button);
    ButtonClickState &= ~button; // clear "click" event
    return click;
}

int16_t XboxUsb::getAnalogHat(AnalogHatEnum a) {
    return hatValue[a];
}

/* Xbox Controller commands */
void XboxUsb::XboxCommand(uint8_t *data, uint16_t nbytes) {
    //bmRequest = Host to device (0x00) | Class (0x20) | Interface (0x01) = 0x21, bRequest = Set Report (0x09), Report ID (0x00), Report Type (Output 0x02), interface (0x00), datalength, datalength, data)
    pUsb->ctrlReq(bAddress, epInfo[XBOX_CONTROL_PIPE].epAddr, bmREQ_HIDOUT, HID_REQUEST_SET_REPORT, 0x00, 0x02, 0x00,
                  nbytes, nbytes, data, nullptr);
}

void XboxUsb::setLedRaw(uint8_t value) {
    writeBuf[0] = 0x01;
    writeBuf[1] = 0x03;
    writeBuf[2] = value;

    XboxCommand(writeBuf, 3);
}

void XboxUsb::setLedOn(LEDEnum led) {
    if (led == OFF)
        setLedRaw(0);
    else if (led != ALL) // All LEDs can't be on a the same time
        setLedRaw(pgm_read_byte(&XBOX_LEDS[(uint8_t) led]) + 4);
}

void XboxUsb::setLedBlink(LEDEnum led) {
    setLedRaw(pgm_read_byte(&XBOX_LEDS[(uint8_t) led]));
}

void
XboxUsb::setLedMode(LEDModeEnum ledMode) { // This function is used to do some special LED stuff the controller supports
    setLedRaw((uint8_t) ledMode);
}

void XboxUsb::setRumbleOn(uint8_t lValue, uint8_t rValue) {
    writeBuf[0] = 0x00;
    writeBuf[1] = 0x08;
    writeBuf[2] = 0x00;
    writeBuf[3] = lValue; // big weight
    writeBuf[4] = rValue; // small weight
    writeBuf[5] = 0x00;
    writeBuf[6] = 0x00;
    writeBuf[7] = 0x00;

    XboxCommand(writeBuf, 8);
}

void XboxUsb::onInit() {
    if (pFuncOnInit)
        pFuncOnInit(); // Call the user function
    else
        setLedOn(static_cast<LEDEnum>(LED1));
}