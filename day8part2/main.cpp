
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



enum State {
	READING_STEPS,
	READING_NODES,
};


enum Step {
	LEFT,
	RIGHT
};


struct Node {
	const char *src;
	const char *dstL;
	const char *dstR;
};


std::vector<Step> steps;

// int current_step;
// int current_node;


std::vector<Node> nodes;




void handleLine(const char *line, State state) {

	const char *tosearch;

	switch (state) {
	case READING_STEPS: {
		for (int i = 0; line[i] != '\0'; i++) {
			if (line[i] == 'L') {
				steps.push_back(LEFT);
			} else {
				assert(line[i] == 'R');
				steps.push_back(RIGHT);
			}
		}
		break;
	}
	case READING_NODES: {
		// AAA = (BBB, CCC)
		tosearch = line;
		char *src = (char *)malloc(4);
		char *dstL = (char *)malloc(4);
		char *dstR = (char *)malloc(4);
		auto res = sscanf(tosearch, "%3s = (%3s, %3s)", src, dstL, dstR);
		assert(res == 3);
		src[3] = '\0';
		dstL[3] = '\0';
		dstR[3] = '\0';
		nodes.push_back(Node{src, dstL, dstR});
		break;
	}
	}
}


//
// day8part2 
//
int main(int argc, char **argv) {

	assert(argc == 2);

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




	int index_to_use;
	auto res2 = sscanf(argv[1], "%d", &index_to_use);
	assert(res2 == 1);





	State state = READING_STEPS;

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


			switch (state) {
			case READING_STEPS:
				if (len != 0) {
					handleLine(line, READING_STEPS);
				} else {
					state = READING_NODES;
				}
				break;
			case READING_NODES:
				if (len != 0) {
					handleLine(line, READING_NODES);
				} else {
					assert(false);
				}
				break;
			default:
				assert(false);
				break;
			}


			tosearch = newline + 1;

			// LOGD("");

		} else {

			break;
		}
	}


	LOGD("steps size: %zu", steps.size());
	LOGD("nodes size: %zu", nodes.size());
	
	LOGD("nodes:");
	for (auto n : nodes) {
		LOGD("%s = (%s, %s)", n.src, n.dstL, n.dstR);
	}
	LOGD();


	int current_step = 0;
	std::vector<int> current_nodes;
	// for (int i = 0; i < nodes.size(); i++) {
	// 	auto n = nodes[i];
	// 	LOGD("looking at: %s = (%s, %s)", n.src, n.dstL, n.dstR);
	// 	if (n.src[2] == 'A') {
	// 		current_nodes.push_back(i);
	// 	}
	// }

	//
	// for JQA etc
	//
	current_nodes.push_back(index_to_use);
	
	// assert(current_node != -1);
	LOGD("current_nodes size: %zu", current_nodes.size());



	int steps_taken = 0;

	while (1) {

		// if (steps_taken % 100000 == 0) {
		// 	LOGD("steps_taken: %d", steps_taken);
		// }

		//
		//
		//
		bool all_end_with_z = true;
		// LOGD("current_nodes:");
		for (int i = 0; i < current_nodes.size(); i++) {
			int current_node = current_nodes[i];
			auto n = nodes[current_node];
			// LOGD("%s", n.src);
			if (n.src[2] != 'Z') {
				all_end_with_z = false;
			}
		}

		// LOGD("current_step: %d", current_step);
		
		if (all_end_with_z) {
			LOGI("found the Z at step: %d", steps_taken);
			// break;
		}

		Step s = steps[current_step];
		// if (s == LEFT) {
		// 	LOGD("step: LEFT");
		// } else {
		// 	LOGD("step: RIGHT");
		// }
		// LOGD();


		if (s == LEFT) {
			for (int i = 0; i < current_nodes.size(); i++) {
				auto current_node = current_nodes[i];
				auto n = nodes[current_node];
				// LOGD("searching for %s...", n.dstL);
				for (int j = 0; j < nodes.size(); j++) {
					auto nn = nodes[j];
					if (strcmp(nn.src, n.dstL) == 0) {
						// LOGD("found!");
						// LOGD("setting current_nodes[%d] to %d", i, j);
						current_nodes[i] = j;
						break;
					}
				}
			}

			// LOGD("after searching: current_nodes:");
			// for (int i = 0; i < current_nodes.size(); i++) {
			// 	auto n = nodes[i];
			// 	LOGD("%s", n.src);
			// }

		} else {
			for (int i = 0; i < current_nodes.size(); i++) {
				auto current_node = current_nodes[i];
				auto n = nodes[current_node];
				// LOGD("searching for %s...", n.dstR);
				for (int j = 0; j < nodes.size(); j++) {
					auto nn = nodes[j];
					if (strcmp(nn.src, n.dstR) == 0) {
						// LOGD("found!");
						current_nodes[i] = j;
						break;
					}
				}
			}
		}
		if (current_step == steps.size() - 1) {
			current_step = 0;
		} else {
			current_step++;
		}

		steps_taken++;
	}


	LOGD("steps_taken: %d", steps_taken);

  	return 0;
}








