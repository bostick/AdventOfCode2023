
#include "common/common.h"

#include <cstdio>
#include <cinttypes>
#include <cstring>



#define DIGIT() \
	if (!firstDigitFound) { \
		first = c; \
		last = c; \
		firstDigitFound = true; \
	} else { \
		last = c; \
	}

#define NEXTC() \
	i++; \
	if (!(i < (newline - tosearch))) { \
		break; \
	} \
	c = line[i];


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

			fprintf(stderr, "line: %s\n", line);


			bool firstDigitFound = false;
			char first = '?';
			char last = '?';
			for (int i = 0; i < (newline - tosearch); i++) {

				char c = line[i];

				if ('0' <= c && c <= '9') {
					
					DIGIT();

				} else {

begin_word:
					if (c == 's') {

						NEXTC();

						if (c == 'e') {
							
							NEXTC();

							if (c == 'v') {
								
								NEXTC();

								if (c == 'e') {
									
									NEXTC();

									if (c == 'n') {
										
										if (!firstDigitFound) {

											first = '7';
											last = '7';

											firstDigitFound = true;

										} else {
											last = '7';
										}

										NEXTC();

										goto begin_n;

									} else if ('0' <= c && c <= '9') {
										DIGIT();
									} else {
										goto begin_e;
									}

								} else if ('0' <= c && c <= '9') {
									DIGIT();
								} else {
									goto begin_word;
								}

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_e;
							}

						} else if (c == 'i') {
									
							NEXTC();

							if (c == 'x') {
								
								if (!firstDigitFound) {

									first = '6';
									last = '6';

									firstDigitFound = true;

								} else {
									last = '6';
								}

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_word;
							}

						} else if ('0' <= c && c <= '9') {
							DIGIT();
						} else {
							goto begin_word;
						}

					} else if (c == 't') {

						NEXTC();

begin_t:
						if (c == 'h') {

							NEXTC();
							
							if (c == 'r') {

								NEXTC();
								
								if (c == 'e') {

									NEXTC();
									
									if (c == 'e') {

										if (!firstDigitFound) {

											first = '3';
											last = '3';

											firstDigitFound = true;

										} else {
											last = '3';
										}

										NEXTC();

										goto begin_e;
										
									} else if ('0' <= c && c <= '9') {
										DIGIT();
									} else {
										goto begin_e;
									}

								} else if ('0' <= c && c <= '9') {
									DIGIT();
								} else {
									goto begin_word;
								}

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_word;
							}

						} else if (c == 'w') {

							NEXTC();

							if (c == 'o') {

								if (!firstDigitFound) {

									first = '2';
									last = '2';

									firstDigitFound = true;

								} else {
									last = '2';
								}
								
								NEXTC();

								goto begin_o;

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_word;
							}

						} else if ('0' <= c && c <= '9') {
							DIGIT();
						} else {
							goto begin_word;
						}

					} else if (c == 'o') {

						NEXTC();

begin_o:
						if (c == 'n') {
							
							NEXTC();

							if (c == 'e') {
								
								if (!firstDigitFound) {

									first = '1';
									last = '1';

									firstDigitFound = true;

								} else {
									last = '1';
								}

								NEXTC();

								goto begin_e;

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_n;
							}

						} else if ('0' <= c && c <= '9') {
							DIGIT();
						} else {
							goto begin_word;
						}

					} else if (c == 'f') {

						NEXTC();

						if (c == 'o') {

							NEXTC();

							if (c == 'u') {
								
								NEXTC();

								if (c == 'r') {
									
									if (!firstDigitFound) {

										first = '4';
										last = '4';

										firstDigitFound = true;

									} else {
										last = '4';
									}

								} else if ('0' <= c && c <= '9') {
									DIGIT();
								} else {
									goto begin_word;
								}

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_o;
							}

						} else if (c == 'i') {

							NEXTC();

							if (c == 'v') {
								
								NEXTC();

								if (c == 'e') {
									
									if (!firstDigitFound) {

										first = '5';
										last = '5';

										firstDigitFound = true;

									} else {
										last = '5';
									}

									NEXTC();

									goto begin_e;

								} else if ('0' <= c && c <= '9') {
									DIGIT();
								} else {
									goto begin_word;
								}

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_word;
							}

						} else if ('0' <= c && c <= '9') {
							DIGIT();
						} else {
							goto begin_word;
						}

					} else if (c == 'e') {

						NEXTC();

begin_e:
						if (c == 'i') {
							
							NEXTC();

							if (c == 'g') {
								
								NEXTC();

								if (c == 'h') {
									
									NEXTC();

									if (c == 't') {
										
										if (!firstDigitFound) {

											first = '8';
											last = '8';

											firstDigitFound = true;

										} else {
											last = '8';
										}

										NEXTC();

										goto begin_t;

									} else if ('0' <= c && c <= '9') {
										DIGIT();
									} else {
										goto begin_word;
									}

								} else if ('0' <= c && c <= '9') {
									DIGIT();
								} else {
									goto begin_word;
								}

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_word;
							}

						} else if ('0' <= c && c <= '9') {
							DIGIT();
						} else {
							goto begin_word;
						}

					} else if (c == 'n') {
							
						NEXTC();

begin_n:
						if (c == 'i') {
							
							NEXTC();

							if (c == 'n') {
								
								NEXTC();

								if (c == 'e') {
									
									if (!firstDigitFound) {

										first = '9';
										last = '9';

										firstDigitFound = true;

									} else {
										last = '9';
									}

									NEXTC();

									goto begin_e;

								} else if ('0' <= c && c <= '9') {
									DIGIT();
								} else {
									goto begin_n;
								}

							} else if ('0' <= c && c <= '9') {
								DIGIT();
							} else {
								goto begin_word;
							}

						} else if ('0' <= c && c <= '9') {
							DIGIT();
						} else {
							goto begin_word;
						}
					}
				}
			}

			int calibration_value = 10 * (int)(first - '0') + (int)(last - '0');
			
			fprintf(stderr, "chars: %c %c = %d\n", first, last, calibration_value);

			sum = sum + calibration_value;

			tosearch = newline + 1;

			fprintf(stderr, "\n");

		} else {

			break;
		}
	}

	fprintf(stderr, "sum: %d\n", sum);


  	return 0;
}








