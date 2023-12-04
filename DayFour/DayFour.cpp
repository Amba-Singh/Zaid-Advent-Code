#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

void ProcessLine(std::string &line, std::vector<int>& winning, std::vector<int>& current) {
    std::istringstream iss(line.substr(line.find(":")));
    std::string number;
    bool scratch = false;
    while (iss >> number)
    {
        if (strcmp(number.c_str(), "|") == 0) {
            scratch = true;
        }
        else if (strcmp(number.c_str(), ":") != 0)
            if (scratch) current.push_back(std::stoi(number.c_str()));
            else winning.push_back(std::stoi(number.c_str()));
    }
}

int FinishLines(std::fstream &file, bool bonusCards) {
    int total = 0;
    int cardNo = 0;
    std::string line;
    std::map<int, int> numbers{};

    numbers.insert({ 1, 0 });
    while (!file.eof()) {

        std::vector<int> winner;
        std::vector<int> current;
        int num = 0;

        getline(file, line);

        ProcessLine(line, winner, current);

        for (int number : current) {
            if (std::find(winner.begin(), winner.end(), number) != winner.end()) {
                if (bonusCards) {
                    num += 1;
                }
                else {
                    if (num == 0) num = 1;
                    else num *= 2;
                }
            }
        }

        if (bonusCards) {
            for (int i = cardNo + 1; i <= cardNo + num; i++) {
                numbers.insert({ i, 0 });
                numbers[i] += numbers[cardNo] + 1;
            }
        }

        cardNo++;
        total += !bonusCards? num : 1;
    }
    if (bonusCards) {
        for (std::pair<int, int> number : numbers) {
            total += number.second;
        }
    }
    return total;
}

int main()
{
    std::fstream file;
    file.open("Input.txt", std::fstream::in);
    std::cout << FinishLines(file, true);
    file.close();
}
