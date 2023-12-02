//DayOne: This program contains the solution of Day 2: Cube Conundrum
//Author: Zaid
//Date: 2/12/2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


// Structure that contains RGB values per section
struct RGBobject {
	int green = 0;
	int blue = 0;
	int red = 0;
};

// Takes a line and builds sections from it onto a vector
void VectorBuilder(std::vector<RGBobject> &sections, std::istringstream &iss) {
	std::string currentWord;

	std::string colors[] = { "green", "blue", "red" };
	int G = 0;
	int B = 0;
	int R = 0;
	int temp = 0;
	bool sequenceBreak = false;

	// Go word by word
	while (std::getline(iss, currentWord, ' ')) {

		sequenceBreak = currentWord.find(';') == std::string::npos ? false : true;												// Check if line is the last of the section

		if (currentWord.find(';') != std::string::npos) { currentWord.pop_back(); }												// Remove ; from word for later usage
		if (currentWord.find(',') != std::string::npos) { currentWord.pop_back(); }												// Remove , from word for later usage

		if (std::all_of(currentWord.begin(), currentWord.end(), ::isdigit)) {													// Check if the current word is a number
			temp = std::stoi(currentWord.c_str());																				// Convert all digits to one int
		}
		else {
			for (int i = 0; i < 3; i++) {																						// Run through list of possible colors
				if (strcmp(currentWord.c_str(), colors[i].c_str()) == 0) {														// Assign the correct number to the correct colors
					switch (i) {
					case 0:
						G = temp;
						break;
					case 1:
						B = temp;
						break;
					case 2:
						R = temp;
						break;
					}
				}
			}
			if (sequenceBreak) {																								// If section is done, add section to vector
				sections.push_back({ G, B, R });
				G = 0;
				B = 0;
				R = 0;
			}
		}
	}
	sections.push_back({ G, B, R });
}

// Checks if provided vector complies with any limitations 
bool Complies(std::vector<RGBobject>& sections, RGBobject& limits) {												

	for (RGBobject section : sections) {
		if (section.blue > limits.blue || section.green > limits.green || section.red > limits.red) return false;
	}

	return true;
}

// Calculates the product of all three 'Least' colors from a vector
int Powers(std::vector<RGBobject>& sections) {

	int total = 0;
	int G = 0;
	int B = 0;
	int R = 0;

	for (RGBobject section : sections) {
		G = section.green > G ? section.green : G;
		R = section.red > R ? section.red : R;
		B = section.blue > B ? section.blue : B;
	}

	return R*G*B;
}

// Responsible for reading file and processing all lines
// Mode false: Powers
// Mode true: Complies
int ProcessLines(std::fstream& file, bool mode) {
	if (!file.is_open()) return 0;
	else if (file.eof()) return 0;

	int total = 0;
	int ID = 1;
	RGBobject limits{ 13, 14, 12 };

	while (!file.eof()) {

		std::vector<RGBobject> sections;
		std::string line;

		if (!file.eof()) getline(file, line);

		std::istringstream stringStream(line);

		VectorBuilder(sections, stringStream);

		if (mode) {
			if (Complies(sections, limits)) total += ID;
			ID++;
		}
		else {
			total += Powers(sections);
		}
	}

	return total;
}

int main() {

	std::fstream file;
	file.open("Input.txt", std::fstream::in);
	std::cout << ProcessLines(file, false);

}