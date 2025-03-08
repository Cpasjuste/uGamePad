//
// Created by cpasjuste on 22/03/23.
//

#include "devices.h"

using namespace uGamePad;

// xbox 360 input report descriptor
// https://www.partsnotincluded.com/understanding-the-xbox-360-wired-controllers-usb-data/
static constexpr InputReportDescriptor xboxReport = {
        .type = REPORT_TYPE_JOYSTICK,
        .report_size = 16,
        .is_xbox = true,
        .joystick = {
                .axis = {
                        // {offset, size, {min, max}}
                        {48, 16, {-32768, 32767}},  // AXIS_L_LEFT / RIGHT
                        {64, 16, {-32768, 32767}},  // AXIS_L_UP / DOWN
                        {80, 16, {-32768, 32767}},  // AXIS_R_LEFT / RIGHT
                        {96, 16, {-32768, 32767}}   // AXIS_R_UP / DOWN
                },
                .buttons = {
                        // byte, bitmask, controller button, ugamepad button
                        3, BIT(4),      // A        -> B1
                        3, BIT(5),      // B        -> B2
                        3, BIT(6),      // X        -> B3
                        3, BIT(7),      // Y        -> B4
                        3, BIT(0),      // LB       -> B5
                        3, BIT(1),      // LR       -> B6
                        2, BIT(4),      // START    -> START
                        2, BIT(5),      // BACK     -> SELECT
                        2, BIT(2),      // LEFT     -> DPAD_LEFT
                        2, BIT(3),      // RIGHT    -> DPAD_RIGHT
                        2, BIT(0),      // UP       -> DPAD_UP
                        2, BIT(1),      // DOWN     -> DPAD_DOWN
                },
                .init = {
                        // values, size
                        {0x01, 0x03, 0x02}, 3 // set led
                },
                .button_count = 12
        }
};

// dual shock 4 input report descriptor
// https://www.psdevwiki.com/ps4/DS4-USB
static constexpr InputReportDescriptor ds4Report = {
        .type = REPORT_TYPE_JOYSTICK,
        .report_size = 63,
        .is_xbox = false,
        .joystick = {
                .axis = {
                        // {offset, size, {min, max}}
                        {8,  8, {0, 255}},  // AXIS_L_LEFT / RIGHT
                        {16, 8, {0, 255}},  // AXIS_L_UP / DOWN
                        {24, 8, {0, 255}},  // AXIS_R_LEFT / RIGHT
                        {32, 8, {0, 255}}   // AXIS_R_UP / DOWN
                },
                .buttons = {
                        // byte, bitmask, controller button, ugamepad button
                        5, BIT(5),      // CROSS        -> B1
                        5, BIT(6),      // CIRCLE       -> B2
                        5, BIT(4),      // SQUARE       -> B3
                        5, BIT(7),      // TRIANGLE     -> B4
                        6, BIT(0),      // L1           -> B5
                        6, BIT(1),      // R1           -> B6
                        6, BIT(5),      // OPTIONS      -> START
                        6, BIT(4),      // SHARE        -> SELECT
                },
                .hat = {
                        // {offset, size, {logical_min, logical_max}, {physical_min, physical_max}
                        40, 4, {0, 7}, {0, 315}   // HAT
                        //32, 4, {0, 7}, {0, 315}   // HAT (as reported by descriptor)
                },
                .button_count = 8
        }
};

// DualSense data
static constexpr InputReportDescriptor ds5Report = {
        .type = REPORT_TYPE_JOYSTICK,
        .report_size = 63,
        .is_xbox = false,
        .joystick = {
                .axis = {
                        // {offset, size, {min, max}}
                        {8,  8, {0, 255}},  // AXIS_L_LEFT / RIGHT
                        {16, 8, {0, 255}},  // AXIS_L_UP / DOWN
                        {24, 8, {0, 255}},  // AXIS_R_LEFT / RIGHT
                        {32, 8, {0, 255}}   // AXIS_R_UP / DOWN
                },
                .buttons = {
                        // byte, bitmask, controller button, ugamepad button
                        8, BIT(5),      // CROSS        -> B1
                        8, BIT(6),      // CIRCLE       -> B2
                        8, BIT(4),      // SQUARE       -> B3
                        8, BIT(7),      // TRIANGLE     -> B4
                        9, BIT(0),      // L1           -> B5
                        9, BIT(1),      // R1           -> B6
                        9, BIT(5),      // OPTIONS      -> START
                        9, BIT(4),      // SHARE        -> SELECT
                },
                .hat = {
                        // {offset, size, {logical_min, logical_max}, {physical_min, physical_max}
                        64, 4, {0, 7}, {0, 315}   // HAT
                },
                .button_count = 8
        }
};

