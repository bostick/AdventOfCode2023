
#include "common/common.h"

#include <optional>
#include <vector>

#include <cstdio>
#include <cinttypes>
#include <cstring>



enum Color {
	RED,
	GREEN,
	BLUE
};

const char* colorStr(Color c) {
	switch (c) {
	case RED:
		return "red";
	case GREEN:
		return "green";
	case BLUE:
		return "blue";
	}
}

struct CountAndColor {
	int count;
	Color color;
};

std::optional<CountAndColor> handleCubes(const char *cubes) {

	auto tosearch = cubes;

	int count;

	if (*tosearch == ' ') {
		tosearch++;
	}

	if (!('0' <= *tosearch && *tosearch <= '9')) {

		LOGE("cannot parse %s %d\n", tosearch, __LINE__);

		return {};
	}

	count = (int)(*tosearch - '0');

	tosearch++;

	if (('0' <= *tosearch && *tosearch <= '9')) {

		count = 10 * count + (int)(*tosearch - '0');

		tosearch++;

		if (*tosearch == ' ') {

			tosearch++;

		} else {

			LOGE("cannot parse %s %d\n", tosearch, __LINE__);

			return {};
		}

	} else if (*tosearch == ' ') {

		tosearch++;

	} else {

		LOGE("cannot parse %s %d\n", tosearch, __LINE__);

		return {};
	}

	Color color;

	if (*tosearch == 'r') {

		tosearch++;

		if (*tosearch == 'e') {

			tosearch++;

			if (*tosearch == 'd') {

				color = RED;

			} else {

				LOGE("cannot parse %s %d\n", tosearch, __LINE__);

				return {};
			}

		} else {

			LOGE("cannot parse %s %d\n", tosearch, __LINE__);

			return {};
		}

	} else if (*tosearch == 'g') {

		tosearch++;

		if (*tosearch == 'r') {

			tosearch++;

			if (*tosearch == 'e') {

				tosearch++;

				if (*tosearch == 'e') {

					tosearch++;

					if (*tosearch == 'n') {

						color = GREEN;

					} else {

						LOGE("cannot parse %s %d\n", tosearch, __LINE__);

						return {};
					}

				} else {

					LOGE("cannot parse %s %d\n", tosearch, __LINE__);

					return {};
				}

			} else {

				LOGE("cannot parse %s %d\n", tosearch, __LINE__);

				return {};
			}

		} else {

			LOGE("cannot parse %s %d\n", tosearch, __LINE__);

			return {};
		}

	} else if (*tosearch == 'b') {

		tosearch++;

		if (*tosearch == 'l') {

			tosearch++;

			if (*tosearch == 'u') {

				tosearch++;

				if (*tosearch == 'e') {

					color = BLUE;

				} else {

					LOGE("cannot parse %s %d\n", tosearch, __LINE__);

					return {};
				}

			} else {

				LOGE("cannot parse %s %d\n", tosearch, __LINE__);

				return {};
			}

		} else {

			LOGE("cannot parse %s %d\n", tosearch, __LINE__);

			return {};
		}

	} else {

		LOGE("cannot parse %s %d\n", tosearch, __LINE__);

		return {};
	}

	// fprintf(stderr, "cubes: %d %d\n", count, color);

	return CountAndColor{count, color};
}

std::optional<std::vector<CountAndColor>> handleSet(const char *setStr) {

	std::vector<CountAndColor> set;

	auto tosearch = setStr;

	while (1) {

		const char *comma = strchr(tosearch, ',');

		if (comma != NULL) {

			auto cubes = new char[comma - tosearch + 1];

			memcpy(cubes, tosearch, comma - tosearch);

			cubes[comma - tosearch] = '\0';

			// fprintf(stderr, "cubes: %s\n", cubes);

			auto res = handleCubes(cubes);
			if (!res) {
				return {};
			}

			// fprintf(stderr, "cubes: %d %s\n", res.count, colorStr(res.color));

			set.push_back(*res);

			tosearch = comma + 1;

		} else {

			auto cubes = tosearch;

			// fprintf(stderr, "cubes: %s\n", cubes);

			auto res = handleCubes(cubes);
			if (!res) {
				return {};
			}

			// fprintf(stderr, "cubes: %d %s\n", res.count, colorStr(res.color));

			set.push_back(*res);

			break;
		}
	}

	return set;
}


void printSet(std::vector<CountAndColor> set) {

	LOGI("set:");
	for (CountAndColor cc : set) {
		LOGI("count and color: %d %s", cc.count, colorStr(cc.color));
	}
	LOGI("");

}


