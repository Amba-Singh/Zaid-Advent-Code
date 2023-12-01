//DayOne: This program contains the solution of Day 1: Trebuchet?!
//Author: Zaid
//Date: 1/12/2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


// Function ReadLineToVector(vector(size_t), vector(int), file, boolean)
// Inputs:
// &indexes: Reference to a vector that will be filled with indices
// &numbers: Reference to a vector that will be filled with numbers at earlier mentioned indices
// &file: File to read from
// &incWords: Flag that tells the function to count "one" or not

void ReadLineToVector(std::vector<size_t>  &indexes, std::vector<int> &numbers , std::fstream& file, bool incWords) {
    if (!file.is_open()) return;                                                                                        // Check if file is open, if isn't, leave function
    if (file.eof()) return;                                                                                             // Check if file is finished
    std::string line;                                                                                                           

    std::getline(file, line);                                                                                           // Read one line
    size_t numericalIndex = 0;
    size_t stringIndex = 0;
    std::string textNumbers[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};               // Set up array of strings

    do
    {
        numericalIndex = line.find_first_of("123456789", numericalIndex);                                               // Search for first instance of a number
        if (numericalIndex != std::string::npos) {                                                                      // If a number has been found, proceed
            indexes.push_back(numericalIndex);                                                                          // Insert current index to vector
            numbers.push_back(line[numericalIndex] - '0');                                                              // Insert decimal representation of number to vector
            numericalIndex++;                                                                                           // Move onto next number
        }
    } while (numericalIndex != std::string::npos);                                                                      // Stop when no more numbers are found

    if (!incWords) return;                                                                                              // Check if we are counting words
    for (int i = 0; i < 9; i++) {
        stringIndex = 0;
        do {    
            stringIndex = line.find(textNumbers[i],stringIndex);                                                        // Find first instance of the string
            if (stringIndex != std::string::npos) {
                indexes.push_back(stringIndex);                                                                         // Insert current index to vector
                numbers.push_back(i + 1);                                                                               // Push decimal representation of number
                stringIndex++;
            }
        } while (stringIndex != std::string::npos);
    }
}

// Function FormatNumber(vector(size_t), vector(int))
// Inputs:
// &indexes: Reference to a vector that filled with indices
// &numbers: Reference to a vector that filled with numbers at earlier mentioned indices
// Outputs:
// int: Representation of number from first and second values in the string

int FormatNumber(std::vector<size_t>& indexes, std::vector<int>& numbers) {

    int minIdx = 0;                                                                                                     // Index pointing to first number
    int minVal = indexes[0];                                                                                            // Value of min index 
    int maxVal = indexes[0];                                                                                            // Value of max index
    int maxIdx = 0;                                                                                                     // Index pointing to last number

                                                                                                                        // Loop that checks if current number is the first or last number in string
    for (int i = 1; i < indexes.size(); i++) {
        if (indexes[i] < minVal) {
            minVal = indexes[i];
            minIdx = i;
        }
        else if (indexes[i] > maxVal) {
            maxVal = indexes[i];
            maxIdx = i;
        }   
    }

    return numbers[maxIdx] + (numbers[minIdx] * 10);                                                                    // Return accurate represntation of "num1num2" 
}

int main()
{
    std::fstream file;
    file.open("Input.txt", std::fstream::in);
    int total = 0;

    while (!file.eof()) {                                                                                               // Go through all lines of file
        std::vector<size_t> indexes;
        std::vector<int> numbers;
        ReadLineToVector(indexes, numbers, file, true);                                                                 // Store all numbers and their indices in the vectors
        total += FormatNumber(indexes, numbers);                                                                        // Process the vectors and total the result
    }
    file.close();
    std::cout << total;                                                                                                 // Print the result
}
