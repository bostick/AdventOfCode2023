
#include "common/common.h"

#include <optional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <array>

#include <cstdio>
#include <cinttypes>
#include <cstring>
#include <cassert>
#include <cstdint>



std::vector<std::string> strs;


void parseLine(const char *start, const char *end) {

	const char *tosearch = start;

	while (1) {

		const char *comma = strchr(tosearch, ',');

		if (comma != NULL) {

			auto len = comma - tosearch;

			auto str = new char[len + 1];

			memcpy(str, tosearch, len);

			str[len] = '\0';

			strs.push_back(std::string(str));

			tosearch = comma + 1;

		} else {

			auto len = end - tosearch;

			auto str = new char[len + 1];

			memcpy(str, tosearch, len);

			str[len] = '\0';

			strs.push_back(std::string(str));

			break;
		}
	}
}



int calculateHash(std::string str) {

	int hash = 0;

	for (auto c : str) {
		hash += (int)c;
		hash = 17 * hash;
		hash = hash % 256;
	}

	LOGD("hash of %s: %d", str.c_str(), hash);

	return hash;
}


int main(int argc, char **argv) {

	const char *path = "input.txt";

	FILE *file = fopen(path, "r");

	if (!file) {

		LOGE("cannot open %s\n", path);

		return 1;
	}

	if (fseek(file, 0, SEEK_END)) {

		LOGE("fseek failed");

		return 1;
	}

	long res = ftell(file);

	if (res < 0) {

		LOGE("ftell failed");

		return 1;
	}

	size_t len = static_cast<size_t>(res);

	rewind(file);

	auto buf = new char[len + 1];

	size_t r = fread(buf, sizeof(char), len, file);

	if (r != len) {

		LOGE("fread failed");

		return 1;
	}

	fclose(file);

	buf[len] = '\0';




	const char *tosearch = buf;

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto len = newline - tosearch;

			// LOGD("len1: %d", len);

			parseLine(buf, newline);

			tosearch = newline + 1;

		} else {
			break;
		}
	}


	// strs.clear();
	// strs.push_back(std::string("HASH"));

	int total = 0;

	for (auto str : strs) {

		auto hash = calculateHash(str);

		total += hash;
	}
	
	LOGD("total: %d", total);

  	return 0;
}








