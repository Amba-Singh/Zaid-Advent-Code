#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>

struct Flags {
    bool FiveK = false;
    bool FourK = false;
    bool FullH = false;
    bool ThreK = false;
    bool TwoP = false;
    bool OneP = false;
    bool High = true;
};

class Hand {
public:
    Flags f;
    std::string cards;
    int bid;
    int Type = 1;
    int rank;

    Hand(std::string c, int b) : bid{ b }, cards{ c } { SetUpFlags(); SetUpType(); }

    int returnCardVal(int idx) {
        if (cards[idx] == 'A') return 14;
        else if (cards[idx] == 'K') return 13;
        else if (cards[idx] == 'Q') return 12;
        else if (cards[idx] == 'J') return 1;
        else if (cards[idx] == 'T') return 10;
        else return cards[idx] - '0';
    }

private:
    void SetUpFlags() {
        std::string doneChars;
        int JAmount = 0;
        for (int i = 0; i < cards.size(); i++) {
            if (doneChars.find(cards[i]) == std::string::npos) {
                auto amount = std::count_if(cards.begin(), cards.end(), [&](char& character) { return cards[i] == character; });
                if (cards[i] != 'J') {
                    switch (amount)
                    {
                    case 1:
                        break;
                    case 2:
                        if (f.OneP) {
                            f.TwoP = true; f.OneP = false;
                        }
                        else f.OneP = true;
                        f.High = false;
                        break;
                    case 3:
                        f.ThreK = true;
                        f.High = false;
                        break;
                    case 4:
                        f.FourK = true;
                        f.High = false;
                        break;
                    case 5:
                        f.FiveK = true;
                        f.High = false;
                        break;
                    }
                }
                else {
                    JAmount = amount;
                }
                doneChars.push_back(cards[i]);
            }

        }
        if (JAmount == 5 || JAmount == 4 || (JAmount == 3 && f.OneP) || (JAmount == 2 && (f.ThreK)) || (JAmount == 1 && f.FourK)) { f.FiveK = true; f.OneP = false; f.ThreK = false; f.FourK = false; f.High = false; }
        else if ((JAmount == 3 && f.High) || (JAmount == 2 && f.OneP) || (JAmount == 1 && (f.ThreK))) { f.FourK = true; f.High = false; f.OneP = false; f.ThreK = false; }
        else if (JAmount == 1 && f.TwoP) { f.FullH = true; f.TwoP = false; }
        else if ((JAmount == 2 && f.High) || (JAmount == 1 && (f.OneP))) { f.ThreK = true; f.High = false; f.OneP = false; f.TwoP = false; }
        else if (JAmount == 1 && f.High) { f.OneP = true; f.High = false; }

        else if (f.OneP && f.ThreK) {
            f.OneP = false;
            f.ThreK = false;
            f.FullH = true;
        }
    }

    void SetUpType() {
        if (f.FiveK) Type = 7;
        if (f.FourK) Type = 6;
        if (f.FullH) Type = 5;
        if (f.ThreK) Type = 4;
        if (f.TwoP) Type = 3;
        if (f.OneP) Type = 2;
        if (f.High) Type = 1;
    }
};

void ProcessLines(std::vector<Hand>& hand, std::fstream& file) {
    std::string line;
    while (!file.eof()) {
        getline(file, line);
        std::istringstream iss{ line };
        std::string cards;
        std::string bid;

        iss >> cards >> bid;
        hand.push_back({cards, std::stoi(bid)});
    }
}

void EditRanks(std::vector<Hand>& hand) {
    std::sort(hand.begin(), hand.end(), [](Hand& pointerA, Hand& pointerB) { 
        if (pointerA.Type > pointerB.Type) return true;
        else if (pointerA.Type == pointerB.Type) {
            for (int i = 0; i < pointerA.cards.size(); i++) {
                if (pointerA.returnCardVal(i) > pointerB.returnCardVal(i)) return true;
                else if (pointerA.returnCardVal(i) < pointerB.returnCardVal(i)) return false;
            }
        }
        else return false;
        });

    int i = hand.size();
    for (Hand& h : hand) {
        h.rank = i--;
    }
}

long long Value(std::fstream& file) {
    std::vector<Hand> hand;
    ProcessLines(hand, file);
    EditRanks(hand);
    long long total = 0;

    for (Hand& h : hand) {
        total += h.rank * h.bid;
    }
    return total;
}

int main()
{
    std::fstream file;
    file.open("Input.txt", std::fstream::in);
    std::cout << Value(file);
    file.close();
}