
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
	std::vector<int> horizontal_candidates_0;
	std::vector<int> horizontal_candidates_1;

	for (int i = 0; i < p.row_count-1; i++) {
		int bads = 0;
		for (int j = 0; j < p.col_count; j++) {
			if (p.image[i][j] != p.image[i+1][j]) {
				bads++;
				// break;
			}
		}
		if (bads == 0) {
			horizontal_candidates_0.push_back(i);
		} else if (bads == 1) {
			horizontal_candidates_1.push_back(i);
		}
	}

	//
	// scan pairs of cols
	//
	std::vector<int> vertical_candidates_0;
	std::vector<int> vertical_candidates_1;

	for (int i = 0; i < p.col_count-1; i++) {
		int bads = 0;
		for (int j = 0; j < p.row_count; j++) {
			if (p.image[j][i] != p.image[j][i+1]) {
				bads++;
				// break;
			}
		}
		if (bads == 0) {
			vertical_candidates_0.push_back(i);
		} else if (bads == 1) {
			vertical_candidates_1.push_back(i);
		}
	}

	LOGD("horizontal_candidates_0:");
	for (auto h : horizontal_candidates_0) {
		LOGD("h: %d", h);
	}
	LOGD();

	LOGD("horizontal_candidates_1:");
	for (auto h : horizontal_candidates_1) {
		LOGD("h: %d", h);
	}
	LOGD();

	LOGD("vertical_candidates_0:");
	for (auto v : vertical_candidates_0) {
		LOGD("v: %d", v);
	}
	LOGD();

	LOGD("vertical_candidates_1:");
	for (auto v : vertical_candidates_1) {
		LOGD("v: %d", v);
	}
	LOGD();

	int count = 0;

	for (auto h : horizontal_candidates_0) {
		//
		// row h and h + 1 already match
		//

		int offset = 1;
		int bads = 0;
		while (1) {
			if (h - offset <= -1) {
				break;
			}
			if (h + 1 + offset >= p.row_count) {
				break;
			}

			for (int j = 0; j < p.col_count; j++) {
				if (p.image[h - offset][j] != p.image[h + 1 + offset][j]) {
					bads++;
					// break;
				}
			}

			offset++;
		}
		if (bads == 1) {
			LOGD("%d is good horizontal with smudge", h);
			count += 100 * (h + 1);
		}
	}

	for (auto v : vertical_candidates_0) {
		//
		// col v and v + 1 already match
		//

		int offset = 1;
		int bads = 0;
		while (1) {
			if (v - offset <= -1) {
				break;
			}
			if (v + 1 + offset >= p.col_count) {
				break;
			}

			for (int j = 0; j < p.row_count; j++) {
				if (p.image[j][v - offset] != p.image[j][v + 1 + offset]) {
					bads++;
					// break;
				}
			}

			offset++;
		}
		if (bads == 1) {
			LOGD("%d is good vertical with smudge", v);
			count += (v + 1);
		}
	}




	for (auto h : horizontal_candidates_1) {
		//
		// row h and h + 1 already match
		//

		int offset = 1;
		int bads = 0;
		while (1) {
			if (h - offset <= -1) {
				break;
			}
			if (h + 1 + offset >= p.row_count) {
				break;
			}

			for (int j = 0; j < p.col_count; j++) {
				if (p.image[h - offset][j] != p.image[h + 1 + offset][j]) {
					bads++;
					// break;
				}
			}

			offset++;
		}
		if (bads == 0) {
			LOGD("%d is good horizontal with smudge", h);
			count += 100 * (h + 1);
		}
	}

	for (auto v : vertical_candidates_1) {
		//
		// col v and v + 1 already match
		//

		int offset = 1;
		int bads = 0;
		while (1) {
			if (v - offset <= -1) {
				break;
			}
			if (v + 1 + offset >= p.col_count) {
				break;
			}

			for (int j = 0; j < p.row_count; j++) {
				if (p.image[j][v - offset] != p.image[j][v + 1 + offset]) {
					bads++;
					// break;
				}
			}

			offset++;
		}
		if (bads == 0) {
			LOGD("%d is good vertical with smudge", v);
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








