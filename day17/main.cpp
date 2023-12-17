
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
	RIGHT,
	DOWN,
	LEFT,
	UP,
	START,
    UNKNOWN,
};

std::string directionStr(Direction d) {
	switch (d) {
	case RIGHT:
		return "RIGHT";
	case DOWN:
		return "DOWN";
	case LEFT:
		return "LEFT";
	case UP:
		return "UP";
	case START:
		return "START";
	default:
		assert(false);
		break;
	}
}

struct Node {
	int row;
	int col;
	Direction direction;
	int segment_len;

	bool operator==(const Node& other) const {
        return row == other.row &&
        	col == other.col &&
        	direction == other.direction &&
        	segment_len == other.segment_len;
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
        
        if (direction < other.direction) {
            return true;
        }
        if (direction > other.direction) {
            return false;
        }
        
        if (segment_len < other.segment_len) {
            return true;
        }
        if (segment_len > other.segment_len) {
            return false;
        }
        
        assert(false);
    }
};


void printNode(Node n) {
	LOGD("node: %d %d %s %d", n.row, n.col, directionStr(n.direction).c_str(), n.segment_len);
}


struct Details {

	int parent_row;
	int parent_col;
	Direction parent_direction;
	int parent_segment_len;

	int cost;
};



struct State {
	char c;
};

int row_count;
int col_count;
State **map;

Details ****detailss;



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


void printPath(Node start, Node end) {


	auto path_map = (char **)malloc(row_count * sizeof(char *));
	for (int i = 0; i < row_count; i++) {
		path_map[i] = (char *)malloc(col_count * sizeof(char));
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			path_map[i][j] = '.';
		}
	}


	int heat_loss = 0;

	auto n = end;

	while (1) {

		if (n.direction == START) {

			path_map[n.row][n.col] = '0';

            break;
            
		} else {
			
			path_map[n.row][n.col] = map[n.row][n.col].c;

			heat_loss += (map[n.row][n.col].c - '0');

            auto details = detailss[n.row][n.col][n.direction][n.segment_len];
            
            if (details.parent_direction == UNKNOWN) {
                LOGD("this particular map was not solved");
                break;
            }
            
            n = Node{details.parent_row, details.parent_col, details.parent_direction, details.parent_segment_len};
		}
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

	LOGD("heat_loss: %d", heat_loss);

	LOGD();


}





int calculateHeatLoss(Node start, Node end) {

	int heat_loss = 0;

	auto n = end;

	while (1) {

		if (n == start) {
            
            break;
            
		} else {

            auto details = detailss[n.row][n.col][n.direction][n.segment_len];
            
            if (details.parent_direction == UNKNOWN) {
                LOGD("this particular map was not solved");
                heat_loss = -1;
                break;
            }

            heat_loss += (map[n.row][n.col].c - '0');

            n = Node{details.parent_row, details.parent_col, details.parent_direction, details.parent_segment_len};
		}
	}

	return heat_loss;
}





