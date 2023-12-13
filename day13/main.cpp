
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




struct Pattern {
	int row_count;
	int col_count;
	char **image;
};


std::vector<Pattern> patterns;


Pattern parsePattern(const char *start, const char *end) {

	// LOGD("parsing...");

	int row_count = 0;
	int col_count = 0;

	const char *tosearch = start;

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto len = newline - tosearch;

			// LOGD("len: %d", len);

			if (len == 0) {
				break;
			}

			row_count++;
			col_count = len;

			tosearch = newline + 1;

		} else {
			break;
		}
	}

	// LOGD("row_count: %d", row_count);
	// LOGD("col_count: %d", col_count);

	char **image = (char **)malloc(row_count * sizeof(char *));
	for (int i = 0; i < row_count; i++) {
		image[i] = (char *)malloc(col_count * sizeof(char));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			auto c = *(start + i * (col_count + 1) + j);
			// LOGD("trying: %c (%d): ", c, (int)c);
			image[i][j] = c;
			// assert(image[i][j] != '\n');
		}
		// assert(*(start + i * col_count) == '\n');
	}

	return Pattern{row_count, col_count, image};
}


void printPattern(Pattern p) {

	LOGD("pattern:");
	LOGD("row col: %d %d", p.row_count, p.col_count);
	std::string str;
	for (int i = 0; i < p.row_count; i++) {
		str.clear();
		for (int j = 0; j < p.col_count; j++) {
			str += p.image[i][j];
		}
		LOGD("%s", str.c_str());
	}
	LOGD();
}


int analyzePattern(Pattern p) {

	//
	// scan pairs of rows
	//
	std::vector<int> horizontal_candidates;
	for (int i = 0; i < p.row_count-1; i++) {
		auto all = true;
		for (int j = 0; j < p.col_count; j++) {
			if (p.image[i][j] != p.image[i+1][j]) {
				all = false;
				break;
			}
		}
		if (all) {
			horizontal_candidates.push_back(i);
		}
	}

	//
	// scan pairs of cols
	//
	std::vector<int> vertical_candidates;
	for (int i = 0; i < p.col_count-1; i++) {
		auto all = true;
		for (int j = 0; j < p.row_count; j++) {
			if (p.image[j][i] != p.image[j][i+1]) {
				all = false;
				break;
			}
		}
		if (all) {
			vertical_candidates.push_back(i);
		}
	}

	LOGD("horizontal_candidates:");
	for (auto h : horizontal_candidates) {
		LOGD("h: %d", h);
	}
	LOGD();

	LOGD("vertical_candidates:");
	for (auto v : vertical_candidates) {
		LOGD("v: %d", v);
	}
	LOGD();

	int count = 0;

	for (auto h : horizontal_candidates) {
		//
		// row h and h + 1 already match
		//

		int offset = 1;
		auto all = true;
		while (1) {
			if (h - offset <= -1) {
				break;
			}
			if (h + 1 + offset >= p.row_count) {
				break;
			}

			for (int j = 0; j < p.col_count; j++) {
				if (p.image[h - offset][j] != p.image[h + 1 + offset][j]) {
					all = false;
					break;
				}
			}

			offset++;
		}
		if (all) {
			LOGD("%d is good horizontal", h);
			count += 100 * (h + 1);
		}
	}

	for (auto v : vertical_candidates) {
		//
		// col v and v + 1 already match
		//

		int offset = 1;
		auto all = true;
		while (1) {
			if (v - offset <= -1) {
				break;
			}
			if (v + 1 + offset >= p.col_count) {
				break;
			}

			for (int j = 0; j < p.row_count; j++) {
				if (p.image[j][v - offset] != p.image[j][v + 1 + offset]) {
					all = false;
					break;
				}
			}

			offset++;
		}
		if (all) {
			LOGD("%d is good vertical", v);
			count += (v + 1);
		}
	}

	return count;
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

	auto pattern_start = tosearch;

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto len = newline - tosearch;

			// LOGD("len1: %d", len);

			if (len == 0) {
				auto pattern = parsePattern(pattern_start, newline);
				patterns.push_back(pattern);
				tosearch = newline + 1;
				pattern_start = tosearch;
			} else {
				tosearch = newline + 1;
			}

		} else {

			auto pattern = parsePattern(pattern_start, newline);
			patterns.push_back(pattern);

			break;
		}
	}

	
	for (auto p : patterns) {
		printPattern(p);
	}


	int total_count = 0;
	for (auto p : patterns) {
		auto count = analyzePattern(p);
		LOGD("count: %d", count);
		total_count += count;
	}

	LOGD("total_count: %d", total_count);


  	return 0;
}








