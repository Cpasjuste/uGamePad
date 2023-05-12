//
// Created by cpasjuste on 22/03/23.
//

#ifdef NATIVE
#define PROGMEM
#else
#include <avr/pgmspace.h>
#endif

#include "devices.h"

using namespace uGamePad;

// xbox 360 data data
static constexpr ReportData xboxReport PROGMEM = {
        .buttons = {
                // byte, bit, controller button, output button
                3, 4,      // A         -> B1
                3, 5,      // B         -> B2
                3, 6,      // X         -> B3
                3, 7,      // Y         -> B4
                3, 0,      // LB        -> B5
                3, 1,      // LR        -> B6
                2, 4,      // START     -> START
                2, 5,      // BACK      -> SELECT
                2, 2,      // LEFT      -> LEFT
                2, 3,      // RIGHT     -> RIGHT
                2, 0,      // UP        -> UP
                2, 1,      // DOWN      -> DOWN
        },
        .axis = {
                // byte, axis type
                6, ReportData::AXIS_I16,  // AXIS
                8, ReportData::AXIS_I16,  // AXIS
                10, ReportData::AXIS_I16,  // AXIS
                12, ReportData::AXIS_I16,  // AXIS
        },
        .hat = {
                // byte, bit
                INDEX_NONE, BUTTON_NONE,  // HAT (none)
        },
        .init = {
                // set led
                {0x01, 0x03, 0x02}, 3
        }
};

// dual shock 4 data data
static constexpr ReportData ds4Report PROGMEM = {
        .buttons = {
                // byte, bit, controller button, output button
                5, 5,      // CROSS        -> B1
                5, 6,      // CIRCLE       -> B2
                5, 4,      // SQUARE       -> B3
                5, 7,      // TRIANGLE     -> B4
                6, 0,      // L1           -> B5
                6, 1,      // R1           -> B6
                6, 5,      // OPTIONS      -> START
                6, 4,      // SHARE        -> SELECT
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
        },
        .axis = {
                // byte, axis type
                1, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
                2, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
                3, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
                4, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
        },
        .hat = {
                // byte, bit
                5, 15, // HAT      -> U/D/L/R
        }
};

// dual shock 5 data data
static constexpr ReportData ds5Report PROGMEM = {
        .buttons = {
                // byte, bit, controller button, output button
                8, 5,      // CROSS        -> B1
                8, 6,      // CIRCLE       -> B2
                8, 4,      // SQUARE       -> B3
                8, 7,      // TRIANGLE     -> B4
                9, 0,      // L1           -> B5
                9, 1,      // R1           -> B6
                9, 5,      // OPTIONS      -> START
                9, 4,      // SHARE        -> SELECT
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
        },
        .axis = {
                // byte, axis type
                1, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
                2, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
                3, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
                4, ReportData::AXIS_UI8 | ReportData::AXIS_FLIP_Y,  // AXIS
        },
        .hat = {
                // byte, bit
                8, 15, // HAT      -> U/D/L/R
        }
};

// Neo-Geo Mini (usb-c) data data
static constexpr ReportData ngMiniReport PROGMEM = {
        .buttons = {
                // byte, value, controller button, output button
                0, 2,      // A        -> B1
                0, 1,      // B        -> B2
                0, 3,      // C        -> B3
                0, 0,      // D        -> B4
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                1, 1,      // START    -> START
                1, 0,      // SELECT   -> SELECT
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
        },
        .axis = {
                // byte, axis type
                INDEX_NONE, ReportData::AXIS_NONE,  // AXIS
                INDEX_NONE, ReportData::AXIS_NONE,  // AXIS
                INDEX_NONE, ReportData::AXIS_NONE,  // AXIS
                INDEX_NONE, ReportData::AXIS_NONE,  // AXIS
        },
        .hat = {
                // byte, value
                2, 0,      // HAT      -> U/D/L/R
        }
};

