
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



struct Platform {
	int row_count;
	int col_count;
	char **image;
};


Platform platform;


void parsePlatform(const char *start, const char *end) {

	LOGD("parsing...");

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

	platform = Platform{row_count, col_count, image};
}



void printPlatform() {

	LOGD("platform:");
	LOGD("row col: %d %d", platform.row_count, platform.col_count);
	std::string str;
	for (int i = 0; i < platform.row_count; i++) {
		str.clear();
		for (int j = 0; j < platform.col_count; j++) {
			str += platform.image[i][j];
		}
		LOGD("%s", str.c_str());
	}
	LOGD();
}


void tiltNorth() {
	// LOGD("tiltNorth...");
	for (int i = 1; i < platform.row_count; i++) {
		for (int j = 0; j < platform.col_count; j++) {
			if (platform.image[i][j] == 'O') {
				bool moved = false;
				int k;
				for (k = i - 1; k >= 0; k--) {
					if (platform.image[k][j] == '.') {
						moved = true;
						continue;
					} else {
						break;
					}
				}
				if (moved) {
					platform.image[k+1][j] = 'O';
					platform.image[i][j] = '.';
				}
			}
		}
	}
}


void tiltEast() {
	// LOGD("tiltEast...");
	for (int i = 0; i < platform.row_count; i++) {
		for (int j = platform.col_count-2; j >= 0; j--) {
			if (platform.image[i][j] == 'O') {
				bool moved = false;
				int k;
				for (k = j + 1; k < platform.col_count; k++) {
					if (platform.image[i][k] == '.') {
						moved = true;
						continue;
					} else {
						break;
					}
				}
				if (moved) {
					platform.image[i][k-1] = 'O';
					platform.image[i][j] = '.';
				}
			}
		}
	}
}


void tiltSouth() {
	// LOGD("tiltSouth...");
	for (int i = platform.row_count-2; i >= 0; i--) {
		for (int j = 0; j < platform.col_count; j++) {
			if (platform.image[i][j] == 'O') {
				bool moved = false;
				int k;
				for (k = i + 1; k < platform.row_count; k++) {
					if (platform.image[k][j] == '.') {
						moved = true;
						continue;
					} else {
						break;
					}
				}
				if (moved) {
					platform.image[k-1][j] = 'O';
					platform.image[i][j] = '.';
				}
			}
		}
	}
}


void tiltWest() {
	// LOGD("tiltWest...");
	for (int i = 0; i < platform.row_count; i++) {
		for (int j = 1; j < platform.col_count; j++) {
			if (platform.image[i][j] == 'O') {
				bool moved = false;
				int k;
				for (k = j - 1; k >= 0; k--) {
					if (platform.image[i][k] == '.') {
						moved = true;
						continue;
					} else {
						break;
					}
				}
				if (moved) {
					platform.image[i][k+1] = 'O';
					platform.image[i][j] = '.';
				}
			}
		}
	}
}


void calculateLoad() {

	int load = 0;

	for (int i = 0; i < platform.row_count; i++) {
		for (int j = 0; j < platform.col_count; j++) {
			if (platform.image[i][j] == 'O') {
				load += (platform.row_count - i);
			}
		}
	}

	LOGD("load: %d", load);
}


void calculateHash() {

	uint64_t hash = 0;

	for (int i = 0; i < platform.row_count; i++) {
		for (int j = 0; j < platform.col_count; j++) {
			if (platform.image[i][j] == 'O') {
				hash = 37 * hash + 43;
			} else if (platform.image[i][j] == '.') {
				hash = 37 * hash + 47;
			} else if (platform.image[i][j] == '#') {
				hash = 37 * hash + 53;
			} else {
				assert(false);
			}
		}
	}

	LOGD("hash: %" PRIu64, hash);
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

			tosearch = newline + 1;

		} else {

			parsePlatform(buf, newline);

			break;
		}
	}

	
	printPlatform();
	
	for (int i = 1; i < 200; i++) {
		tiltNorth();
		tiltWest();
		tiltSouth();
		tiltEast();
		LOGD("after cycle %d", i);
		calculateHash();
		calculateLoad();
	}

  	return 0;
}