std::vector<Node> calculateSuccessors(Node n) {

	std::vector<Node> ns;

	if (n.direction != DOWN) {
		if (n.direction != UP) {
			if (n.row > 0) ns.push_back(Node{n.row-1, n.col, UP, 1});
			// if (n.row > 1) ns.push_back(Node{n.row-2, n.col, UP, 2});
			// if (n.row > 2) ns.push_back(Node{n.row-3, n.col, UP, 3});
		} else {
			switch (n.segment_len) {
			case 1:
				if (n.row > 0) ns.push_back(Node{n.row-1, n.col, UP, 2});
				// if (n.row > 1) ns.push_back(Node{n.row-2, n.col, UP, 3});
				break;
			case 2:
				if (n.row > 0) ns.push_back(Node{n.row-1, n.col, UP, 3});
				break;
			case 3:
				break;
			default:
				break;
			}
		}
	}

	{
		if (n.direction != RIGHT) {
			if (n.direction != LEFT) {
				if (n.col > 0) ns.push_back(Node{n.row, n.col-1, LEFT, 1});
				// if (n.col > 1) ns.push_back(Node{n.row, n.col-2, LEFT, 2});
				// if (n.col > 2) ns.push_back(Node{n.row, n.col-3, LEFT, 3});
			} else {
				switch (n.segment_len) {
				case 1:
					if (n.col > 0) ns.push_back(Node{n.row, n.col-1, LEFT, 2});
					// if (n.col > 1) ns.push_back(Node{n.row, n.col-2, LEFT, 3});
					break;
				case 2:
					if (n.col > 0) ns.push_back(Node{n.row, n.col-1, LEFT, 3});
					break;
				case 3:
					break;
				default:
					break;
				}
			}
		}

		if (n.direction != LEFT) {
			if (n.direction != RIGHT) {
				if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col+1, RIGHT, 1});
				// if (n.col < col_count - 2) ns.push_back(Node{n.row, n.col+2, RIGHT, 2});
				// if (n.col < col_count - 3) ns.push_back(Node{n.row, n.col+3, RIGHT, 3});
			} else {
				switch (n.segment_len) {
				case 1:
					if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col+1, RIGHT, 2});
					// if (n.col < col_count - 2) ns.push_back(Node{n.row, n.col+2, RIGHT, 3});
					break;
				case 2:
					if (n.col < col_count - 1) ns.push_back(Node{n.row, n.col+1, RIGHT, 3});
					break;
				case 3:
					break;
				default:
					break;
				}
			}
		}
	}

	if (n.direction != UP) {
		if (n.direction != DOWN) {
			if (n.row < row_count - 1) ns.push_back(Node{n.row+1, n.col, DOWN, 1});
			// if (n.row < row_count - 2) ns.push_back(Node{n.row+2, n.col, DOWN, 2});
			// if (n.row < row_count - 3) ns.push_back(Node{n.row+3, n.col, DOWN, 3});
		} else {
			switch (n.segment_len) {
			case 1:
				if (n.row < row_count - 1) ns.push_back(Node{n.row+1, n.col, DOWN, 2});
				// if (n.row < row_count - 2) ns.push_back(Node{n.row+2, n.col, DOWN, 3});
				break;
			case 2:
				if (n.row < row_count - 1) ns.push_back(Node{n.row+1, n.col, DOWN, 3});
				break;
			case 3:
				break;
			default:
				break;
			}
		}
	}

	return ns;
}



