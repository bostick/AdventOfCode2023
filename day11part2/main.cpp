
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


// const int FACTOR = 100;
const int FACTOR = 1000000;


int row_count = 0;
int col_count = 0;

char **image;

std::vector<int> empty_rows;
std::vector<int> empty_cols;

// char **image_expanded;
// std::vector<std::vector<char>> image_expanded;

struct Pair {
	int row;
	int col;
};

std::vector<Pair> galaxies;



void printImage() {
	for (int i = 0; i < row_count; i++) {
		std::string row;
		for (int j = 0; j < col_count; j++) {
			row += image[i][j];
		}
		LOGD("%s", row.c_str());
	}
}



void findEmptySpace() {

	for (int i = 0; i < row_count; i++) {
		bool empty_row = true;
		for (int j = 0; j < col_count; j++) {
			if (image[i][j] != '.') {
				empty_row = false;
				break;
			}
		}
		if (empty_row) {
			empty_rows.push_back(i);
		}
	}

	for (int i = 0; i < col_count; i++) {
		bool empty_col = true;
		for (int j = 0; j < row_count; j++) {
			if (image[j][i] != '.') {
				empty_col = false;
				break;
			}
		}
		if (empty_col) {
			empty_cols.push_back(i);
		}
	}

	for (int i = 0; i < row_count; i++) {
		if (find(empty_rows.begin(), empty_rows.end(), i) != empty_rows.end()) {
			for (int j = 0; j < col_count; j++) {
				image[i][j] = 'E';
			}
		}
	}

	for (int i = 0; i < col_count; i++) {
		if (find(empty_cols.begin(), empty_cols.end(), i) != empty_cols.end()) {
			for (int j = 0; j < row_count; j++) {
				if (image[j][i] == 'E') {
					image[j][i] = 'F';
				} else {
					image[j][i] = 'E';
				}
			}
		}
	}

	LOGD("expanded:");
	printImage();
	LOGD();
}


void findGalaxies() {

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			if (image[i][j] == '#') {
				galaxies.push_back(Pair{i, j});
			}
		}
	}

	LOGD("galaxies:");
	LOGD("{");
	for (int i = 0; i < galaxies.size(); i++) {
		auto g = galaxies[i];
		LOGD("%d -> {%d, %d},", i, g.row, g.col);
	}
	LOGD("}");
}


void computeGalaxyPaths() {

	uint64_t total_dist = 0;

	for (int i = 0; i < galaxies.size() - 1; i++) {
		auto a = galaxies[i];
		for (int j = i + 1; j < galaxies.size(); j++) {
			// LOGD("computing galaxy %d to galaxy %d", i, j);
			uint64_t distance = 0;
			auto b = galaxies[j];
			auto current_location = a;
			//
			// first go to correct row
			//
			if (current_location.row < b.row) {
				for (int k = current_location.row + 1; k < b.row; k++) {
					if (image[k][current_location.col] == '.' || image[k][current_location.col] == '#') {
						distance += 1;
					} else if (image[k][current_location.col] == 'E') {
						distance += FACTOR;
					} else if (image[k][current_location.col] == 'F') {
						assert(false);
						distance += FACTOR;
					} else {
						assert(false);
					}
				}

				distance += 1;

			} else if (current_location.row > b.row) {
				for (int k = current_location.row - 1; k > b.row; k--) {
					if (image[k][current_location.col] == '.' || image[k][current_location.col] == '#') {
						distance += 1;
					} else if (image[k][current_location.col] == 'E') {
						distance += FACTOR;
					} else if (image[k][current_location.col] == 'F') {
						assert(false);
						distance += FACTOR;
					} else {
						assert(false);
					}
				}

				distance += 1;
			}

			current_location.row = b.row;
			// assert(current_location.row == b.row);

			if (current_location.col != b.col) {
				if (image[current_location.row][current_location.col] == 'E') {
					assert(false);
				}
				if (image[current_location.row][current_location.col] == 'F') {
					assert(false);
				}
			}

			//
			// then go to correct col
			//
			if (current_location.col < b.col) {
				for (int k = current_location.col + 1; k < b.col; k++) {
					if (image[current_location.row][k] == '.' || image[current_location.row][k] == '#') {
						distance += 1;
					} else if (image[current_location.row][k] == 'E') {
						distance += FACTOR;
					} else if (image[current_location.row][k] == 'F') {
						assert(false);
						distance += FACTOR;
					} else {
						assert(false);
					}
				}

				distance += 1;

			} else if (current_location.col > b.col) {
				for (int k = current_location.col - 1; k > b.col; k--) {
					if (image[current_location.row][k] == '.' || image[current_location.row][k] == '#') {
						distance += 1;
					} else if (image[current_location.row][k] == 'E') {
						distance += FACTOR;
					} else if (image[current_location.row][k] == 'F') {
						assert(false);
						distance += FACTOR;
					} else {
						assert(false);
					}
				}

				distance += 1;
			}

			current_location.col = b.col;
			// assert(current_location.col == b.col);

			LOGD("distance galaxy %d to galaxy %d: %" PRId64, i, j, distance);

			total_dist += distance;
		}
	}

	LOGD("total_dist: %" PRId64, total_dist);
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

			col_count = len;

			row_count++;

			tosearch = newline + 1;

			// LOGD("");

		} else {

			break;
		}
	}


	image = (char **)malloc(row_count * sizeof(char *));
	for (int i = 0; i < row_count; i++) {
		image[i] = (char *)malloc(col_count * sizeof(char));
	}




	int row = 0;

	tosearch = buf;

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto len = newline - tosearch;
			
			assert(len == col_count);

			// auto line = new char[len + 1];

			// memcpy(line, tosearch, len);

			// line[len] = '\0';

			// LOGD("line: %s", line);
			// LOGD("len: %ld", newline - tosearch);


			for (int i = 0; i < len; i++) {
				image[row][i] = tosearch[i];
			}


			tosearch = newline + 1;

			row++;

			// LOGD("");

		} else {

			break;
		}
	}

	assert(row == row_count);

	LOGD("image:");
	printImage();
	LOGD();

	findEmptySpace();


	// expandSpace();


	findGalaxies();


	computeGalaxyPaths();


  	return 0;
}








