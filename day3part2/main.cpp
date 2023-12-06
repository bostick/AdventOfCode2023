
#include "common/common.h"

#include <optional>
#include <vector>
#include <set>
#include <map>

#include <cstdio>
#include <cinttypes>
#include <cstring>
#include <cassert>


// 
// symbols = {"@", "#", "%", "&", "*", "-", "+", "=", "/", "$"}
// 

std::vector<int> get_gear_cols(const char *line) {

	std::vector<int> cols;

	for (int i = 0; line[i] != '\0'; i++) {
		switch (line[i]) {
		// case '@':
		// case '#':
		// case '%':
		// case '&':
		case '*':
		// case '-':
		// case '+':
		// case '=':
		// case '/':
		// case '$':
			cols.push_back(i);
			break;
		}
	}

	return cols;
}


struct PartNumber {
	int num;
	int row;
	int col_start;
	int col_end;
};

bool operator<(const PartNumber& l, const PartNumber& r) {
	if (l.row < r.row) {
		return true;
	}
	if (l.row > r.row) {
		return false;
	}
	if (l.col_start < r.col_start) {
		return true;
	}
	if (l.col_start > r.col_start) {
		return false;
	}

	return false;
}



std::vector<PartNumber> get_part_numbers(const char *line, int line_number) {

	std::vector<PartNumber> part_numbers;

	bool is_part_number = false;
	int num = 0;
	int col_start;

	int i;
	for (i = 0; line[i] != '\0'; i++) {
		switch (line[i]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (is_part_number) {
				num = 10 * num + (int)(line[i] - '0');
			} else {
				num = (int)(line[i] - '0');
				col_start = i;
				is_part_number = true;
			}
			break;
		default:
			if (is_part_number) {
				//
				// done with part number
				//
				part_numbers.push_back(PartNumber{num, line_number, col_start, i-1});

				// reset
				is_part_number = false;
				num = 0;
			} else {
				;
			}
			break;
		}
	}

	if (is_part_number) {
		//
		// done with part number
		//
		part_numbers.push_back(PartNumber{num, line_number, col_start, i-1});
	}

	return part_numbers;
}



struct RowCol {
	int row;
	int col;
};


