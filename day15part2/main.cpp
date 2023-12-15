
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



struct Lens {
	std::string label;
	char operation;
	int focal_length;

	// bool operator==(const Lens& other) const {
    //     return label == other.label &&
    //     	focal_length == other.focal_length;
    // }
};


void printLens(Lens lens) {
	LOGD("lens: label: %s, op: %c, focal_length: %d", lens.label.c_str(), lens.operation, lens.focal_length);
}


struct Box {
	std::vector<Lens> slots;
};



std::vector<Lens> lenss;


std::array<Box, 256> boxes;


int calculateHash(std::string str) {

	int hash = 0;

	for (auto c : str) {
		hash += (int)c;
		hash = 17 * hash;
		hash = hash % 256;
	}

	// LOGD("hash of %s: %d", str.c_str(), hash);

	return hash;
}

void calculateHashMap() {
	
	for (auto lens : lenss) {

		auto hash = calculateHash(lens.label);

		if (lens.operation == '=') {

			LOGD("assigning %s to box %d", lens.label.c_str(), hash);

			auto& box = boxes[hash];

			int i;
			bool found = false;
			for (i = 0; i < box.slots.size(); i++) {
				if (box.slots[i].label == lens.label) {
					LOGD("found!");
					found = true;
					break;
				}
			}

			if (found) {

				box.slots[i] = lens;

			} else {
				box.slots.push_back(lens);
			}

		} else if (lens.operation == '-') {

			LOGD("removing %s from box %d", lens.label.c_str(), hash);

			auto& box = boxes[hash];

			int i;
			bool found = false;
			for (i = 0; i < box.slots.size(); i++) {
				// LOGD("comparing %s and %s", box.slots[i].label.c_str(), lens.label.c_str());
				if (box.slots[i].label == lens.label) {
					LOGD("found!");
					found = true;
					break;
				}
			}

			if (found) {
				// LOGD("box.slots size before: %d", box.slots.size());
				box.slots.erase(box.slots.begin() + i);
				// LOGD("box.slots size after: %d", box.slots.size());
			}

		} else {
			assert(false);
		}
	}

}




Lens parseLens(const char *str, int len) {

	std::string label;
	char operation;
	int focal_length;

	int i;
	for (i = 0; i < len; i++) {
		if (str[i] == '=') {
			operation = '=';
			break;
		} else if (str[i] == '-') {
			operation = '-';
			break;
		} else {
			label += str[i];
		}
	}

	if (operation == '=') {
		auto res = sscanf(str + i + 1, "%d", &focal_length);
		assert(res == 1);
	}

	return Lens{label, operation, focal_length};
}


void parseLine(const char *start, const char *end) {

	const char *tosearch = start;

	while (1) {

		const char *comma = strchr(tosearch, ',');

		if (comma != NULL) {

			auto len = comma - tosearch;

			auto str = new char[len + 1];

			memcpy(str, tosearch, len);

			str[len] = '\0';

			auto lens = parseLens(str, len);

			lenss.push_back(lens);

			tosearch = comma + 1;

		} else {

			auto len = end - tosearch;

			auto str = new char[len + 1];

			memcpy(str, tosearch, len);

			str[len] = '\0';

			auto lens = parseLens(str, len);

			lenss.push_back(lens);

			break;
		}
	}
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

	// int total = 0;

	// for (auto str : strs) {

	// 	auto hash = calculateHash(str);

	// 	total += hash;
	// }
	
	// LOGD("total: %d", total);


	LOGD("lenss:");
	for (auto lens : lenss) {
		printLens(lens);
	}
	LOGD();



	calculateHashMap();


	LOGD("boxes:");
	for (int i = 0; i < 256; i++) {

		auto box = boxes[i];

		if (box.slots.empty()) {
			continue;
		}

		for (int j = 0; j < box.slots.size(); j++) {

			auto hash = calculateHash(box.slots[j].label);

			LOGD("box %d slot %d label %s hash %d focal_length %d", i, j+1, box.slots[j].label.c_str(), hash, box.slots[j].focal_length);
		}
	}
	LOGD();


	int total = 0;

	for (int i = 0; i < 256; i++) {

		auto box = boxes[i];

		for (int j = 0; j < box.slots.size(); j++) {

			auto hash = calculateHash(box.slots[j].label);

			LOGD("box %d slot %d label %s hash %d focal_length %d: %d", i, j+1, box.slots[j].label.c_str(), hash, box.slots[j].focal_length, (i + 1) * (j + 1) * (box.slots[j].focal_length));

			total += (i + 1) * (j + 1) * (box.slots[j].focal_length);
		}
	}
	
	LOGD("total: %d", total);

  	return 0;
}








