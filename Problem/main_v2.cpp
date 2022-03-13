#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct {
    int days, score, bestbefore;
    vector<pair<string, int>> roles;
} Project;

void read_input(ifstream& input, map<string, unordered_map<string, int>>& contributors, map<string, Project>& projects) {
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
    for(int i = 0; i < proj_count; ++i) {
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
}

void process_projects(map<string, unordered_map<string, int>>& contributors, map<string, Project>& projects, vector<pair<string, vector<string>>>& solution) {
    // sort projects by score into a vector "projects_todo"
    vector<pair<int, string>> projects_todo;    // {(score, "name")...}
    for(auto& p : projects) {
        projects_todo.push_back(make_pair(p.second.score, p.first));
    }
    sort(projects_todo.begin(), projects_todo.end(), [](auto l, auto r) {
        return l.first > r.first;
    });

    // copy all contributors' skills into a map "skills_available"
    // also: map contributors to their availability
    map<string, map<int, vector<string>>> skills_available;     // ["skill"][level]{"contr_names"...}
    map<string, bool> contributor_availability;                 // ["contr_name"] is_available
    for(auto& contr : contributors) {
        for(auto& skill : contr.second) {
            auto search_result_skill = skills_available.find(skill.first);
            if(search_result_skill != skills_available.end()) {
                auto search_result_level = skills_available.at(skill.first).find(skill.second);
                if(search_result_level != skills_available.at(skill.first).end()) {
                    // add new name to corresponding skill and level
                    skills_available.at(skill.first).at(skill.second).push_back(contr.first);
                }
                else {
                    // add new level and name to corresponding skill
                    vector<string> name;
                    name.push_back(contr.first);
                    skills_available.at(skill.first).insert({skill.second, name});
                }
            } 
            else {
                // add new skill, level and name
                vector<string> name;
                name.push_back(contr.first);
                map<int, vector<string>> lvl;
                lvl.insert({skill.second, name});
                skills_available.insert({skill.first, lvl});
            }
        }
        contributor_availability.insert({contr.first, true});
    }
    
    //TODO loop over projects and assign !available! contributors
    //TODO on proj_assign_contr: update contributor_availability (also temporary on a copy when checking if every role can be assigned), update soloution
    //TODO on contr_skill_levelup: update contributors and skills_available

    cout << "stop here" << "\n";
}

void push_solution(vector<pair<string, vector<string>>>& solution, ofstream& output) {
    output << solution.size() << "\n";
    for(auto& s : solution) {
        output << s.first << "\n";
        string contrs = "";
        for(auto& c : s.second) {
            contrs = contrs + c + " ";
        }
        output << contrs << "\n";
    }
}

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
    map<string, unordered_map<string, int>> contributors;
    map<string, Project> projects;
    vector<pair<string, vector<string>>> solution;

    // read input
    read_input(input, contributors, projects);

    //* process projects and generate a solution
    process_projects(contributors, projects, solution);

    // push solution to outfile
    push_solution(solution, output);

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