bool operator<(const RowCol& l, const RowCol& r) {
	if (l.row < r.row) {
		return true;
	}
	if (l.row > r.row) {
		return false;
	}
	if (l.col < r.col) {
		return true;
	}
	if (l.col > r.col) {
		return false;
	}

	return false;
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

	const char *newline = strchr(tosearch, '\n');

	int total_cols = newline - tosearch;

	int total_rows = 0;

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			total_rows++;

			tosearch = newline + 1;

		} else {

			break;
		}
	}

	// LOGI("total_rows total_cols: %d %d", total_rows, total_cols);





	tosearch = buf;

	int line_number = 0;

	std::map<RowCol, std::vector<RowCol>> gear_neighbors;
	// std::set<RowCol> part_number_neighbors;

	while (1) {
		
		// LOGI("loop");

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto line = new char[newline - tosearch + 1];

			memcpy(line, tosearch, newline - tosearch);

			line[newline - tosearch] = '\0';

			// fprintf(stderr, "line: %s\n", line);


			auto cols = get_gear_cols(line);

			for (int c : cols) {
				// LOGI("symbol (r,c): %d %d", line_number, c);

				if (line_number >= 1) {

					if (c >= 1) {
						// gear_neighbors.insert(RowCol{line_number-1, c-1});
						auto& gears = gear_neighbors[RowCol{line_number-1, c-1}];
						gears.push_back(RowCol{line_number, c});
					}

					// gear_neighbors.insert(RowCol{line_number-1, c});
					auto& gears = gear_neighbors[RowCol{line_number-1, c}];
					gears.push_back(RowCol{line_number, c});

					if (c <= total_cols-2) {
						// gear_neighbors.insert(RowCol{line_number-1, c+1});
						auto& gears = gear_neighbors[RowCol{line_number-1, c+1}];
						gears.push_back(RowCol{line_number, c});
					}
				}	

				if (c >= 1) {
					// gear_neighbors.insert(RowCol{line_number, c-1});
					auto& gears = gear_neighbors[RowCol{line_number, c-1}];
					gears.push_back(RowCol{line_number, c});
				}

				if (c <= total_cols-2) {
					// gear_neighbors.insert(RowCol{line_number, c+1});
					auto& gears = gear_neighbors[RowCol{line_number, c+1}];
					gears.push_back(RowCol{line_number, c});
				}

				if (line_number <= total_rows-2) {

					if (c >= 1) {
						// gear_neighbors.insert(RowCol{line_number+1, c-1});
						auto& gears = gear_neighbors[RowCol{line_number+1, c-1}];
						gears.push_back(RowCol{line_number, c});
					}

					// gear_neighbors.insert(RowCol{line_number+1, c});
					auto& gears = gear_neighbors[RowCol{line_number+1, c}];
					gears.push_back(RowCol{line_number, c});

					if (c <= total_cols-2) {
						// gear_neighbors.insert(RowCol{line_number+1, c+1});
						auto& gears = gear_neighbors[RowCol{line_number+1, c+1}];
						gears.push_back(RowCol{line_number, c});
					}
				}
			}

			line_number++;

			tosearch = newline + 1;

		} else {

			break;
		}
	}



	tosearch = buf;

	line_number = 0;

	std::vector<PartNumber> part_numbers;

	while (1) {
		
		// LOGI("loop");

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto line = new char[newline - tosearch + 1];

			memcpy(line, tosearch, newline - tosearch);

			line[newline - tosearch] = '\0';

			fprintf(stderr, "line: %s\n", line);
			LOGD("get_part_numbers for line %d", line_number);
			auto part_numbers_on_line = get_part_numbers(line, line_number);

			for (auto part_number : part_numbers_on_line) {
				LOGI("part_number on line: num: %d, row: %d, col_start: %d, col_end: %d", part_number.num, part_number.row, part_number.col_start, part_number.col_end);
			}

			for (auto part_number : part_numbers_on_line) {
				part_numbers.push_back(part_number);
			}

			line_number++;

			tosearch = newline + 1;

		} else {

			break;
		}
	}




	// int sum = 0;

	std::map<RowCol, std::set<PartNumber>> gear_count_map;

	for (auto part_number : part_numbers) {

		// bool neighboring_a_gear = false;

		for (int col = part_number.col_start; col <= part_number.col_end; col++) {
			
			int row = part_number.row;

			auto& gears = gear_neighbors[RowCol{row, col}];

			if (!gears.empty()) {

				// LOGI("part number at row col: %d %d has gear neighbors", row, col);

				for (RowCol g : gears) {

					// LOGI("part number at row col: %d %d has gear neighbor %d %d", row, col, g.row, g.col);

					auto& part_number_set = gear_count_map[g];
					part_number_set.insert(part_number);
				}
			}
		}

		// LOGD("");

		// LOGI("part_number: num: %d, neighboring_a_gear: %d", part_number.num, neighboring_a_gear);

		// LOGD("");
		// LOGD("");

		// if (neighboring_a_gear) {
		// 	sum = sum + part_number.num;
		// 	LOGD("sum: %d", sum);
		// }
	}



	int sum = 0;

	for (auto &p : gear_count_map) {
		auto g = p.first;
		auto& part_number_set = p.second;
		// LOGI("gear: row col: %d %d, count: %zu", g.row, g.col, part_number_set.size());

		if (part_number_set.size() == 2) {

			int ratio = 1;

			for (auto& part_number : part_number_set) {
				ratio = ratio * part_number.num;
			}

			LOGI("gear: row col: %d %d, ratio: %d", g.row, g.col, ratio);

			sum = sum + ratio;
		}
	}

	LOGI("sum: %d", sum);


  	return 0;
}








