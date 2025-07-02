#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C"
#endif

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <set>
#include "json.hpp"

using namespace std;
using nlohmann::json;

unsigned int get_true_index(vector<unsigned int>& aux, unsigned int index) {
    if (aux.size() == 0) return index;
    unsigned int count = 0;
    unsigned int first = 0, last = aux.size();
    
    unsigned int mid = (first + last)/2;
    while (last - first > 1) {
        if (aux[mid] > index) {
            last = mid;
            mid = (first + mid)/2;
        } else {first = mid; mid = (mid + last)/2;}
    }
    if (aux[mid] < index) {
        count += mid + 1;
        index += count;
        mid++;
        while ((!(index < aux[mid]) && mid < aux.size())) {
            index++;
            mid++;
        }
    } else {
        count += mid;
        index += count;
        while ((!(index < aux[mid]) && mid < aux.size())) {
            index++;
            mid++;
        }
    }

    return index;
}


pair<string, unsigned int> text_processing(string const& s, string const& t, vector<unsigned int>& aux, bool case_s, bool present, set<char>& puncs) {
    string text(s.size() + t.size() + 3, ' ');
    for (unsigned int i = 0; i < s.size(); i++)
    {
        text[i] = s[i];
    }
    
    text[s.size()] = '\0';
    text[s.size() + 1] = ' ';

    unsigned int j = s.size() + 2;

    if (case_s) {
        for (unsigned int i = 0; i < t.size(); i++) {
        if ((t[i] - 'a' >= 0 && t[i] - 'a' < 26) || (t[i] - 'A' >= 0 && t[i] - 'A' < 26))
            {text[j] = t[i]; j++;}
        else { switch(t[i]) {
            case ' ':
            if (text[j-1] == ' ') aux.push_back(i);
            else text[j++] = t[i];
            break;
            case ',':
            if ((present && puncs.contains(',')) || (!present && !puncs.contains(','))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            
            case '.':
            if ((present && puncs.contains('.')) || (!present && !puncs.contains('.'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case '?':
            if ((present && puncs.contains('?')) || (!present && !puncs.contains('?'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case '!':
            if ((present && puncs.contains('!')) || (!present && !puncs.contains('!'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case '\'':
            if ((present && puncs.contains('\'')) || (!present && !puncs.contains('\''))) {text[j++] = t[i];}
            else aux.push_back(i);
            break;
            case '\"':
            if ((present && puncs.contains('\"')) || (!present && !puncs.contains('\"'))) {text[j++] = t[i];}
            else aux.push_back(i);
            break;
            case '-':
            if ((present && puncs.contains('-')) || (!present && !puncs.contains('-'))) {text[j++] = t[i];}
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else {text[j++] = ' ';}}
            break;
            case '_':
            if ((present && puncs.contains('_')) || (!present && !puncs.contains('_'))) {text[j++] = t[i];}
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else {text[j++] = ' ';}}
            break;
            case '(':
            if ((present && puncs.contains('(')) || (!present && !puncs.contains('('))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case ')':
            if ((present && puncs.contains(')')) || (!present && !puncs.contains(')'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case ':':
            if ((present && puncs.contains(':')) || (!present && !puncs.contains(':'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case ';':
            if ((present && puncs.contains(';')) || (!present && !puncs.contains(';'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            default:
            text[j++] = t[i];
            break;
        }
        }
    }
    } else {
        for (unsigned int i = 0; i < t.size(); i++) {
        if ((t[i] - 'a' >= 0 && t[i] - 'a' < 26))
            {text[j++] = t[i];}
        else if (t[i] - 'A' >= 0 && t[i] - 'A' < 26) 
            {text[j] = t[i] - 'A' + 'a';
            j++;}
        else { switch(t[i]) {
            case ' ':
            if (text[j-1] == ' ') aux.push_back(i);
            else text[j++] = t[i];
            break;
            case ',':
            if ((present && puncs.contains(',')) || (!present && !puncs.contains(','))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            
            case '.':
            if ((present && puncs.contains('.')) || (!present && !puncs.contains('.'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case '?':
            if ((present && puncs.contains('?')) || (!present && !puncs.contains('?'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case '!':
            if ((present && puncs.contains('!')) || (!present && !puncs.contains('!'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case '\'':
            if ((present && puncs.contains('\'')) || (!present && !puncs.contains('\''))) {text[j++] = t[i];}
            else aux.push_back(i);
            break;
            case '\"':
            if ((present && puncs.contains('\"')) || (!present && !puncs.contains('\"'))) {text[j++] = t[i];}
            else aux.push_back(i);
            break;
            case '-':
            if ((present && puncs.contains('-')) || (!present && !puncs.contains('-'))) {text[j++] = t[i];}
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else {text[j++] = ' ';}}
            break;
            case '_':
            if ((present && puncs.contains('_')) || (!present && !puncs.contains('_'))) {text[j++] = t[i];}
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else {text[j++] = ' ';}}
            break;
            case '(':
            if ((present && puncs.contains('(')) || (!present && !puncs.contains('('))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case ')':
            if ((present && puncs.contains(')')) || (!present && !puncs.contains(')'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case ':':
            if ((present && puncs.contains(':')) || (!present && !puncs.contains(':'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            case ';':
            if ((present && puncs.contains(';')) || (!present && !puncs.contains(';'))) text[j++] = t[i];
            else {if (text[j-1] == ' ') aux.push_back(i);
                                                else text[j++] = ' ';}
            break;
            default:
            text[j++] = t[i];
            break;
        }
        }
    }
    }

    if (text.back() != ' ') text[j++] = ' ';

    return {text, j};
}

vector<unsigned int> prefix_function(pair<string, unsigned int> p) {
    unsigned int n = (unsigned int)p.second;
    vector<unsigned int> pi(n, 0);
    for (unsigned int i = 1; i < n; i++) {
        unsigned int j = pi[i-1];
        while (j > 0 && (p.first[i] != p.first[j]))
            j = pi[j-1];
        if (p.first[i] == p.first[j])
            j++;
        pi[i] = j;
    }
    return pi;
}

vector<unsigned int> kmp(const vector<unsigned int> pi, const unsigned int ls) {
    vector<unsigned int> occurences;
    for (unsigned int i = 2*ls; i < pi.size(); i++) {
        if (pi[i] == ls) {
            occurences.push_back(i - 2*ls - 1);
        }
        else {
            i += ls - pi[i] - 1; // -1 because of i++
        }
    }
    return occurences;
}

bool is_text_file(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in);
    if (!file.is_open()) return false;

    char ch;
    int count = 0;
    while (file.get(ch) && count < 100) {
        if (!(isprint(ch) || isspace(ch))) return false;
        count++;
    }
    return true;
}

DLL_EXPORT const char* string_matching(const char* s_c, const char* dir_c, bool case_s, const char* puncts) {
    string s(s_c);
    string dir(dir_c);
    string t;

    std::string puncts_str(puncts);
    set<char> punct;

    for (char c : puncts_str) {
        if (c != ' ' && c != '\n' && c != '\0') punct.insert(c);
    }
    
    // cout << "Pattern : ";
    // getline(cin, s);
    
    vector<unsigned int> out;
    vector<unsigned int> auxi;
    // cout << "Directory to search in : ";
    // cin >> dir;
    // set<char> punct;
    // cout << "Enter punctuation characters to match (as a single line, or leave empty for none): ";
    // string punc_line;
    // getline(cin, punc_line, '\n');
    // for (char ch : punc_line) {
    //     if (ch != ' ') {
    //         punct.insert(ch);
    //     }
    // }
    bool present = true;

    // cout << "Case sensitive? ";
    // cin >> case_s;

    int line_count = 0;
    int file_count = 0;
    string curr;
    json results = json::array();
    vector<string> supported_ext{".txt", ".cpp", ".c", ".h", ".json"};
    bool can_do = 0;
    for(const auto& entry : filesystem::directory_iterator(dir)) {
        curr = entry.path().filename().string();
        if (is_text_file(dir + curr))
        {
        ifstream file(dir + curr);
        file_count++;
        line_count = 0;
        json fileResult;
        fileResult["filePath"] = curr;
        fileResult["matches"] = json::array();
        while (getline(file, t)) {
            line_count++;
            out = kmp(prefix_function(text_processing(s, t, auxi, case_s, present, punct)), s.size());
            
            for (auto x : out) {
                fileResult["matches"].push_back({
                {"line", line_count},
                {"position", get_true_index(auxi, x) + 1},
                {"lineText", t}
                });
                // cout << curr << " : " << line_count << " " << get_true_index(auxi, x) + 1 << '\n';
            }
            auxi.clear();
        }
        file.close();
        if (!fileResult["matches"].empty())
            results.push_back(fileResult);}
        else continue;
   }

    std::string result = results.dump();
    char* buffer = (char*) malloc(result.size() + 1);
    strcpy(buffer, result.c_str());
    return buffer;
}

DLL_EXPORT void free_result(const char* result) {
    free((void*)result);
}