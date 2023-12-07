#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct map {
	long long dest;
	long long sour;
	long long range;
};

void mapSingular(map& map, std::string& line) {
	std::istringstream iss(line);
	std::string source;
	std::string destination;
	std::string range;

	iss >> destination >> source >> range;

	map.dest = std::stoll(destination);
	map.sour = std::stoll(source);
	map.range = std::stoll(range);
}

void mapSetup(std::vector<map>& locations, std::fstream& file) {
	std::string line;

	getline(file, line);
	while (!line.empty() && !file.eof()) {
		map map;
		mapSingular(map, line);
		locations.push_back(map);
		getline(file, line);
	}
}

void mapLocations(std::vector<std::vector<map>>& locations, std::fstream& file) {
	std::string line;
	while (!file.eof()) {
		std::vector<map> locationMap;
		getline(file, line);
		if (line.find("-") != std::string::npos) {
			mapSetup(locationMap, file);
			locations.push_back(locationMap);
		}
	}
}

void mapRanges(std::vector<std::pair<long long, long long>>& ranges, std::string& line) {
	std::istringstream iss(line);
	std::string num;
	std::string range;
	iss >> num;
	while (iss >> num >> range)
	{
		ranges.push_back({ std::stoll(num), std::stoll(num) + std::stoll(range) - 1});
	}
}

void mapFile(std::vector<std::pair<long long, long long>>& ranges, std::vector<std::vector<map>>& locations, std::fstream& file) {
	std::string line;
	getline(file, line);
	mapRanges(ranges, line);
	mapLocations(locations, file);
}

long long closest(long long& pointer, long long& bounds, std::vector<map>& locations) {
	long long close = NULL;
	for (map& location : locations) {
		if (location.sour > pointer && location.sour <= bounds) {
			close = close == NULL || close > location.sour ? location.sour : close;
		}
	}
	return close;
}

void splitRange(std::pair<long long, long long>& range, std::vector<std::pair<long long, long long>>& newRanges,  std::vector<map>& locations) {
	bool finished = false;
	long long currPointer = range.first;
	while (!finished) {
		auto iter = std::find_if(locations.begin(), locations.end(), [&currPointer](map& instruction) { return currPointer >= instruction.sour && currPointer < instruction.sour + instruction.range; });
		if (iter != locations.end()) {
			if (range.second < iter->sour + iter->range) {
				newRanges.push_back({ (currPointer - iter->sour + iter->dest), (range.second - iter->sour  + iter->dest) });
				finished = true;
			}
			else {
				newRanges.push_back({ (currPointer - iter->sour + iter->dest), (iter->dest + iter->range) });
				currPointer = iter->sour + iter->range;
			}
		}
		else {
			auto val = closest(currPointer, range.second, locations);
			if (val != NULL){
				newRanges.push_back({range.first, val});
				currPointer = val +1;
			}
			else{
				newRanges.push_back({currPointer, range.second});
				finished = true;
			}
		}
	}
}

long long getLowestInRange(std::pair<long long, long long>& range, std::vector<std::vector<map>>& locations, int location) {
	if (location == 7) { return range.first; }

	std::vector<std::pair<long long, long long>> subRanges;
	splitRange(range, subRanges, locations[location]);
	location += 1;

	long long lowest = NULL;
	for (std::pair<long long, long long>& secondaryRange : subRanges) {
		auto val = getLowestInRange(secondaryRange, locations, location);
		lowest = lowest == NULL || lowest > val ? val : lowest;
	}
	return lowest;
}

long long getLowest(std::fstream& file) {
	std::vector<std::vector<map>> locations;
	std::vector<std::pair<long long, long long>> ranges;
	mapFile(ranges, locations, file);
	long long lowest = -1;
	for (std::pair<long long, long long>& range : ranges) {
		long long value = getLowestInRange(range, locations, 0);
		lowest = lowest == -1 || lowest > value ? value : lowest;
	}
	return lowest;
}

int main() {
	std::fstream file;
	file.open("Input.txt", std::fstream::in);
	std::cout << getLowest(file);
}