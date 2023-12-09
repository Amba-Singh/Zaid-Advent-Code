#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

void GetOASISPrompts(std::vector<std::vector<long long>>& number, std::fstream& file) {
    std::string line;
    while (!file.eof()) {
        std::vector<long long> pattern;
        getline(file, line);
        std::stringstream iss{ line };
        std::string num;
        while (iss >> num) {
            pattern.push_back(std::stoll(num));
        }
        number.push_back(pattern);
    }
}

long long PredictNext(std::vector<long long>& series) {
    bool consistent = true;
    std::vector<long long> range;
    long long lastDiff = series[1] - series[0];
    range.push_back(lastDiff);
    for (int i = 1; i < series.size() - 1; i++) {
        consistent = ((series[i + 1] - series[i]) == lastDiff && consistent) ? true : false;
        range.push_back(series[i + 1] - series[i]);
    }
    if (consistent) return (*(series.end() - 1) + *(range.end() - 1));
    else {
        return *(series.end() - 1) + PredictNext(range);
    }
}

long long PredictPrevious(std::vector<long long>& series) {
    bool consistent = true;
    std::vector<long long> range;
    long long lastDiff = *(series.end() - 1) - *(series.end() - 2);
    range.push_back(lastDiff);
    for (int i = series.size() - 2; i > 0; i--) {
        consistent = ((series[i] - series[i-1]) == lastDiff && consistent) ? true : false;
        range.push_back(series[i] - series[i-1]);
    }
    if (consistent) return (*(series.begin()) - *(range.end() - 1));
    else {
        std::reverse(range.begin(), range.end());
        return *(series.begin()) - PredictPrevious(range);
    }
}

long long Total(std::fstream& file) {
    std::vector<std::vector<long long>> patterns;
    GetOASISPrompts(patterns, file);
    long long total = 0;
    for (std::vector<long long> pattern : patterns) {
        total += PredictPrevious(pattern);
    }
    return total;
}


int main()
{
    std::fstream file;
    file.open("Input.txt", std::fstream::in);
    std::cout << Total(file);
    file.close();
}
