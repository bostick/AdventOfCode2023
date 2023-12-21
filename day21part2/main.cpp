
#include "common/common.h"

#include <optional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <array>
#include <utility>

#include <cstdio>
#include <cinttypes>
#include <cstring>
#include <cassert>
#include <cstdint>




const int FACTOR = 21;




int row_count;
int col_count;

char **proto_board;

char **world_a;
char **world_b;


int start_row;
int start_col;







void printProto() {

	LOGD("proto:");
	LOGD("row_count col_count: %d %d", row_count, col_count);
	std::string str;
	for (int i = 0; i < row_count; i++) {
		str.clear();
		for (int j = 0; j < col_count; j++) {
			str += proto_board[i][j];
		}
		LOGD("%s", str.c_str());
	}
	LOGD();
}






void parseMap(const char *start) {

	LOGD("parsing...");

	row_count = 0;
	col_count = 0;

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

	proto_board = (char **)malloc(row_count * sizeof(char *));
	for (int i = 0; i < row_count; i++) {
		proto_board[i] = (char *)malloc(col_count * sizeof(char));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			auto c = *(start + i * (col_count + 1) + j);
			proto_board[i][j] = c;

			if (c == 'S') {
				start_row = i;
				start_col = j;
			}
		}
	}
}




int good_mod(int a, int b) {

	assert(b > 0);

	while (a > b - 1) {
		a -= b;
	}

	while (a < 0) {
		a += b;
	}

	assert(a >= 0);
	assert(a < b);

	return a;
}



void step(char **world_read, char **world_write) {

	for (int i = 0; i < FACTOR * row_count; i++) {
		for (int j = 0; j < FACTOR * col_count; j++) {
			if (world_read[i][j] == 'O') {

                auto i_mapped = good_mod(i - 1, row_count);
                auto j_mapped = good_mod(j, col_count);
                
				if (proto_board[i_mapped][j_mapped] != '#') {
					world_write[i - 1][j] = 'O';
				}

                i_mapped = good_mod(i, row_count);
                j_mapped = good_mod(j + 1, col_count);
                
				if (proto_board[i_mapped][j_mapped] != '#') {
					world_write[i][j + 1] = 'O';
				}

                i_mapped = good_mod(i + 1, row_count);
                j_mapped = good_mod(j, col_count);
                
				if (proto_board[i_mapped][j_mapped] != '#') {
					world_write[i + 1][j] = 'O';
				}

                i_mapped = good_mod(i, row_count);
                j_mapped = good_mod(j - 1, col_count);
                
				if (proto_board[i_mapped][j_mapped] != '#') {
					world_write[i][j - 1] = 'O';
				}
			}
		}
	}
}



int calculatePositions(char **world_read) {

	int positions = 0;

	for (int i = 0; i < FACTOR * row_count; i++) {
		for (int j = 0; j < FACTOR * col_count; j++) {
			if (world_read[i][j] == 'O') {
				positions++;
			}
		}
	}

	std::array<std::array<int, 13>, 13> positions_array;

	for (int k = -6; k <= 6; k++) {
		for (int l = -6; l <= 6; l++) {

			positions_array[k+6][l+6] = 0;

			for (int i = 0; i < row_count; i++) {
				for (int j = 0; j < col_count; j++) {
					if (world_read[(FACTOR / 2 + k) * row_count + i][(FACTOR / 2 + l) * col_count + j] == 'O') {
						positions_array[k+6][l+6]++;
					}
				}
			}

		}
	}


	// for (int i = 0; i < 13; i++) {
	// 	LOGD("%4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d %4d",
	// 		positions_array[i][0], positions_array[i][1], positions_array[i][2],
	// 		positions_array[i][3], positions_array[i][4], positions_array[i][5],
	// 		positions_array[i][6], positions_array[i][7], positions_array[i][8],
	// 		positions_array[i][9], positions_array[i][10], positions_array[i][11], positions_array[i][12]);
	// }

	return positions;
}



int main(int argc, char **argv) {

	assert(argc == 3);

	const char *path = argv[1];

	int step_count;
	auto res2 = sscanf(argv[2], "%d", &step_count);
	assert(res2 == 1);


	FILE *file = fopen(path, "r");

	if (!file) {

		LOGE("cannot open %s\n", path);

		return 1;
	}

	if (fseek(file, 0, SEEK_END)) {

		LOGE("fseek failed");

		return 1;
	}

	long res1 = ftell(file);

	if (res1 < 0) {

		LOGE("ftell failed");

		return 1;
	}

	size_t len = static_cast<size_t>(res1);

	rewind(file);

	auto buf = new char[len + 1];

	size_t r = fread(buf, sizeof(char), len, file);

	if (r != len) {

		LOGE("fread failed");

		return 1;
	}

	fclose(file);

	buf[len] = '\0';


	parseMap(buf);

	printProto();


	world_a = (char **)malloc(FACTOR * row_count * sizeof(char *));
	world_b = (char **)malloc(FACTOR * row_count * sizeof(char *));
	for (int i = 0; i < FACTOR * row_count; i++) {
		world_a[i] = (char *)malloc(FACTOR * col_count * sizeof(char));
		world_b[i] = (char *)malloc(FACTOR * col_count * sizeof(char));
	}

	for (int i = 0; i < FACTOR * row_count; i++) {
		for (int j = 0; j < FACTOR * col_count; j++) {
			world_a[i][j] = ' ';
		}
	}

	// int center_row = (FACTOR / 2) * row_count + start_row;
	// int center_col = (FACTOR / 2) * col_count + start_col;
	int center_row = (FACTOR / 2) * row_count + start_row;
	int center_col = (FACTOR / 2) * col_count + start_col;
	world_a[center_row][center_col] = 'O';

	for (int i = 0; i < step_count; i++) {

		char **world_read;
		char **world_write;

        if (i % 2 == 0) {
            world_read = world_a;
            world_write = world_b;
		} else {
			world_read = world_b;
            world_write = world_a;
		}
        
        step(world_read, world_write);

        // int count_of_tile = calculateCountOfTile(world_b);
        // LOGD("step %d: count_of_tile: %d", i, count_of_tile);

        if ((i - (row_count / 2) + 1) % row_count == 0) {

        	LOGD("after step %d: positions:", i);

	        auto positions = calculatePositions(world_write);

	        LOGD("total %d", positions);

	        LOGD();
        }
	}

  	return 0;
}








