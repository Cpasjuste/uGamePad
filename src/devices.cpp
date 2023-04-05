//
// Created by cpasjuste on 22/03/23.
//

#include "devices.h"

using namespace uGamePad;

// xbox 360 report data
static constexpr Report xboxReport PROGMEM = {
        .buttons = {
                // byte, index, controller button, output button
                3, BIT(4),      // A         -> B1
                3, BIT(5),      // B         -> B2
                3, BIT(6),      // X         -> B3
                3, BIT(7),      // Y         -> B4
                3, BIT(0),      // LB        -> B5
                3, BIT(1),      // LR        -> B6
                2, BIT(4),      // START     -> START
                2, BIT(5),      // BACK      -> SELECT
                2, BIT(2),      // LEFT      -> LEFT
                2, BIT(3),      // RIGHT     -> RIGHT
                2, BIT(0),      // UP        -> UP
                2, BIT(1),      // DOWN      -> DOWN
        },
        .axis = {
                // byte, axis type
                6, Report::AXIS_I16,  // AXIS
                8, Report::AXIS_I16,  // AXIS
                10, Report::AXIS_I16,  // AXIS
                12, Report::AXIS_I16,  // AXIS
        },
        .hat = {
                // byte, index
                INDEX_NONE, BUTTON_NONE,  // HAT (none)
        },
        .init = {
                // set led
                {0x01, 0x03, 0x02}, 3
        }
};

// dual shock 4 report data
static constexpr Report ds4Report PROGMEM = {
        .buttons = {
                // byte, index, controller button, output button
                5, BIT(5),      // CROSS        -> B1
                5, BIT(6),      // CIRCLE       -> B2
                5, BIT(4),      // SQUARE       -> B3
                5, BIT(7),      // TRIANGLE     -> B4
                6, BIT(0),      // L1           -> B5
                6, BIT(1),      // R1           -> B6
                6, BIT(5),      // OPTIONS      -> START
                6, BIT(4),      // SHARE        -> SELECT
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
        },
        .axis = {
                // byte, axis type
                1, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
                2, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
                3, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
                4, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
        },
        .hat = {
                // byte, index
                5, BIT(15),  // HAT (none)
        }
};

// dual shock 5 report data
static constexpr Report ds5Report PROGMEM = {
        .buttons = {
                // byte, index, controller button, output button
                8, BIT(5),      // CROSS        -> B1
                8, BIT(6),      // CIRCLE       -> B2
                8, BIT(4),      // SQUARE       -> B3
                8, BIT(7),      // TRIANGLE     -> B4
                9, BIT(0),      // L1           -> B5
                9, BIT(1),      // R1           -> B6
                9, BIT(5),      // OPTIONS      -> START
                9, BIT(4),      // SHARE        -> SELECT
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
        },
        .axis = {
                // byte, axis type
                1, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
                2, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
                3, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
                4, Report::AXIS_UI8 | Report::AXIS_FLIP_Y,  // AXIS
        },
        .hat = {
                // byte, index
                8, BIT(15),  // HAT (none)
        }
};

// Neo-Geo Mini (usb-c) report data
static constexpr Report ngMiniReport PROGMEM = {
        .buttons = {
                // byte, index, controller button, output button
                0, BIT(2),      // A        -> B1
                0, BIT(1),      // B        -> B2
                0, BIT(3),      // C        -> B3
                0, BIT(0),      // D        -> B4
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                1, BIT(1),      // START    -> START
                1, BIT(0),      // SELECT   -> SELECT
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
                INDEX_NONE, BUTTON_NONE,
        },
        .axis = {
                // byte, axis type
                INDEX_NONE, Report::AXIS_NONE,  // AXIS
                INDEX_NONE, Report::AXIS_NONE,  // AXIS
                INDEX_NONE, Report::AXIS_NONE,  // AXIS
                INDEX_NONE, Report::AXIS_NONE,  // AXIS
        },
        .hat = {
                // byte, index
                2, BIT(0),      // HAT      -> U/D/L/R
        }
};

