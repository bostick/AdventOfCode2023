
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



enum Direction {
	UNUSED_DIRECTION,
	
	LEFT_TO_RIGHT,
	RIGHT_TO_LEFT,

	TOP_TO_BOTTOM,
	BOTTOM_TO_TOP,

	BOTTOM_TO_RIGHT,
	RIGHT_TO_BOTTOM,

	LEFT_TO_BOTTOM,
	BOTTOM_TO_LEFT,

	LEFT_TO_TOP,
	TOP_TO_LEFT,

	TOP_TO_RIGHT,
	RIGHT_TO_TOP,
};

void printDirection(Direction d) {
	switch (d) {
	case LEFT_TO_RIGHT:
		LOGD("dir: LEFT_TO_RIGHT");
		break;
	case RIGHT_TO_LEFT:
		LOGD("dir: RIGHT_TO_LEFT");
		break;
	case TOP_TO_BOTTOM:
		LOGD("dir: TOP_TO_BOTTOM");
		break;
	case BOTTOM_TO_TOP:
		LOGD("dir: BOTTOM_TO_TOP");
		break;
	case BOTTOM_TO_RIGHT:
		LOGD("dir: BOTTOM_TO_RIGHT");
		break;
	case RIGHT_TO_BOTTOM:
		LOGD("dir: RIGHT_TO_BOTTOM");
		break;
	case LEFT_TO_BOTTOM:
		LOGD("dir: LEFT_TO_BOTTOM");
		break;
	case BOTTOM_TO_LEFT:
		LOGD("dir: BOTTOM_TO_LEFT");
		break;
	case LEFT_TO_TOP:
		LOGD("dir: LEFT_TO_TOP");
		break;
	case TOP_TO_LEFT:
		LOGD("dir: TOP_TO_LEFT");
		break;
	case TOP_TO_RIGHT:
		LOGD("dir: TOP_TO_RIGHT");
		break;
	case RIGHT_TO_TOP:
		LOGD("dir: RIGHT_TO_TOP");
		break;
	case UNUSED_DIRECTION:
		assert(false);
		break;
	}
}


// enum CameFrom {
// 	UNUSED_CAMEFROM,
// 	TOP,
// 	BOTTOM,
// 	LEFT,
// 	RIGHT,
// };

// void printCameFrom(CameFrom c) {
// 	switch (c) {
// 	case LEFT:
// 		LOGD("came_from: LEFT");
// 		break;
// 	case RIGHT:
// 		LOGD("came_from: RIGHT");
// 		break;
// 	case TOP:
// 		LOGD("came_from: TOP");
// 		break;
// 	case BOTTOM:
// 		LOGD("came_from: BOTTOM");
// 		break;
// 	case UNUSED_CAMEFROM:
// 		assert(false);
// 		break;
// 	}
// }




// enum Test {
// 	UNUSED_TEST,
// 	INSIDE,
// 	OUTSIDE,
// };


struct Tile {
	char input;
	int row;
	int col;
	Direction direction;
	int test_count;
};


bool operator==(const Tile& l, const Tile& r) {
	return l.row == r.row && l.col == r.col;
}

