
#include "common/common.h"

#include <cstdio>
#include <cinttypes>
#include <cstring>

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

			// fprintf(stderr, "line: %s\n", line);


			bool firstDigitFound = false;
			char first = '?';
			char last = '?';
			for (int i = 0; i < (newline - tosearch); i++) {

				char c = line[i];

				if ('0' <= c && c <= '9') {
					if (!firstDigitFound) {

						first = c;
						last = c;	

						firstDigitFound = true;

					} else {
						last = c;
					}
				}
			}

			int calibration_value = 10 * (int)(first - '0') + (int)(last - '0');
			
			// fprintf(stderr, "chars: %c %c %d\n", first, last, calibration_value);

			sum = sum + calibration_value;

			tosearch = newline + 1;

		} else {

			break;
		}
	}

	fprintf(stderr, "sum: %d\n", sum);


  	return 0;
}








