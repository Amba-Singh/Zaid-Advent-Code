

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

void MapLine(std::vector<size_t>& indices, std::string& line) {
	size_t currentIndex;
	currentIndex = line.find_first_not_of("1234567890.");
	while (currentIndex != std::string::npos)
	{
		indices.push_back(currentIndex);
		currentIndex = line.find_first_not_of("1234567890.", ++currentIndex);
	}
}

void MapGears(std::vector<size_t>& indices, std::string& line) {
	size_t currentIndex;
	currentIndex = line.find_first_of("*");
	while (currentIndex != std::string::npos)
	{
		indices.push_back(currentIndex);
		currentIndex = line.find_first_of("*", ++currentIndex);
	}
}

void FindNumbersPerLine(std::string& line, size_t Gindex, std::vector<int> &numbers) {

	int index = line.find_first_of("1234567890");
	bool found = false;
	int begin = -1;

	while (index != std::string::npos)
	{

		for (int i = index - 1; i < index + 2; i++) {
			if (i == Gindex) found = true;
		}

		if (begin == -1) begin = index;

		if ((index == line.length() - 1) || (!std::isdigit(line[++index]))) {
			if (found) {
				found = false;
				numbers.push_back(std::stoi(line.substr(begin, index - begin)));
			}
			begin = -1;
		}

		index = line.find_first_of("1234567890", index);

		if (index == line.length() - 1) {
			if (found) {
				numbers.push_back(std::stoi(line.substr(begin, index - begin + 1)));
			}
			break;
		}
	}
}

int TotalGear(std::string& current, std::string& above, std::string& below) {

	std::vector<size_t> currentLine;

	MapGears(currentLine, current);

	int total = 0;

	for (size_t gearIndex : currentLine) {

		std::vector<int> numbers;

		FindNumbersPerLine(above, gearIndex, numbers);
		FindNumbersPerLine(current, gearIndex, numbers);
		FindNumbersPerLine(below, gearIndex, numbers);

		if (numbers.size() == 2) { total += numbers[0] * numbers[1]; }
	}

	return total;
}

int TotalGear(std::string& current, std::string& limit) {


	std::string empty = "";
	return TotalGear(current, limit, empty);

}

int TotalGear(std::string& current) {


	std::string empty = "";
	return TotalGear(current, empty, empty);

}

int TotalLine(std::string &current, std::string &above, std::string &below) {

	std::vector<size_t> currentLine;
	std::vector<size_t> aboveLine;
	std::vector<size_t> belowLine;

	MapLine(currentLine, current);
	MapLine(aboveLine, above);
	MapLine(belowLine, below);

	int total = 0;
	bool found = false;
	int index = current.find_first_of("1234567890");
	int begin = -1;
	while (index != std::string::npos)
	{

		for (int i = index - 1; i < index + 2; i++) {
			if (std::find(currentLine.begin(), currentLine.end(), i) != currentLine.end() || std::find(aboveLine.begin(), aboveLine.end(), i) != aboveLine.end() || std::find(belowLine.begin(), belowLine.end(), i) != belowLine.end()) found = true;
		}

		if (begin == -1) begin = index;

		if ((index == current.length() - 1) || (!std::isdigit(current[++index]))) {
			if (found) {
				found = false;
				total += std::stoi(current.substr(begin, index-begin));
			}
			begin = -1;
		}

		index = current.find_first_of("1234567890", index);

		if (index == current.length() - 1) {
			if (found) {
				total += std::stoi(current.substr(begin, index - begin + 1));
			}
			break;
		}
	}
	return total;
}



int TotalLine(std::string &current, std::string &limit) {


	std::string empty = "";
	return TotalLine(current, limit, empty);

}

int TotalLine(std::string& current) {


	std::string empty = "";
	return TotalLine(current, empty, empty);

}

int SumPartNumbers(std::fstream& file) {
	if (!file.is_open()) return 0;

	std::string current;
	std::string above;
	std::string below;
	int total = 0;

	if (!file.eof()) getline(file, above);
	else return 0;

	if (!file.eof()) getline(file, current);
	else return TotalLine(above);

	if (!file.eof()) getline(file, below);
	else return TotalLine(above, current) + TotalLine(current, above);

	total += TotalLine(above, current);

	while (!file.eof()) {
		total += TotalLine(current, above, below);

		above = current;
		current = below;
		getline(file, below);
	}

	total += TotalLine(current, above, below) + TotalLine(below, current);

	return total;
}

int GearRatios(std::fstream& file) {
	if (!file.is_open()) return 0;

	std::string current;
	std::string above;
	std::string below;
	int total = 0;

	if (!file.eof()) getline(file, above);
	else return 0;

	if (!file.eof()) getline(file, current);
	else return TotalGear(above);

	if (!file.eof()) getline(file, below);
	else return TotalGear(above, current) + TotalGear(current, above);

	total += TotalGear(above, current);

	while (!file.eof()) {
		total += TotalGear(current, above, below);

		above = current;
		current = below;
		getline(file, below);
	}

	total += TotalGear(current, above, below) + TotalGear(below, current);

	return total;
}

int main()
{
	std::fstream file;
	file.open("Input.txt", std::fstream::in);
	std::cout << GearRatios(file);
	file.close();
}