// Neo-Geo Mini (usb-c) data data
static constexpr InputReportDescriptor ngMiniReport = {
        .type = REPORT_TYPE_JOYSTICK,
        .report_size = 27,
        .is_xbox = false,
        .joystick = {
                .buttons = {
                        // byte, bitmask, controller button, ugamepad button
                        0, BIT(1),      // B        -> B1
                        0, BIT(0),      // D        -> B2
                        0, BIT(2),      // A        -> B3
                        0, BIT(3),      // C        -> B4
                        INPUT_DUMMY, INPUT_DUMMY, //-> B5
                        INPUT_DUMMY, INPUT_DUMMY, //-> B6
                        1, BIT(1),      // START    -> START
                        1, BIT(0),      // SELECT   -> SELECT
                },
                .hat = {
                        // {offset, size, {logical_min, logical_max}, {physical_min, physical_max}
                        16, 4, {0, 7}, {0, 315}   // HAT
                },
                .button_count = 8
        }
};

// cheap snes gamepad ("USB Gamepad" (descriptor) / "DragonRise Inc. Gamepad" (linux))
static constexpr InputReportDescriptor cheapSnesReport = {
        .type = REPORT_TYPE_JOYSTICK,
        .report_size = 8,
        .joystick = {
                .axis = {
                        // {offset, size, {min, max}}
                        {24, 8, {0, 255}},  // AXIS_L_LEFT / RIGHT
                        {32, 8, {0, 255}}   // AXIS_L_UP / DOWN
                },
                .buttons = {
                        // byte, bitmask, controller button, ugamepad button
                        5, BIT(6),      // B        -> B1
                        5, BIT(5),      // A        -> B2
                        5, BIT(7),      // Y        -> B3
                        5, BIT(4),      // X        -> B4
                        6, BIT(0),      // L        -> B5
                        6, BIT(1),      // R        -> B6
                        6, BIT(5),      // START    -> START
                        6, BIT(4),      // SELECT   -> SELECT
                },
                .button_count = 8
        }
};

