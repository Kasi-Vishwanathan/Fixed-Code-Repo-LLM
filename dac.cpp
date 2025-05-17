// File: dac.cpp

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

struct DACSetup {
    int dac_ID;
    const char* user_code;
    void (*send_function)(const char*, int);

    DACSetup(int ID, const char* code, void (*sf)(const char*, int) = nullptr)
        : dac_ID(ID), user_code(code), send_function(sf) {}
};

void InitializeDAC(DACSetup* dac) {
    if (!dac) return;

    std::string user_code = dac->user_code ? dac->user_code : "";
    // Truncate if necessary to fit buffer
    if (user_code.length() >= 256) {
        user_code.resize(255);
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s", user_code.c_str());
    
    std::printf("DAC initialized with code: %s\n", buffer);
    
    if (dac->send_function) {
        dac->send_function(buffer, dac->dac_ID);
    }
}

int main() {
    auto dac = std::make_unique<DACSetup>(42, "TEST");
    InitializeDAC(dac.get());
    
    // Example of setting a send function
    // dac->send_function = [](const char* data, int id) { /* implementation */ };
    
    return 0;
}
