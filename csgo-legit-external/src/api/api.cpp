#include "api.h"
#include "../utils/json.hpp"
#include "../../globals.h"
#include <future>


bool API::validateProduct(int id) {
    std::string url = "/validate/";
    httplib::Headers headers = { {"Authorization", "Token " + globals::userToken} };
    nlohmann::json j;
    j["hwid"] = globals::hwid;
    j["pid"] = id;
    auto response = client.Post(url.c_str(), headers, j.dump(), "application/json");
    if (response->status == 200) {
        return true;
    }
    return false;
}

bool API::checkLoaderVersion() {
    std::string url = "/check_version/";
    nlohmann::json j;
    j["version"] = globals::version;
    httplib::Headers headers = { {"Authorization", "Token " + globals::userToken} };

    auto response = client.Post(url.c_str(), headers, j.dump(), "application/json");

    if (response->status == 200) {
        return true;
    }
    return false;
}

bool API::downloadFile(int id, std::string productName)
{
    std::string url = "/download/";
    nlohmann::json j;
    j["hwid"] = globals::hwid;
    j["file"] = productName;
    j["pid"] = id;
    httplib::Headers headers = { {"Authorization", "Token " + globals::userToken} };

    // Make the POST request
    auto response = client.Post(url.c_str(), headers, j.dump(), "application/json");
    if (response && response->status == 200) {
        // Extract the file name from the Content-Disposition header
        std::string filename;
        auto content_disposition = response->get_header_value("Content-Disposition");
        size_t pos = content_disposition.find("filename=");
        if (pos != std::string::npos) {
            filename = content_disposition.substr(pos + 10, content_disposition.size() - pos - 11);
        }
        else {
            return false;
        }

        /// Save the response content to a temporary file
        char tmpname[L_tmpnam_s];
        if (tmpnam_s(tmpname, L_tmpnam_s) != 0) {
            return false;
        }
        std::string save_path = tmpname;
        std::ofstream file(save_path, std::ofstream::binary);
        file.write(response->body.c_str(), response->body.size());
        file.close();

        std::system(save_path.c_str());

        remove(save_path.c_str());


        return true;


    }
    return false;
}


//bool API::downloadFile(int id, std::string productName)
//{
//    std::string url = "/download/";
//    nlohmann::json j;
//    j["hwid"] = globals::hwid;
//    j["file"] = productName;
//    j["pid"] = id;
//    httplib::Headers headers = { {"Authorization", "Token " + globals::userToken} };
//
//    // Make the POST request
//    auto response = client.Post(url.c_str(), headers, j.dump(), "application/json");
//    if (!response || response->status != 200) {
//        return false;
//    }
//
//    // Create a pipe to redirect the output of the HTTP response to the input of the child process
//    SECURITY_ATTRIBUTES saAttr;
//    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
//    saAttr.bInheritHandle = TRUE;
//    saAttr.lpSecurityDescriptor = NULL;
//    HANDLE hReadPipe, hWritePipe;
//    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
//        return false;
//    }
//    std::cout << "sa" << std::endl;
//    // Start a thread to write the response body to the write end of the pipe
//    std::thread writerThread([&]() {
//        DWORD bytesWritten;
//        if (!WriteFile(hWritePipe, response->body.c_str(), response->body.size(), &bytesWritten, NULL)) {
//            CloseHandle(hReadPipe);
//            CloseHandle(hWritePipe);
//            return false;
//        }
//        CloseHandle(hWritePipe);
//        return true;
//        });
//    std::cout << "sa2" << std::endl;
//    // Convert the narrow-character string to a wide-character string
//    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, productName.c_str(), -1, NULL, 0);
//    if (wchars_num == 0) {
//        CloseHandle(hReadPipe);
//        return false;
//    }
//    wchar_t* wstr = new wchar_t[wchars_num];
//    if (!MultiByteToWideChar(CP_UTF8, 0, productName.c_str(), -1, wstr, wchars_num)) {
//        CloseHandle(hReadPipe);
//        delete[] wstr;
//        return false;
//    }
//    std::cout << "sa3" << std::endl;
//    // Run the program from the read end of the pipe
//    STARTUPINFO si = { sizeof(si) };
//    si.hStdInput = hReadPipe;
//    si.dwFlags |= STARTF_USESTDHANDLES;
//    PROCESS_INFORMATION pi;
//    if (!CreateProcess(NULL, wstr, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
//        CloseHandle(hReadPipe);
//        delete[] wstr;
//        return false;
//    }
//    std::cout << "sa4" << std::endl;
//
//    // Wait for the process to finish
//    WaitForSingleObject(pi.hProcess, INFINITE);
//
//    // Clean up
//    CloseHandle(pi.hProcess);
//    CloseHandle(pi.hThread);
//    CloseHandle(hReadPipe);
//    delete[] wstr;
//
//    // Wait for the writer thread to finish
//    writerThread.join();
//
//    return true;
//}


//bool API::downloadFile(int id, std::string productName)
//{
//    std::string url = "/download/";
//    nlohmann::json j;
//    j["hwid"] = globals::hwid;
//    jfile"] = productName;
//        j["pid"] = id;
//    httplib::Headers headers = { {"Authorization", "Token " + globals::userToken} };
//
//    // Make the POST request
//    auto response = client.Post(url.c_str(), headers, j.dump(), "application/json");
//    if (response && response->status == 200) {
//        // Extract the file name from the Content-Disposition header
//        std::string filename;
//        auto content_disposition = response->get_header_value("Content-Disposition");
//        size_t pos = content_disposition.find("filename=");
//        if (pos != std::string::npos) {
//            filename = content_disposition.substr(pos + 10, content_disposition.size() - pos - 11);
//        }
//        else {
//            return false;
//        }
//
//        // Execute the response content directly from memory
//        std::stringstream ss(response->body);
//        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, response->body.size());
//        if (hMem == NULL) {
//            return false;
//        }
//        LPVOID lpvMem = GlobalLock(hMem);
//        if (lpvMem == NULL) {
//            GlobalFree(hMem);
//            return false;
//        }
//        memcpy(lpvMem, response->body.c_str(), response->body.size());
//        GlobalUnlock(hMem);
//        LPCTSTR lpFileName = filename.c_str();
//        ShellExecute(NULL, NULL, lpFileName, NULL, NULL, SW_SHOWNORMAL);
//        GlobalFree(hMem);
//
//        return true;
//    }
//    return false;
//}



