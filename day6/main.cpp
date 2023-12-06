
#include "common/common.h"

#include <optional>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

#include <cstdio>
#include <cinttypes>
#include <cstring>
#include <cassert>
#include <cstdint>



enum State {
	READING_SEEDS,
	READING_SEED_TO_SOIL,
	READING_SOIL_TO_FERTILIZER,
	READING_FERTILIZER_TO_WATER,
	READING_WATER_TO_LIGHT,
	READING_LIGHT_TO_TEMPERATURE,
	READING_TEMPERATURE_TO_HUMIDITY,
	READING_HUMIDITY_TO_LOCATION
};



struct Entry {
	int64_t dest;
	int64_t source;
	int64_t range;
};


struct Pair {
	int64_t start;
	int64_t length;
    
    Pair(int64_t s, int64_t l) {
        start = s;
        length = l;
        // if (length < 0) {
        //     LOGD();
        // }
        // if (start == 0 && length == 15516332) {
        //     assert(false);
        // }
    }
};


std::vector<Pair> seeds;

std::vector<Entry> seedToSoil;

std::vector<Entry> soilToFertilizer;

std::vector<Entry> fertilizerToWater;

std::vector<Entry> waterToLight;

std::vector<Entry> lightToTemperature;

std::vector<Entry> temperatureToHumidity;

std::vector<Entry> humidityToLocation;




void printEntry(Entry entry) {
	LOGD("entry: [%" PRId64 " to %" PRId64 ") -> [%" PRId64 " to %" PRId64 ")",
		entry.source, entry.source + entry.range,
		entry.dest, entry.dest + entry.range);
}

void printPair(Pair pair) {
//    if (pair.length < 0) {
//        LOGD();
//    }
	LOGD("pair: start: %" PRId64 ", len: %" PRId64 "  [%" PRId64 " %" PRId64 ")", pair.start, pair.length, pair.start, pair.start + pair.length);
}

bool EntrySortFunction(Entry& i, Entry& j) {
	if (i.source < j.source) {
		return true;
	}

	return false;
}

void sortMap(std::vector<Entry>& map) {
	std::sort(map.begin(), map.end(), EntrySortFunction);
}







void makeContiguous(std::vector<Entry>& map) {

	std::vector<Entry> glues;

    if (map.size() <= 1) {
        return;
    }
    
	for (int i = 0; i < map.size() - 1; i++) {

		auto a = map[i];
		auto b = map[i+1];

		if (a.source + a.range != b.source) {
			LOGD("NOT CONTIGUOUS!!");

			assert(a.source + a.range < b.source);

			auto glue = Entry{a.source + a.range, a.source + a.range, b.source - (a.source + a.range)};

			glues.push_back(glue);
		}
	}

	for (auto glue : glues) {
		map.push_back(glue);
	}
}








Entry readEntry(const char *line) {

	int64_t dest;
	int64_t source;
	int64_t range;

	// LOGD("reading entry");

	auto res = sscanf(line, "%" SCNd64 " %" SCNd64 " %" SCNd64, &dest, &source, &range);
	assert(res == 3);

	return Entry{dest, source, range};
}




// struct Entry {
// 	int64_t dest;
// 	int64_t source;
// 	int64_t range;
// };


// struct Pair {
// 	int64_t start;
// 	int64_t length;
// };

