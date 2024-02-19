#include "ConfigManager.h"

// this section is for the implementation
ConfigManager::ConfigManager(const char* filename) {
    this->filename = filename;

    // start LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Could not initialize LittleFS");
        return;
    }
    
    // load config
    this->load_config();
}

ConfigManager::ConfigManager() {
    this->filename = DEFAULT_CONFIG_FILE;

    // start LittleFS
    if (!LittleFS.begin()) {
        Serial.println("Could not initialize LittleFS");
        return;
    }

    // load config
    this->load_config();
}

ConfigManager::~ConfigManager() { }

// load config from LittleFS
void ConfigManager::load_config() {
    Serial.println("Loading config");
    Serial.print("Filename: ");
    Serial.println(this->filename);
    // Open file for reading
    File file = LittleFS.open(this->filename, "r");

    if (!file) {
        Serial.println("Failed to open config file");
        return;
    }
    Serial.println("File opened");

    // Allocate a buffer to store contents of the file
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        Serial.println("Failed to read file, using default configuration");
        return;
    }

    // Copy values from the JsonDocument to the Config
    strlcpy(this->config.wifi.ssid, doc["wifi"]["ssid"] | "", sizeof(this->config.wifi.ssid));
    strlcpy(this->config.wifi.password, doc["wifi"]["password"] | "", sizeof(this->config.wifi.password));
    
    strcpy(this->config.ntp.server, doc["ntp"]["server"] | "");
    strcpy(this->config.ntp.tz, doc["ntp"]["tz"] | "");

    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
}

const char* ConfigManager::get_wifi_ssid() {
    return this->config.wifi.ssid;
}

const char* ConfigManager::get_wifi_password() {
    return this->config.wifi.password;
}

const char* ConfigManager::get_ntp_server() {
    return this->config.ntp.server;
}

const char* ConfigManager::get_ntp_tz() {
    return this->config.ntp.tz;
}