
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



enum Type {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	FIVE_OF_A_KIND
};


void printType(Type type) {
	switch (type) {
	case HIGH_CARD:
		LOGD("HIGH_CARD");
		break;
	case ONE_PAIR:
		LOGD("ONE_PAIR");
		break;
	case TWO_PAIR:
		LOGD("TWO_PAIR");
		break;
	case THREE_OF_A_KIND:
		LOGD("THREE_OF_A_KIND");
		break;
	case FULL_HOUSE:
		LOGD("FULL_HOUSE");
		break;
	case FOUR_OF_A_KIND:
		LOGD("FOUR_OF_A_KIND");
		break;
	case FIVE_OF_A_KIND:
		LOGD("FIVE_OF_A_KIND");
		break;
	}
}


struct Hand {
	const char *str;
	Type type;

	Hand(const char *s) {
		str = s;
		assert(strlen(str) == 5);


		std::string tmp = std::string(str);

		int j_count = std::count(tmp.begin(), tmp.end(), 'J');
		LOGD("j_count: %d", j_count);

		if (j_count == 5) {
			type = FIVE_OF_A_KIND;
			return;
		}
		if (j_count == 4) {
			type = FIVE_OF_A_KIND;
			return;
		}

		//
		// j_count is at most 3
		//
		for (int i = 0; i < 5; i++) {
			if (tmp[i] == 'J') {
				tmp[i] = '!';
				break;
			}
		}
		for (int i = 0; i < 5; i++) {
			if (tmp[i] == 'J') {
				tmp[i] = '@';
				break;
			}
		}
		for (int i = 0; i < 5; i++) {
			if (tmp[i] == 'J') {
				tmp[i] = '#';
				break;
			}
		}

		std::sort(tmp.begin(), tmp.end());

		LOGD("tmp: %s", tmp.c_str());

		std::array<int, 5> runs{};
		int current_run = 0;
		int current_val = tmp[0];
		runs[0]++;
		for (int c = 1; c < 5; c++) {
			if (tmp[c] == current_val) {
				runs[current_run]++;
			} else {
				current_val = tmp[c];
				current_run++;
				runs[current_run]++;
			}
		}

		std::sort(runs.rbegin(), runs.rend());

		// add Js to longest run;

		// LOGD("hand %s has runs %d%d%d%d%d", str, runs[0], runs[1], runs[2], runs[3], runs[4]);

		switch (runs[0]) {
		case 1:
			if (j_count == 1) {
				type = ONE_PAIR;
			} else if (j_count == 2) {
				type = THREE_OF_A_KIND;
			} else if (j_count == 3) {
				type = FOUR_OF_A_KIND;
			} else {
				assert(j_count == 0);
				type = HIGH_CARD;
			}
			break;
		case 2:
			switch (runs[1]) {
			case 1:
				if (j_count == 1) {
					type = THREE_OF_A_KIND;
				} else if (j_count == 2) {
					type = FOUR_OF_A_KIND;
				} else if (j_count == 3) {
					type = FIVE_OF_A_KIND;
				} else {
					assert(j_count == 0);
					type = ONE_PAIR;
				}
				break;
			case 2:
				if (j_count == 1) {
					type = FULL_HOUSE;
				} else {
					assert(j_count == 0);
					type = TWO_PAIR;
				}
				break;
			default:
				assert(false);
				break;
			}
			break;
		case 3:
			switch (runs[1]) {
			case 1:
				if (j_count == 1) {
					type = FOUR_OF_A_KIND;
				} else if (j_count == 2) {
					// type = FULL_HOUSE;
					type = FIVE_OF_A_KIND;
				} else {
					assert(j_count == 0);
					type = THREE_OF_A_KIND;
				}
				break;
			case 2:
				assert(j_count == 0);
				type = FULL_HOUSE;
				break;
			default:
				assert(false);
				break;
			}
			break;
		case 4:
			assert(runs[1] == 1);
			if (j_count == 1) {
				type = FIVE_OF_A_KIND;
			} else {
				assert(j_count == 0);
				type = FOUR_OF_A_KIND;
			}
			break;
		case 5:
			assert(j_count == 0);
			type = FIVE_OF_A_KIND;
			break;
		default:
			assert(false);
			break;
		}

		// LOGD("hand %s has type:", str);
		// printType(type);		
	}
};



int cardVal(char c) {
	switch(c) {
	case 'J':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'T':
		return 10;
	// case 'J':
	// 	return 11;
	case 'Q':
		return 12;
	case 'K':
		return 13;
	case 'A':
		return 14;
	default:
		assert(false);
		return -1;
	}
}


bool operator<(const Hand& l, const Hand& r) {
	
	if (l.type < r.type) {
		return true;
	}

	if (l.type > r.type) {
		return false;
	}

	if (cardVal(l.str[0]) < cardVal(r.str[0])) {
		return true;
	}
	if (cardVal(l.str[0]) > cardVal(r.str[0])) {
		return false;
	}

	if (cardVal(l.str[1]) < cardVal(r.str[1])) {
		return true;
	}
	if (cardVal(l.str[1]) > cardVal(r.str[1])) {
		return false;
	}

	if (cardVal(l.str[2]) < cardVal(r.str[2])) {
		return true;
	}
	if (cardVal(l.str[2]) > cardVal(r.str[2])) {
		return false;
	}

	if (cardVal(l.str[3]) < cardVal(r.str[3])) {
		return true;
	}
	if (cardVal(l.str[3]) > cardVal(r.str[3])) {
		return false;
	}

	if (cardVal(l.str[4]) < cardVal(r.str[4])) {
		return true;
	}
	if (cardVal(l.str[4]) > cardVal(r.str[4])) {
		return false;
	}

	// LOGD("same card");
	return false;
}


void printHand(Hand h) {
	LOGD("%s", h.str);
}


struct HandAndBid {
	Hand hand;
	int bid;
};


std::vector<HandAndBid> handAndBids;


Hand parseHand(const char *str) {
	return Hand{str};
}


void handleLine(const char *line) {


	auto tosearch = line;
	const char *space = strchr(tosearch, ' ');

	assert(space);

	auto len = space - tosearch;
	auto handStr = new char[len + 1];

	memcpy(handStr, tosearch, len);

	handStr[len] = '\0';

	tosearch = space + 1;

	auto bidStr = tosearch;

	Hand hand = parseHand(handStr);

	int bid;
	auto res = sscanf(bidStr, "%d", &bid);
	assert(res == 1);

	handAndBids.push_back(HandAndBid{hand, bid});
}


bool HandAndBidSortFunction(HandAndBid& i, HandAndBid& j) {
	return i.hand < j.hand;
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

	while (1) {

		const char *newline = strchr(tosearch, '\n');

		if (newline != NULL) {

			auto len = newline - tosearch;
			auto line = new char[len + 1];

			memcpy(line, tosearch, len);

			line[len] = '\0';

			// LOGD("line: %s", line);
			// LOGD("len: %ld", newline - tosearch);


			handleLine(line);


			tosearch = newline + 1;

			// LOGD("");

		} else {

			break;
		}
	}


	// LOGD("handAndBids size: %zu", handAndBids.size());

	std::sort(handAndBids.begin(), handAndBids.end(), HandAndBidSortFunction);

	int total_winnings = 0;
	int rank = 1;
	for (auto hb : handAndBids) {

		printHand(hb.hand);

		LOGD("bid: %d", hb.bid);
		LOGD("rank: %d", rank);

		total_winnings = total_winnings + hb.bid * rank;

		rank++;
		LOGD();
	}

	LOGI("total_winnings: %d", total_winnings);

  	return 0;
}