std::vector<Pair> findInMap(std::vector<Pair> source, std::vector<Entry> map) {

	// LOGD("");
	// LOGD("");
	// LOGD("enter findInMap");


	// LOGD("findInMap: source:");
	// for (auto source_pair : source) {
	// 	printPair(source_pair);
	// }
	// LOGD();

	// LOGD("findInMap: map:");
	// for (auto entry : map) {
	// 	printEntry(entry);
	// }
	// LOGD();


	std::vector<Pair> dest;


	for (auto source_pair : source) {

		// LOGD("current source_pair:");
		// printPair(source_pair);

		int start_entry_index = -1;
		int end_entry_index = -1;

		for (auto i = 0; i < map.size(); i++) {

			auto entry = map[i];

			// printEntry(entry);

			if (entry.source <= source_pair.start && source_pair.start < entry.source + entry.range) {
				// LOGD("start inside entry %d", i);
				// break;
				start_entry_index = i;
			} else {
				// LOGD("start is NOT inside!");
			}

			if (entry.source <= source_pair.start + source_pair.length && source_pair.start + source_pair.length <= entry.source + entry.range) {
				// LOGD("end inside entry %d", i);
				// break;
				end_entry_index = i;
			} else {
				// LOGD("end is NOT inside!");
			}
		}

		// LOGD("start_entry_index %d", start_entry_index);
		// LOGD("end_entry_index %d", end_entry_index);

		// assert(start_entry_index != -1);
		// assert(end_entry_index != -1);

		if (start_entry_index == -1 && end_entry_index == -1) {

            // if (map.size() > 0) {
            //     assert((source_pair.start + source_pair.length < map[0].source) || (map[map.size()-1].source + map[map.size()-1].range < source_pair.start));
            //     assert((source_pair.start + source_pair.length <= map[0].source) || (map[map.size()-1].source + map[map.size()-1].range <= source_pair.start));
            // }

			// LOGD("all outside of map");

			// LOGD("mapped_pair:");
			// printPair(source_pair);

			dest.push_back(source_pair);

		} else if (start_entry_index == end_entry_index) {

			// LOGD("all within single entry");

			auto entry = map[start_entry_index];

			// printEntry(entry);

			auto mapped_start = source_pair.start - entry.source + entry.dest;
			auto mapped_end = (source_pair.start + source_pair.length) - entry.source + entry.dest;

			// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);

			auto mapped_pair = Pair{mapped_start, mapped_end - mapped_start};

			// LOGD("mapped_pair:");
			// printPair(mapped_pair);

			dest.push_back(mapped_pair);

		} else if (start_entry_index != -1 && end_entry_index == -1) {

			// LOGD("3!!");

			//
			// count from start_entry_index to last entry index
			//
			{
				auto entry = map[start_entry_index];

				// LOGD("partial start entry:");
				// printEntry(entry);

				auto start = source_pair.start;
				auto end = entry.source + entry.range;

				assert(end > start);

				// LOGD("chopped up source pair:");
				// printPair(Pair{start, end - start});

				auto mapped_start = start - entry.source + entry.dest;
				auto mapped_end = end - entry.source + entry.dest;

				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	source_pair.start, entry.source + entry.range,
				// 	mapped_start, mapped_end);

				auto mapped_pair = Pair{mapped_start, mapped_end - mapped_start};

				// LOGD("mapped_pair:");
				// printPair(mapped_pair);

				dest.push_back(mapped_pair);
			}

			//
			// entire entries to end
			//
			for (int i = start_entry_index + 1; i < map.size(); i++) {

				auto entry = map[i];

				// LOGD("entire entry:");
				// printEntry(entry);

				auto start = entry.source;
				auto end = entry.source + entry.range;

				assert(end > start);

				auto mapped_start = start - entry.source + entry.dest;
				auto mapped_end = end - entry.source + entry.dest;

				// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);
				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	entry.source, entry.source + entry.range,
				// 	mapped_start, mapped_end);

				auto mapped_pair = Pair{mapped_start, mapped_end - mapped_start};

				// LOGD("mapped_pair:");
				// printPair(mapped_pair);

				dest.push_back(mapped_pair);
			}

			//
			// then handle remaining that is not mapped
			//
			{
				auto entry = map[map.size()-1];

				// LOGD("off the end entry:");
				// printEntry(entry);

				auto start = entry.source + entry.range;
				auto end = source_pair.start + source_pair.length;

				assert(end > start);

				// LOGD("chopped up source pair:");
				// printPair(Pair{start, end - start});
				// xxx;

				// old
				// auto mapped_start = entry.source + entry.range;
				// auto mapped_end = source_pair.start + source_pair.length;

				// auto mapped_start = start - entry.source + entry.dest;
				// auto mapped_end = end - entry.source + entry.dest;

				// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);
				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	mapped_start, mapped_end,
				// 	mapped_start, mapped_end);

				auto mapped_start = start;
				auto mapped_end = end;

				auto mapped_pair = Pair{mapped_start, mapped_end - mapped_start};

				// LOGD("mapped_pair:");
				// printPair(mapped_pair);

				dest.push_back(mapped_pair);
			}

		} else if (start_entry_index != -1 && end_entry_index != -1) {

			 // LOGD("4!!");

			//
			// partial entry
			//
			{
				// assert(false);

				auto entry = map[start_entry_index];

				// LOGD("partial start entry:");
				// printEntry(entry);

				auto start = source_pair.start;
				auto end = entry.source + entry.range;

				assert(end > start);

				// LOGD("chopped up source pair:");
				// printPair(Pair{start, end - start});

				auto mapped_start = start - entry.source + entry.dest;
				auto mapped_end = end - entry.source + entry.dest;

				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	source_pair.start, entry.source + entry.range,
				// 	mapped_start, mapped_end);

				auto mapped_pair = Pair{mapped_start, mapped_end - mapped_start};

				// LOGD("mapped_pair:");
				// printPair(mapped_pair);

				dest.push_back(mapped_pair);
			}

			//
			// entire entries
			//
			for (int i = start_entry_index + 1; i < end_entry_index; i++) {

				// assert(false);

				auto entry = map[i];

				auto start = entry.source;
				auto end = entry.source + entry.range;

				assert(end > start);
				
				auto mapped_start = start - entry.source + entry.dest;
				auto mapped_end = end - entry.source + entry.dest;

				// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);
				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	entry.source, entry.source + entry.range,
				// 	mapped_start, mapped_end);

				auto mapped_pair = Pair{mapped_start, mapped_end - mapped_start};

				// LOGD("mapped_pair:");
				// printPair(mapped_pair);

				dest.push_back(mapped_pair);
			}

			//
			// partial
			//
			{
				// assert(false);

				auto entry = map[end_entry_index];

				// LOGD("partial end entry:");
				// printEntry(entry);

				auto start = entry.source;
				auto end = source_pair.start + source_pair.length;

				assert(end > start);

				// LOGD("chopped up source pair:");
				// printPair(Pair{start, end - start});

				auto mapped_start = start - entry.source + entry.dest;
				auto mapped_end = end - entry.source + entry.dest;

				// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);
				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	mapped_start, mapped_end,
				// 	mapped_start, mapped_end);

				auto mapped_pair = Pair{mapped_start, mapped_end - mapped_start};

				// LOGD("mapped_pair:");
				// printPair(mapped_pair);

				dest.push_back(mapped_pair);
			}

		} else if (start_entry_index == -1 && end_entry_index != -1) {

			// LOGD("5!!");

			//
			// handle what is not mapped
			//
			{
				// assert(false);

				auto entry = map[0];

				auto start = source_pair.start;
				auto end = entry.source;

				assert(end > start);

				auto mapped_start = start;
				auto mapped_end = end;

				// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);
				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	mapped_start, mapped_end,
				// 	mapped_start, mapped_end);

				dest.push_back(Pair{mapped_start, mapped_end - mapped_start});
			}

			//
			// entire entries
			//
			for (int i = 0; i < end_entry_index; i++) {

				// assert(false);

				auto entry = map[i];

				auto start = entry.source;
				auto end = entry.source + entry.range;

				assert(end > start);
				
				auto mapped_start = start - entry.source + entry.dest;
				auto mapped_end = end - entry.source + entry.dest;

				// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);
				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	entry.source, entry.source + entry.range,
				// 	mapped_start, mapped_end);

				dest.push_back(Pair{mapped_start, mapped_end - mapped_start});
			}

			//
			// partial
			//
			{
				// assert(false);

				auto entry = map[end_entry_index];

				auto start = entry.source;
				auto end = source_pair.start + source_pair.length;

				assert(end > start);

				auto mapped_start = start - entry.source + entry.dest;
				auto mapped_end = end - entry.source + entry.dest;

				// LOGD("mapped_start and length  %" PRId64 " %" PRId64, mapped_start, mapped_end - mapped_start);
				// LOGD("source -> mapped: [%" PRId64 " %" PRId64 ") -> [%" PRId64 " %" PRId64 ")",
				// 	mapped_start, mapped_end,
				// 	mapped_start, mapped_end);

				dest.push_back(Pair{mapped_start, mapped_end - mapped_start});
			}

		} else {
			assert(false);
		}

		// LOGD();
	}

	// LOGD("exit findInMap");
	// LOGD("");
	// LOGD("");

	return dest;
}