bool operator<(const Tile& l, const Tile& r) {
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



Tile **tile_map;

Tile **tile_map_new;

Tile s_tile;

std::map<Tile, int> distance_map;




void printTile(Tile t) {
	LOGD("tile: input: %c row col: %d %d", t.input, t.row, t.col);
}


Tile single_neighbor_to_proceed(Tile t) {
		
	// auto row = t.row;
	// auto col = t.col;
	// auto input = t.input;

	Tile candidate1;
	Tile candidate2;

	switch (t.input) {
	case 'L': {

		candidate1 = tile_map[t.row-1][t.col];
		candidate2 = tile_map[t.row][t.col+1];

		if (distance_map.find(candidate1) == distance_map.end()) {
			// candidate1 not in distance_map
			assert(distance_map.find(candidate2) != distance_map.end());
			return candidate1;
		} else {
			assert(distance_map.find(candidate1) != distance_map.end());
			assert(distance_map.find(candidate2) == distance_map.end());
			return candidate2;
		}

		break;
	}
	case '-': {

		candidate1 = tile_map[t.row][t.col-1];
		candidate2 = tile_map[t.row][t.col+1];

		if (distance_map.find(candidate1) == distance_map.end()) {
			// candidate1 not in distance_map
			assert(distance_map.find(candidate2) != distance_map.end());
			return candidate1;
		} else {
			assert(distance_map.find(candidate1) != distance_map.end());
			assert(distance_map.find(candidate2) == distance_map.end());
			return candidate2;
		}

		break;
	}
	case 'F': {

		candidate1 = tile_map[t.row+1][t.col];
		candidate2 = tile_map[t.row][t.col+1];

		if (distance_map.find(candidate1) == distance_map.end()) {
			// candidate1 not in distance_map
			assert(distance_map.find(candidate2) != distance_map.end());
			return candidate1;
		} else {
			assert(distance_map.find(candidate1) != distance_map.end());
			assert(distance_map.find(candidate2) == distance_map.end());
			return candidate2;
		}

		break;
	}
	case 'J': {

		candidate1 = tile_map[t.row-1][t.col];
		candidate2 = tile_map[t.row][t.col-1];

		if (distance_map.find(candidate1) == distance_map.end()) {
			// candidate1 not in distance_map
			assert(distance_map.find(candidate2) != distance_map.end());
			return candidate1;
		} else {
			assert(distance_map.find(candidate1) != distance_map.end());
			assert(distance_map.find(candidate2) == distance_map.end());
			return candidate2;
		}

		break;
	}
	case '7': {

		candidate1 = tile_map[t.row+1][t.col];
		candidate2 = tile_map[t.row][t.col-1];

		if (distance_map.find(candidate1) == distance_map.end()) {
			// candidate1 not in distance_map
			assert(distance_map.find(candidate2) != distance_map.end());
			return candidate1;
		} else {
			assert(distance_map.find(candidate1) != distance_map.end());
			assert(distance_map.find(candidate2) == distance_map.end());
			return candidate2;
		}

		break;
	}
	case '|': {

		candidate1 = tile_map[t.row-1][t.col];
		candidate2 = tile_map[t.row+1][t.col];

		if (distance_map.find(candidate1) == distance_map.end()) {
			// candidate1 not in distance_map
			assert(distance_map.find(candidate2) != distance_map.end());
			return candidate1;
		} else {
			assert(distance_map.find(candidate1) != distance_map.end());
			assert(distance_map.find(candidate2) == distance_map.end());
			return candidate2;
		}

		break;
	}
	default:
		LOGD("t.input: %c", t.input);
		assert(false);
		return Tile{};
	}
}


Tile single_neighbor_to_proceed_direction(Tile t) {
		
	// auto row = t.row;
	// auto col = t.col;
	// auto input = t.input;

	Tile candidate1;
	Tile candidate2;

	switch (t.input) {
	case 'L': {

		candidate1 = tile_map_new[t.row-1][t.col];
		candidate2 = tile_map_new[t.row][t.col+1];

		if (candidate1.direction == 0) {
			assert(candidate2.direction != 0);
			return candidate1;
		} else {
			assert(candidate2.direction == 0);
			return candidate2;
		}

		break;
	}
	case '-': {

		candidate1 = tile_map_new[t.row][t.col-1];
		candidate2 = tile_map_new[t.row][t.col+1];

		if (candidate1.direction == 0) {
			assert(candidate2.direction != 0);
			return candidate1;
		} else {
			assert(candidate2.direction == 0);
			return candidate2;
		}

		break;
	}
	case 'F': {

		candidate1 = tile_map_new[t.row+1][t.col];
		candidate2 = tile_map_new[t.row][t.col+1];

		if (candidate1.direction == 0) {
			assert(candidate2.direction != 0);
			return candidate1;
		} else {
			assert(candidate2.direction == 0);
			return candidate2;
		}

		break;
	}
	case 'J': {

		candidate1 = tile_map_new[t.row-1][t.col];
		candidate2 = tile_map_new[t.row][t.col-1];

		if (candidate1.direction == 0) {
			assert(candidate2.direction != 0);
			return candidate1;
		} else {
			assert(candidate2.direction == 0);
			return candidate2;
		}

		break;
	}
	case '7': {

		candidate1 = tile_map_new[t.row+1][t.col];
		candidate2 = tile_map_new[t.row][t.col-1];

		if (candidate1.direction == 0) {
			assert(candidate2.direction != 0);
			return candidate1;
		} else {
			assert(candidate2.direction == 0);
			return candidate2;
		}

		break;
	}
	case '|': {

		candidate1 = tile_map_new[t.row-1][t.col];
		candidate2 = tile_map_new[t.row+1][t.col];

		if (candidate1.direction == 0) {
			assert(candidate2.direction != 0);
			return candidate1;
		} else {
			assert(candidate2.direction == 0);
			return candidate2;
		}

		break;
	}
	default:
		LOGD("t.input: %c", t.input);
		assert(false);
		return Tile{};
	}
}


char examine_s_tile() {

	auto row = s_tile.row;
	auto col = s_tile.col;

	if (tile_map[row][col+1].input == '-' && tile_map[row+1][col].input == '|') {
		return 'F';
	}

	if (tile_map[row][col-1].input == 'F' && tile_map[row][col+1].input == '-') {
		return '-';
	}


	assert(false);
	return '@';
}


std::vector<Tile> both_neighbors_to_proceed(Tile t) {
		
	// auto row = t.row;
	// auto col = t.col;
	// auto input = t.input;

	Tile candidate1;
	Tile candidate2;

	std::vector<Tile> ns;

	char input = examine_s_tile();

	switch (input) {
	case '-': {

		// move clockwise
		candidate1 = tile_map[t.row][t.col+1];
		candidate2 = tile_map[t.row][t.col-1];

		ns.push_back(candidate1);
		ns.push_back(candidate2);

		break;
	}
	case 'F': {

		// move clockwise
		candidate1 = tile_map[t.row][t.col+1];
		candidate2 = tile_map[t.row+1][t.col];

		ns.push_back(candidate1);
		ns.push_back(candidate2);

		break;
	}
	default:
		LOGD("input: %c", input);
		assert(false);
		break;
	}

	return ns;
}

std::vector<Tile> both_neighbors_to_proceed_direction(Tile t) {
		
	// auto row = t.row;
	// auto col = t.col;
	// auto input = t.input;

	Tile candidate1;
	Tile candidate2;

	std::vector<Tile> ns;

	char input = examine_s_tile();

	switch (input) {
	case '-': {

		// move clockwise
		candidate1 = tile_map_new[t.row][t.col+1];
		candidate2 = tile_map_new[t.row][t.col-1];

		ns.push_back(candidate1);
		ns.push_back(candidate2);

		break;
	}
	case 'F': {

		// move clockwise
		candidate1 = tile_map_new[t.row][t.col+1];
		candidate2 = tile_map_new[t.row+1][t.col];

		ns.push_back(candidate1);
		ns.push_back(candidate2);

		break;
	}
	default:
		LOGD("input: %c", input);
		assert(false);
		break;
	}

	return ns;
}




std::vector<Tile> neighbors(std::vector<Tile> set1) {
	
	if (set1.size() == 1) {
		if (set1[0].input == 'S') {
			return both_neighbors_to_proceed(set1[0]);
		}
	}

	std::vector<Tile> ns;

	for (auto t : set1) {

		auto n = single_neighbor_to_proceed(t);

		ns.push_back(n);

	}

	return ns;
}



void searchMap() {

	int distance_to_use = 0;

	std::vector<Tile> set1;
	set1.push_back(s_tile);

	for (auto t : set1) {
		distance_map[t] = distance_to_use;
	}
	LOGD("distance_map size: %zu", distance_map.size());

	distance_to_use++;

	while (1) {

		auto set2 = neighbors(set1);
		LOGD("neighbors:");
		for (auto t : set2) {
			printTile(t);
		}
		LOGD();

		assert(set2.size() == 2);

		if (set2[0] == set2[1]) {
			LOGD("both paths have met!!");
			printTile(set2[0]);
			printTile(set2[1]);
			// if (distance_map.find(set2[0]) != distance_map.end()) {
			// 	LOGD("set2[0] distance: %d", distance_map[set2[0]]);
			// }
			// if (distance_map.find(set2[1]) != distance_map.end()) {
			// 	LOGD("set2[1] distance: %d", distance_map[set2[1]]);
			// }
			// break;
			set2.erase(set2.begin() + 1);
		}

		std::vector<Tile> to_remove;
		for (auto t : set2) {
			if (distance_map.find(t) != distance_map.end()) {
				to_remove.push_back(t);
			}
		}
		// LOGD("to_remove size: %zu", to_remove.size());
		for (auto t : to_remove) {
			set2.erase(std::remove(set2.begin(), set2.end(), t), set2.end());
		}

		if (set2.empty()) {
			LOGD("set2 empty");
			break;
		}

		LOGD("distance_to_use: %d", distance_to_use);
		for (auto t : set2) {
			printTile(t);
			distance_map[t] = distance_to_use;
		}
		LOGD("distance_map size: %zu", distance_map.size());
		LOGD();

		if (set2.size() == 1) {
			break;
		}

		distance_to_use++;

		set1 = set2;
	}

	LOGD("DONE!!");
	LOGD();

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





	int row_count = 0;
	int col_count = 0;

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


	tile_map = (Tile **)malloc(row_count * sizeof(Tile *));
	for (int i = 0; i < row_count; i++) {
		tile_map[i] = (Tile *)malloc(col_count * sizeof(Tile));
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
				Tile t;
				t.input = tosearch[i];
				t.row = row;
				t.col = i;
				t.direction = UNUSED_DIRECTION;
				tile_map[row][i] = t;
				if (t.input == 'S') {
					s_tile = t;
				}
			}


			tosearch = newline + 1;

			row++;

			// LOGD("");

		} else {

			break;
		}
	}

	assert(row == row_count);

	printTile(s_tile);


	searchMap();



	tile_map_new = (Tile **)malloc(row_count * sizeof(Tile *));
	for (int i = 0; i < row_count; i++) {
		tile_map_new[i] = (Tile *)malloc(col_count * sizeof(Tile));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			auto t = tile_map[i][j];
			if (distance_map.find(t) == distance_map.end()) {
				t.input = '.';
			}
			tile_map_new[i][j] = t;
		}
	}



	//
	// calculate direction
	//
	{
		auto start_end = both_neighbors_to_proceed_direction(s_tile);

		auto start = start_end[0];
		auto end = start_end[1];

		char input = examine_s_tile();

		switch (input) {
		case '-': {

			tile_map_new[s_tile.row][s_tile.col].direction = LEFT_TO_RIGHT;

			break;
		}
		case 'F': {

			tile_map_new[s_tile.row][s_tile.col].direction = BOTTOM_TO_RIGHT;

			break;
		}
		default:
			LOGD("input: %c", input);
			assert(false);
			break;
		}


		auto t = start;
		auto prev = tile_map_new[s_tile.row][s_tile.col];

		while (1) {
			
			switch (t.input) {
			case '-':
				switch (prev.direction) {
				case BOTTOM_TO_RIGHT:
					t.direction = LEFT_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				case LEFT_TO_RIGHT:
					t.direction = LEFT_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_LEFT:
					t.direction = RIGHT_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_LEFT:
					t.direction = RIGHT_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case BOTTOM_TO_LEFT:
					t.direction = RIGHT_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_RIGHT:
					t.direction = LEFT_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				default:
					printDirection(prev.direction);
					assert(false);
					break;
				}
				break;
			case '7':
				switch (prev.direction) {
				case LEFT_TO_RIGHT:
					t.direction = LEFT_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case BOTTOM_TO_TOP:
					t.direction = BOTTOM_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_RIGHT:
					t.direction = LEFT_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_TOP:
					t.direction = BOTTOM_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case BOTTOM_TO_RIGHT:
					t.direction = LEFT_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case LEFT_TO_TOP:
					t.direction = BOTTOM_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				default:
					printDirection(prev.direction);
					assert(false);
					break;
				}
				break;
			case '|':
				switch (prev.direction) {
				case LEFT_TO_BOTTOM:
					t.direction = TOP_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_BOTTOM:
					t.direction = TOP_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_TOP:
					t.direction = BOTTOM_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				case LEFT_TO_TOP:
					t.direction = BOTTOM_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				case BOTTOM_TO_TOP:
					t.direction = BOTTOM_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_BOTTOM:
					t.direction = TOP_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				default:
					printDirection(prev.direction);
					assert(false);
					break;
				}
				break;
			case 'J':
				switch (prev.direction) {
				case TOP_TO_BOTTOM:
					t.direction = TOP_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case LEFT_TO_RIGHT:
					t.direction = LEFT_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				case LEFT_TO_BOTTOM:
					t.direction = TOP_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_BOTTOM:
					t.direction = TOP_TO_LEFT;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_RIGHT:
					t.direction = LEFT_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				case BOTTOM_TO_RIGHT:
					t.direction = LEFT_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				default:
					printDirection(prev.direction);
					assert(false);
					break;
				}
				break;
			case 'L':
				switch (prev.direction) {
				case RIGHT_TO_LEFT:
					t.direction = RIGHT_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_BOTTOM:
					t.direction = TOP_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				case LEFT_TO_BOTTOM:
					t.direction = TOP_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_BOTTOM:
					t.direction = TOP_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_LEFT:
					t.direction = RIGHT_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				case BOTTOM_TO_LEFT:
					t.direction = RIGHT_TO_TOP;
					tile_map_new[t.row][t.col] = t;
					break;
				default:
					printDirection(prev.direction);
					assert(false);
					break;
				}
				break;
			case 'F':
				switch (prev.direction) {
				case BOTTOM_TO_TOP:
					t.direction = BOTTOM_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_LEFT:
					t.direction = RIGHT_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case BOTTOM_TO_LEFT:
					t.direction = RIGHT_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case TOP_TO_LEFT:
					t.direction = RIGHT_TO_BOTTOM;
					tile_map_new[t.row][t.col] = t;
					break;
				case LEFT_TO_TOP:
					t.direction = BOTTOM_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				case RIGHT_TO_TOP:
					t.direction = BOTTOM_TO_RIGHT;
					tile_map_new[t.row][t.col] = t;
					break;
				default:
					printDirection(prev.direction);
					assert(false);
					break;
				}
				break;
			default:
				LOGD("t.input: %c", t.input);
				assert(false);
				break;
			}

			if (t == end) {
				break;
			}

			prev = t;
			t = single_neighbor_to_proceed_direction(t);
		}

		LOGD("all finished directioning!");
	}




	//
	// now calculate crossings
	//
	// inspired by:
	// https://en.wikipedia.org/wiki/Even%E2%80%93odd_rule
	//
	for (int i = 0; i < row_count; i++) {
		
		int test_count = 0;
		Direction came_from = UNUSED_DIRECTION;


		for (int j = 0; j < col_count; j++) {
			
			auto t = tile_map_new[i][j];
			
			auto c = t.input;

			if (c == 'S') {
				c  = examine_s_tile();
			}

			switch (c) {
			case '.':
				t.test_count = test_count;
				tile_map_new[i][j] = t;
				break;
			case 'F':
				switch (t.direction) {
				case BOTTOM_TO_RIGHT:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						// case LEFT_TO_BOTTOM:
						// 	xxx;
						// 	break;
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				case RIGHT_TO_BOTTOM:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				default:
					printDirection(t.direction);
					assert(false);
				}
				break;
			case '-':
				switch (t.direction) {
				case LEFT_TO_RIGHT:
					break;
				case RIGHT_TO_LEFT:
					break;
				default:
					printDirection(t.direction);
					assert(false);
				}
				break;
			case '7':
				switch (t.direction) {
				case LEFT_TO_BOTTOM:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						case BOTTOM_TO_RIGHT:
							// nothing
							came_from = UNUSED_DIRECTION;
							break;
						case TOP_TO_RIGHT:
							test_count++;
							came_from = UNUSED_DIRECTION;
							break;
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				case BOTTOM_TO_LEFT:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						case RIGHT_TO_BOTTOM:
							// nothing
							came_from = UNUSED_DIRECTION;
							break;
						case RIGHT_TO_TOP:
							test_count--;
							came_from = UNUSED_DIRECTION;
							break;
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				default:
					printDirection(t.direction);
					assert(false);
				}
				break;
			case 'L':
				switch (t.direction) {
				case RIGHT_TO_TOP:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				case TOP_TO_RIGHT:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				default:
					printDirection(t.direction);
					assert(false);
				}
				break;
			case 'J':
				switch (t.direction) {
				case TOP_TO_LEFT:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						case RIGHT_TO_TOP:
							// nothing
							came_from = UNUSED_DIRECTION;
							break;
						case RIGHT_TO_BOTTOM:
							test_count++;
							came_from = UNUSED_DIRECTION;
							break;
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				case LEFT_TO_TOP:
					if (came_from == UNUSED_DIRECTION) {
						came_from = t.direction;
					} else {
						switch (came_from) {
						case BOTTOM_TO_RIGHT:
							test_count--;
							came_from = UNUSED_DIRECTION;
							break;
						case TOP_TO_RIGHT:
							// nothing
							came_from = UNUSED_DIRECTION;
							break;
						default:
							printDirection(came_from);
							assert(false);
							break;
						}
					}
					break;
				default:
					printDirection(t.direction);
					assert(false);
				}
				break;
			case '|':
				switch (t.direction) {
				case BOTTOM_TO_TOP:
					test_count--;
					break;
				case TOP_TO_BOTTOM:
					test_count++;
					break;
				default:
					printDirection(t.direction);
					assert(false);
				}
				break;
			default:
				LOGD("row col: %d %d, c: %c", i, j, c);
				assert(false);
				break;
			}
		}
	}


	// // O|L-7IF-J|O

	// // O
	// assert(tile_map_new[5][0].direction == UNUSED_DIRECTION);
	
	// // |
	// assert(tile_map_new[5][1].direction == BOTTOM_TO_TOP);
	// // test--
	
	// // L
	// assert(tile_map_new[5][2].direction == TOP_TO_RIGHT);
	// // test++

	// // -
	// assert(tile_map_new[5][3].direction == LEFT_TO_RIGHT);
	// // 

	// // 7
	// assert(tile_map_new[5][4].direction == LEFT_TO_BOTTOM);
	// // test++

	// // I
	// assert(tile_map_new[5][5].direction == UNUSED_DIRECTION);
	// // 

	// // F
	// assert(tile_map_new[5][6].direction == BOTTOM_TO_RIGHT);
	// // test--

	// // -
	// assert(tile_map_new[5][7].direction == LEFT_TO_RIGHT);
	// // 

	// // J
	// assert(tile_map_new[5][8].direction == LEFT_TO_TOP);
	// // test--

	// // |
	// assert(tile_map_new[5][9].direction == TOP_TO_BOTTOM);
	// // test++

	// // O
	// assert(tile_map_new[5][10].direction == UNUSED_DIRECTION);
	// //




	int inside_count = 0;

	LOGD("tile_map_new:");
	for (int i = 0; i < row_count; i++) {
		std::string row;
		for (int j = 0; j < col_count; j++) {
			auto t = tile_map_new[i][j];
			if (t.input == '.') {
				if (t.test_count == 0) {
					row += 'O';
				} else {
					inside_count++;
					row += 'I';
				}
			} else {
				row += t.input;
			}
		}
		LOGD("%s", row.c_str());
	}
	LOGD();

	LOGI("inside_count: %d", inside_count);

  	return 0;
}








