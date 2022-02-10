#include <algorithm>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <fstream>
#include <set>
#include <vector>


#define MAX_THREADS 16 // maximum support threads

using namespace std;

thread *threads = new thread[MAX_THREADS];
condition_variable cv[MAX_THREADS];

mutex m_;
int counter=0;
set<string> unique_words;

bool isSeparator(const char c) {
    const string pattern = ".,;:\"~!#%^*()=+[]{}\\|<>?/\'@&¹";
    for (unsigned int i = 0; i < pattern.size(); i++) {
        if (pattern[i] == c)
            return true;
    }
    return false;
}

bool isDigit(const char c) {
    if ((int) c >= 0x30 && (int) c <= 0x39) return true;
    else return false;
}


int find_words(string line, vector<string>& words){

    //std::cout << endl << "LINE: "<< line << endl << "WORDS: ";
    line += ')';
    string word;

    for (unsigned int i=0; i < static_cast<uint32_t>(line.size()); i++) {
        if ( isSeparator(line[i]) || isDigit(line[i]) || line[i] == '\t' || line[i] == ' ') {
            if (word.length() > 0) {

               if ( *(word.end() - 1) == '-' ) // for avoid minus on end, word "Dniptro-" become "Dnipro"
                    word.pop_back();
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);  // make "CAT" and "cat" words as same

                words.emplace_back(word);
                //cout << *(words.cend() - 1) << " ";
                word.clear();
            }
        }
        else if (line[i] == '-' ) {
           if( word.size() > 0 && line[i-1] != '-' )  //for avoid words like "--" "Kharkov----City"
                word += line[i];
        }
        else
            word += line[i];

    }


    return words.size(); // cound words founded in a line
}



void printString(int tid, ifstream &inFile, int t_num, set<string>& _words)
{
    unique_lock<mutex> lock(m_);

    while(1)
    {
        string line;
        inFile >> line;
        string a = "";
        if(line==a)break;
        cv[(tid+1)%t_num].notify_all();
        cv[tid].wait(lock);
        vector<string> words_in_line;
        counter += find_words(line, words_in_line);

        for(auto& word: words_in_line) {
           // std::cout << tid << "  " << word << " ";
            _words.insert(word);
        }
    }
    cv[(tid+1)%t_num].notify_all();
}


int main(int argc, char* argv[])
{
    //setlocale(LC_ALL, "");

    int t_num; //number of using CPU cores
    std::ifstream inFile;
    string name;
    if( argc > 0 ) {
        inFile.open(argv[1]);
        if (inFile.fail()) {
                cerr << "Error: File not found" << endl;
                cout << "opening default data file..." <<endl;
                inFile.open("data.txt");
                if(inFile.fail())
                {
                    cerr << "Error: data.txt not found" << endl;
                    exit(1);
                }

        }
    } else {
        cerr << "Error: no given file path" << endl;
        exit(1);
    }
    string str;
    t_num =  std::thread::hardware_concurrency() % MAX_THREADS;  //recomended number of threads

    vector<set<string>> entire_words_storage(t_num);  // unique word set's for each thread

    cout << "Number of threads used: " << t_num << endl << endl;


    threads[0] = thread(printString, 0, ref(inFile), t_num, ref(unique_words));

    for(int i = 1; i < t_num; ++i) {
        threads[i] = thread(printString, i, ref(inFile), t_num, ref(entire_words_storage[i])); //init thread functions
    }

    for (int i = 0; i < t_num; ++i)
        threads[i].join();     //run parsing threads

    for (auto& words_scoupe: entire_words_storage )
        unique_words.merge(words_scoupe);       //merge results of all parser threaads, REQUIRED C++17 standart support

    /*
    cout << endl << endl << endl;                            //print unique words set's of each thread
    cout << endl << "Storage from thread 0:" << endl;
    for( auto& word: unique_words )
        cout << word << " ";
    for (int i = 1; i < t_num; i++){
        cout << endl << "Storage from thread " << i << " :" << endl;
      for( auto& word: entire_words_storage[i] )
        cout << word << " ";
   }
    */

    cout << endl << endl << "All words counter: "<< counter
         << endl << "Unique words count: " << unique_words.size() << endl;

    return 0;
}
