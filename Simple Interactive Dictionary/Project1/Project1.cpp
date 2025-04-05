#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Word {
    string name;
    string type;
    string definition;
};

string formatType(const string& type) {
    if (type == "n") return "Noun (n.)";
    if (type == "v") return "Verb (v.)";
    if (type == "adv") return "Adverb (adv.)";
    if (type == "adj") return "Adjective (adj.)";
    if (type == "prep") return "Preposition (prep.)";
    if (type == "misc") return "MiscWords (misc.)";
    if (type == "pn") return "ProperNoun (pn.)";
    if (type == "n_and_v") return "NounAndVerb (n. v.)";
    return "Unknown type: (" + type + ")";
}

vector<string> splitDefinitions(const string& definition) {
    vector<string> result;
    size_t start = 0;
    size_t pos;

    while ((pos = definition.find(";  ", start)) != string::npos) {
        result.push_back(definition.substr(start, pos - start));
        start = pos + 3;
    }
    result.push_back(definition.substr(start));
    return result;
}

bool loadDictionary(vector<Word>& dictionary, const string& filename, string& headerLine) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    if (!getline(file, headerLine)) return false;

    while (getline(file, line)) {
        if (line.empty()) continue;

        Word word;
        word.name = line.substr(0, line.find(';'));

        if (!getline(file, line)) break;
        word.type = line.substr(0, line.find(';'));

        if (!getline(file, line)) break;
        word.definition = line;

        getline(file, line); 

        dictionary.push_back(word);
    }

    file.close();
    return true;
}

void displayMenu() {
    cout << "\n--- Dictionary Menu ---\n";
    cout << "1. Load dictionary\n";
    cout << "2. Search for a word\n";
    cout << "3. Get a random word\n";
    cout << "4. Exit\n";
    cout << "\nEnter your choice: ";
}

void displayWord(const Word& word) {
    cout << "\nWord: " << word.name << endl;
    cout << "Type: " << formatType(word.type) << endl;
    cout << "Definition:\n";
    vector<string> definitions = splitDefinitions(word.definition);
    for (const string& def : definitions) {
        if (!def.empty())
            cout << "- " << def << endl;
    }
}

int main() {
    vector<Word> dictionary;
    string filename;
    string headerLine;
    bool isLoaded = false;

    srand(static_cast<unsigned>(time(0)));

    int choice = -1;
    do {
        displayMenu();

        string input;
        getline(cin, input);

        try {
            choice = stoi(input);
        }
        catch (...) {
            cout << "Invalid option. Please try again.\n";
            continue;
        }

        switch (choice) {
        case 1: { 
            cout << "Enter dictionary file name: ";
            cin >> filename;
            cin.ignore();

            if (!loadDictionary(dictionary, filename, headerLine)) {
                cout << "Failed to load '" << filename << "'. Please check the file and try again.\n";
            }
            else {
                isLoaded = true;
                cout << "\nDictionary Successfully Loaded!!\n\n";
                if (!headerLine.empty()) {
                    cout << headerLine << endl;
                }
            }
            break;
        }

        case 2: { 
            if (!isLoaded) {
                cout << "Error Dictionary empty!!!\n";
                break;
            }

            string searchTerm;
            cout << "Enter a word to search: ";
            cin >> searchTerm;
            cin.ignore();

            transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);

            bool found = false;
            for (const Word& word : dictionary) {
                string dictWord = word.name;
                transform(dictWord.begin(), dictWord.end(), dictWord.begin(), ::tolower);
                if (dictWord == searchTerm) {
                    displayWord(word);
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Word not found." << endl;
            }
            break;
        }

        case 3: { 
            if (!isLoaded) {
                cout << "Error Dictionary empty!!!\n";
                break;
            }

            int index = rand() % dictionary.size();
            displayWord(dictionary[index]);
            break;
        }

        case 4: // Exit
            cout << "\nExiting Program..... \nGoodbye!\n";
            break;

        default:
            cout << "Invalid option. Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}