struct Game {
	int redCount;
	int greenCount;
	int blueCount;
};

bool testSet(Game game) {

	if (game.redCount > 12) {
		return false;
	}
	
	if (game.greenCount > 13) {
		return false;
	}
	
	if (game.blueCount > 14) {
		return false;
	}
	
	return true;
}


// struct GamePossibility {
// 	int id;
// 	bool possible;
// };

struct GameMinimums {
	int id;
	int redMin;
	int greenMin;
	int blueMin;
};

std::optional<GameMinimums> handleGame(const char *line) {

	auto tosearch = line;

	const char *colon = strchr(tosearch, ':');

	if (colon == NULL) {

		LOGE("cannot parse %s\n", tosearch);

		return {};
	}

	auto gameStr = new char[colon - line + 2];

	memcpy(gameStr, line, colon - line + 2);

	gameStr[colon - line + 1] = '\0';

	// LOGD("gameStr byte count is: %d\n", colon - line + 1);
	// LOGD("gameStr (should include ':'): %s\n", gameStr);

	int id;
	{
		const char *space = strchr(gameStr, ' ');

		if (space == NULL) {

			LOGE("cannot parse %s\n", gameStr);

			return {};
		}

		auto tosearch2 = space + 1;

		if ('0' <= *tosearch2 && *tosearch2 <= '9') {

			id = (int)(*tosearch2 - '0');

			tosearch2++;

		} else {

			LOGE("cannot parse %s\n", gameStr);

			return {};
		}

		if ('0' <= *tosearch2 && *tosearch2 <= '9') {

			id = 10 * id + (int)(*tosearch2 - '0');

		} else if (*tosearch2 == ':') {

			;

		} else {
			
			LOGE("cannot parse %s\n", gameStr);

			return {};
		}
	}

	tosearch = colon + 1;


	// bool possible = true;
	int redMax = 0;
	int greenMax = 0;
	int blueMax = 0;

	while (1) {

		const char *semi = strchr(tosearch, ';');

		if (semi != NULL) {

			auto set = new char[semi - tosearch + 1];

			memcpy(set, tosearch, semi - tosearch);

			set[semi - tosearch] = '\0';

			// fprintf(stderr, "set: %s\n", set);

			auto res = handleSet(set);
			if (!res) {
				return {};
			}

			// printSet(*res);
			// game.push_back(*res);

			for (CountAndColor cc : *res) {
				switch (cc.color) {
				case RED:
					if (cc.count > redMax) {
						redMax = cc.count;
					}
					break;
				case GREEN:
					if (cc.count > greenMax) {
						greenMax = cc.count;
					}
					break;
				case BLUE:
					if (cc.count > blueMax) {
						blueMax = cc.count;
					}
					break;
				}
			}

			tosearch = semi + 1;

		} else {

			auto set = tosearch;

			// fprintf(stderr, "set: %s\n", set);

			auto res = handleSet(set);
			if (!res) {
				return {};
			}

			// printSet(*res);
			// game.push_back(*res);

			for (CountAndColor cc : *res) {
				switch (cc.color) {
				case RED:
					if (cc.count > redMax) {
						redMax = cc.count;
					}
					break;
				case GREEN:
					if (cc.count > greenMax) {
						greenMax = cc.count;
					}
					break;
				case BLUE:
					if (cc.count > blueMax) {
						blueMax = cc.count;
					}
					break;
				}
			}

			break;
		}
	}

	// auto res = testGame(game);

	// if (possible) {
	// 	LOGI("game %d POSSIBLE", id);
	// } else {
	// 	LOGI("game %d IMPOSSIBLE", id);
	// }

	// LOGI("game %d: %d %d %d", id, redMax, greenMax, blueMax);

	return GameMinimums{id, redMax, greenMax, blueMax};
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

	int sum = 0;

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto line = new char[newline - tosearch + 1];

			memcpy(line, tosearch, newline - tosearch);

			line[newline - tosearch] = '\0';

			LOGD("line: %s\n", line);


			auto res = handleGame(line);

			if (!res) {
				return 1;
			}

			GameMinimums gmins = *res;

			int power = gmins.redMin * gmins.greenMin * gmins.blueMin;

			LOGD("power: %d", power);

			sum = sum + power;

			tosearch = newline + 1;

			LOGD();

		} else {

			break;
		}
	}

	LOGI("sum: %d", sum);

  	return 0;
}








