#pragma once

#include <DueFlashStorage.h>

class SystemModuleSettingsBase {
    public:
        virtual void initialize(const int& settingsStartAddress, const bool& eepromIsInitialized, DueFlashStorage* storage) = 0;
        virtual int getSettingsSize() = 0;
        virtual void resetSettings() = 0;
        virtual void restoreSettings() = 0;
        virtual void saveSettings() = 0;
};

template<typename TSettings>
class SystemModuleSettings: public SystemModuleSettingsBase {
    protected:
        DueFlashStorage* storage;
        TSettings _data;
        const TSettings defaultSettings;
        int settingsStartAddress = 0;

    public:
        SystemModuleSettings(const TSettings defaultSettings):
            defaultSettings(defaultSettings),
            _data(defaultSettings) {}

        void initialize(const int& settingsStartAddress, const bool& eepromIsInitialized, DueFlashStorage* storage) {
            this->settingsStartAddress = settingsStartAddress;
            this->storage = storage;

            if (eepromIsInitialized) restoreSettings();
            else resetSettings();
        }

        int getSettingsSize() {
            return sizeof(TSettings);
        }

        void restoreSettings() {
            uint8_t* buffer = storage->readAddress(settingsStartAddress);
            memcpy(&_data, buffer, sizeof(TSettings));
        }

        void resetSettings() {
            _data = defaultSettings;
            saveSettings();
        }

        void saveSettings() {            
            byte buffer[sizeof(TSettings)];
            memcpy(buffer, &_data, sizeof(TSettings));
            storage->write(settingsStartAddress, buffer, sizeof(TSettings));
        }

        TSettings& data() {
            return _data;
        }
};