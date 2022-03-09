#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <map>
#include <vector>

using namespace std;

void solve(const string& filename) {
    // prepare in- and out-files
    cout << "Processing: " << filename << "\n";
    const string outfilename = "solution." + filename;

    // set up file streams
    ifstream input;
    ofstream output;
    input.open(".\\..\\input_data\\"+filename, ios::in);
    output.open(".\\..\\output\\"+outfilename, ios::out);
    if(input.fail() || output.fail()){
        cout << "ERROR: handling the files went wrong." << "\n";
        exit(1);
    }

    // prepare data containers
    map<string, map<string, int>> contributors;
    typedef struct {
        int days, score, bestbefore;
        vector<pair<string, int>> roles;
    } project;
    
    map<string, project> projects;

    // get cont and proj count
    int cont_count = 0, proj_count = 0;   
    input >> cont_count >> proj_count; // input knows that those are ints, so it reads them as such.

    // get contributor data
    for (int i = 1; i <= cont_count; ++i) {
        // cout << "contributor " << i << "\n";
    }

    // get project data
    for (int i = 1; i <= proj_count; ++i) {
        // cout << "project " << i << "\n";
    }

    // queue and "process" projects

    input.close();
    output.close();
}

int main(int, char**) {
    const array<string, 6> filenames {
        "a_an_example.in.txt",
        "b_better_start_small.in.txt",
        "c_collaboration.in.txt",
        "d_dense_schedule.in.txt",
        "e_exceptional_skills.in.txt",
        "f_find_great_mentors.in.txt"
    };
        
    for (int i = 0; i < 6; i++) {
        solve(filenames.at(i));
    }
        
    cout << "DONE." << "\n";
}
