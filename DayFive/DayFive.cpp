#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct instruction {
	long long source;
	long long destination;
	long long range;
};


void LineHandler(std::vector<instruction>& rellocation, std::string& line) {
	std::istringstream iss(line);
	std::string source;
	std::string destination;
	std::string range;

	iss >> destination >> source >> range;

	rellocation.push_back({ std::stoll(source), std::stoll(destination), std::stoll(range) });

}

void LocationHandler(std::vector<std::pair<long long, bool>>& seeds, std::vector<instruction>& newLocation) {
	long long i = 0;
	while (i != seeds.size()) {
		auto iter = std::find_if(newLocation.begin(), newLocation.end(), [&](instruction& element) {return (seeds[i].first >= (element.source) && seeds[i].first < (element.source + element.range)) && seeds[i].second != true; });
		if (iter != newLocation.end()) {
			seeds[i].first = (iter->destination + (seeds[i].first - iter->source));
			seeds[i].second = true;
		}
		i++;
	}
}

void GetSeeds(std::vector<std::pair<long long, bool>>& seeds, std::string& line) {
	std::istringstream iss(line);
	std::string num;
	iss >> num;
	while (iss >> num)
	{
		seeds.push_back({ std::stoll(num), false });
	}
}

long long FileHandler(std::fstream& file, bool rangeMode) {
	std::string line;
	std::vector<std::pair<long long, bool>> seeds;

	while (!file.eof()) {

		std::vector<instruction> locations;
		getline(file, line);
		if (line.find("seeds:") != std::string::npos) 
		{
			GetSeeds(seeds, line);
		}
		else if (line.empty()){}
		else if (line.find("-") == std::string::npos) {
			LineHandler(locations, line);
			LocationHandler(seeds, locations);
		}
		else {
			for (std::pair<long long, bool>& element : seeds) {
				element.second = false;
			}
		}
	}

	return std::min_element(seeds.begin(), seeds.end())->first;

}

int _main()
{
	std::fstream file;
	file.open("Input.txt", std::fstream::in);
	std::cout << FileHandler(file, false);
	file.close();
	return 0;
}
