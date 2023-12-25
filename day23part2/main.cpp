
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
	// Direction direction;
	// int segment_len;

	// Node(int r, int c, Direction d, int s) {
	// 	row = r;
	// 	col = c;
	// 	direction = d;
	// 	segment_len = s;
	// 	assert(segment_len <= 10);
	// }

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

	int parent_row;
	int parent_col;

	int cost;
};



int row_count;
int col_count;
char **map;

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

	map = (char **)malloc(row_count * sizeof(char *));
	for (int i = 0; i < row_count; i++) {
		map[i] = (char *)malloc(col_count * sizeof(char));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			auto c = *(start + i * (col_count + 1) + j);
			map[i][j] = c;
		}
	}
}


void printMap() {

	LOGD("map:");
	LOGD("row col: %d %d", row_count, col_count);
	std::string str;
	for (int i = 0; i < row_count; i++) {
		str.clear();
		for (int j = 0; j < col_count; j++) {
			str += map[i][j];
		}
		LOGD("%s", str.c_str());
	}
	LOGD();
}


void printPath(Node start, Node end) {


	auto path_map = (char **)malloc(row_count * sizeof(char *));
	for (int i = 0; i < row_count; i++) {
		path_map[i] = (char *)malloc(col_count * sizeof(char));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			path_map[i][j] = map[i][j];
		}
	}


	auto n = end;

	int steps = 0;

	while (1) {

		if (n == start) {

			path_map[n.row][n.col] = 'S';

            break;
            
        } else if (n.row == -1 && n.col == -1) {
            
            LOGD("this map was not solved");
            
            steps = -1;
            
            break;
            
        } else {
			
			path_map[n.row][n.col] = 'O';

			steps++;

            auto details = detailss[n.row][n.col];
            
            n = Node{details.parent_row, details.parent_col};
		}
	}


	// LOGD("path map:");
	// LOGD("row col: %d %d", row_count, col_count);
	// std::string str;
	// for (int i = 0; i < row_count; i++) {
	// 	str.clear();
	// 	for (int j = 0; j < col_count; j++) {
	// 		str += path_map[i][j];
	// 	}
	// 	LOGD("%s", str.c_str());
	// }
	// LOGD();

	LOGD("steps: %d", steps);
	LOGD();

}



bool is_arrow(char c) {
	switch (c) {
	case '^':
	case '>':
	case 'v':
	case '<':
		return true;
	default:
		return false;
	}
}



int choice_index;

std::vector<int> choice_vector;


std::map<Node, int> remembered_choices;


std::vector<Node> calculateSuccessors(Node n) {

	std::vector<Node> ns;

	switch (map[n.row][n.col]) {
	case '^':
		if (n.row > 0) ns.push_back(Node{n.row - 1, n.col});
		if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col + 1});
		if (n.row < row_count - 1) ns.push_back(Node{n.row + 1, n.col});
		if (n.col > 0) ns.push_back(Node{n.row, n.col - 1});
		break;
	case '>':
		if (n.row > 0) ns.push_back(Node{n.row - 1, n.col});
		if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col + 1});
		if (n.row < row_count - 1) ns.push_back(Node{n.row + 1, n.col});
		if (n.col > 0) ns.push_back(Node{n.row, n.col - 1});
		break;
	case 'v':
		if (n.row > 0) ns.push_back(Node{n.row - 1, n.col});
		if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col + 1});
		if (n.row < row_count - 1) ns.push_back(Node{n.row + 1, n.col});
		if (n.col > 0) ns.push_back(Node{n.row, n.col - 1});
		break;
	case '<':
		if (n.row > 0) ns.push_back(Node{n.row - 1, n.col});
		if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col + 1});
		if (n.row < row_count - 1) ns.push_back(Node{n.row + 1, n.col});
		if (n.col > 0) ns.push_back(Node{n.row, n.col - 1});
		break;
	case '.': {

		std::vector<Node> choices;

		if (n.row > 0) {
			if (map[n.row - 1][n.col] == '.') {
				ns.push_back(Node{n.row - 1, n.col});
			} else if (map[n.row - 1][n.col] == 'v') {
				choices.push_back(Node{n.row - 1, n.col});
			} else if (is_arrow(map[n.row - 1][n.col])) {
				choices.push_back(Node{n.row - 1, n.col});
			}
		}
		if (n.col < col_count - 1) {
			if (map[n.row][n.col + 1] == '.') {
				ns.push_back(Node{n.row, n.col + 1});
			} else if (map[n.row][n.col + 1] == '<') {
				choices.push_back(Node{n.row, n.col + 1});
			} else if (is_arrow(map[n.row][n.col + 1])) {
				choices.push_back(Node{n.row, n.col + 1});
			}
		}
		if (n.row < row_count - 1) {
			if (map[n.row + 1][n.col] == '.') {
				ns.push_back(Node{n.row + 1, n.col});
			} else if (map[n.row + 1][n.col] == '^') {
				choices.push_back(Node{n.row + 1, n.col});
			} else if (is_arrow(map[n.row + 1][n.col])) {
				choices.push_back(Node{n.row + 1, n.col});
			}
		}
		if (n.col > 0) {
			if (map[n.row][n.col - 1] == '.') {
				ns.push_back(Node{n.row, n.col - 1});
			} else if (map[n.row][n.col - 1] == '>') {
				choices.push_back(Node{n.row, n.col - 1});
			} else if (is_arrow(map[n.row][n.col - 1])) {
				choices.push_back(Node{n.row, n.col - 1});
			}
		}

        switch (choices.size()) {
        case 0:
            break;
        case 1:
            ns.push_back(choices[0]);
            break;
        case 2: {

            if (choice_index >= choice_vector.size()) {
            	exit(1);
            }
            
            auto choice = choice_vector[choice_index];

            if (remembered_choices.find(n) == remembered_choices.end()) {
            	remembered_choices[n] = choice;
            } else {
            	assert(choice == remembered_choices[n]);
            }

            ns.push_back(choices[choice % 2]);
            
            choice_index++;
            break;
        }
    	case 3: {
            
            if (choice_index >= choice_vector.size()) {
            	exit(1);
            }
            
            auto choice = choice_vector[choice_index];
            
            if (remembered_choices.find(n) == remembered_choices.end()) {
            	remembered_choices[n] = choice;
            } else {
            	assert(choice == remembered_choices[n]);
            }

            ns.push_back(choices[choice % 3]);
            
            choice_index++;
            break;
        }
	    case 4: {
            
            if (choice_index >= choice_vector.size()) {
            	exit(1);
            }
            
            auto choice = choice_vector[choice_index];
            
            if (remembered_choices.find(n) == remembered_choices.end()) {
            	remembered_choices[n] = choice;
            } else {
            	assert(choice == remembered_choices[n]);
            }

            ns.push_back(choices[choice % 4]);
            
            choice_index++;
            break;
        }
        default:
        	printNode(n);
        	LOGD("choices.size(): %zu", choices.size());
            assert(false);
            break;
        }

		break;
	}
	default:
		assert(false);
		break;
	}

	return ns;
}



