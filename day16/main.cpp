
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
	default:
		assert(false);
		break;
	}
}


struct Ray {
	int row_start;
	int col_start;
	Direction direction;
	int row_end;
	int col_end;
};

void printRay(Ray r) {
	LOGD("ray: %d %d %s %d %d", r.row_start, r.col_start, directionStr(r.direction).c_str(), r.row_end, r.col_end);
}

struct Contraption {
	int row_count;
	int col_count;

	char **image;

	// char **paths;
};


Contraption contraption;


std::vector<Ray> stable_rays;


std::vector<Ray> calculating_rays;


void parseContraption(const char *start) {

	LOGD("parsing...");

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

	// char **paths = (char **)malloc(row_count * sizeof(char *));
	// for (int i = 0; i < row_count; i++) {
	// 	paths[i] = (char *)malloc(col_count * sizeof(char));
	// }

	for (int i = 0; i < row_count; i++) {
		for (int j = 0; j < col_count; j++) {
			auto c = *(start + i * (col_count + 1) + j);
			// LOGD("trying: %c (%d): ", c, (int)c);
			// image[i][j] = '.';
			image[i][j] = c;
			// assert(image[i][j] != '\n');
		}
		// assert(*(start + i * col_count) == '\n');
	}

	contraption = Contraption{row_count, col_count, image};
}


void printContraption() {

	LOGD("contraption:");
	LOGD("row col: %d %d", contraption.row_count, contraption.col_count);
	std::string str;
	for (int i = 0; i < contraption.row_count; i++) {
		str.clear();
		for (int j = 0; j < contraption.col_count; j++) {
			str += contraption.image[i][j];
		}
		LOGD("%s", str.c_str());
	}
	LOGD();
}