static constexpr Device devices[] = {
        ///
        /// XBOX
        ///
        {0x0079, 0x18d4, "GPD Win 2 X-Box Controller",                           (InputReportDescriptor *) &xboxReport},
        {0x03eb, 0xff01, "Wooting One (Legacy)",                                 (InputReportDescriptor *) &xboxReport},
        {0x03eb, 0xff02, "Wooting Two (Legacy)",                                 (InputReportDescriptor *) &xboxReport},
        {0x044f, 0x0f00, "Thrustmaster Wheel",                                   (InputReportDescriptor *) &xboxReport},
        {0x044f, 0x0f03, "Thrustmaster Wheel",                                   (InputReportDescriptor *) &xboxReport},
        {0x044f, 0x0f07, "Thrustmaster, Inc. Controller",                        (InputReportDescriptor *) &xboxReport},
        {0x044f, 0x0f10, "Thrustmaster Modena GT Wheel",                         (InputReportDescriptor *) &xboxReport},
        {0x044f, 0xb326, "Thrustmaster Gamepad GP XID",                          (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0202, "Microsoft X-Box pad v1 (US)",                          (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0285, "Microsoft X-Box pad (Japan)",                          (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0287, "Microsoft Xbox Controller S",                          (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0288, "Microsoft Xbox Controller S v2",                       (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0289, "Microsoft X-Box pad v2 (US)",                          (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x028e, "Microsoft X-Box 360 pad",                              (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x028f, "Microsoft X-Box 360 pad v2",                           (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0291, "Xbox 360 Wireless Receiver (XBOX)",                    (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x02d1, "Microsoft X-Box One pad",                              (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x02dd, "Microsoft X-Box One pad (Firmware 2015)",              (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x02e3, "Microsoft X-Box One Elite pad",                        (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0b00, "Microsoft X-Box One Elite 2 pad",                      (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x02ea, "Microsoft X-Box One S pad",                            (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0719, "Xbox 360 Wireless Receiver",                           (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0b0a, "Microsoft X-Box Adaptive Controller",                  (InputReportDescriptor *) &xboxReport},
        {0x045e, 0x0b12, "Microsoft Xbox Series S|X Controller",                 (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xc21d, "Logitech Gamepad F310",                                (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xc21e, "Logitech Gamepad F510",                                (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xc21f, "Logitech Gamepad F710",                                (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xc242, "Logitech Chillstream Controller",                      (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xca84, "Logitech Xbox Cordless Controller",                    (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xca88, "Logitech Compact Controller for Xbox",                 (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xca8a, "Logitech Precision Vibration Feedback Wheel",          (InputReportDescriptor *) &xboxReport},
        {0x046d, 0xcaa3, "Logitech DriveFx Racing Wheel",                        (InputReportDescriptor *) &xboxReport},
        {0x056e, 0x2004, "Elecom JC-U3613M",                                     (InputReportDescriptor *) &xboxReport},
        {0x05fd, 0x1007, "Mad Catz Controller (unverified)",                     (InputReportDescriptor *) &xboxReport},
        {0x05fd, 0x107a, "InterAct 'PowerPad Pro' X-Box pad (Germany)",          (InputReportDescriptor *) &xboxReport},
        {0x05fe, 0x3030, "Chic Controller",                                      (InputReportDescriptor *) &xboxReport},
        {0x05fe, 0x3031, "Chic Controller",                                      (InputReportDescriptor *) &xboxReport},
        {0x062a, 0x0020, "Logic3 Xbox GamePad",                                  (InputReportDescriptor *) &xboxReport},
        {0x062a, 0x0033, "Competition Pro Steering Wheel",                       (InputReportDescriptor *) &xboxReport},
        {0x06a3, 0x0200, "Saitek Racing Wheel",                                  (InputReportDescriptor *) &xboxReport},
        {0x06a3, 0x0201, "Saitek Adrenalin",                                     (InputReportDescriptor *) &xboxReport},
        {0x06a3, 0xf51a, "Saitek P3600",                                         (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4506, "Mad Catz 4506 Wireless Controller",                    (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4516, "Mad Catz Control Pad",                                 (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4520, "Mad Catz Control Pad Pro",                             (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4522, "Mad Catz LumiCON",                                     (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4526, "Mad Catz Control Pad Pro",                             (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4530, "Mad Catz Universal MC2 Racing Wheel and Pedals",       (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4536, "Mad Catz MicroCON",                                    (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4540, "Mad Catz Beat Pad",                                    (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4556, "Mad Catz Lynx Wireless Controller",                    (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4586, "Mad Catz MicroCon Wireless Controller",                (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4588, "Mad Catz Blaster",                                     (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x45ff, "Mad Catz Beat Pad (w/ Handle)",                        (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4716, "Mad Catz Wired Xbox 360 Controller",                   (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4718, "Mad Catz Street Fighter IV FightStick SE",             (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4726, "Mad Catz Xbox 360 Controller",                         (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4728, "Mad Catz Street Fighter IV FightPad",                  (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4736, "Mad Catz MicroCon Gamepad",                            (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4738, "Mad Catz Wired Xbox 360 Controller (SFIV)",            (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4740, "Mad Catz Beat Pad",                                    (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4743, "Mad Catz Beat Pad Pro",                                (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4758, "Mad Catz Arcade Game Stick",                           (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x4a01, "Mad Catz FightStick TE 2",                             (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x6040, "Mad Catz Beat Pad Pro",                                (InputReportDescriptor *) &xboxReport},
        {0x0738, 0x9871, "Mad Catz Portable Drum",                               (InputReportDescriptor *) &xboxReport},
        {0x0738, 0xb726, "Mad Catz Xbox controller - MW2",                       (InputReportDescriptor *) &xboxReport},
        {0x0738, 0xb738, "Mad Catz MVC2TE Stick 2",                              (InputReportDescriptor *) &xboxReport},
        {0x0738, 0xbeef, "Mad Catz JOYTECH NEO SE Advanced GamePad",             (InputReportDescriptor *) &xboxReport},
        {0x0738, 0xcb02, "Saitek Cyborg Rumble Pad - PC/Xbox 360",               (InputReportDescriptor *) &xboxReport},
        {0x0738, 0xcb03, "Saitek P3200 Rumble Pad - PC/Xbox 360",                (InputReportDescriptor *) &xboxReport},
        {0x0738, 0xcb29, "Saitek Aviator Stick AV8R02",                          (InputReportDescriptor *) &xboxReport},
        {0x0738, 0xf738, "Super SFIV FightStick TE S",                           (InputReportDescriptor *) &xboxReport},
        {0x07ff, 0xffff, "Mad Catz GamePad",                                     (InputReportDescriptor *) &xboxReport},
        {0x0c12, 0x0005, "Intec wireless",                                       (InputReportDescriptor *) &xboxReport},
        {0x0c12, 0x8801, "Nyko Xbox Controller",                                 (InputReportDescriptor *) &xboxReport},
        {0x0c12, 0x8802, "Zeroplus Xbox Controller",                             (InputReportDescriptor *) &xboxReport},
        {0x0c12, 0x880a, "Pelican Eclipse PL-2023",                              (InputReportDescriptor *) &xboxReport},
        {0x0c12, 0x8810, "Zeroplus Xbox Controller",                             (InputReportDescriptor *) &xboxReport},
        {0x0c12, 0x9902, "HAMA VibraX - *FAULTY HARDWARE*",                      (InputReportDescriptor *) &xboxReport},
        {0x0d2f, 0x0002, "Andamiro Pump It Up pad",                              (InputReportDescriptor *) &xboxReport},
        {0x0e4c, 0x1097, "Radica Gamester Controller",                           (InputReportDescriptor *) &xboxReport},
        {0x0e4c, 0x1103, "Radica Gamester Reflex",                               (InputReportDescriptor *) &xboxReport},
        {0x0e4c, 0x2390, "Radica Games Jtech Controller",                        (InputReportDescriptor *) &xboxReport},
        {0x0e4c, 0x3510, "Radica Gamester",                                      (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0003, "Logic3 Freebird wireless Controller",                  (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0005, "Eclipse wireless Controller",                          (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0006, "Edge wireless Controller",                             (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0008, "After Glow Pro Controller",                            (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0105, "HSM3 Xbox360 dancepad",                                (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0113, "Afterglow AX.1 Gamepad for Xbox 360",                  (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x011f, "Rock Candy Gamepad Wired Controller",                  (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0131, "PDP EA Sports Controller",                             (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0133, "Xbox 360 Wired Controller",                            (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0139, "Afterglow Prismatic Wired Controller",                 (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x013a, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0146, "Rock Candy Wired Controller for Xbox One",             (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0147, "PDP Marvel Xbox One Controller",                       (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x015c, "PDP Xbox One Arcade Stick",                            (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0161, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0162, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0163, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0164, "PDP Battlefield One",                                  (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0165, "PDP Titanfall 2",                                      (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0201, "Pelican PL-3601 'TSZ' Wired Xbox 360 Controller",      (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0213, "Afterglow Gamepad for Xbox 360",                       (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x021f, "Rock Candy Gamepad for Xbox 360",                      (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0246, "Rock Candy Gamepad for Xbox One 2015",                 (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02a0, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02a1, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02a2, "PDP Wired Controller for Xbox One - Crimson Red",      (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02a4, "PDP Wired Controller for Xbox One - Stealth Series",   (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02a6, "PDP Wired Controller for Xbox One - Camo Series",      (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02a7, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02a8, "PDP Xbox One Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02ab, "PDP Controller for Xbox One",                          (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02ad, "PDP Wired Controller for Xbox One - Stealth Series",   (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02b3, "Afterglow Prismatic Wired Controller",                 (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x02b8, "Afterglow Prismatic Wired Controller",                 (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0301, "Logic3 Controller",                                    (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0346, "Rock Candy Gamepad for Xbox One 2016",                 (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0401, "Logic3 Controller",                                    (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0413, "Afterglow AX.1 Gamepad for Xbox 360",                  (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0x0501, "PDP Xbox 360 Controller",                              (InputReportDescriptor *) &xboxReport},
        {0x0e6f, 0xf900, "PDP Afterglow AX.1",                                   (InputReportDescriptor *) &xboxReport},
        {0x0e8f, 0x0201, "SmartJoy Frag Xpad/PS2 adaptor",                       (InputReportDescriptor *) &xboxReport},
        {0x0e8f, 0x3008, "Generic xbox control (dealextreme)",                   (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x000a, "Hori Co. DOA4 FightStick",                             (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x000c, "Hori PadEX Turbo",                                     (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x000d, "Hori Fighting Stick EX2",                              (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x0016, "Hori Real Arcade Pro.EX",                              (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x001b, "Hori Real Arcade Pro VX",                              (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x0063, "Hori Real Arcade Pro Hayabusa (USA) Xbox One",         (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x0067, "HORIPAD ONE",                                          (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x0078, "Hori Real Arcade Pro V Kai Xbox One",                  (InputReportDescriptor *) &xboxReport},
        {0x0f0d, 0x00c5, "Hori Fighting Commander ONE",                          (InputReportDescriptor *) &xboxReport},
        {0x0f30, 0x010b, "Philips Recoil",                                       (InputReportDescriptor *) &xboxReport},
        {0x0f30, 0x0202, "Joytech Advanced Controller",                          (InputReportDescriptor *) &xboxReport},
        {0x0f30, 0x8888, "BigBen XBMiniPad Controller",                          (InputReportDescriptor *) &xboxReport},
        {0x102c, 0xff0c, "Joytech Wireless Advanced Controller",                 (InputReportDescriptor *) &xboxReport},
        {0x1038, 0x1430, "SteelSeries Stratus Duo",                              (InputReportDescriptor *) &xboxReport},
        {0x1038, 0x1431, "SteelSeries Stratus Duo",                              (InputReportDescriptor *) &xboxReport},
        {0x11c9, 0x55f0, "Nacon GC-100XF",                                       (InputReportDescriptor *) &xboxReport},
        {0x1209, 0x2882, "Ardwiino Controller",                                  (InputReportDescriptor *) &xboxReport},
        {0x12ab, 0x0004, "Honey Bee Xbox360 dancepad",                           (InputReportDescriptor *) &xboxReport},
        {0x12ab, 0x0301, "PDP AFTERGLOW AX.1",                                   (InputReportDescriptor *) &xboxReport},
        {0x12ab, 0x0303, "Mortal Kombat Klassic FightStick",                     (InputReportDescriptor *) &xboxReport},
        {0x12ab, 0x8809, "Xbox DDR dancepad",                                    (InputReportDescriptor *) &xboxReport},
        {0x1430, 0x4748, "RedOctane Guitar Hero X-plorer",                       (InputReportDescriptor *) &xboxReport},
        {0x1430, 0x8888, "TX6500+ Dance Pad (first generation)",                 (InputReportDescriptor *) &xboxReport},
        {0x1430, 0xf801, "RedOctane Controller",                                 (InputReportDescriptor *) &xboxReport},
        {0x146b, 0x0601, "BigBen Interactive XBOX 360 Controller",               (InputReportDescriptor *) &xboxReport},
        {0x146b, 0x0604, "Bigben Interactive DAIJA Arcade Stick",                (InputReportDescriptor *) &xboxReport},
        {0x1532, 0x0037, "Razer Sabertooth",                                     (InputReportDescriptor *) &xboxReport},
        {0x1532, 0x0a00, "Razer Atrox Arcade Stick",                             (InputReportDescriptor *) &xboxReport},
        {0x1532, 0x0a03, "Razer Wildcat",                                        (InputReportDescriptor *) &xboxReport},
        {0x15e4, 0x3f00, "Power A Mini Pro Elite",                               (InputReportDescriptor *) &xboxReport},
        {0x15e4, 0x3f0a, "Xbox Airflo wired controller",                         (InputReportDescriptor *) &xboxReport},
        {0x15e4, 0x3f10, "Batarang Xbox 360 controller",                         (InputReportDescriptor *) &xboxReport},
        {0x162e, 0xbeef, "Joytech Neo-Se Take2",                                 (InputReportDescriptor *) &xboxReport},
        {0x1689, 0xfd00, "Razer Onza Tournament Edition",                        (InputReportDescriptor *) &xboxReport},
        {0x1689, 0xfd01, "Razer Onza Classic Edition",                           (InputReportDescriptor *) &xboxReport},
        {0x1689, 0xfe00, "Razer Sabertooth",                                     (InputReportDescriptor *) &xboxReport},
        {0x1949, 0x041a, "Amazon Game Controller",                               (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0x0002, "Harmonix Rock Band Guitar",                            (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0x0003, "Harmonix Rock Band Drumkit",                           (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0x0130, "Ion Drum Rocker",                                      (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf016, "Mad Catz Xbox 360 Controller",                         (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf018, "Mad Catz Street Fighter IV SE Fighting Stick",         (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf019, "Mad Catz Brawlstick for Xbox 360",                     (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf021, "Mad Cats Ghost Recon FS GamePad",                      (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf023, "MLG Pro Circuit Controller (Xbox)",                    (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf025, "Mad Catz Call Of Duty",                                (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf027, "Mad Catz FPS Pro",                                     (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf028, "Street Fighter IV FightPad",                           (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf02e, "Mad Catz Fightpad",                                    (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf030, "Mad Catz Xbox 360 MC2 MicroCon Racing Wheel",          (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf036, "Mad Catz MicroCon GamePad Pro",                        (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf038, "Street Fighter IV FightStick TE",                      (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf039, "Mad Catz MvC2 TE",                                     (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf03a, "Mad Catz SFxT Fightstick Pro",                         (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf03d, "Street Fighter IV Arcade Stick TE - Chun Li",          (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf03e, "Mad Catz MLG FightStick TE",                           (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf03f, "Mad Catz FightStick SoulCaliber",                      (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf042, "Mad Catz FightStick TES+",                             (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf080, "Mad Catz FightStick TE2",                              (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf501, "HoriPad EX2 Turbo",                                    (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf502, "Hori Real Arcade Pro.VX SA",                           (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf503, "Hori Fighting Stick VX",                               (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf504, "Hori Real Arcade Pro. EX",                             (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf505, "Hori Fighting Stick EX2B",                             (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf506, "Hori Real Arcade Pro.EX Premium VLX",                  (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf900, "Harmonix Xbox 360 Controller",                         (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf901, "Gamestop Xbox 360 Controller",                         (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf903, "Tron Xbox 360 controller",                             (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf904, "PDP Versus Fighting Pad",                              (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xf906, "MortalKombat FightStick",                              (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xfa01, "MadCatz GamePad",                                      (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xfd00, "Razer Onza TE",                                        (InputReportDescriptor *) &xboxReport},
        {0x1bad, 0xfd01, "Razer Onza",                                           (InputReportDescriptor *) &xboxReport},
        {0x20d6, 0x2001, "BDA Xbox Series X Wired Controller",                   (InputReportDescriptor *) &xboxReport},
        {0x20d6, 0x2009, "PowerA Enhanced Wired Controller for Xbox Series X|S", (InputReportDescriptor *) &xboxReport},
        {0x20d6, 0x281f, "PowerA Wired Controller For Xbox 360",                 (InputReportDescriptor *) &xboxReport},
        {0x2e24, 0x0652, "Hyperkin Duke X-Box One pad",                          (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5000, "Razer Atrox Arcade Stick",                             (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5300, "PowerA MINI PROEX Controller",                         (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5303, "Xbox Airflo wired controller",                         (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x530a, "Xbox 360 Pro EX Controller",                           (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x531a, "PowerA Pro Ex",                                        (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5397, "FUS1ON Tournament Controller",                         (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x541a, "PowerA Xbox One Mini Wired Controller",                (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x542a, "Xbox ONE spectra",                                     (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x543a, "PowerA Xbox One wired controller",                     (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5500, "Hori XBOX 360 EX 2 with Turbo",                        (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5501, "Hori Real Arcade Pro VX-SA",                           (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5502, "Hori Fighting Stick VX Alt",                           (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5503, "Hori Fighting Edge",                                   (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5506, "Hori SOULCALIBUR V Stick",                             (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5510, "Hori Fighting Commander ONE (Xbox 360/PC Mode)",       (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x550d, "Hori GEM Xbox controller",                             (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x550e, "Hori Real Arcade Pro V Kai 360",                       (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x551a, "PowerA FUSION Pro Controller",                         (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x561a, "PowerA FUSION Controller",                             (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5b00, "ThrustMaster Ferrari 458 Racing Wheel",                (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5b02, "Thrustmaster, Inc. GPX Controller",                    (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5b03, "Thrustmaster Ferrari 458 Racing Wheel",                (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0x5d04, "Razer Sabertooth",                                     (InputReportDescriptor *) &xboxReport},
        {0x24c6, 0xfafe, "Rock Candy Gamepad for Xbox 360",                      (InputReportDescriptor *) &xboxReport},
        {0x2563, 0x058d, "OneXPlayer Gamepad",                                   (InputReportDescriptor *) &xboxReport},
        {0x2dc8, 0x2000, "8BitDo Pro 2 Wired Controller fox Xbox",               (InputReportDescriptor *) &xboxReport},
        {0x2dc8, 0x3106, "8BitDo Pro 2 Wired Controller",                        (InputReportDescriptor *) &xboxReport},
        {0x31e3, 0x1100, "Wooting One",                                          (InputReportDescriptor *) &xboxReport},
        {0x31e3, 0x1200, "Wooting Two",                                          (InputReportDescriptor *) &xboxReport},
        {0x31e3, 0x1210, "Wooting Lekker",                                       (InputReportDescriptor *) &xboxReport},
        {0x31e3, 0x1220, "Wooting Two HE",                                       (InputReportDescriptor *) &xboxReport},
        {0x31e3, 0x1300, "Wooting 60HE (AVR)",                                   (InputReportDescriptor *) &xboxReport},
        {0x31e3, 0x1310, "Wooting 60HE (ARM)",                                   (InputReportDescriptor *) &xboxReport},
        {0x3285, 0x0607, "Nacon GC-100",                                         (InputReportDescriptor *) &xboxReport},
        {0x3767, 0x0101, "Fanatec Speedster 3 Forceshock Wheel",                 (InputReportDescriptor *) &xboxReport},
        {0xffff, 0xffff, "Chinese-made Xbox Controller",                         (InputReportDescriptor *) &xboxReport},
        ///
        /// DS4
        ///
        {0x054c, 0x05c4, "DualShock 4",                                          (InputReportDescriptor *) &ds4Report},
        {0x054c, 0x09cc, "DualShock 4 (2nd Gen)",                                (InputReportDescriptor *) &ds4Report},
        {0x054c, 0x0ce6, "DualSense",                                            (InputReportDescriptor *) &ds5Report},
        ///
        {0xe6f,  0x1112, "SNK Neo-Geo Mini",                                     (InputReportDescriptor *) &ngMiniReport},
        ///
        {0x0079, 0x0011, "USB Gamepad (SNES)",                                   (InputReportDescriptor *) &cheapSnesReport},
        ///
        /// Unknown
        ///
        {0x0000, 0x0000, "Unknown device",                                       nullptr},
};

Device *get_device(uint16_t vid, uint16_t pid) {
    // lookup for a known game controller
    int i = 0;
    int p = INT32_MAX;

    while (p > 0) {
        p = devices[i].pid;
        if (p == pid && devices[i].vid == vid) {
            auto device = (Device *) malloc(sizeof(Device));
            memcpy(device, &devices[i], sizeof(Device));
            device->report = (InputReportDescriptor *) malloc(sizeof(InputReportDescriptor));
            memcpy(device->report, devices[i].report, sizeof(InputReportDescriptor));
            return device;
        }
        i++;
    }

    return nullptr;
}

Device *get_device_at(uint16_t i) {
    auto device = (Device *) malloc(sizeof(Device));
    memcpy(device, &devices[i], sizeof(Device));
    device->report = (InputReportDescriptor *) malloc(sizeof(InputReportDescriptor));
    memcpy(device->report, devices[i].report, sizeof(InputReportDescriptor));
    return device;
}
