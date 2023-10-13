#pragma once
#include <Windows.h>
#include <WinINet.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include "json.hpp"

#pragma comment(lib, "wininet.lib")
using json = nlohmann::json;
namespace fs = std::filesystem;


namespace updater {
	const std::string file_path = "offsets.json";

	bool check_and_update(bool prompt_update);
	bool get_last_commit_date(json& commit);
	bool download_file(const char* url, const char* localPath);
	bool file_good(const std::string& name);

	extern bool read();
	extern void save();

	inline std::string github_repo_api = "https://api.github.com/repos/a2x/cs2-dumper/commits";
	inline std::string raw_updated_offets = "https://github.com/a2x/cs2-dumper/raw/main/generated/offsets.json";

}