void searchMap(Node start, Node end) {

	LOGD("searching...");

	std::vector<Node> open;

	bool ****closed;



	detailss = (Details ****)malloc(row_count * sizeof(Details ***));
	for (int i = 0; i < row_count; i++) {
		detailss[i] = (Details ***)malloc(col_count * sizeof(Details **));
		for (int j = 0; j < col_count; j++) {
			detailss[i][j] = (Details **)malloc(4 * sizeof(Details *));
			for (int k = 0; k < 4; k++) {
				detailss[i][j][k] = (Details *)malloc(4 * sizeof(Details));
			}
		}
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			for (int k = 0; k < 4; k++) {
				for (int l = 0; l < 4; l++) {
					detailss[i][j][k][l].cost = 1000;
					detailss[i][j][k][l].parent_row = -1;
					detailss[i][j][k][l].parent_col = -1;
					detailss[i][j][k][l].parent_direction = UNKNOWN;
					detailss[i][j][k][l].parent_segment_len = -1;
				}
			}
		}
	}


	closed = (bool ****)malloc(row_count * sizeof(bool ***));
	for (int i = 0; i < row_count; i++) {
		closed[i] = (bool ***)malloc(col_count * sizeof(bool **));
		for (int j = 0; j < col_count; j++) {
			closed[i][j] = (bool **)malloc(4 * sizeof(bool *));
			for (int k = 0; k < 4; k++) {
				closed[i][j][k] = (bool *)malloc(4 * sizeof(bool));
			}
		}
	}

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			for (int k = 0; k < 4; k++) {
				for (int l = 0; l < 4; l++) {
					closed[i][j][k][l] = false;
				}
			}
		}
	}



	detailss[start.row][start.col][start.direction][start.segment_len].cost = 0;


	open.push_back(start);

	while (1) {

		if (open.empty()) {
			break;
		}

		Node q;

		int index;
		int cost_least = 1000;
		for (int i = 0; i < open.size(); i++) {
			auto tmp = open[i];
			if (detailss[tmp.row][tmp.col][tmp.direction][tmp.segment_len].cost < cost_least) {
				index = i;
				cost_least = detailss[tmp.row][tmp.col][tmp.direction][tmp.segment_len].cost;
			}
		}

		// LOGD("index: %d", index);
		// LOGD("leastF: %d", leastF);

		q = open[index];
		open.erase(open.begin() + index);

		if (q == end) {
			LOGD("found!");
			break;
		}

        // assert(find(closed.begin(), closed.end(), q) == closed.end());
		// closed.insert(q);
		assert(closed[q.row][q.col][q.direction][q.segment_len] == false);
		closed[q.row][q.col][q.direction][q.segment_len] = true;

		// LOGD("q:");
		// printNode(q);

		auto successors = calculateSuccessors(q);

		for (auto s : successors) {

			// LOGD("s:");
			// printNode(s);

			// if (find(closed.begin(), closed.end(), s) != closed.end()) {
			// 	continue;
			// }
			if (closed[s.row][s.col][s.direction][s.segment_len]) {
				continue;
			}

			//
			// g = the movement cost to move from the starting point to a given square on the grid, following the path generated to get there. 
			//
			// s.g = q.g + distance between q and successor

			auto cost_new = detailss[q.row][q.col][q.direction][q.segment_len].cost + (int)(map[s.row][s.col].c - '0');

            auto details = detailss[s.row][s.col][(int)s.direction][s.segment_len];
			if (cost_new < details.cost) {

				open.push_back(s);

				detailss[s.row][s.col][s.direction][s.segment_len].cost = cost_new;
				detailss[s.row][s.col][s.direction][s.segment_len].parent_row = q.row;
				detailss[s.row][s.col][s.direction][s.segment_len].parent_col = q.col;
				detailss[s.row][s.col][s.direction][s.segment_len].parent_direction = q.direction;
				detailss[s.row][s.col][s.direction][s.segment_len].parent_segment_len = q.segment_len;

			}
		}
	}

	LOGD("done searching");
}




int main(int argc, char **argv) {

	assert(argc == 8);

	const char *path = argv[1];

	int start_row;
	int start_col;

	int end_row;
	int end_col;

	auto res2 = sscanf(argv[2], "%d", &start_row);
	assert(res2 == 1);
	res2 = sscanf(argv[3], "%d", &start_col);
	assert(res2 == 1);

	res2 = sscanf(argv[4], "%d", &end_row);
	assert(res2 == 1);
	res2 = sscanf(argv[5], "%d", &end_col);
	assert(res2 == 1);

	char directionC;
	res2 = sscanf(argv[6], "%c", &directionC);
	assert(res2 == 1);

	Direction start_direction;
	switch (directionC) {
	case 'U':
		start_direction = UP;
		break;
	case 'R':
		start_direction = RIGHT;
		break;
	case 'D':
		start_direction = DOWN;
		break;
	case 'L':
		start_direction = LEFT;
		break;
	case 'S':
		start_direction = START;
		break;
	default:
		assert(false);
		break;
	}

	int start_segment_len;
	res2 = sscanf(argv[7], "%d", &start_segment_len);
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

	printMap();


	Node start = Node{start_row, start_col, start_direction, start_segment_len};
	
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			Node end = Node{end_row, end_col, (Direction)i, j};

			LOGD("searching for end: %s %d", directionStr((Direction)i).c_str(), j);

			searchMap(start, end);

			auto heat_loss = calculateHeatLoss(start, end);

			if (heat_loss != -1) {
				printPath(start, end);
			}

			LOGD();
		}
	}


  	return 0;
}








