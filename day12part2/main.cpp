
#include "common/common.h"

#include <optional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include <array>
#include <utility>
#include <unordered_map>

#include <cstdio>
#include <cinttypes>
#include <cstring>
#include <cassert>
#include <cstdint>



struct Record {
	uint64_t damage;
	// uint32_t damage_mask;
	uint64_t unknown;
	// uint32_t unknown_mask;
	int len;
	std::vector<int> sizes;

	// int possible_counts = -1;

	bool operator==(const Record& other) const {
        return damage == other.damage &&
        	unknown == other.unknown &&
        	len == other.len &&
        	sizes == other.sizes;
    }
};

namespace std {
    template <>
    struct hash<Record> {
        size_t operator()(const Record& p) const {
            // size_t h1 = damage;
            // size_t h2 = unknown;
            // size_t h2 = len;
            // size_t h3 = hash<std::string>()(p.address);
            // return h1 ^ (h2 << 1) ^ (h3 << 2);  // Combine the hash values

            size_t res = 43;

            res = 37 * res + p.damage;

            res = 37 * res + p.unknown;

        	res = 37 * res + p.len;
            
            res = 37 * res + p.sizes.size();

        	// for (auto s : p.sizes) {
        	// 	res = 37 * res + s;
        	// }

            // size_t h1 = damage;
            // size_t h2 = unknown;
            // size_t h2 = len;
            // size_t h3 = hash<std::string>()(p.address);
            // return h1 ^ (h2 << 1) ^ (h3 << 2);  // Combine the hash values

            return res;
        }
    };
}

// bool operator<(const Record& l, const Record& r) {
	
// 	if (l.len < r.len) {
// 		return true;
// 	}
// 	if (l.len > r.len) {
// 		return false;
// 	}

// 	if (l.damage < r.damage) {
// 		return true;
// 	}
// 	if (l.damage > r.damage) {
// 		return false;
// 	}

// 	if (l.unknown < r.unknown) {
// 		return true;
// 	}
// 	if (l.unknown > r.unknown) {
// 		return false;
// 	}

// 	if (l.sizes.size() < r.sizes.size()) {
// 		return true;
// 	}
// 	if (l.sizes.size() > r.sizes.size()) {
// 		return false;
// 	}

// 	for (int i = 0; i < l.sizes.size(); i++) {
// 		if (l.sizes[i] < r.sizes[i]) {
// 			return true;
// 		}
// 		if (l.sizes[i] > r.sizes[i]) {
// 			return false;
// 		}
// 	}

// 	return false;
// }



struct State {
	bool inside_group;
	int current_group_length;
	int sizes_index;

	bool operator==(const State& other) const {
        return inside_group == other.inside_group &&
        	current_group_length == other.current_group_length &&
        	sizes_index == other.sizes_index;
    }
};

namespace std {
    template <>
    struct hash<State> {
        size_t operator()(const State& p) const {
            // size_t h1 = damage;
            // size_t h2 = unknown;
            // size_t h2 = len;
            // size_t h3 = hash<std::string>()(p.address);
            // return h1 ^ (h2 << 1) ^ (h3 << 2);  // Combine the hash values

            size_t res = 43;

        	res = 37 * res + (p.inside_group ? 0 : 1);

        	res = 37 * res + p.current_group_length;

        	res = 37 * res + p.sizes_index;

        	return res;
        }
    };
}

// bool operator<(const State& l, const State& r) {
// 	if (l.current_group_length < r.current_group_length) {
// 		return true;
// 	}
// 	if (l.current_group_length > r.current_group_length) {
// 		return false;
// 	}

// 	if (l.sizes_index < r.sizes_index) {
// 		return true;
// 	}
// 	if (l.sizes_index > r.sizes_index) {
// 		return false;
// 	}

// 	if (!l.inside_group && r.inside_group) {
// 		return true;
// 	}

// 	return false;
// }



namespace std {
    template <>
    struct hash<std::pair<Record, State>> {
        size_t operator()(const std::pair<Record, State>& p) const {

        	size_t res = 43;

        	res = 37 * res + hash<Record>()(p.first);

        	res = 37 * res + hash<State>()(p.second);

            // size_t h1 = damage;
            // size_t h2 = unknown;
            // size_t h2 = len;
            // size_t h3 = hash<std::string>()(p.address);
            // return h1 ^ (h2 << 1) ^ (h3 << 2);  // Combine the hash values

            return res;
        }
    };
}

std::unordered_map<std::pair<Record, State>, std::unordered_map<State, uint64_t>> state_map;