void searchMap(Node start, Node end) {

	LOGD("searching...");

	std::vector<Node> open;

	bool **closed;



	detailss = (Details **)malloc(row_count * sizeof(Details *));
	for (int i = 0; i < row_count; i++) {
		detailss[i] = (Details *)malloc(col_count * sizeof(Details));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			detailss[i][j].cost = COST_MAX;
			detailss[i][j].parent_row = -1;
			detailss[i][j].parent_col = -1;
		}
	}


	closed = (bool **)malloc(row_count * sizeof(bool *));
	for (int i = 0; i < row_count; i++) {
		closed[i] = (bool *)malloc(col_count * sizeof(bool));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			closed[i][j] = false;
		}
	}



	detailss[start.row][start.col].cost = 0;


	open.push_back(start);

	while (1) {

		if (open.empty()) {
			break;
		}

		auto q = open[0];
		open.erase(open.begin());

		// if (s == end) {
		// 	detailss[s.row][s.col].parent_row = q.row;
		// 	detailss[s.row][s.col].parent_col = q.col;
		// 	LOGD("found!");
		// }

		assert(closed[q.row][q.col] == false);
		closed[q.row][q.col] = true;

		auto successors = calculateSuccessors(q);

		for (auto s : successors) {

			// LOGD("s:");
			// printNode(s);

			if (closed[s.row][s.col]) {
				continue;
			}

			int cost_new;
			if (map[s.row][s.col] == '#') {
				cost_new = COST_MAX;
			} else {
				cost_new = detailss[q.row][q.col].cost + 1;
			}

            auto details = detailss[s.row][s.col];
			if (cost_new < details.cost) {

				detailss[s.row][s.col].cost = cost_new;
				detailss[s.row][s.col].parent_row = q.row;
				detailss[s.row][s.col].parent_col = q.col;

				//
				// greatest value less than tmp cost
				//
				if (open.empty()) {
					open.push_back(s);
				} else {

					//
					// XXX: should actually use std::upper_bound or something
					//
					int i;
					for (i = 0; i < open.size(); i++) {
						auto tmp = open[i];
						if (cost_new < detailss[tmp.row][tmp.col].cost) {
							break;
						}
					}

					open.insert(open.begin() + i, s);
				}
			}
		}
	}

	LOGD("done searching");
}




int main(int argc, char **argv) {

	assert(argc == 3);

	const char *path = argv[1];
	// const char *path = "/Users/brenton/development/github/AdventOfCode2023/day23/input2.txt";


	auto choices_str = argv[2];
	// char *choices_str = "1,0";
	// char *choices_str = "1,0,0,1,1,1,1,1,1";


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



	auto tosearch = choices_str;

	while (1) {

		char *comma = strchr(tosearch, ',');

		if (comma != NULL) {

			assert(comma == tosearch + 1);

			auto choice_str = *tosearch;

			auto choice = (int)(choice_str - '0');

			choice_vector.push_back(choice);

			tosearch = comma + 1;

		} else {

			auto choice_str = *tosearch;

			auto choice = (int)(choice_str - '0');

			choice_vector.push_back(choice);

			break;
		}
	}



	parseMap(buf);

	// printMap();


	Node start = Node{0, 1};
	Node end = Node{22, 21};

	// Node start = Node{0, 1};
	// Node end = Node{140, 139};

	searchMap(start, end);

	printPath(start, end);


  	return 0;
}








