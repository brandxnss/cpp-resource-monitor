#include <iostream>
#include <windows.h>
#include <pdh.h>

int main() {
    PDH_HQUERY query;
    PDH_HCOUNTER counter;
    
    LPCWSTR cpuString = L"\\Processor(_Total)\\% Processor Time";

    if (PdhOpenQuery(NULL, 0, &query) != ERROR_SUCCESS) {
        std::cerr << "Failed to open query" << std::endl;
        return 1;
    }

    // Pull CPU Usuage
    if (PdhAddCounter(query, cpuString, 0, &counter) != ERROR_SUCCESS) {
        std::cerr << "Failed to add counter." << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    PDH_FMT_COUNTERVALUE value;
    DWORD type;

    while (true) {
        if (PdhCollectQueryData(query) != ERROR_SUCCESS) {
            std::cerr << "Failed to collect data." << std::endl;
            break;
        }

        if (PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, &type, &value) != ERROR_SUCCESS) {
            std::cerr << "Failed to get counter value." << std::endl;
            break;
        }

        std::cout << "CPU Usage: " << value.doubleValue << "%" << std::endl;
        Sleep(1000); // Update every second
    }

    PdhCloseQuery(query);
    return 0;
}