void analyzeRays() {

	std::vector<Ray> new_stable_rays;
	std::vector<Ray> new_calculating_rays;

	while (1) {

		LOGD("loop...");

		new_stable_rays.clear();
		new_calculating_rays.clear();

		LOGD("calculating_rays:");
		for (auto r : calculating_rays) {
			printRay(r);
		}
		LOGD();

		LOGD("stable_rays:");
		for (auto r : stable_rays) {
			printRay(r);
		}
		LOGD();

		if (calculating_rays.empty()) {
			break;
		}

		auto r = calculating_rays[0];
		calculating_rays.erase(calculating_rays.begin());

		// for (auto stable_r : stable_rays) {
		// 	if (r.row_start == stable_r.row_start &&
		// 		r.col_start == stable_r.col_start &&
		// 		r.direction == stable_r.direction) {
		// 		//
		// 		// already in stable_rays
		// 		//
		// 		continue;
		// 	}
		// }

		switch (r.direction) {
		case RIGHT: {
			bool stop = false;
			for (int i = r.col_start; i < contraption.col_count; i++) {
				if (stop) {
					break;
				}
				switch (contraption.image[r.row_start][i]) {
				case '.':
					break;
				case '|': {

					stop = true;

					r.row_end = r.row_start;
					r.col_end = i;
					new_stable_rays.push_back(r);

					if (r.row_start > 0) {
						new_calculating_rays.push_back(Ray{r.row_start - 1, i, UP, -1, -1});
					}
					if (r.row_start < contraption.row_count - 1) {
						new_calculating_rays.push_back(Ray{r.row_start + 1, i, DOWN, -1, -1});
					}

					break;
				}
				case '-':
					break;
				case '/': {

					stop = true;

					r.row_end = r.row_start;
					r.col_end = i;
					new_stable_rays.push_back(r);

					if (r.row_start > 0) {
						new_calculating_rays.push_back(Ray{r.row_start - 1, i, UP, -1, -1});
					}

					break;
				}
				case '\\': {

					stop = true;

					r.row_end = r.row_start;
					r.col_end = i;
					new_stable_rays.push_back(r);

					if (r.row_start < contraption.row_count - 1) {
						new_calculating_rays.push_back(Ray{r.row_start + 1, i, DOWN, -1, -1});
					}

					break;
				}
				default:
					LOGD("c: %c", contraption.image[r.row_start][i]);
					assert(false);
					break;
				}
			}
			if (!stop) {
				// hit wall
				r.row_end = r.row_start;
				r.col_end = contraption.col_count - 1;
				new_stable_rays.push_back(r);
			}
			break;
		}
		case DOWN: {
			bool stop = false;
			for (int i = r.row_start; i < contraption.row_count; i++) {
				if (stop) {
					break;
				}
				switch (contraption.image[i][r.col_start]) {
				case '.':
					break;
				case '-': {

					stop = true;

					r.row_end = i;
					r.col_end = r.col_start;
					new_stable_rays.push_back(r);

					if (r.col_start > 0) {
						new_calculating_rays.push_back(Ray{i, r.col_start - 1, LEFT, -1, -1});
					}
					if (r.col_start < contraption.col_count - 1) {
						new_calculating_rays.push_back(Ray{i, r.col_start + 1, RIGHT, -1, -1});
					}

					break;
				}
				case '|':
					break;
				case '\\': {

					stop = true;

					r.row_end = i;
					r.col_end = r.col_start;
					new_stable_rays.push_back(r);

					if (r.col_start < contraption.col_count - 1) {
						new_calculating_rays.push_back(Ray{i, r.col_start + 1, RIGHT, -1, -1});
					}

					break;
				}
				case '/': {

					stop = true;

					r.row_end = i;
					r.col_end = r.col_start;
					new_stable_rays.push_back(r);

					if (r.col_start > 0) {
						new_calculating_rays.push_back(Ray{i, r.col_start - 1, LEFT, -1, -1});
					}

					break;
				}
				default:
					LOGD("c: %c", contraption.image[i][r.col_start]);
					assert(false);
					break;
				}
			}
			if (!stop) {
				// hit wall
				r.row_end = contraption.row_count - 1;
				r.col_end = r.col_start;
				new_stable_rays.push_back(r);
			}
			break;
		}
		case LEFT: {
			bool stop = false;
			for (int i = r.col_start; i >= 0; i--) {
				if (stop) {
					break;
				}
				switch (contraption.image[r.row_start][i]) {
				case '.':
					break;
				case '|': {

					stop = true;

					r.row_end = r.row_start;
					r.col_end = i;
					new_stable_rays.push_back(r);

					if (r.row_start > 0) {
						new_calculating_rays.push_back(Ray{r.row_start - 1, i, UP, -1, -1});
					}
					if (r.row_start < contraption.row_count - 1) {
						new_calculating_rays.push_back(Ray{r.row_start + 1, i, DOWN, -1, -1});
					}

					break;
				}
				case '\\': {

					stop = true;

					r.row_end = r.row_start;
					r.col_end = i;
					new_stable_rays.push_back(r);

					if (r.row_start > 0) {
						new_calculating_rays.push_back(Ray{r.row_start - 1, i, UP, -1, -1});
					}

					break;
				}
				case '/': {

					stop = true;

					r.row_end = r.row_start;
					r.col_end = i;
					new_stable_rays.push_back(r);

					if (r.row_start < contraption.row_count - 1) {
						new_calculating_rays.push_back(Ray{r.row_start + 1, i, DOWN, -1, -1});
					}

					break;
				}
				case '-':
					break;
				default:
					LOGD("c: %c", contraption.image[r.row_start][i]);
					assert(false);
					break;
				}
			}
			if (!stop) {
				// hit wall
				r.row_end = r.row_start;
				r.col_end = 0;
				new_stable_rays.push_back(r);
			}
			break;
		}
		case UP: {
			bool stop = false;
			for (int i = r.row_start; i >= 0; i--) {
				if (stop) {
					break;
				}
				switch (contraption.image[i][r.col_start]) {
				case '.':
					break;
				case '/': {

					stop = true;

					r.row_end = i;
					r.col_end = r.col_start;
					new_stable_rays.push_back(r);

					if (r.col_start < contraption.col_count - 1) {
						new_calculating_rays.push_back(Ray{i, r.col_start + 1, RIGHT, -1, -1});
					}

					break;
				}
				case '-': {

					stop = true;

					r.row_end = i;
					r.col_end = r.col_start;
					new_stable_rays.push_back(r);

					if (r.col_start > 0) {
						new_calculating_rays.push_back(Ray{i, r.col_start - 1, LEFT, -1, -1});
					}
					if (r.col_start < contraption.col_count - 1) {
						new_calculating_rays.push_back(Ray{i, r.col_start + 1, RIGHT, -1, -1});
					}

					break;
				}
				case '|':
					break;
				case '\\': {

					stop = true;

					r.row_end = i;
					r.col_end = r.col_start;
					new_stable_rays.push_back(r);

					if (r.col_start > 0) {
						new_calculating_rays.push_back(Ray{i, r.col_start - 1, LEFT, -1, -1});
					}

					break;
				}
				default:
					LOGD("c: %c", contraption.image[i][r.col_start]);
					assert(false);
					break;
				}
			}
			if (!stop) {
				// hit wall
				r.row_end = 0;
				r.col_end = r.col_start;
				new_stable_rays.push_back(r);
			}
			break;
		}
		default:
			assert(false);
			break;
		}

		
		LOGD("new_stable_rays:");
		for (auto r : new_stable_rays) {
			printRay(r);
		}
		LOGD();


		LOGD("new_calculating_rays:");
		for (auto r : new_calculating_rays) {
			printRay(r);
		}
		LOGD();


		for (auto r : new_stable_rays) {

			bool found = false;
			for (auto stable_r : stable_rays) {
				if (r.row_start == stable_r.row_start &&
					r.col_start == stable_r.col_start &&
					r.direction == stable_r.direction) {
					
					assert(r.row_end == stable_r.row_end);
					assert(r.col_end == stable_r.col_end);

					//
					// already in stable_rays
					//
					found = true;
					break;
				}
			}

			if (!found) {
				stable_rays.push_back(r);
			}
		}

		for (auto r : new_calculating_rays) {

			bool found = false;
			for (auto calculating_r : calculating_rays) {
				if (r.row_start == calculating_r.row_start &&
					r.col_start == calculating_r.col_start &&
					r.direction == calculating_r.direction) {
					
					assert(r.row_end == calculating_r.row_end);
					assert(r.col_end == calculating_r.col_end);

					//
					// already in calculating_rays
					//
					found = true;
					break;
				}
			}
			if (!found) {
				for (auto stable_r : stable_rays) {
					if (r.row_start == stable_r.row_start &&
						r.col_start == stable_r.col_start &&
						r.direction == stable_r.direction) {
						
						// assert(r.row_end == stable_r.row_end);
						// assert(r.col_end == stable_r.col_end);

						//
						// already in stable_rays
						//
						found = true;
						break;
					}
				}
			}

			if (!found) {
				calculating_rays.push_back(r);
			}
		}
	}

}


