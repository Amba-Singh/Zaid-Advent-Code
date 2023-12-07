#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

long long CalculateRange(long long& time, long long& distance) {
    auto a = -1;
    auto b = time;
    auto c = -distance;
    long long low = std::floor((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
    long long high = std::floor((-b - sqrt(b * b - 4 * a * c)) / (2 * a));

    if ((time - high) * (high) == distance) high -= 1;
    return high - low;
}

void GetTimeDist(std::vector<std::pair<long long, long long>>& TimeDistance, std::istringstream& time, std::istringstream& dist, bool mode) {
    std::string tim;
    std::string dis;

    time >> tim;
    dist >> dis;

    std::string finalT;
    std::string disT;

    if (!mode) {
        while (time >> tim) {
            dist >> dis;
            TimeDistance.push_back({ std::stol(tim), std::stol(dis) });
        }
    }
    else {
        while (time >> tim) {
            dist >> dis;
            finalT.append(tim);
            disT.append(dis);
        }
        TimeDistance.push_back({ std::stoll(finalT), std::stoll(disT) });
    }
}

long long TotalRange(std::fstream& file) {
    std::string line;
    if (!file.eof()) getline(file, line);
    std::istringstream iss1(line);
    if (!file.eof()) getline(file, line);
    std::istringstream iss2(line);
    
    std::vector<std::pair<long long, long long>> TimeDist;
    GetTimeDist(TimeDist, iss1, iss2, true);

    int total = 1;
    for (std::pair<long long, long long> pair : TimeDist) {
        total *= CalculateRange(pair.first, pair.second);
    }
    return total;
}

int main()
{
    std::fstream file;
    file.open("Input.txt", std::fstream::in);
    std::cout << TotalRange(file);
    file.close();
}