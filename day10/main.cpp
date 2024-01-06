
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



struct Tile {
	char input;
	int row;
	int col;
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



Tile map[140][140];

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

		candidate1 = map[t.row-1][t.col];
		candidate2 = map[t.row][t.col+1];

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

		candidate1 = map[t.row][t.col-1];
		candidate2 = map[t.row][t.col+1];

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

		candidate1 = map[t.row+1][t.col];
		candidate2 = map[t.row][t.col+1];

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

		candidate1 = map[t.row-1][t.col];
		candidate2 = map[t.row][t.col-1];

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

		candidate1 = map[t.row+1][t.col];
		candidate2 = map[t.row][t.col-1];

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

		candidate1 = map[t.row-1][t.col];
		candidate2 = map[t.row+1][t.col];

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

	// if (row > 0) {
	// 	if (col > 0) {
	// 		neighbors.push_back(map[row-1][col-1]);
	// 	}
	// 	{
	// 		neighbors.push_back(map[row-1][col]);
	// 	}
	// 	if (col < 140 - 1) {
	// 		neighbors.push_back(map[row-1][col+1]);
	// 	}
	// }

	// {
	// 	if (col > 0) {
	// 		neighbors.push_back(map[row][col-1]);
	// 	}
	// 	{
	// 		neighbors.push_back(map[row][col]);
	// 	}
	// 	if (col < 140 - 1) {
	// 		neighbors.push_back(map[row][col+1]);
	// 	}
	// }

	// if (row < 140 - 1) {
	// 	if (col > 0) {
	// 		neighbors.push_back(map[row+1][col-1]);
	// 	}
	// 	{
	// 		neighbors.push_back(map[row+1][col]);
	// 	}
	// 	if (col < 140 - 1) {
	// 		neighbors.push_back(map[row+1][col+1]);
	// 	}
	// }

}

std::vector<Tile> both_neighbors_to_proceed(Tile t) {
		
	// auto row = t.row;
	// auto col = t.col;
	// auto input = t.input;

	Tile candidate1;
	Tile candidate2;

	std::vector<Tile> ns;

	switch (t.input) {
	case 'S':
	case '-': {

		candidate1 = map[t.row][t.col-1];
		candidate2 = map[t.row][t.col+1];

		ns.push_back(candidate1);
		ns.push_back(candidate2);

		break;
	}
	default:
		LOGD("t.input: %c", t.input);
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

	LOGI("distance_to_use: %d", distance_to_use);

	LOGD("DONE!!");

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




	

	int row = 0;

	const char *tosearch = buf;

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto len = newline - tosearch;
			
			assert(len == 140);

			// auto line = new char[len + 1];

			// memcpy(line, tosearch, len);

			// line[len] = '\0';

			// LOGD("line: %s", line);
			// LOGD("len: %ld", newline - tosearch);


			for (int i = 0; i < 140; i++) {
				Tile t;
				t.input = tosearch[i];
				t.row = row;
				t.col = i;
				map[row][i] = t;
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

	assert(row == 140);

	printTile(s_tile);


	searchMap();


  	return 0;
}








