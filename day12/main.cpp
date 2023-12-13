
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



struct Record {
	uint32_t damage;
	uint32_t damage_mask;
	uint32_t unknown;
	uint32_t unknown_mask;
	int len;
	std::vector<int> sizes;
};


void printRecord(Record record) {
	// ???.### 1,1,3

	std::string str;

	uint32_t cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.damage & cursor) != 0) {
			str += '#';
		} else if ((record.unknown & cursor) != 0) {
			str += '?';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	str += ' ';

	for (auto size : record.sizes) {
		str += std::to_string(size);
		str += ',';
	}

	LOGD("             record: %s", str.c_str());

	str.clear();
	cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.damage & cursor) != 0) {
			str += '#';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	LOGD("      damage (%4d): %s", record.damage, str.c_str());

	str.clear();
	cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.damage_mask & cursor) != 0) {
			str += '#';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	LOGD(" damage_mask (%4d): %s", record.damage_mask, str.c_str());

	str.clear();
	cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.unknown & cursor) != 0) {
			str += '?';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	LOGD("     unknown (%4d): %s", record.unknown, str.c_str());

	str.clear();
	cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.unknown_mask & cursor) != 0) {
			str += '?';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	LOGD("unknown_mask (%4d): %s", record.unknown_mask, str.c_str());
	LOGD();
}



int findArrangements(Record record) {

	// LOGD("findArrangements...");

	int possible_count = 0;

	auto limit = 1 << record.len;

	// LOGD("limit: %d", limit);

	for (auto i = 0; i < limit; i++) {

		if ((record.unknown | i) != record.unknown) {

			// if (i % 99 == 17) {
			// 	LOGD("i: %d, skipping", i);
			// }

			// LOGD("i: %d, skipping (not inside unknown_mask)", i);
			continue;
		}

		if ((record.damage & i) != 0) {

			// if (i % 99 == 17) {
			// 	LOGD("i: %d, skipping", i);
			// }

			// LOGD("i: %d, skipping (inside damage)", i);
			continue;
		}

		bool possible = true;

		// LOGD("trying with i: %d", i);

		uint32_t new_damage = record.damage | i;

		uint32_t cursor = 1;
		std::string str;
		for (int i = 0; i < record.len; i++) {
			if ((new_damage & cursor) != 0) {
				str += '#';
			} else {
				str += '.';
			}
			cursor = cursor << 1;
		}

		// LOGD("new_damage (%4d): %s", new_damage, str.c_str());

		bool inside_group = false;
		// int group_count = 0;
		int current_group_length = 0;
		int sizes_index = 0;
		cursor = 1;
		for (int i = 0; i < record.len; i++) {
			if ((new_damage & cursor) != 0) {
				if (inside_group) {
					current_group_length = current_group_length + 1;
				} else {
					inside_group = true;
					// group_count = group_count + 1;
					current_group_length = 1;
				}
			} else {
				if (inside_group) {
					inside_group = false;
					// LOGD("current_group_length: %d", current_group_length);
					// LOGD("sizes_index: %d", sizes_index);
					// LOGD("record.sizes[sizes_index]: %d", record.sizes[sizes_index]);
					if (current_group_length != record.sizes[sizes_index]) {
						// LOGD("NO 1!");
						possible = false;
					}
					if (sizes_index >= record.sizes.size()) {
						// LOGD("NO 2!");
						possible = false;
					}
					sizes_index = sizes_index + 1;
				} else {
					// nothing
				}
			}
			cursor = cursor << 1;
		}

		if (inside_group) {
			if (current_group_length != record.sizes[sizes_index]) {
				// LOGD("NO 1!");
				possible = false;
			}
			if (sizes_index >= record.sizes.size()) {
				// LOGD("NO 2!");
				possible = false;
			}
			sizes_index = sizes_index + 1;
		}

		// LOGD("sizes_index: %d", sizes_index);

		if (sizes_index != record.sizes.size()) {
			// LOGD("NO 3!");
			possible = false;
		}

		// LOGD();

		if (possible) {
			possible_count++;
		}
	}

	LOGD("possible_count: %d", possible_count);

	return possible_count;
}




std::vector<Record> records;


Record parseRecord(const char *line) {

	auto tosearch = line;

	const char *space = strchr(tosearch, ' ');
	assert(space != NULL);

	// ?#?#?#?#?#?#?#? 1,3,1,6

	uint32_t damage = 0;
	uint32_t unknown = 0;
	uint32_t damage_mask = 0;
	uint32_t unknown_mask = 0;
	const char *damage_start = NULL;
	const char *damage_end = NULL;
	const char *unknown_start = NULL;
	const char *unknown_end = NULL;

	int len = 0;
	for (auto i = space - 1; i >= tosearch; i--) {
		if (*i == '#') {
			damage = damage << 1 | 1;
			unknown = unknown << 1 | 0;
			if (damage_start == NULL) {
				damage_start = i;
			}
			damage_end = i;
			len++;
		} else if (*i == '?') {
			damage = damage << 1 | 0;
			unknown = unknown << 1 | 1;
			if (unknown_start == NULL) {
				unknown_start = i;
			}
			unknown_end = i;
			len++;
		} else {
			assert(*i == '.');
			damage = damage << 1 | 0;
			unknown = unknown << 1 | 0;
			len++;
		}
	}

	for (auto i = space - 1; i >= tosearch; i--) {
		if (damage_end <= i && i <= damage_start) {
			damage_mask = damage_mask << 1 | 1;
		} else {
			damage_mask = damage_mask << 1 | 0;
		}
		if (unknown_end <= i && i <= unknown_start) {
			unknown_mask = unknown_mask << 1 | 1;
		} else {
			unknown_mask = unknown_mask << 1 | 0;
		}
	}

	// ???.### 1,1,3

	// uint32_t cursor = 1;
	// bool seen = false;
	// for (auto i = 0; i < 32; i++) {
	// 	if ((record.damage & cursor) != 0) {
	// 		seen = true;
	// 	}
	// 	cursor = cursor << 1;
	// }

	tosearch = space + 1;

	std::vector<int> sizes;

	while (1) {
		const char *comma = strchr(tosearch, ',');
		if (comma != NULL) {

			int size;
			auto res = sscanf(tosearch, "%d", &size);
			assert(res == 1);

			sizes.push_back(size);

			tosearch = comma + 1;

		} else {

			int size;
			auto res = sscanf(tosearch, "%d", &size);
			assert(res == 1);

			sizes.push_back(size);

			break;
		}
	}
	// LOGD("sizes size: %zu", sizes.size());

	return Record{damage, damage_mask, unknown, unknown_mask, len, sizes};
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

			auto line = new char[len + 1];

			memcpy(line, tosearch, len);

			line[len] = '\0';

			// LOGD("line: %s", line);
			// LOGD("len: %ld", newline - tosearch);


			auto record = parseRecord(line);
			records.push_back(record);

			tosearch = newline + 1;

			// LOGD("");

		} else {

			break;
		}
	}


	// LOGD("records:");
	// for (auto record : records) {
	// 	printRecord(record);
	// }
	// LOGD();


	// findArrangements(records[2]);


	int total_possible_counts = 0;
	for (auto record : records) {
		auto possible_count = findArrangements(record);
		total_possible_counts = total_possible_counts + possible_count;
	}

	LOGD("total_possible_counts: %d", total_possible_counts);

  	return 0;
}








