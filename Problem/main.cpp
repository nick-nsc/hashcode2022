#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <map>
#include <unordered_map>
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
    typedef struct {
        int days, score, bestbefore;
        vector<pair<string, int>> roles;
    } Project;
    map<string, unordered_map<string, int>> contributors;
    map<string, Project> projects;

    // get cont and proj count
    int cont_count = 0, proj_count = 0;   
    input >> cont_count >> proj_count; // input knows that those are ints, so it reads them as such.

    // get contributor data
    for(int i = 0; i < cont_count; ++i) {
        string cont_name;
        int skill_count;
        input >> cont_name >> skill_count;
        unordered_map<string, int> skills;
        for(int i = 0; i < skill_count; i++) {
            string skill_name;
            int skill_level;
            input >> skill_name >> skill_level;
            skills.insert({skill_name, skill_level});
        }
        contributors.insert({cont_name, skills});
    }

    // get project data
    for(int i = 1; i <= proj_count; ++i) {
        string proj_name;
        int proj_days, proj_score, proj_bb, role_count;
        input >> proj_name >> proj_days >> proj_score >> proj_bb >> role_count;
        vector<pair<string, int>> roles;
        for(int i = 0; i < role_count; ++i) {
            string role_name;
            int role_level;
            input >> role_name >> role_level;
            roles.push_back({role_name, role_level});
        }
        Project tmp{proj_days, proj_score, proj_bb, roles};
        projects.insert({proj_name, tmp});
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
        
    for(int i = 0; i < 6; i++) {
        solve(filenames.at(i));
    }
        
    cout << "DONE." << "\n";
}