void handleLine(const char *line, State state) {

	const char *tosearch;
	Entry entry;

	switch (state) {
	case READING_SEEDS: {
		tosearch = line;
		const char *colon = strchr(tosearch, ':');
		assert(colon != NULL);
		tosearch = colon + 1;
		const char *space = strchr(tosearch, ' ');
		assert(space != NULL);
		tosearch = space + 1;
		int64_t start;
		int64_t length;
		while (1) {
			// space = strchr(tosearch, ' ');
			// if (space == NULL) {
			// 	LOGD("reading seed pair str: %s", tosearch);
			// 	auto res = sscanf(tosearch, "%" SCNd64 " %" SCNd64, &start, &length);
			// 	// LOGD("read seed: %" PRId64, seed);
			// 	// LOGD("%d", res);
			// 	assert(res == 2);
			// 	seeds.push_back(SeedPair{start, length});
			// 	// LOGD();
			// 	break;
			// }
			// LOGD("reading seed pair str: %s", tosearch);
			auto res = sscanf(tosearch, "%" SCNd64 " %" SCNd64 "", &start, &length);
			// LOGD("read seed: %" PRId64, seed);
			assert(res == 2);
			seeds.push_back(Pair{start, length});
			// LOGD("seeds size: %zu", seeds.size());
			space = strchr(tosearch, ' ');
			tosearch = space + 1;
			space = strchr(tosearch, ' ');
			if (space == NULL) {
				break;
			}
			tosearch = space + 1;
		}
		break;
	}
	case READING_SEED_TO_SOIL:
		entry = readEntry(line);
		seedToSoil.push_back(entry);
		// LOGD("seedToSoil size: %zu", seedToSoil.size());
		break;
	case READING_SOIL_TO_FERTILIZER:
		entry = readEntry(line);
		soilToFertilizer.push_back(entry);
		// LOGD("soilToFertilizer size: %zu", soilToFertilizer.size());
		break;
	case READING_FERTILIZER_TO_WATER:
		entry = readEntry(line);
		fertilizerToWater.push_back(entry);
		// LOGD("fertilizerToWater size: %zu", fertilizerToWater.size());
		break;
	case READING_WATER_TO_LIGHT:
		entry = readEntry(line);
		waterToLight.push_back(entry);
		// LOGD("waterToLight size: %zu", waterToLight.size());
		break;
	case READING_LIGHT_TO_TEMPERATURE:
		entry = readEntry(line);
		lightToTemperature.push_back(entry);
		// LOGD("lightToTemperature size: %zu", lightToTemperature.size());
		break;
	case READING_TEMPERATURE_TO_HUMIDITY:
		entry = readEntry(line);
		temperatureToHumidity.push_back(entry);
		// LOGD("temperatureToHumidity size: %zu", temperatureToHumidity.size());
		break;
	case READING_HUMIDITY_TO_LOCATION:
		entry = readEntry(line);
		humidityToLocation.push_back(entry);
		// LOGD("humidityToLocation size: %zu", humidityToLocation.size());
		break;
	default:
		assert(false);
		break;
	}
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

	State state = READING_SEEDS;

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
			case READING_SEEDS:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_SEEDS);
					}
				} else {
					state = READING_SEED_TO_SOIL;
				}
				break;
			case READING_SEED_TO_SOIL:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_SEED_TO_SOIL);
					}
				} else {
					state = READING_SOIL_TO_FERTILIZER;
				}
				break;
			case READING_SOIL_TO_FERTILIZER:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_SOIL_TO_FERTILIZER);
					}
				} else {
					state = READING_FERTILIZER_TO_WATER;
				}
				break;
			case READING_FERTILIZER_TO_WATER:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_FERTILIZER_TO_WATER);
					}
				} else {
					state = READING_WATER_TO_LIGHT;
				}
				break;
			case READING_WATER_TO_LIGHT:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_WATER_TO_LIGHT);
					}
				} else {
					state = READING_LIGHT_TO_TEMPERATURE;
				}
				break;
			case READING_LIGHT_TO_TEMPERATURE:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_LIGHT_TO_TEMPERATURE);
					}
				} else {
					state = READING_TEMPERATURE_TO_HUMIDITY;
				}
				break;
			case READING_TEMPERATURE_TO_HUMIDITY:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_TEMPERATURE_TO_HUMIDITY);
					}
				} else {
					state = READING_HUMIDITY_TO_LOCATION;
				}
				break;
			case READING_HUMIDITY_TO_LOCATION:
				if (len != 0) {
					if (line[len-1] == ':') {
						;
					} else {
						handleLine(line, READING_HUMIDITY_TO_LOCATION);
					}
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





	sortMap(seedToSoil);

	sortMap(soilToFertilizer);

	sortMap(fertilizerToWater);

	sortMap(waterToLight);

	sortMap(lightToTemperature);

	sortMap(temperatureToHumidity);

	sortMap(humidityToLocation);



	makeContiguous(seedToSoil);

	makeContiguous(soilToFertilizer);

	makeContiguous(fertilizerToWater);

	makeContiguous(waterToLight);

	makeContiguous(lightToTemperature);

	makeContiguous(temperatureToHumidity);

	makeContiguous(humidityToLocation);


	sortMap(seedToSoil);

	sortMap(soilToFertilizer);

	sortMap(fertilizerToWater);

	sortMap(waterToLight);

	sortMap(lightToTemperature);

	sortMap(temperatureToHumidity);

	sortMap(humidityToLocation);




	// LOGD("sorted seedToSoil:");
	// for (auto entry : seedToSoil) {
	// 	printEntry(entry);
	// }
	// LOGD();

	// LOGD("sorted soilToFertilizer:");
	// for (auto entry : soilToFertilizer) {
	// 	printEntry(entry);
	// }
	// LOGD();

	// LOGD("sorted fertilizerToWater:");
	// for (auto entry : fertilizerToWater) {
	// 	printEntry(entry);
	// }
	// LOGD();

	// LOGD("sorted waterToLight:");
	// for (auto entry : waterToLight) {
	// 	printEntry(entry);
	// }
	// LOGD();

	// LOGD("sorted lightToTemperature:");
	// for (auto entry : lightToTemperature) {
	// 	printEntry(entry);
	// }
	// LOGD();

	// LOGD("sorted temperatureToHumidity:");
	// for (auto entry : temperatureToHumidity) {
	// 	printEntry(entry);
	// }
	// LOGD();

	// LOGD("sorted humidityToLocation:");
	// for (auto entry : humidityToLocation) {
	// 	printEntry(entry);
	// }
	// LOGD();



	int64_t lowest_location = INT64_MAX;



	// LOGD("seed pairs:");
	// for (auto pair : seeds) {
	// 	printPair(pair);
	// }
	// LOGD();


	for (auto pair : seeds) {

		// LOGD("loop");

		auto start = pair.start;
		auto length = pair.length;

		// printPair(pair);

		std::vector<Pair> pair_list;
		pair_list.push_back(pair);

		// if (i % 1000000 == 0) {
		// 	LOGD("i %" PRId64, i);
		// }

		// LOGD("start seed: %" PRId64, seed);

		LOGD("calling findInMap with seedToSoil");
		auto soil = findInMap(pair_list, seedToSoil);

		LOGD("after seedToSoil:");
		for (auto pair : soil) {
			printPair(pair);
		}
		LOGD();

		// LOGD("soil: %" PRId64, soil);

		LOGD("calling findInMap with soilToFertilizer");
		auto fertilizer = findInMap(soil, soilToFertilizer);

		// // LOGD("fertilizer: %" PRId64, fertilizer);
		LOGD("after soilToFertilizer:");
		for (auto pair : fertilizer) {
			printPair(pair);
		}
		LOGD();

		LOGD("calling findInMap with fertilizerToWater");
		auto water = findInMap(fertilizer, fertilizerToWater);

		// // LOGD("water: %" PRId64, water);
		LOGD("after fertilizerToWater:");
		for (auto pair : water) {
			printPair(pair);
		}
		LOGD();

		LOGD("calling findInMap with waterToLight");
		auto light = findInMap(water, waterToLight);

		// // LOGD("light: %" PRId64, light);
		LOGD("after waterToLight:");
		for (auto pair : light) {
			printPair(pair);
		}
		LOGD();

		LOGD("calling findInMap with lightToTemperature");
		auto temperature = findInMap(light, lightToTemperature);

		// // LOGD("temperature: %" PRId64, temperature);
		LOGD("after lightToTemperature:");
		for (auto pair : temperature) {
			printPair(pair);
		}
		LOGD();

		LOGD("calling findInMap with temperatureToHumidity");
		auto humidity = findInMap(temperature, temperatureToHumidity);

		// // LOGD("humidity: %" PRId64, humidity);
		LOGD("after temperatureToHumidity:");
		for (auto pair : humidity) {
			printPair(pair);
		}
		LOGD();

		LOGD("calling findInMap with humidityToLocation");
		auto location = findInMap(humidity, humidityToLocation);

		// // LOGD("location: %" PRId64, location);
		LOGD("after humidityToLocation:");
		for (auto pair : location) {
			printPair(pair);
		}
		LOGD();

		for (auto pair : location) {

			if (pair.start < lowest_location) {
				LOGD("new lowest_location: %" PRId64, pair.start);
				lowest_location = pair.start;
			}
		}


		// LOGD("");
	}


	assert(lowest_location != INT64_MAX);


	// LOGI("sum: %d", sum);
	LOGI("lowest_location: %" PRId64, lowest_location);


  	return 0;
}