static constexpr Device devices[] PROGMEM = {
        ///
        /// XBOX
        ///
        {0x0079, 0x18d4, (char *) "GPD Win 2 X-Box Controller",                           (Report *) &xboxReport},
        {0x03eb, 0xff01, (char *) "Wooting One (Legacy)",                                 (Report *) &xboxReport},
        {0x03eb, 0xff02, (char *) "Wooting Two (Legacy)",                                 (Report *) &xboxReport},
        {0x044f, 0x0f00, (char *) "Thrustmaster Wheel",                                   (Report *) &xboxReport},
        {0x044f, 0x0f03, (char *) "Thrustmaster Wheel",                                   (Report *) &xboxReport},
        {0x044f, 0x0f07, (char *) "Thrustmaster, Inc. Controller",                        (Report *) &xboxReport},
        {0x044f, 0x0f10, (char *) "Thrustmaster Modena GT Wheel",                         (Report *) &xboxReport},
        {0x044f, 0xb326, (char *) "Thrustmaster Gamepad GP XID",                          (Report *) &xboxReport},
        {0x045e, 0x0202, (char *) "Microsoft X-Box pad v1 (US)",                          (Report *) &xboxReport},
        {0x045e, 0x0285, (char *) "Microsoft X-Box pad (Japan)",                          (Report *) &xboxReport},
        {0x045e, 0x0287, (char *) "Microsoft Xbox Controller S",                          (Report *) &xboxReport},
        {0x045e, 0x0288, (char *) "Microsoft Xbox Controller S v2",                       (Report *) &xboxReport},
        {0x045e, 0x0289, (char *) "Microsoft X-Box pad v2 (US)",                          (Report *) &xboxReport},
        {0x045e, 0x028e, (char *) "Microsoft X-Box 360 pad",                              (Report *) &xboxReport},
        {0x045e, 0x028f, (char *) "Microsoft X-Box 360 pad v2",                           (Report *) &xboxReport},
        {0x045e, 0x0291, (char *) "Xbox 360 Wireless Receiver (XBOX)",                    (Report *) &xboxReport},
        {0x045e, 0x02d1, (char *) "Microsoft X-Box One pad",                              (Report *) &xboxReport},
        {0x045e, 0x02dd, (char *) "Microsoft X-Box One pad (Firmware 2015)",              (Report *) &xboxReport},
        {0x045e, 0x02e3, (char *) "Microsoft X-Box One Elite pad",                        (Report *) &xboxReport},
        {0x045e, 0x0b00, (char *) "Microsoft X-Box One Elite 2 pad",                      (Report *) &xboxReport},
        {0x045e, 0x02ea, (char *) "Microsoft X-Box One S pad",                            (Report *) &xboxReport},
        {0x045e, 0x0719, (char *) "Xbox 360 Wireless Receiver",                           (Report *) &xboxReport},
        {0x045e, 0x0b0a, (char *) "Microsoft X-Box Adaptive Controller",                  (Report *) &xboxReport},
        {0x045e, 0x0b12, (char *) "Microsoft Xbox Series S|X Controller",                 (Report *) &xboxReport},
        {0x046d, 0xc21d, (char *) "Logitech Gamepad F310",                                (Report *) &xboxReport},
        {0x046d, 0xc21e, (char *) "Logitech Gamepad F510",                                (Report *) &xboxReport},
        {0x046d, 0xc21f, (char *) "Logitech Gamepad F710",                                (Report *) &xboxReport},
        {0x046d, 0xc242, (char *) "Logitech Chillstream Controller",                      (Report *) &xboxReport},
        {0x046d, 0xca84, (char *) "Logitech Xbox Cordless Controller",                    (Report *) &xboxReport},
        {0x046d, 0xca88, (char *) "Logitech Compact Controller for Xbox",                 (Report *) &xboxReport},
        {0x046d, 0xca8a, (char *) "Logitech Precision Vibration Feedback Wheel",          (Report *) &xboxReport},
        {0x046d, 0xcaa3, (char *) "Logitech DriveFx Racing Wheel",                        (Report *) &xboxReport},
        {0x056e, 0x2004, (char *) "Elecom JC-U3613M",                                     (Report *) &xboxReport},
        {0x05fd, 0x1007, (char *) "Mad Catz Controller (unverified)",                     (Report *) &xboxReport},
        {0x05fd, 0x107a, (char *) "InterAct 'PowerPad Pro' X-Box pad (Germany)",          (Report *) &xboxReport},
        {0x05fe, 0x3030, (char *) "Chic Controller",                                      (Report *) &xboxReport},
        {0x05fe, 0x3031, (char *) "Chic Controller",                                      (Report *) &xboxReport},
        {0x062a, 0x0020, (char *) "Logic3 Xbox GamePad",                                  (Report *) &xboxReport},
        {0x062a, 0x0033, (char *) "Competition Pro Steering Wheel",                       (Report *) &xboxReport},
        {0x06a3, 0x0200, (char *) "Saitek Racing Wheel",                                  (Report *) &xboxReport},
        {0x06a3, 0x0201, (char *) "Saitek Adrenalin",                                     (Report *) &xboxReport},
        {0x06a3, 0xf51a, (char *) "Saitek P3600",                                         (Report *) &xboxReport},
        {0x0738, 0x4506, (char *) "Mad Catz 4506 Wireless Controller",                    (Report *) &xboxReport},
        {0x0738, 0x4516, (char *) "Mad Catz Control Pad",                                 (Report *) &xboxReport},
        {0x0738, 0x4520, (char *) "Mad Catz Control Pad Pro",                             (Report *) &xboxReport},
        {0x0738, 0x4522, (char *) "Mad Catz LumiCON",                                     (Report *) &xboxReport},
        {0x0738, 0x4526, (char *) "Mad Catz Control Pad Pro",                             (Report *) &xboxReport},
        {0x0738, 0x4530, (char *) "Mad Catz Universal MC2 Racing Wheel and Pedals",       (Report *) &xboxReport},
        {0x0738, 0x4536, (char *) "Mad Catz MicroCON",                                    (Report *) &xboxReport},
        {0x0738, 0x4540, (char *) "Mad Catz Beat Pad",                                    (Report *) &xboxReport},
        {0x0738, 0x4556, (char *) "Mad Catz Lynx Wireless Controller",                    (Report *) &xboxReport},
        {0x0738, 0x4586, (char *) "Mad Catz MicroCon Wireless Controller",                (Report *) &xboxReport},
        {0x0738, 0x4588, (char *) "Mad Catz Blaster",                                     (Report *) &xboxReport},
        {0x0738, 0x45ff, (char *) "Mad Catz Beat Pad (w/ Handle)",                        (Report *) &xboxReport},
        {0x0738, 0x4716, (char *) "Mad Catz Wired Xbox 360 Controller",                   (Report *) &xboxReport},
        {0x0738, 0x4718, (char *) "Mad Catz Street Fighter IV FightStick SE",             (Report *) &xboxReport},
        {0x0738, 0x4726, (char *) "Mad Catz Xbox 360 Controller",                         (Report *) &xboxReport},
        {0x0738, 0x4728, (char *) "Mad Catz Street Fighter IV FightPad",                  (Report *) &xboxReport},
        {0x0738, 0x4736, (char *) "Mad Catz MicroCon Gamepad",                            (Report *) &xboxReport},
        {0x0738, 0x4738, (char *) "Mad Catz Wired Xbox 360 Controller (SFIV)",            (Report *) &xboxReport},
        {0x0738, 0x4740, (char *) "Mad Catz Beat Pad",                                    (Report *) &xboxReport},
        {0x0738, 0x4743, (char *) "Mad Catz Beat Pad Pro",                                (Report *) &xboxReport},
        {0x0738, 0x4758, (char *) "Mad Catz Arcade Game Stick",                           (Report *) &xboxReport},
        {0x0738, 0x4a01, (char *) "Mad Catz FightStick TE 2",                             (Report *) &xboxReport},
        {0x0738, 0x6040, (char *) "Mad Catz Beat Pad Pro",                                (Report *) &xboxReport},
        {0x0738, 0x9871, (char *) "Mad Catz Portable Drum",                               (Report *) &xboxReport},
        {0x0738, 0xb726, (char *) "Mad Catz Xbox controller - MW2",                       (Report *) &xboxReport},
        {0x0738, 0xb738, (char *) "Mad Catz MVC2TE Stick 2",                              (Report *) &xboxReport},
        {0x0738, 0xbeef, (char *) "Mad Catz JOYTECH NEO SE Advanced GamePad",             (Report *) &xboxReport},
        {0x0738, 0xcb02, (char *) "Saitek Cyborg Rumble Pad - PC/Xbox 360",               (Report *) &xboxReport},
        {0x0738, 0xcb03, (char *) "Saitek P3200 Rumble Pad - PC/Xbox 360",                (Report *) &xboxReport},
        {0x0738, 0xcb29, (char *) "Saitek Aviator Stick AV8R02",                          (Report *) &xboxReport},
        {0x0738, 0xf738, (char *) "Super SFIV FightStick TE S",                           (Report *) &xboxReport},
        {0x07ff, 0xffff, (char *) "Mad Catz GamePad",                                     (Report *) &xboxReport},
        {0x0c12, 0x0005, (char *) "Intec wireless",                                       (Report *) &xboxReport},
        {0x0c12, 0x8801, (char *) "Nyko Xbox Controller",                                 (Report *) &xboxReport},
        {0x0c12, 0x8802, (char *) "Zeroplus Xbox Controller",                             (Report *) &xboxReport},
        {0x0c12, 0x880a, (char *) "Pelican Eclipse PL-2023",                              (Report *) &xboxReport},
        {0x0c12, 0x8810, (char *) "Zeroplus Xbox Controller",                             (Report *) &xboxReport},
        {0x0c12, 0x9902, (char *) "HAMA VibraX - *FAULTY HARDWARE*",                      (Report *) &xboxReport},
        {0x0d2f, 0x0002, (char *) "Andamiro Pump It Up pad",                              (Report *) &xboxReport},
        {0x0e4c, 0x1097, (char *) "Radica Gamester Controller",                           (Report *) &xboxReport},
        {0x0e4c, 0x1103, (char *) "Radica Gamester Reflex",                               (Report *) &xboxReport},
        {0x0e4c, 0x2390, (char *) "Radica Games Jtech Controller",                        (Report *) &xboxReport},
        {0x0e4c, 0x3510, (char *) "Radica Gamester",                                      (Report *) &xboxReport},
        {0x0e6f, 0x0003, (char *) "Logic3 Freebird wireless Controller",                  (Report *) &xboxReport},
        {0x0e6f, 0x0005, (char *) "Eclipse wireless Controller",                          (Report *) &xboxReport},
        {0x0e6f, 0x0006, (char *) "Edge wireless Controller",                             (Report *) &xboxReport},
        {0x0e6f, 0x0008, (char *) "After Glow Pro Controller",                            (Report *) &xboxReport},
        {0x0e6f, 0x0105, (char *) "HSM3 Xbox360 dancepad",                                (Report *) &xboxReport},
        {0x0e6f, 0x0113, (char *) "Afterglow AX.1 Gamepad for Xbox 360",                  (Report *) &xboxReport},
        {0x0e6f, 0x011f, (char *) "Rock Candy Gamepad Wired Controller",                  (Report *) &xboxReport},
        {0x0e6f, 0x0131, (char *) "PDP EA Sports Controller",                             (Report *) &xboxReport},
        {0x0e6f, 0x0133, (char *) "Xbox 360 Wired Controller",                            (Report *) &xboxReport},
        {0x0e6f, 0x0139, (char *) "Afterglow Prismatic Wired Controller",                 (Report *) &xboxReport},
        {0x0e6f, 0x013a, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x0146, (char *) "Rock Candy Wired Controller for Xbox One",             (Report *) &xboxReport},
        {0x0e6f, 0x0147, (char *) "PDP Marvel Xbox One Controller",                       (Report *) &xboxReport},
        {0x0e6f, 0x015c, (char *) "PDP Xbox One Arcade Stick",                            (Report *) &xboxReport},
        {0x0e6f, 0x0161, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x0162, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x0163, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x0164, (char *) "PDP Battlefield One",                                  (Report *) &xboxReport},
        {0x0e6f, 0x0165, (char *) "PDP Titanfall 2",                                      (Report *) &xboxReport},
        {0x0e6f, 0x0201, (char *) "Pelican PL-3601 'TSZ' Wired Xbox 360 Controller",      (Report *) &xboxReport},
        {0x0e6f, 0x0213, (char *) "Afterglow Gamepad for Xbox 360",                       (Report *) &xboxReport},
        {0x0e6f, 0x021f, (char *) "Rock Candy Gamepad for Xbox 360",                      (Report *) &xboxReport},
        {0x0e6f, 0x0246, (char *) "Rock Candy Gamepad for Xbox One 2015",                 (Report *) &xboxReport},
        {0x0e6f, 0x02a0, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x02a1, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x02a2, (char *) "PDP Wired Controller for Xbox One - Crimson Red",      (Report *) &xboxReport},
        {0x0e6f, 0x02a4, (char *) "PDP Wired Controller for Xbox One - Stealth Series",   (Report *) &xboxReport},
        {0x0e6f, 0x02a6, (char *) "PDP Wired Controller for Xbox One - Camo Series",      (Report *) &xboxReport},
        {0x0e6f, 0x02a7, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x02a8, (char *) "PDP Xbox One Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0x02ab, (char *) "PDP Controller for Xbox One",                          (Report *) &xboxReport},
        {0x0e6f, 0x02ad, (char *) "PDP Wired Controller for Xbox One - Stealth Series",   (Report *) &xboxReport},
        {0x0e6f, 0x02b3, (char *) "Afterglow Prismatic Wired Controller",                 (Report *) &xboxReport},
        {0x0e6f, 0x02b8, (char *) "Afterglow Prismatic Wired Controller",                 (Report *) &xboxReport},
        {0x0e6f, 0x0301, (char *) "Logic3 Controller",                                    (Report *) &xboxReport},
        {0x0e6f, 0x0346, (char *) "Rock Candy Gamepad for Xbox One 2016",                 (Report *) &xboxReport},
        {0x0e6f, 0x0401, (char *) "Logic3 Controller",                                    (Report *) &xboxReport},
        {0x0e6f, 0x0413, (char *) "Afterglow AX.1 Gamepad for Xbox 360",                  (Report *) &xboxReport},
        {0x0e6f, 0x0501, (char *) "PDP Xbox 360 Controller",                              (Report *) &xboxReport},
        {0x0e6f, 0xf900, (char *) "PDP Afterglow AX.1",                                   (Report *) &xboxReport},
        {0x0e8f, 0x0201, (char *) "SmartJoy Frag Xpad/PS2 adaptor",                       (Report *) &xboxReport},
        {0x0e8f, 0x3008, (char *) "Generic xbox control (dealextreme)",                   (Report *) &xboxReport},
        {0x0f0d, 0x000a, (char *) "Hori Co. DOA4 FightStick",                             (Report *) &xboxReport},
        {0x0f0d, 0x000c, (char *) "Hori PadEX Turbo",                                     (Report *) &xboxReport},
        {0x0f0d, 0x000d, (char *) "Hori Fighting Stick EX2",                              (Report *) &xboxReport},
        {0x0f0d, 0x0016, (char *) "Hori Real Arcade Pro.EX",                              (Report *) &xboxReport},
        {0x0f0d, 0x001b, (char *) "Hori Real Arcade Pro VX",                              (Report *) &xboxReport},
        {0x0f0d, 0x0063, (char *) "Hori Real Arcade Pro Hayabusa (USA) Xbox One",         (Report *) &xboxReport},
        {0x0f0d, 0x0067, (char *) "HORIPAD ONE",                                          (Report *) &xboxReport},
        {0x0f0d, 0x0078, (char *) "Hori Real Arcade Pro V Kai Xbox One",                  (Report *) &xboxReport},
        {0x0f0d, 0x00c5, (char *) "Hori Fighting Commander ONE",                          (Report *) &xboxReport},
        {0x0f30, 0x010b, (char *) "Philips Recoil",                                       (Report *) &xboxReport},
        {0x0f30, 0x0202, (char *) "Joytech Advanced Controller",                          (Report *) &xboxReport},
        {0x0f30, 0x8888, (char *) "BigBen XBMiniPad Controller",                          (Report *) &xboxReport},
        {0x102c, 0xff0c, (char *) "Joytech Wireless Advanced Controller",                 (Report *) &xboxReport},
        {0x1038, 0x1430, (char *) "SteelSeries Stratus Duo",                              (Report *) &xboxReport},
        {0x1038, 0x1431, (char *) "SteelSeries Stratus Duo",                              (Report *) &xboxReport},
        {0x11c9, 0x55f0, (char *) "Nacon GC-100XF",                                       (Report *) &xboxReport},
        {0x1209, 0x2882, (char *) "Ardwiino Controller",                                  (Report *) &xboxReport},
        {0x12ab, 0x0004, (char *) "Honey Bee Xbox360 dancepad",                           (Report *) &xboxReport},
        {0x12ab, 0x0301, (char *) "PDP AFTERGLOW AX.1",                                   (Report *) &xboxReport},
        {0x12ab, 0x0303, (char *) "Mortal Kombat Klassic FightStick",                     (Report *) &xboxReport},
        {0x12ab, 0x8809, (char *) "Xbox DDR dancepad",                                    (Report *) &xboxReport},
        {0x1430, 0x4748, (char *) "RedOctane Guitar Hero X-plorer",                       (Report *) &xboxReport},
        {0x1430, 0x8888, (char *) "TX6500+ Dance Pad (first generation)",                 (Report *) &xboxReport},
        {0x1430, 0xf801, (char *) "RedOctane Controller",                                 (Report *) &xboxReport},
        {0x146b, 0x0601, (char *) "BigBen Interactive XBOX 360 Controller",               (Report *) &xboxReport},
        {0x146b, 0x0604, (char *) "Bigben Interactive DAIJA Arcade Stick",                (Report *) &xboxReport},
        {0x1532, 0x0037, (char *) "Razer Sabertooth",                                     (Report *) &xboxReport},
        {0x1532, 0x0a00, (char *) "Razer Atrox Arcade Stick",                             (Report *) &xboxReport},
        {0x1532, 0x0a03, (char *) "Razer Wildcat",                                        (Report *) &xboxReport},
        {0x15e4, 0x3f00, (char *) "Power A Mini Pro Elite",                               (Report *) &xboxReport},
        {0x15e4, 0x3f0a, (char *) "Xbox Airflo wired controller",                         (Report *) &xboxReport},
        {0x15e4, 0x3f10, (char *) "Batarang Xbox 360 controller",                         (Report *) &xboxReport},
        {0x162e, 0xbeef, (char *) "Joytech Neo-Se Take2",                                 (Report *) &xboxReport},
        {0x1689, 0xfd00, (char *) "Razer Onza Tournament Edition",                        (Report *) &xboxReport},
        {0x1689, 0xfd01, (char *) "Razer Onza Classic Edition",                           (Report *) &xboxReport},
        {0x1689, 0xfe00, (char *) "Razer Sabertooth",                                     (Report *) &xboxReport},
        {0x1949, 0x041a, (char *) "Amazon Game Controller",                               (Report *) &xboxReport},
        {0x1bad, 0x0002, (char *) "Harmonix Rock Band Guitar",                            (Report *) &xboxReport},
        {0x1bad, 0x0003, (char *) "Harmonix Rock Band Drumkit",                           (Report *) &xboxReport},
        {0x1bad, 0x0130, (char *) "Ion Drum Rocker",                                      (Report *) &xboxReport},
        {0x1bad, 0xf016, (char *) "Mad Catz Xbox 360 Controller",                         (Report *) &xboxReport},
        {0x1bad, 0xf018, (char *) "Mad Catz Street Fighter IV SE Fighting Stick",         (Report *) &xboxReport},
        {0x1bad, 0xf019, (char *) "Mad Catz Brawlstick for Xbox 360",                     (Report *) &xboxReport},
        {0x1bad, 0xf021, (char *) "Mad Cats Ghost Recon FS GamePad",                      (Report *) &xboxReport},
        {0x1bad, 0xf023, (char *) "MLG Pro Circuit Controller (Xbox)",                    (Report *) &xboxReport},
        {0x1bad, 0xf025, (char *) "Mad Catz Call Of Duty",                                (Report *) &xboxReport},
        {0x1bad, 0xf027, (char *) "Mad Catz FPS Pro",                                     (Report *) &xboxReport},
        {0x1bad, 0xf028, (char *) "Street Fighter IV FightPad",                           (Report *) &xboxReport},
        {0x1bad, 0xf02e, (char *) "Mad Catz Fightpad",                                    (Report *) &xboxReport},
        {0x1bad, 0xf030, (char *) "Mad Catz Xbox 360 MC2 MicroCon Racing Wheel",          (Report *) &xboxReport},
        {0x1bad, 0xf036, (char *) "Mad Catz MicroCon GamePad Pro",                        (Report *) &xboxReport},
        {0x1bad, 0xf038, (char *) "Street Fighter IV FightStick TE",                      (Report *) &xboxReport},
        {0x1bad, 0xf039, (char *) "Mad Catz MvC2 TE",                                     (Report *) &xboxReport},
        {0x1bad, 0xf03a, (char *) "Mad Catz SFxT Fightstick Pro",                         (Report *) &xboxReport},
        {0x1bad, 0xf03d, (char *) "Street Fighter IV Arcade Stick TE - Chun Li",          (Report *) &xboxReport},
        {0x1bad, 0xf03e, (char *) "Mad Catz MLG FightStick TE",                           (Report *) &xboxReport},
        {0x1bad, 0xf03f, (char *) "Mad Catz FightStick SoulCaliber",                      (Report *) &xboxReport},
        {0x1bad, 0xf042, (char *) "Mad Catz FightStick TES+",                             (Report *) &xboxReport},
        {0x1bad, 0xf080, (char *) "Mad Catz FightStick TE2",                              (Report *) &xboxReport},
        {0x1bad, 0xf501, (char *) "HoriPad EX2 Turbo",                                    (Report *) &xboxReport},
        {0x1bad, 0xf502, (char *) "Hori Real Arcade Pro.VX SA",                           (Report *) &xboxReport},
        {0x1bad, 0xf503, (char *) "Hori Fighting Stick VX",                               (Report *) &xboxReport},
        {0x1bad, 0xf504, (char *) "Hori Real Arcade Pro. EX",                             (Report *) &xboxReport},
        {0x1bad, 0xf505, (char *) "Hori Fighting Stick EX2B",                             (Report *) &xboxReport},
        {0x1bad, 0xf506, (char *) "Hori Real Arcade Pro.EX Premium VLX",                  (Report *) &xboxReport},
        {0x1bad, 0xf900, (char *) "Harmonix Xbox 360 Controller",                         (Report *) &xboxReport},
        {0x1bad, 0xf901, (char *) "Gamestop Xbox 360 Controller",                         (Report *) &xboxReport},
        {0x1bad, 0xf903, (char *) "Tron Xbox 360 controller",                             (Report *) &xboxReport},
        {0x1bad, 0xf904, (char *) "PDP Versus Fighting Pad",                              (Report *) &xboxReport},
        {0x1bad, 0xf906, (char *) "MortalKombat FightStick",                              (Report *) &xboxReport},
        {0x1bad, 0xfa01, (char *) "MadCatz GamePad",                                      (Report *) &xboxReport},
        {0x1bad, 0xfd00, (char *) "Razer Onza TE",                                        (Report *) &xboxReport},
        {0x1bad, 0xfd01, (char *) "Razer Onza",                                           (Report *) &xboxReport},
        {0x20d6, 0x2001, (char *) "BDA Xbox Series X Wired Controller",                   (Report *) &xboxReport},
        {0x20d6, 0x2009, (char *) "PowerA Enhanced Wired Controller for Xbox Series X|S", (Report *) &xboxReport},
        {0x20d6, 0x281f, (char *) "PowerA Wired Controller For Xbox 360",                 (Report *) &xboxReport},
        {0x2e24, 0x0652, (char *) "Hyperkin Duke X-Box One pad",                          (Report *) &xboxReport},
        {0x24c6, 0x5000, (char *) "Razer Atrox Arcade Stick",                             (Report *) &xboxReport},
        {0x24c6, 0x5300, (char *) "PowerA MINI PROEX Controller",                         (Report *) &xboxReport},
        {0x24c6, 0x5303, (char *) "Xbox Airflo wired controller",                         (Report *) &xboxReport},
        {0x24c6, 0x530a, (char *) "Xbox 360 Pro EX Controller",                           (Report *) &xboxReport},
        {0x24c6, 0x531a, (char *) "PowerA Pro Ex",                                        (Report *) &xboxReport},
        {0x24c6, 0x5397, (char *) "FUS1ON Tournament Controller",                         (Report *) &xboxReport},
        {0x24c6, 0x541a, (char *) "PowerA Xbox One Mini Wired Controller",                (Report *) &xboxReport},
        {0x24c6, 0x542a, (char *) "Xbox ONE spectra",                                     (Report *) &xboxReport},
        {0x24c6, 0x543a, (char *) "PowerA Xbox One wired controller",                     (Report *) &xboxReport},
        {0x24c6, 0x5500, (char *) "Hori XBOX 360 EX 2 with Turbo",                        (Report *) &xboxReport},
        {0x24c6, 0x5501, (char *) "Hori Real Arcade Pro VX-SA",                           (Report *) &xboxReport},
        {0x24c6, 0x5502, (char *) "Hori Fighting Stick VX Alt",                           (Report *) &xboxReport},
        {0x24c6, 0x5503, (char *) "Hori Fighting Edge",                                   (Report *) &xboxReport},
        {0x24c6, 0x5506, (char *) "Hori SOULCALIBUR V Stick",                             (Report *) &xboxReport},
        {0x24c6, 0x5510, (char *) "Hori Fighting Commander ONE (Xbox 360/PC Mode)",       (Report *) &xboxReport},
        {0x24c6, 0x550d, (char *) "Hori GEM Xbox controller",                             (Report *) &xboxReport},
        {0x24c6, 0x550e, (char *) "Hori Real Arcade Pro V Kai 360",                       (Report *) &xboxReport},
        {0x24c6, 0x551a, (char *) "PowerA FUSION Pro Controller",                         (Report *) &xboxReport},
        {0x24c6, 0x561a, (char *) "PowerA FUSION Controller",                             (Report *) &xboxReport},
        {0x24c6, 0x5b00, (char *) "ThrustMaster Ferrari 458 Racing Wheel",                (Report *) &xboxReport},
        {0x24c6, 0x5b02, (char *) "Thrustmaster, Inc. GPX Controller",                    (Report *) &xboxReport},
        {0x24c6, 0x5b03, (char *) "Thrustmaster Ferrari 458 Racing Wheel",                (Report *) &xboxReport},
        {0x24c6, 0x5d04, (char *) "Razer Sabertooth",                                     (Report *) &xboxReport},
        {0x24c6, 0xfafe, (char *) "Rock Candy Gamepad for Xbox 360",                      (Report *) &xboxReport},
        {0x2563, 0x058d, (char *) "OneXPlayer Gamepad",                                   (Report *) &xboxReport},
        {0x2dc8, 0x2000, (char *) "8BitDo Pro 2 Wired Controller fox Xbox",               (Report *) &xboxReport},
        {0x2dc8, 0x3106, (char *) "8BitDo Pro 2 Wired Controller",                        (Report *) &xboxReport},
        {0x31e3, 0x1100, (char *) "Wooting One",                                          (Report *) &xboxReport},
        {0x31e3, 0x1200, (char *) "Wooting Two",                                          (Report *) &xboxReport},
        {0x31e3, 0x1210, (char *) "Wooting Lekker",                                       (Report *) &xboxReport},
        {0x31e3, 0x1220, (char *) "Wooting Two HE",                                       (Report *) &xboxReport},
        {0x31e3, 0x1300, (char *) "Wooting 60HE (AVR)",                                   (Report *) &xboxReport},
        {0x31e3, 0x1310, (char *) "Wooting 60HE (ARM)",                                   (Report *) &xboxReport},
        {0x3285, 0x0607, (char *) "Nacon GC-100",                                         (Report *) &xboxReport},
        {0x3767, 0x0101, (char *) "Fanatec Speedster 3 Forceshock Wheel",                 (Report *) &xboxReport},
        {0xffff, 0xffff, (char *) "Chinese-made Xbox Controller",                         (Report *) &xboxReport},
        ///
        /// DS4
        ///
        {0x054c, 0x05c4, (char *) "DualShock 4",                                          (Report *) &ds4Report},
        {0x054c, 0x09cc, (char *) "DualShock 4 (2nd Gen)",                                (Report *) &ds4Report},
        {0x054c, 0x0ce6, (char *) "DualSense",                                            (Report *) &ds5Report},
        ///
        {0xe6f,  0x1112, (char *) "SNK Neo-Geo Mini",                                     (Report *) &ngMiniReport},
        ///
        /// Unknown
        ///
        {0x0000, 0x0000, (char *) "Unknown device",                                       nullptr},
};

const Device *get_device(uint16_t vid, uint16_t pid) {
    // lookup for a known game controller
    int i = 0;
    int p = INT32_MAX;

    while (p > 0) {
        p = devices[i].idProduct;
        if (p == pid && devices[i].idVendor == vid) {
            return &devices[i];
        }
        i++;
    }

    return nullptr;
}
