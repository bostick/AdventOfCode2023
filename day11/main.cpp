
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



int row_count = 0;
int col_count = 0;

char **image;

std::vector<int> empty_rows;
std::vector<int> empty_cols;

// char **image_expanded;
std::vector<std::vector<char>> image_expanded;

struct Pair {
	int row;
	int col;
};

std::vector<Pair> galaxies;



void printImage() {
	LOGD("image:");
	for (int i = 0; i < row_count; i++) {
		std::string row;
		for (int j = 0; j < col_count; j++) {
			row += image[i][j];
		}
		LOGD("%s", row.c_str());
	}
	LOGD();
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

	LOGD("image #2:");
	std::string row1;
	row1 += ' ';
	for (int i = 0; i < col_count; i++) {
		if (find(empty_cols.begin(), empty_cols.end(), i) != empty_cols.end()) {
			row1 += 'V';
		} else {
			row1 += ' ';
		}
	}
	LOGD("%s", row1.c_str());
	for (int i = 0; i < row_count; i++) {
		std::string row;
		if (find(empty_rows.begin(), empty_rows.end(), i) != empty_rows.end()) {
			row += '>';
		} else {
			row += ' ';
		}
		for (int j = 0; j < col_count; j++) {
			row += image[i][j];
		}
		LOGD("%s", row.c_str());
	}
	LOGD();
}


void expandSpace() {

	// image_expanded = (char **)malloc((row_count + empty_rows.size()) * sizeof(char *));
	// for (int i = 0; i < row_count + empty_rows.size(); i++) {
	// 	image_expanded[i] = (char *)malloc((col_count + empty_cols.size()) * sizeof(char));
	// }

	// for (int i = 0; i < row_count; i++) {

	// }

	for (int i = 0; i < row_count; i++) {

		if (find(empty_rows.begin(), empty_rows.end(), i) != empty_rows.end()) {
			
			std::vector<char> row_vector;

			for (int j = 0; j < col_count; j++) {
				if (find(empty_cols.begin(), empty_cols.end(), j) != empty_cols.end()) {
					row_vector.push_back('.');
					row_vector.push_back('.');
				} else {
					row_vector.push_back('.');
				}
			}

			assert(row_vector.size() == col_count + empty_cols.size());

			image_expanded.push_back(row_vector);
			image_expanded.push_back(row_vector);

		} else {

			std::vector<char> row_vector;

			for (int j = 0; j < col_count; j++) {
				if (find(empty_cols.begin(), empty_cols.end(), j) != empty_cols.end()) {
					row_vector.push_back('.');
					row_vector.push_back('.');
				} else {
					row_vector.push_back(image[i][j]);
				}
			}

			// LOGD("row_vector.size(): %zu", row_vector.size());
			// LOGD("col_count: %d", col_count);
			// LOGD("empty_cols.size(): %zu", empty_cols.size());
			// LOGD("col_count + empty_cols.size(): %zu", col_count + empty_cols.size());
			assert(row_vector.size() == col_count + empty_cols.size());

			image_expanded.push_back(row_vector);
		}
	}

	assert(image_expanded.size() == row_count + empty_rows.size());


	LOGD("expanded:");
	for (int i = 0; i < image_expanded.size(); i++) {
		auto expanded_row = image_expanded[i];
		std::string row_str;
		for (int j = 0; j < expanded_row.size(); j++) {
			auto expanded_col = expanded_row[j];
			row_str += expanded_col;
		}
		LOGD("%s", row_str.c_str());
	}
	LOGD();
}


void findGalaxiesInExpandedSpace() {

	for (int i = 0; i < image_expanded.size(); i++) {
		auto expanded_row = image_expanded[i];
		for (int j = 0; j < expanded_row.size(); j++) {
			if (expanded_row[j] == '#') {
				galaxies.push_back(Pair{i, j});
			}
		}
	}

	LOGD("galaxies:");
	LOGD("{");
	for (auto g : galaxies) {
		LOGD("{%d, %d},", g.row, g.col);
	}
	LOGD("}");
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

	printImage();


	findEmptySpace();


	expandSpace();


	findGalaxiesInExpandedSpace();


  	return 0;
}