void printRecord(Record record) {
	// ???.### 1,1,3

	std::string str;

	uint64_t cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.damage & cursor) != 0) {
			str += '#';
		} else if ((record.unknown & cursor) != 0) {
			str += '?';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	str += ' ';

	for (auto size : record.sizes) {
		str += std::to_string(size);
		str += ',';
	}

	LOGD("                           record: %s", str.c_str());

	str.clear();
	cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.damage & cursor) != 0) {
			str += '#';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	LOGD("    damage (%20" PRIu64 "): %s", record.damage, str.c_str());

	// str.clear();
	// cursor = 1;
	// for (int i = 0; i < record.len; i++) {
	// 	if ((record.damage_mask & cursor) != 0) {
	// 		str += '#';
	// 	} else {
	// 		str += '.';
	// 	}
	// 	cursor = cursor << 1;
	// }

	// LOGD(" damage_mask (%4d): %s", record.damage_mask, str.c_str());

	str.clear();
	cursor = 1;
	for (int i = 0; i < record.len; i++) {
		if ((record.unknown & cursor) != 0) {
			str += '?';
		} else {
			str += '.';
		}
		cursor = cursor << 1;
	}

	LOGD("   unknown (%20" PRIu64 "): %s", record.unknown, str.c_str());

	// str.clear();
	// cursor = 1;
	// for (int i = 0; i < record.len; i++) {
	// 	if ((record.unknown_mask & cursor) != 0) {
	// 		str += '?';
	// 	} else {
	// 		str += '.';
	// 	}
	// 	cursor = cursor << 1;
	// }

	// LOGD("unknown_mask (%4d): %s", record.unknown_mask, str.c_str());

	LOGD("len: %d", record.len);

	LOGD();
}









static_assert(sizeof(1ULL) == 8);

std::unordered_map<State, uint64_t> analyze(const Record& record, const State& input_state) {

	// LOGD("findArrangements...");

    auto it = state_map.find(std::make_pair(record, input_state));
    
	if (it != state_map.end()) {
		// LOGD("cached!");
//		auto states = state_map[std::make_pair(record, input_state)];
        auto states = (*it).second;
		// LOGD("possible_counts: %d", possible_counts);
		// return possible_counts;
		return states;
	}

	// LOGD("computing");

	// int possible_counts = 0;
	std::unordered_map<State, uint64_t> states;

	// int possible_syntactic_count = 0;

	uint64_t limit = 1ULL << record.len;

	// LOGD("limit: %" PRIu64, limit);

	// limit may be 2^64
	// xxx;

	for (uint64_t i = 0; i < limit; i++) {

		// if (i % 1000000000 == 0) {
		// 	LOGD("i: %" PRIu64, i);
		// }

		if ((record.unknown | i) != record.unknown) {

			// if (i % 99 == 17) {
			// 	LOGD("i: %d, skipping", i);
			// }

			// LOGD("i: %d, skipping (not inside unknown_mask)", i);
			continue;
		}

		if ((record.damage & i) != 0) {

			// if (i % 99 == 17) {
			// 	LOGD("i: %d, skipping", i);
			// }

			// LOGD("i: %d, skipping (inside damage)", i);
			continue;
		}

		bool possible = true;

		// possible_syntactic_count++;

		// LOGD("trying with i: %" PRIu64, i);

		uint64_t new_damage = record.damage | i;

		// uint64_t cursor = 1;
		// std::string str;
		// for (int i = 0; i < record.len; i++) {
		// 	if ((new_damage & cursor) != 0) {
		// 		str += '#';
		// 	} else {
		// 		str += '.';
		// 	}
		// 	cursor = cursor << 1;
		// }

		// LOGD("new_damage (%20" PRIu64 "): %s", new_damage, str.c_str());

		bool inside_group = input_state.inside_group;
		int current_group_length = input_state.current_group_length;
		int sizes_index = input_state.sizes_index;

		uint64_t cursor = 1;
		for (int i = 0; i < record.len; i++) {
			if ((new_damage & cursor) != 0) {
				if (inside_group) {
                    current_group_length++;
				} else {
					inside_group = true;
					// group_count = group_count + 1;
					current_group_length = 1;
                    // if (sizes_index == -1) {
                    //     sizes_index = 0;
                    // } else {
                    //     sizes_index++;
                    // }
                    sizes_index++;
				}
			} else {
				if (inside_group) {
					inside_group = false;
					// LOGD("current_group_length: %d", current_group_length);
					// LOGD("sizes_index: %d", sizes_index);
					// LOGD("record.sizes[sizes_index]: %d", record.sizes[sizes_index]);
					if (sizes_index >= record.sizes.size()) {
						// LOGD("NO 2!");
						possible = false;
						// assert(false);
						break;
					} else if (current_group_length != record.sizes[sizes_index]) {
						// assert();
                        // LOGD("NO 1!");
                        possible = false;
                        break;
                    }
//					sizes_index++;
				} else {
					// nothing
				}
			}
			cursor <<= 1;
		}

		if (possible && inside_group) {
            if (sizes_index >= record.sizes.size()) {
                // LOGD("NO 2!");
               possible = false;
            	// assert(false);
            }
            // else if (current_group_length != record.sizes[sizes_index]) {
            // 	assert(current_group_length > record.sizes[sizes_index]);
			// 	// LOGD("NO 1!");
			// 	possible = false;
			// }
			else if (current_group_length > record.sizes[sizes_index]) {
            	// assert(current_group_length > record.sizes[sizes_index]);
				// LOGD("NO 1!");
				possible = false;
			}
//			sizes_index++;
		}

		// LOGD("sizes_index: %d", sizes_index);

		// if (sizes_index != record.sizes.size()) {
		// 	// LOGD("NO 3!");
		// 	possible = false;
		// }

		// LOGD();

		if (possible) {
			// LOGD("i IS possible: %" PRIu64, i);
			
			// states.push_back(State{inside_group, current_group_length, sizes_index});

			auto state = State{inside_group, current_group_length, sizes_index};
			auto it = states.find(state);
			if (it != states.end()) {
		        auto& count = (*it).second;
				count++;
			} else {
				states[state] = 1;
			}

		} else {
			// LOGD("i is NOT possible: %" PRIu64, i);
		}
	}

	state_map[std::make_pair(record, input_state)] = states;

	// LOGD("possible_counts: %d", possible_counts);

	// LOGD("possible_syntactic_count: %d", possible_syntactic_count);

	// return possible_count;
	return states;
}