void calculateEnergized() {

	char **image_energized = (char **)malloc(contraption.row_count * sizeof(char *));
	for (int i = 0; i < contraption.row_count; i++) {
		image_energized[i] = (char *)malloc(contraption.col_count * sizeof(char));
	}

	for (int i = 0; i < contraption.row_count; i++) {
		for (int j = 0; j < contraption.col_count; j++) {
			image_energized[i][j] = '.';
		}
	}

	for (auto r : stable_rays) {
		// LOGD("loop in calculateEnergized:");
		// printRay(r);
		switch (r.direction) {
		case RIGHT:
			for (int i = r.col_start; i <= r.col_end; i++) {
				image_energized[r.row_start][i] = '#';
			}
			break;
		case DOWN:
			for (int i = r.row_start; i <= r.row_end; i++) {
				image_energized[i][r.col_start] = '#';
			}
			break;
		case LEFT:
			for (int i = r.col_start; i >= r.col_end; i--) {
				// LOGD("setting image_energized[%d][%d] to #", r.row_start, i);
				image_energized[r.row_start][i] = '#';
			}
			break;
		case UP:
			for (int i = r.row_start; i >= r.row_end; i--) {
				image_energized[i][r.col_start] = '#';
			}
			break;
		default:
			assert(false);
			break;
		}
	}

	LOGD("energized contraption:");
	LOGD("row col: %d %d", contraption.row_count, contraption.col_count);
	std::string str;
	for (int i = 0; i < contraption.row_count; i++) {
		str.clear();
		for (int j = 0; j < contraption.col_count; j++) {
			str += image_energized[i][j];
		}
		LOGD("%s", str.c_str());
	}
	LOGD();


	int energized_count = 0;

	for (int i = 0; i < contraption.row_count; i++) {
		for (int j = 0; j < contraption.col_count; j++) {
			if (image_energized[i][j] == '#') {
				energized_count++;
			}
		}
	}


	LOGI("energized_count: %d", energized_count);
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



	parseContraption(buf);

	printContraption();
	

	//
	// first path
	//
	Ray ray = Ray{0, 0, RIGHT, -1, -1};

	calculating_rays.push_back(ray);


	analyzeRays();


	calculateEnergized();

  	return 0;
}








