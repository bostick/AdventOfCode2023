
#include "common/common.h"

#include <optional>
#include <vector>
#include <set>

#include <cstdio>
#include <cinttypes>
#include <cstring>



std::set<int> handleNumbers(const char *str) {

	std::set<int> numbers;

	bool inside_number = false;

	int n = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		auto c = str[i];
		if ('0' <= c && c <= '9') {

			if (!inside_number) {
				
				//
				// new number
				//
				n = (int)(c - '0');

				inside_number = true;

			} else {
				n = 10 * n + (int)(c - '0');
			}

		} else {
			if (inside_number) {
				//
				// finish number
				//
				numbers.insert(n);

				//
				// reset
				//
				inside_number = false;
				n = 0;

			} else {
				;
			}
		}
	}

	if (inside_number) {
		//
		// finish number
		//
		numbers.insert(n);
	}

	return numbers;
}


struct Card {
	const char *str;
	int points;
};

std::optional<Card> handleCard(const char *line) {

	auto tosearch = line;

	const char *colon = strchr(tosearch, ':');

	if (colon == NULL) {

		LOGE("cannot parse %s\n", tosearch);

		return {};
	}

	auto cardStr = new char[colon - line + 2];

	memcpy(cardStr, line, colon - line + 2);

	cardStr[colon - line + 1] = '\0';

	// LOGD("gameStr byte count is: %d\n", colon - line + 1);
	// LOGD("gameStr (should include ':'): %s\n", gameStr);

	tosearch = colon + 1;


	// std::vector<std::vector<CountAndColor>> game;

	// while (1) {

	const char *bar = strchr(tosearch, '|');

	if (bar == NULL) {

		LOGE("cannot parse %s %d\n", tosearch, __LINE__);

		return {};
	}

	auto winning_numbersStr = new char[bar - tosearch + 1];

	memcpy(winning_numbersStr, tosearch, bar - tosearch);

	winning_numbersStr[bar - tosearch] = '\0';

	tosearch = bar + 1;

	auto my_numbersStr = tosearch;

	auto winning_numbers = handleNumbers(winning_numbersStr);

	auto my_numbers = handleNumbers(my_numbersStr);

	// LOGD("begin winning_numbers");
	// for (int n : winning_numbers) {
	// 	LOGD("%d", n);
	// }
	// LOGD("end winning_numbers");

	// LOGD("begin my_numbers");
	// for (int n : my_numbers) {
	// 	LOGD("%d", n);
	// }
	// LOGD("end my_numbers");

	int points = 0;

	for (int my : my_numbers) {
		for (int winning : winning_numbers) {
			if (my == winning) {
				if (points == 0) {
					points = 1;
				} else {
					points = 2 * points;
				}
			}
		}
	}


	return Card{cardStr, points};
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


			auto res = handleCard(line);

			if (!res) {
				return 1;
			}

			Card card = *res;

			LOGD("%s points: %d", card.str, card.points);

			sum = sum + card.points;


			tosearch = newline + 1;

			LOGD();

		} else {

			break;
		}
	}

	LOGI("sum: %d", sum);

  	return 0;
}








