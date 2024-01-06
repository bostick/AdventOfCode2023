
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



const int COST_MAX = 5000;




struct Node {
	int row;
	int col;

	bool operator==(const Node& other) const {
        return row == other.row &&
        	col == other.col;
    }
    
    bool operator<(const Node& other) const {
        if (row < other.row) {
            return true;
        }
        if (row > other.row) {
            return false;
        }
        if (col < other.col) {
            return true;
        }
        if (col > other.col) {
            return false;
        }
        
        return false;
    }
};


void printNode(Node n) {
	LOGD("node: %d %d", n.row, n.col);
}


struct Details {
	int cost;
};



struct State {
	char c;
};

int row_count;
int col_count;
State **map;

int start_row;
int start_col;


Details **detailss;



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

	map = (State **)malloc(row_count * sizeof(State *));
	for (int i = 0; i < row_count; i++) {
		map[i] = (State *)malloc(col_count * sizeof(State));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			auto c = *(start + i * (col_count + 1) + j);
			// LOGD("trying: %c (%d): ", c, (int)c);
			// image[i][j] = '.';
			map[i][j].c = c;

			if (c == 'S') {
				start_row = i;
				start_col = j;
			}

			// assert(image[i][j] != '\n');
		}
		// assert(*(start + i * col_count) == '\n');
	}
}


void printMap() {

	LOGD("map:");
	LOGD("row col: %d %d", row_count, col_count);
	std::string str;
	for (int i = 0; i < row_count; i++) {
		str.clear();
		for (int j = 0; j < col_count; j++) {
			str += map[i][j].c;
		}
		LOGD("%s", str.c_str());
	}
	LOGD();
}



void printSteps(std::vector<Node> steps) {


	auto path_map = (char **)malloc(row_count * sizeof(char *));
	for (int i = 0; i < row_count; i++) {
		path_map[i] = (char *)malloc(col_count * sizeof(char));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			path_map[i][j] = map[i][j].c;
		}
	}

	for (auto s : steps) {
		path_map[s.row][s.col] = 'O';
	}

	LOGD("path map:");
	LOGD("row col: %d %d", row_count, col_count);
	std::string str;
	for (int i = 0; i < row_count; i++) {
		str.clear();
		for (int j = 0; j < col_count; j++) {
			str += path_map[i][j];
		}
		LOGD("%s", str.c_str());
	}
	LOGD();

	LOGD();


}




std::vector<Node> calculateSuccessors(Node n) {

	std::vector<Node> ns;

	if (n.row > 0) ns.push_back(Node{n.row - 1, n.col});

	if (n.col > 0) ns.push_back(Node{n.row, n.col - 1});

	if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col + 1});

	if (n.row < row_count - 1) ns.push_back(Node{n.row + 1, n.col});

	return ns;
}



// void searchMap(Node start) {

// 	// LOGD("searching...");

// 	std::vector<Node> open;

// 	bool **closed;



// 	detailss = (Details **)malloc(row_count * sizeof(Details *));
// 	for (int i = 0; i < row_count; i++) {
// 		detailss[i] = (Details *)malloc(col_count * sizeof(Details));
// 	}

// 	for (int i = 0; i < row_count; i++) {
// 		for (int j = 0; j < col_count; j++) {
// 			detailss[i][j].cost = COST_MAX;
// 		}
// 	}


// 	closed = (bool **)malloc(row_count * sizeof(bool *));
// 	for (int i = 0; i < row_count; i++) {
// 		closed[i] = (bool *)malloc(col_count * sizeof(bool));
// 	}

// 	for (int i = 0; i < row_count; i++) {
// 		for (int j = 0; j < col_count; j++) {
// 			closed[i][j] = false;
// 		}
// 	}


// 	detailss[start_row][start_col].cost = 0;

// 	open.push_back(Node{start_row, start_col});


// 	while (1) {

// 		if (open.empty()) {
// 			break;
// 		}

// 		auto q = open[0];
// 		open.erase(open.begin());

// 		assert(closed[q.row][q.col] == false);
// 		closed[q.row][q.col] = true;

// 		// LOGD("q:");
// 		// printNode(q);

// 		auto successors = calculateSuccessors(q);

// 		for (auto s : successors) {

// 			if (closed[s.row][s.col]) {
// 				continue;
// 			}

// 			int cost_new;
// 			{
// 				switch (map[s.row][s.col].c) {
// 				case 'S':
// 				case '.':
// 					cost_new = detailss[q.row][q.col].cost + 1;
// 					break;
// 				case '#':
// 					cost_new = COST_MAX;
// 					break;
// 				default:
// 					assert(false);
// 					break;
// 				}
// 			}

//             auto details = detailss[s.row][s.col];
// 			if (cost_new < details.cost) {

// 				//
// 				// greatest value less than tmp cost
// 				//
// 				if (open.empty()) {
// 					open.push_back(s);
// 				} else {

// 					//
// 					// XXX: should actually do binary search or have a heap data structure or something
// 					//
// 					int i;
// 					for (i = 0; i < open.size(); i++) {
// 						auto tmp = open[i];
// 						if (cost_new < detailss[tmp.row][tmp.col].cost) {
// 							break;
// 						}
// 					}

// 					open.insert(open.begin() + i, s);
// 				}

// 				detailss[s.row][s.col].cost = cost_new;
// 			}
// 		}
// 	}

// 	LOGD("done searching");
// }


std::vector<Node> step(std::vector<Node> starts) {

	std::set<Node> after_set;

	std::vector<Node> after;

	for (auto start : starts) {

		auto successors = calculateSuccessors(start);

		for (auto successor : successors) {

			switch (map[successor.row][successor.col].c) {
			case 'S':
			case '.':
				after_set.insert(successor);
				break;
			case '#':
				continue;
			default:
				assert(false);
				break;
			}
		}

	}

	for (auto a : after_set) {
		after.push_back(a);
	}

	return after;
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

	printMap();


	// searchMap();


	std::vector<Node> steps;

	steps.push_back(Node{start_row, start_col});

	for (int i = 0; i < 64; i++) {
		steps = step(steps);
	}

	printSteps(steps);

	LOGI("# steps: %zu", steps.size());

  	return 0;
}