std::vector<Record> records;


Record parseRecord(const char *line) {

	auto tosearch = line;

	const char *space = strchr(tosearch, ' ');
	assert(space != NULL);

	// ?#?#?#?#?#?#?#? 1,3,1,6

	uint64_t damage = 0;
	uint64_t unknown = 0;
	// uint32_t damage_mask = 0;
	// uint32_t unknown_mask = 0;
	const char *damage_start = NULL;
	const char *damage_end = NULL;
	const char *unknown_start = NULL;
	const char *unknown_end = NULL;

	int len = 0;
	for (auto i = space - 1; i >= tosearch; i--) {
		if (*i == '#') {
			damage = damage << 1 | 1;
			unknown = unknown << 1 | 0;
			if (damage_start == NULL) {
				damage_start = i;
			}
			damage_end = i;
			len++;
		} else if (*i == '?') {
			damage = damage << 1 | 0;
			unknown = unknown << 1 | 1;
			if (unknown_start == NULL) {
				unknown_start = i;
			}
			unknown_end = i;
			len++;
		} else {
			assert(*i == '.');
			damage = damage << 1 | 0;
			unknown = unknown << 1 | 0;
			len++;
		}
	}

	// for (auto i = space - 1; i >= tosearch; i--) {
	// 	if (damage_end <= i && i <= damage_start) {
	// 		damage_mask = damage_mask << 1 | 1;
	// 	} else {
	// 		damage_mask = damage_mask << 1 | 0;
	// 	}
	// 	if (unknown_end <= i && i <= unknown_start) {
	// 		unknown_mask = unknown_mask << 1 | 1;
	// 	} else {
	// 		unknown_mask = unknown_mask << 1 | 0;
	// 	}
	// }

	// ???.### 1,1,3

	// uint32_t cursor = 1;
	// bool seen = false;
	// for (auto i = 0; i < 32; i++) {
	// 	if ((record.damage & cursor) != 0) {
	// 		seen = true;
	// 	}
	// 	cursor = cursor << 1;
	// }

	tosearch = space + 1;

	std::vector<int> sizes;

	while (1) {
		const char *comma = strchr(tosearch, ',');
		if (comma != NULL) {

			int size;
			auto res = sscanf(tosearch, "%d", &size);
			assert(res == 1);

			sizes.push_back(size);

			tosearch = comma + 1;

		} else {

			int size;
			auto res = sscanf(tosearch, "%d", &size);
			assert(res == 1);

			sizes.push_back(size);

			break;
		}
	}
	// LOGD("sizes size: %zu", sizes.size());

	// auto damage_tmp = damage;
	// auto unknown_tmp = unknown;
	// auto sizes_tmp = sizes;
	// damage = 0;
	// unknown = 0;
	// sizes.clear();
	// for (int i = 0; i < 5 - 1; i++) {
	// 	damage = (damage << len) | damage_tmp;
	// 	unknown = (unknown << len) | unknown_tmp;
	// 	sizes.insert(sizes.end(), sizes_tmp.begin(), sizes_tmp.end());
	// }

	// len = len * 2;

	return Record{damage, unknown, len, sizes};
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


			auto record = parseRecord(line);
			records.push_back(record);

			tosearch = newline + 1;

			// LOGD("");

		} else {

			break;
		}
	}


    
    // records.clear();
    
    // records.push_back(parseRecord("?????.??##?????????. 2,6,2"));



	// LOGD("records:");
	// for (auto record : records) {
	// 	printRecord(record);
	// }
	// LOGD();



	uint64_t total_possible_counts = 0;

	for (int i = 0; i < records.size(); i++) {

		auto record = records[i];

//        struct State {
//        bool inside_group;
//        int current_group_length;
//        int sizes_index;
//    };
		auto input_state = State{false, 0, -1};

		// record.sizes = std::vector<int>{3,2,1,3,2,1,3,2,1,3,2,1,3,2,1,};

		auto sizes_tmp = record.sizes;
		record.sizes.clear();
		record.sizes.insert(record.sizes.end(), sizes_tmp.begin(), sizes_tmp.end());
		record.sizes.insert(record.sizes.end(), sizes_tmp.begin(), sizes_tmp.end());
		record.sizes.insert(record.sizes.end(), sizes_tmp.begin(), sizes_tmp.end());
		record.sizes.insert(record.sizes.end(), sizes_tmp.begin(), sizes_tmp.end());
		record.sizes.insert(record.sizes.end(), sizes_tmp.begin(), sizes_tmp.end());

		std::unordered_map<State, uint64_t> states;

		uint64_t possible_counts = 0;

        {
            auto states_stage_1 = analyze(record, input_state);

            states = states_stage_1;

            // for (auto p : states) {
            // 	auto state = p.first;
            //     if (state.sizes_index == record.sizes.size() - 1) {
            //         possible_counts += p.second;
            //     }
            // }
            
            // LOGD("states size after stage 1: %zu", states.size());
            // LOGD("possible_counts after stage 1: %d", possible_counts);
        }
        
		{
			//
			// now for ? #1
			//
			std::unordered_map<State, uint64_t> states_stage_2;

			// damage
			// unknown
			// len
			// sizes
            auto opt_record = parseRecord("? 1");
            opt_record.sizes = record.sizes;
			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(opt_record, state);
				// states_stage_2.insert(states_stage_2.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_2.find(state2);
					if (it != states_stage_2.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_2[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_2;

			// int possible_counts = 0;
			// for (auto p : states) {
			// 	auto state = p.first;
			// 	if (state.sizes_index == record.sizes.size() - 1) {
			// 		possible_counts += p.second;
			// 	}
			// }

			// LOGD("states size after stage 2: %zu", states.size());
			// LOGD("possible_counts after stage 2: %d", possible_counts);
		}

		{
			std::unordered_map<State, uint64_t> states_stage_3;

			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(record, state);
				// states_stage_3.insert(states_stage_3.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_3.find(state2);
					if (it != states_stage_3.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_3[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_3;

			// int possible_counts = 0;
			// for (auto p : states) {
			// 	auto state = p.first;
			// 	if (state.sizes_index == record.sizes.size() - 1) {
			// 		possible_counts += p.second;
			// 	}
			// }

			// LOGD("states size after stage 3: %zu", states.size());
			// LOGD("possible_counts after stage 3: %d", possible_counts);
		}

		{
			//
			// now for ? #2
			//
			std::unordered_map<State, uint64_t> states_stage_4;

			// damage
			// unknown
			// len
			// sizes
            auto opt_record = parseRecord("? 1");
            opt_record.sizes = record.sizes;
			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(opt_record, state);
				// states_stage_4.insert(states_stage_4.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_4.find(state2);
					if (it != states_stage_4.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_4[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_4;

			// int possible_counts = 0;
			// for (auto p : states) {
			// 	auto state = p.first;
			// 	if (state.sizes_index == record.sizes.size() - 1) {
			// 		possible_counts += p.second;
			// 	}
			// }

			// LOGD("states size after stage 4: %zu", states.size());
			// LOGD("possible_counts after stage 4: %d", possible_counts);
		}

		{
			std::unordered_map<State, uint64_t> states_stage_5;

			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(record, state);
				// states_stage_5.insert(states_stage_5.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_5.find(state2);
					if (it != states_stage_5.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_5[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_5;

			// int possible_counts = 0;
			// for (auto p : states) {
			// 	auto state = p.first;
			// 	if (state.sizes_index == record.sizes.size() - 1) {
			// 		possible_counts += p.second;
			// 	}
			// }

			// LOGD("states size after stage 5: %zu", states.size());
			// LOGD("possible_counts after stage 5: %d", possible_counts);
		}

		{
			//
			// now for ? #3
			//
			std::unordered_map<State, uint64_t> states_stage_6;

			// damage
			// unknown
			// len
			// sizes
            auto opt_record = parseRecord("? 1");
            opt_record.sizes = record.sizes;
			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(opt_record, state);
				// states_stage_6.insert(states_stage_6.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_6.find(state2);
					if (it != states_stage_6.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_6[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_6;

			// int possible_counts = 0;
			// for (auto p : states) {
			// 	auto state = p.first;
			// 	if (state.sizes_index == record.sizes.size() - 1) {
			// 		possible_counts += p.second;
			// 	}
			// }

			// LOGD("states size after stage 6: %zu", states.size());
			// LOGD("possible_counts after stage 6: %d", possible_counts);
		}

		{
			std::unordered_map<State, uint64_t> states_stage_7;

			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(record, state);
				// states_stage_7.insert(states_stage_7.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_7.find(state2);
					if (it != states_stage_7.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_7[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_7;

			// int possible_counts = 0;
			// for (auto p : states) {
			// 	auto state = p.first;
			// 	if (state.sizes_index == record.sizes.size() - 1) {
			// 		possible_counts += p.second;
			// 	}
			// }

			// LOGD("states size after stage 7: %zu", states.size());
			// LOGD("possible_counts after stage 7: %d", possible_counts);
		}

		{
			//
			// now for ? #4
			//
			std::unordered_map<State, uint64_t> states_stage_8;

			// damage
			// unknown
			// len
			// sizes
            auto opt_record = parseRecord("? 1");
            opt_record.sizes = record.sizes;
			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(opt_record, state);
				// states_stage_8.insert(states_stage_8.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_8.find(state2);
					if (it != states_stage_8.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_8[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_8;

			// int possible_counts = 0;
			// for (auto p : states) {
			// 	auto state = p.first;
			// 	if (state.sizes_index == record.sizes.size() - 1) {
			// 		possible_counts += p.second;
			// 	}
			// }

			// LOGD("states size after stage 8: %zu", states.size());
			// LOGD("possible_counts after stage 8: %d", possible_counts);
		}

		{
			std::unordered_map<State, uint64_t> states_stage_9;

			for (auto p1 : states) {
				auto state = p1.first;
				auto states_tmp = analyze(record, state);
				// states_stage_9.insert(states_stage_9.end(), states_tmp.begin(), states_tmp.end()); xxx;
				// p1.second xxx;
				for (auto p2 : states_tmp) {
					auto state2 = p2.first;
					auto it = states_stage_9.find(state2);
					if (it != states_stage_9.end()) {
						auto& count = (*it).second;
						count += p1.second * p2.second;
					} else {
						states_stage_9[state2] = p1.second * p2.second;
					}
				}
			}

			states = states_stage_9;

			

			// LOGD("states size after stage 9: %zu", states.size());
			// LOGD("possible_counts after stage 9: %d", possible_counts);
		}

		// possible_counts = 0;
		for (auto p : states) {
			// bool inside_group;
			// int current_group_length;
			// int sizes_index;
			auto state = p.first;
			if (state.sizes_index == record.sizes.size() - 1) {
				if (state.inside_group) {
					// assert(state.current_group_length == record.sizes[state.sizes_index]);
					if (state.current_group_length == record.sizes[state.sizes_index]) {
						;
					} else {
						continue;
					}
				} else {
					;
				}
			} else {
				continue;
			}

			possible_counts += p.second;
		}

		LOGD("record #%d possible_counts: %" PRIu64, i, possible_counts);

		total_possible_counts += possible_counts;
	}

	LOGD("total_possible_counts: %" PRIu64, total_possible_counts);



  	return 0;
}








