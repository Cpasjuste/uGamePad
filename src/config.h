//
// Created by cpasjuste on 11/05/23.
//

#ifndef U_GAMEPAD_CONFIG_H
#define U_GAMEPAD_CONFIG_H

#include "fs.h"
#include "devices.h"

namespace uGamePad {
    class Config {
    public:
        enum OptionType {
            SubMenu = 0,
            Option
        };

        enum OptionId {
            OutputMode = 0,
            ButtonsRemap,
            AutoFire,
            GamePadTest,
            Exit
        };

        struct Option {
            uint8_t id = 0;
            uint8_t type = 0;
            std::string name;
            std::vector<std::string> values;
            int16_t index = 0;

            std::string value() {
                return values.size() > index ? values[index] : "";
            }

            void next() {
                index++;
                if (index >= values.size()) index = 0;
            }

            void prev() {
                index--;
                if (index < 0) index = (int16_t)(values.size() - 1);
            }
        };

        explicit Config(Fs *fs);

        Device *loadDevice(uint16_t vid, uint16_t pid);

        bool saveDevice(Device *device);

        //std::vector<Option> *getOptions() { return &m_options; };

    private:
        Fs *p_fs;

        //std::vector<Option> m_options;

        Device *loadDevice(const std::string &path);
    };

} // uGamePad

#endif //U_GAMEPAD_CONFIG_H