static constexpr Device devices[] PROGMEM = {
        ///
        /// XBOX
        ///
        {0x0079, 0x18d4, "GPD Win 2 X-Box Controller",                           (ReportData *) &xboxReport},
        {0x03eb, 0xff01, "Wooting One (Legacy)",                                 (ReportData *) &xboxReport},
        {0x03eb, 0xff02, "Wooting Two (Legacy)",                                 (ReportData *) &xboxReport},
        {0x044f, 0x0f00, "Thrustmaster Wheel",                                   (ReportData *) &xboxReport},
        {0x044f, 0x0f03, "Thrustmaster Wheel",                                   (ReportData *) &xboxReport},
        {0x044f, 0x0f07, "Thrustmaster, Inc. Controller",                        (ReportData *) &xboxReport},
        {0x044f, 0x0f10, "Thrustmaster Modena GT Wheel",                         (ReportData *) &xboxReport},
        {0x044f, 0xb326, "Thrustmaster Gamepad GP XID",                          (ReportData *) &xboxReport},
        {0x045e, 0x0202, "Microsoft X-Box pad v1 (US)",                          (ReportData *) &xboxReport},
        {0x045e, 0x0285, "Microsoft X-Box pad (Japan)",                          (ReportData *) &xboxReport},
        {0x045e, 0x0287, "Microsoft Xbox Controller S",                          (ReportData *) &xboxReport},
        {0x045e, 0x0288, "Microsoft Xbox Controller S v2",                       (ReportData *) &xboxReport},
        {0x045e, 0x0289, "Microsoft X-Box pad v2 (US)",                          (ReportData *) &xboxReport},
        {0x045e, 0x028e, "Microsoft X-Box 360 pad",                              (ReportData *) &xboxReport},
        {0x045e, 0x028f, "Microsoft X-Box 360 pad v2",                           (ReportData *) &xboxReport},
        {0x045e, 0x0291, "Xbox 360 Wireless Receiver (XBOX)",                    (ReportData *) &xboxReport},
        {0x045e, 0x02d1, "Microsoft X-Box One pad",                              (ReportData *) &xboxReport},
        {0x045e, 0x02dd, "Microsoft X-Box One pad (Firmware 2015)",              (ReportData *) &xboxReport},
        {0x045e, 0x02e3, "Microsoft X-Box One Elite pad",                        (ReportData *) &xboxReport},
        {0x045e, 0x0b00, "Microsoft X-Box One Elite 2 pad",                      (ReportData *) &xboxReport},
        {0x045e, 0x02ea, "Microsoft X-Box One S pad",                            (ReportData *) &xboxReport},
        {0x045e, 0x0719, "Xbox 360 Wireless Receiver",                           (ReportData *) &xboxReport},
        {0x045e, 0x0b0a, "Microsoft X-Box Adaptive Controller",                  (ReportData *) &xboxReport},
        {0x045e, 0x0b12, "Microsoft Xbox Series S|X Controller",                 (ReportData *) &xboxReport},
        {0x046d, 0xc21d, "Logitech Gamepad F310",                                (ReportData *) &xboxReport},
        {0x046d, 0xc21e, "Logitech Gamepad F510",                                (ReportData *) &xboxReport},
        {0x046d, 0xc21f, "Logitech Gamepad F710",                                (ReportData *) &xboxReport},
        {0x046d, 0xc242, "Logitech Chillstream Controller",                      (ReportData *) &xboxReport},
        {0x046d, 0xca84, "Logitech Xbox Cordless Controller",                    (ReportData *) &xboxReport},
        {0x046d, 0xca88, "Logitech Compact Controller for Xbox",                 (ReportData *) &xboxReport},
        {0x046d, 0xca8a, "Logitech Precision Vibration Feedback Wheel",          (ReportData *) &xboxReport},
        {0x046d, 0xcaa3, "Logitech DriveFx Racing Wheel",                        (ReportData *) &xboxReport},
        {0x056e, 0x2004, "Elecom JC-U3613M",                                     (ReportData *) &xboxReport},
        {0x05fd, 0x1007, "Mad Catz Controller (unverified)",                     (ReportData *) &xboxReport},
        {0x05fd, 0x107a, "InterAct 'PowerPad Pro' X-Box pad (Germany)",          (ReportData *) &xboxReport},
        {0x05fe, 0x3030, "Chic Controller",                                      (ReportData *) &xboxReport},
        {0x05fe, 0x3031, "Chic Controller",                                      (ReportData *) &xboxReport},
        {0x062a, 0x0020, "Logic3 Xbox GamePad",                                  (ReportData *) &xboxReport},
        {0x062a, 0x0033, "Competition Pro Steering Wheel",                       (ReportData *) &xboxReport},
        {0x06a3, 0x0200, "Saitek Racing Wheel",                                  (ReportData *) &xboxReport},
        {0x06a3, 0x0201, "Saitek Adrenalin",                                     (ReportData *) &xboxReport},
        {0x06a3, 0xf51a, "Saitek P3600",                                         (ReportData *) &xboxReport},
        {0x0738, 0x4506, "Mad Catz 4506 Wireless Controller",                    (ReportData *) &xboxReport},
        {0x0738, 0x4516, "Mad Catz Control Pad",                                 (ReportData *) &xboxReport},
        {0x0738, 0x4520, "Mad Catz Control Pad Pro",                             (ReportData *) &xboxReport},
        {0x0738, 0x4522, "Mad Catz LumiCON",                                     (ReportData *) &xboxReport},
        {0x0738, 0x4526, "Mad Catz Control Pad Pro",                             (ReportData *) &xboxReport},
        {0x0738, 0x4530, "Mad Catz Universal MC2 Racing Wheel and Pedals",       (ReportData *) &xboxReport},
        {0x0738, 0x4536, "Mad Catz MicroCON",                                    (ReportData *) &xboxReport},
        {0x0738, 0x4540, "Mad Catz Beat Pad",                                    (ReportData *) &xboxReport},
        {0x0738, 0x4556, "Mad Catz Lynx Wireless Controller",                    (ReportData *) &xboxReport},
        {0x0738, 0x4586, "Mad Catz MicroCon Wireless Controller",                (ReportData *) &xboxReport},
        {0x0738, 0x4588, "Mad Catz Blaster",                                     (ReportData *) &xboxReport},
        {0x0738, 0x45ff, "Mad Catz Beat Pad (w/ Handle)",                        (ReportData *) &xboxReport},
        {0x0738, 0x4716, "Mad Catz Wired Xbox 360 Controller",                   (ReportData *) &xboxReport},
        {0x0738, 0x4718, "Mad Catz Street Fighter IV FightStick SE",             (ReportData *) &xboxReport},
        {0x0738, 0x4726, "Mad Catz Xbox 360 Controller",                         (ReportData *) &xboxReport},
        {0x0738, 0x4728, "Mad Catz Street Fighter IV FightPad",                  (ReportData *) &xboxReport},
        {0x0738, 0x4736, "Mad Catz MicroCon Gamepad",                            (ReportData *) &xboxReport},
        {0x0738, 0x4738, "Mad Catz Wired Xbox 360 Controller (SFIV)",            (ReportData *) &xboxReport},
        {0x0738, 0x4740, "Mad Catz Beat Pad",                                    (ReportData *) &xboxReport},
        {0x0738, 0x4743, "Mad Catz Beat Pad Pro",                                (ReportData *) &xboxReport},
        {0x0738, 0x4758, "Mad Catz Arcade Game Stick",                           (ReportData *) &xboxReport},
        {0x0738, 0x4a01, "Mad Catz FightStick TE 2",                             (ReportData *) &xboxReport},
        {0x0738, 0x6040, "Mad Catz Beat Pad Pro",                                (ReportData *) &xboxReport},
        {0x0738, 0x9871, "Mad Catz Portable Drum",                               (ReportData *) &xboxReport},
        {0x0738, 0xb726, "Mad Catz Xbox controller - MW2",                       (ReportData *) &xboxReport},
        {0x0738, 0xb738, "Mad Catz MVC2TE Stick 2",                              (ReportData *) &xboxReport},
        {0x0738, 0xbeef, "Mad Catz JOYTECH NEO SE Advanced GamePad",             (ReportData *) &xboxReport},
        {0x0738, 0xcb02, "Saitek Cyborg Rumble Pad - PC/Xbox 360",               (ReportData *) &xboxReport},
        {0x0738, 0xcb03, "Saitek P3200 Rumble Pad - PC/Xbox 360",                (ReportData *) &xboxReport},
        {0x0738, 0xcb29, "Saitek Aviator Stick AV8R02",                          (ReportData *) &xboxReport},
        {0x0738, 0xf738, "Super SFIV FightStick TE S",                           (ReportData *) &xboxReport},
        {0x07ff, 0xffff, "Mad Catz GamePad",                                     (ReportData *) &xboxReport},
        {0x0c12, 0x0005, "Intec wireless",                                       (ReportData *) &xboxReport},
        {0x0c12, 0x8801, "Nyko Xbox Controller",                                 (ReportData *) &xboxReport},
        {0x0c12, 0x8802, "Zeroplus Xbox Controller",                             (ReportData *) &xboxReport},
        {0x0c12, 0x880a, "Pelican Eclipse PL-2023",                              (ReportData *) &xboxReport},
        {0x0c12, 0x8810, "Zeroplus Xbox Controller",                             (ReportData *) &xboxReport},
        {0x0c12, 0x9902, "HAMA VibraX - *FAULTY HARDWARE*",                      (ReportData *) &xboxReport},
        {0x0d2f, 0x0002, "Andamiro Pump It Up pad",                              (ReportData *) &xboxReport},
        {0x0e4c, 0x1097, "Radica Gamester Controller",                           (ReportData *) &xboxReport},
        {0x0e4c, 0x1103, "Radica Gamester Reflex",                               (ReportData *) &xboxReport},
        {0x0e4c, 0x2390, "Radica Games Jtech Controller",                        (ReportData *) &xboxReport},
        {0x0e4c, 0x3510, "Radica Gamester",                                      (ReportData *) &xboxReport},
        {0x0e6f, 0x0003, "Logic3 Freebird wireless Controller",                  (ReportData *) &xboxReport},
        {0x0e6f, 0x0005, "Eclipse wireless Controller",                          (ReportData *) &xboxReport},
        {0x0e6f, 0x0006, "Edge wireless Controller",                             (ReportData *) &xboxReport},
        {0x0e6f, 0x0008, "After Glow Pro Controller",                            (ReportData *) &xboxReport},
        {0x0e6f, 0x0105, "HSM3 Xbox360 dancepad",                                (ReportData *) &xboxReport},
        {0x0e6f, 0x0113, "Afterglow AX.1 Gamepad for Xbox 360",                  (ReportData *) &xboxReport},
        {0x0e6f, 0x011f, "Rock Candy Gamepad Wired Controller",                  (ReportData *) &xboxReport},
        {0x0e6f, 0x0131, "PDP EA Sports Controller",                             (ReportData *) &xboxReport},
        {0x0e6f, 0x0133, "Xbox 360 Wired Controller",                            (ReportData *) &xboxReport},
        {0x0e6f, 0x0139, "Afterglow Prismatic Wired Controller",                 (ReportData *) &xboxReport},
        {0x0e6f, 0x013a, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x0146, "Rock Candy Wired Controller for Xbox One",             (ReportData *) &xboxReport},
        {0x0e6f, 0x0147, "PDP Marvel Xbox One Controller",                       (ReportData *) &xboxReport},
        {0x0e6f, 0x015c, "PDP Xbox One Arcade Stick",                            (ReportData *) &xboxReport},
        {0x0e6f, 0x0161, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x0162, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x0163, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x0164, "PDP Battlefield One",                                  (ReportData *) &xboxReport},
        {0x0e6f, 0x0165, "PDP Titanfall 2",                                      (ReportData *) &xboxReport},
        {0x0e6f, 0x0201, "Pelican PL-3601 'TSZ' Wired Xbox 360 Controller",      (ReportData *) &xboxReport},
        {0x0e6f, 0x0213, "Afterglow Gamepad for Xbox 360",                       (ReportData *) &xboxReport},
        {0x0e6f, 0x021f, "Rock Candy Gamepad for Xbox 360",                      (ReportData *) &xboxReport},
        {0x0e6f, 0x0246, "Rock Candy Gamepad for Xbox One 2015",                 (ReportData *) &xboxReport},
        {0x0e6f, 0x02a0, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x02a1, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x02a2, "PDP Wired Controller for Xbox One - Crimson Red",      (ReportData *) &xboxReport},
        {0x0e6f, 0x02a4, "PDP Wired Controller for Xbox One - Stealth Series",   (ReportData *) &xboxReport},
        {0x0e6f, 0x02a6, "PDP Wired Controller for Xbox One - Camo Series",      (ReportData *) &xboxReport},
        {0x0e6f, 0x02a7, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x02a8, "PDP Xbox One Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0x02ab, "PDP Controller for Xbox One",                          (ReportData *) &xboxReport},
        {0x0e6f, 0x02ad, "PDP Wired Controller for Xbox One - Stealth Series",   (ReportData *) &xboxReport},
        {0x0e6f, 0x02b3, "Afterglow Prismatic Wired Controller",                 (ReportData *) &xboxReport},
        {0x0e6f, 0x02b8, "Afterglow Prismatic Wired Controller",                 (ReportData *) &xboxReport},
        {0x0e6f, 0x0301, "Logic3 Controller",                                    (ReportData *) &xboxReport},
        {0x0e6f, 0x0346, "Rock Candy Gamepad for Xbox One 2016",                 (ReportData *) &xboxReport},
        {0x0e6f, 0x0401, "Logic3 Controller",                                    (ReportData *) &xboxReport},
        {0x0e6f, 0x0413, "Afterglow AX.1 Gamepad for Xbox 360",                  (ReportData *) &xboxReport},
        {0x0e6f, 0x0501, "PDP Xbox 360 Controller",                              (ReportData *) &xboxReport},
        {0x0e6f, 0xf900, "PDP Afterglow AX.1",                                   (ReportData *) &xboxReport},
        {0x0e8f, 0x0201, "SmartJoy Frag Xpad/PS2 adaptor",                       (ReportData *) &xboxReport},
        {0x0e8f, 0x3008, "Generic xbox control (dealextreme)",                   (ReportData *) &xboxReport},
        {0x0f0d, 0x000a, "Hori Co. DOA4 FightStick",                             (ReportData *) &xboxReport},
        {0x0f0d, 0x000c, "Hori PadEX Turbo",                                     (ReportData *) &xboxReport},
        {0x0f0d, 0x000d, "Hori Fighting Stick EX2",                              (ReportData *) &xboxReport},
        {0x0f0d, 0x0016, "Hori Real Arcade Pro.EX",                              (ReportData *) &xboxReport},
        {0x0f0d, 0x001b, "Hori Real Arcade Pro VX",                              (ReportData *) &xboxReport},
        {0x0f0d, 0x0063, "Hori Real Arcade Pro Hayabusa (USA) Xbox One",         (ReportData *) &xboxReport},
        {0x0f0d, 0x0067, "HORIPAD ONE",                                          (ReportData *) &xboxReport},
        {0x0f0d, 0x0078, "Hori Real Arcade Pro V Kai Xbox One",                  (ReportData *) &xboxReport},
        {0x0f0d, 0x00c5, "Hori Fighting Commander ONE",                          (ReportData *) &xboxReport},
        {0x0f30, 0x010b, "Philips Recoil",                                       (ReportData *) &xboxReport},
        {0x0f30, 0x0202, "Joytech Advanced Controller",                          (ReportData *) &xboxReport},
        {0x0f30, 0x8888, "BigBen XBMiniPad Controller",                          (ReportData *) &xboxReport},
        {0x102c, 0xff0c, "Joytech Wireless Advanced Controller",                 (ReportData *) &xboxReport},
        {0x1038, 0x1430, "SteelSeries Stratus Duo",                              (ReportData *) &xboxReport},
        {0x1038, 0x1431, "SteelSeries Stratus Duo",                              (ReportData *) &xboxReport},
        {0x11c9, 0x55f0, "Nacon GC-100XF",                                       (ReportData *) &xboxReport},
        {0x1209, 0x2882, "Ardwiino Controller",                                  (ReportData *) &xboxReport},
        {0x12ab, 0x0004, "Honey Bee Xbox360 dancepad",                           (ReportData *) &xboxReport},
        {0x12ab, 0x0301, "PDP AFTERGLOW AX.1",                                   (ReportData *) &xboxReport},
        {0x12ab, 0x0303, "Mortal Kombat Klassic FightStick",                     (ReportData *) &xboxReport},
        {0x12ab, 0x8809, "Xbox DDR dancepad",                                    (ReportData *) &xboxReport},
        {0x1430, 0x4748, "RedOctane Guitar Hero X-plorer",                       (ReportData *) &xboxReport},
        {0x1430, 0x8888, "TX6500+ Dance Pad (first generation)",                 (ReportData *) &xboxReport},
        {0x1430, 0xf801, "RedOctane Controller",                                 (ReportData *) &xboxReport},
        {0x146b, 0x0601, "BigBen Interactive XBOX 360 Controller",               (ReportData *) &xboxReport},
        {0x146b, 0x0604, "Bigben Interactive DAIJA Arcade Stick",                (ReportData *) &xboxReport},
        {0x1532, 0x0037, "Razer Sabertooth",                                     (ReportData *) &xboxReport},
        {0x1532, 0x0a00, "Razer Atrox Arcade Stick",                             (ReportData *) &xboxReport},
        {0x1532, 0x0a03, "Razer Wildcat",                                        (ReportData *) &xboxReport},
        {0x15e4, 0x3f00, "Power A Mini Pro Elite",                               (ReportData *) &xboxReport},
        {0x15e4, 0x3f0a, "Xbox Airflo wired controller",                         (ReportData *) &xboxReport},
        {0x15e4, 0x3f10, "Batarang Xbox 360 controller",                         (ReportData *) &xboxReport},
        {0x162e, 0xbeef, "Joytech Neo-Se Take2",                                 (ReportData *) &xboxReport},
        {0x1689, 0xfd00, "Razer Onza Tournament Edition",                        (ReportData *) &xboxReport},
        {0x1689, 0xfd01, "Razer Onza Classic Edition",                           (ReportData *) &xboxReport},
        {0x1689, 0xfe00, "Razer Sabertooth",                                     (ReportData *) &xboxReport},
        {0x1949, 0x041a, "Amazon Game Controller",                               (ReportData *) &xboxReport},
        {0x1bad, 0x0002, "Harmonix Rock Band Guitar",                            (ReportData *) &xboxReport},
        {0x1bad, 0x0003, "Harmonix Rock Band Drumkit",                           (ReportData *) &xboxReport},
        {0x1bad, 0x0130, "Ion Drum Rocker",                                      (ReportData *) &xboxReport},
        {0x1bad, 0xf016, "Mad Catz Xbox 360 Controller",                         (ReportData *) &xboxReport},
        {0x1bad, 0xf018, "Mad Catz Street Fighter IV SE Fighting Stick",         (ReportData *) &xboxReport},
        {0x1bad, 0xf019, "Mad Catz Brawlstick for Xbox 360",                     (ReportData *) &xboxReport},
        {0x1bad, 0xf021, "Mad Cats Ghost Recon FS GamePad",                      (ReportData *) &xboxReport},
        {0x1bad, 0xf023, "MLG Pro Circuit Controller (Xbox)",                    (ReportData *) &xboxReport},
        {0x1bad, 0xf025, "Mad Catz Call Of Duty",                                (ReportData *) &xboxReport},
        {0x1bad, 0xf027, "Mad Catz FPS Pro",                                     (ReportData *) &xboxReport},
        {0x1bad, 0xf028, "Street Fighter IV FightPad",                           (ReportData *) &xboxReport},
        {0x1bad, 0xf02e, "Mad Catz Fightpad",                                    (ReportData *) &xboxReport},
        {0x1bad, 0xf030, "Mad Catz Xbox 360 MC2 MicroCon Racing Wheel",          (ReportData *) &xboxReport},
        {0x1bad, 0xf036, "Mad Catz MicroCon GamePad Pro",                        (ReportData *) &xboxReport},
        {0x1bad, 0xf038, "Street Fighter IV FightStick TE",                      (ReportData *) &xboxReport},
        {0x1bad, 0xf039, "Mad Catz MvC2 TE",                                     (ReportData *) &xboxReport},
        {0x1bad, 0xf03a, "Mad Catz SFxT Fightstick Pro",                         (ReportData *) &xboxReport},
        {0x1bad, 0xf03d, "Street Fighter IV Arcade Stick TE - Chun Li",          (ReportData *) &xboxReport},
        {0x1bad, 0xf03e, "Mad Catz MLG FightStick TE",                           (ReportData *) &xboxReport},
        {0x1bad, 0xf03f, "Mad Catz FightStick SoulCaliber",                      (ReportData *) &xboxReport},
        {0x1bad, 0xf042, "Mad Catz FightStick TES+",                             (ReportData *) &xboxReport},
        {0x1bad, 0xf080, "Mad Catz FightStick TE2",                              (ReportData *) &xboxReport},
        {0x1bad, 0xf501, "HoriPad EX2 Turbo",                                    (ReportData *) &xboxReport},
        {0x1bad, 0xf502, "Hori Real Arcade Pro.VX SA",                           (ReportData *) &xboxReport},
        {0x1bad, 0xf503, "Hori Fighting Stick VX",                               (ReportData *) &xboxReport},
        {0x1bad, 0xf504, "Hori Real Arcade Pro. EX",                             (ReportData *) &xboxReport},
        {0x1bad, 0xf505, "Hori Fighting Stick EX2B",                             (ReportData *) &xboxReport},
        {0x1bad, 0xf506, "Hori Real Arcade Pro.EX Premium VLX",                  (ReportData *) &xboxReport},
        {0x1bad, 0xf900, "Harmonix Xbox 360 Controller",                         (ReportData *) &xboxReport},
        {0x1bad, 0xf901, "Gamestop Xbox 360 Controller",                         (ReportData *) &xboxReport},
        {0x1bad, 0xf903, "Tron Xbox 360 controller",                             (ReportData *) &xboxReport},
        {0x1bad, 0xf904, "PDP Versus Fighting Pad",                              (ReportData *) &xboxReport},
        {0x1bad, 0xf906, "MortalKombat FightStick",                              (ReportData *) &xboxReport},
        {0x1bad, 0xfa01, "MadCatz GamePad",                                      (ReportData *) &xboxReport},
        {0x1bad, 0xfd00, "Razer Onza TE",                                        (ReportData *) &xboxReport},
        {0x1bad, 0xfd01, "Razer Onza",                                           (ReportData *) &xboxReport},
        {0x20d6, 0x2001, "BDA Xbox Series X Wired Controller",                   (ReportData *) &xboxReport},
        {0x20d6, 0x2009, "PowerA Enhanced Wired Controller for Xbox Series X|S", (ReportData *) &xboxReport},
        {0x20d6, 0x281f, "PowerA Wired Controller For Xbox 360",                 (ReportData *) &xboxReport},
        {0x2e24, 0x0652, "Hyperkin Duke X-Box One pad",                          (ReportData *) &xboxReport},
        {0x24c6, 0x5000, "Razer Atrox Arcade Stick",                             (ReportData *) &xboxReport},
        {0x24c6, 0x5300, "PowerA MINI PROEX Controller",                         (ReportData *) &xboxReport},
        {0x24c6, 0x5303, "Xbox Airflo wired controller",                         (ReportData *) &xboxReport},
        {0x24c6, 0x530a, "Xbox 360 Pro EX Controller",                           (ReportData *) &xboxReport},
        {0x24c6, 0x531a, "PowerA Pro Ex",                                        (ReportData *) &xboxReport},
        {0x24c6, 0x5397, "FUS1ON Tournament Controller",                         (ReportData *) &xboxReport},
        {0x24c6, 0x541a, "PowerA Xbox One Mini Wired Controller",                (ReportData *) &xboxReport},
        {0x24c6, 0x542a, "Xbox ONE spectra",                                     (ReportData *) &xboxReport},
        {0x24c6, 0x543a, "PowerA Xbox One wired controller",                     (ReportData *) &xboxReport},
        {0x24c6, 0x5500, "Hori XBOX 360 EX 2 with Turbo",                        (ReportData *) &xboxReport},
        {0x24c6, 0x5501, "Hori Real Arcade Pro VX-SA",                           (ReportData *) &xboxReport},
        {0x24c6, 0x5502, "Hori Fighting Stick VX Alt",                           (ReportData *) &xboxReport},
        {0x24c6, 0x5503, "Hori Fighting Edge",                                   (ReportData *) &xboxReport},
        {0x24c6, 0x5506, "Hori SOULCALIBUR V Stick",                             (ReportData *) &xboxReport},
        {0x24c6, 0x5510, "Hori Fighting Commander ONE (Xbox 360/PC Mode)",       (ReportData *) &xboxReport},
        {0x24c6, 0x550d, "Hori GEM Xbox controller",                             (ReportData *) &xboxReport},
        {0x24c6, 0x550e, "Hori Real Arcade Pro V Kai 360",                       (ReportData *) &xboxReport},
        {0x24c6, 0x551a, "PowerA FUSION Pro Controller",                         (ReportData *) &xboxReport},
        {0x24c6, 0x561a, "PowerA FUSION Controller",                             (ReportData *) &xboxReport},
        {0x24c6, 0x5b00, "ThrustMaster Ferrari 458 Racing Wheel",                (ReportData *) &xboxReport},
        {0x24c6, 0x5b02, "Thrustmaster, Inc. GPX Controller",                    (ReportData *) &xboxReport},
        {0x24c6, 0x5b03, "Thrustmaster Ferrari 458 Racing Wheel",                (ReportData *) &xboxReport},
        {0x24c6, 0x5d04, "Razer Sabertooth",                                     (ReportData *) &xboxReport},
        {0x24c6, 0xfafe, "Rock Candy Gamepad for Xbox 360",                      (ReportData *) &xboxReport},
        {0x2563, 0x058d, "OneXPlayer Gamepad",                                   (ReportData *) &xboxReport},
        {0x2dc8, 0x2000, "8BitDo Pro 2 Wired Controller fox Xbox",               (ReportData *) &xboxReport},
        {0x2dc8, 0x3106, "8BitDo Pro 2 Wired Controller",                        (ReportData *) &xboxReport},
        {0x31e3, 0x1100, "Wooting One",                                          (ReportData *) &xboxReport},
        {0x31e3, 0x1200, "Wooting Two",                                          (ReportData *) &xboxReport},
        {0x31e3, 0x1210, "Wooting Lekker",                                       (ReportData *) &xboxReport},
        {0x31e3, 0x1220, "Wooting Two HE",                                       (ReportData *) &xboxReport},
        {0x31e3, 0x1300, "Wooting 60HE (AVR)",                                   (ReportData *) &xboxReport},
        {0x31e3, 0x1310, "Wooting 60HE (ARM)",                                   (ReportData *) &xboxReport},
        {0x3285, 0x0607, "Nacon GC-100",                                         (ReportData *) &xboxReport},
        {0x3767, 0x0101, "Fanatec Speedster 3 Forceshock Wheel",                 (ReportData *) &xboxReport},
        {0xffff, 0xffff, "Chinese-made Xbox Controller",                         (ReportData *) &xboxReport},
        ///
        /// DS4
        ///
        {0x054c, 0x05c4, "DualShock 4",                                          (ReportData *) &ds4Report},
        {0x054c, 0x09cc, "DualShock 4 (2nd Gen)",                                (ReportData *) &ds4Report},
        {0x054c, 0x0ce6, "DualSense",                                            (ReportData *) &ds5Report},
        ///
        {0xe6f,  0x1112, "SNK Neo-Geo Mini",                                     (ReportData *) &ngMiniReport},
        ///
        /// Unknown
        ///
        {0x0000, 0x0000, "Unknown device",                                       nullptr},
};

const Device *get_device(uint16_t vid, uint16_t pid) {
    // lookup for a known game controller
    int i = 0;
    int p = INT32_MAX;

    while (p > 0) {
        p = devices[i].product;
        if (p == pid && devices[i].vendor == vid) {
            return &devices[i];
        }
        i++;
    }

    return nullptr;
}

const Device *get_device_at(uint16_t i) {
    return &devices[i];
}