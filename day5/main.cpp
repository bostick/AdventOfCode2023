
#include "common/common.h"

#include <optional>
#include <vector>
#include <set>
#include <map>

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


std::vector<int64_t> seeds;

std::vector<Entry> seedToSoil;

std::vector<Entry> soilToFertilizer;

std::vector<Entry> fertilizerToWater;

std::vector<Entry> waterToLight;

std::vector<Entry> lightToTemperature;

std::vector<Entry> temperatureToHumidity;

std::vector<Entry> humidityToLocation;



Entry readEntry(const char *line) {

	int64_t dest;
	int64_t source;
	int64_t range;

	// LOGD("reading entry");

	auto res = sscanf(line, "%" SCNd64 " %" SCNd64 " %" SCNd64, &dest, &source, &range);
	assert(res == 3);

	return Entry{dest, source, range};
}



int64_t findInMap(int64_t source, std::vector<Entry> map) {

	for (auto entry : map) {
		if (entry.source <= source && source <= entry.source + entry.range) {
			return source - entry.source + entry.dest;
		}
	}

	return source;
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
		int64_t seed;
		while (1) {
			space = strchr(tosearch, ' ');
			if (space == NULL) {
				// LOGD("reading seed");
				auto res = sscanf(tosearch, "%" SCNd64, &seed);
				// LOGD("read seed: %" PRId64, seed);
				assert(res == 1);
				seeds.push_back(seed);
				// LOGD();
				break;
			}
			// LOGD("reading seed");
			auto res = sscanf(tosearch, "%" SCNd64, &seed);
			// LOGD("read seed: %" PRId64, seed);
			assert(res == 1);
			seeds.push_back(seed);
			// LOGD("seeds size: %zu", seeds.size());
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



	int64_t lowest_location = INT64_MAX;


	for (auto seed : seeds) {

		LOGD("seed: %" PRId64, seed);

		auto soil = findInMap(seed, seedToSoil);

		LOGD("soil: %" PRId64, soil);

		auto fertilizer = findInMap(soil, soilToFertilizer);

		LOGD("fertilizer: %" PRId64, fertilizer);

		auto water = findInMap(fertilizer, fertilizerToWater);

		LOGD("water: %" PRId64, water);

		auto light = findInMap(water, waterToLight);

		LOGD("light: %" PRId64, light);

		auto temperature = findInMap(light, lightToTemperature);

		LOGD("temperature: %" PRId64, temperature);

		auto humidity = findInMap(temperature, temperatureToHumidity);

		LOGD("humidity: %" PRId64, humidity);

		auto location = findInMap(humidity, humidityToLocation);

		LOGD("location: %" PRId64, location);


		if (location < lowest_location) {
			lowest_location = location;
		}


		LOGD("");
	}




	// LOGI("sum: %d", sum);
	LOGI("lowest_location: %" PRId64, lowest_location);


  	return 0;
}








