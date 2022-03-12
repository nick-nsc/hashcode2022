#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

const bool is_wip(const vector<tuple<int, string, int>>& v) {
    bool wip = false;
    for(auto& p : v) {
        const int status = get<2>(p);
        if (status==1) {
            wip = true;
            break;
        }
    }
    return wip;
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

    // sort projects by (bestbefore - days) in a vector
    vector<tuple<int, string, int>> projects_ordered;   // urgency, name, status
    for(auto& i : projects) {
        projects_ordered.push_back(make_tuple((i.second.bestbefore-i.second.days), i.first, 0));
    }
    sort(projects_ordered.begin(), projects_ordered.end(), [](auto a, auto b) {
        return get<0>(a) < get<0>(b);
    });

    // get contributors by availability in a vector
    vector<pair<string, int>> contributors_ordered;    // name, available
    for(auto& i : contributors) {
        contributors_ordered.push_back({i.first, -1});
    }

    // queue and "process" projects
    int day = 0;
    vector<pair<string, vector<string>>> solution;
    bool occ_has_changed_yesterday = true;
    while(is_wip(projects_ordered) || day == 0)
    {
        bool occ_has_changed_today = false;
        bool project_has_finished = false;

        // get WIP projects, update days and determine if any finishes -> update status then
        for (auto& p : projects_ordered) {
            int& status = get<2>(p);
            if(status == 0) {
                continue;
            }
            else if(status == 2) {
                continue;
            }
            else if(status == 1) {
                // update days
                projects.at(get<1>(p)).days -= 1;
                // update status
                if(projects.at(get<1>(p)).days <= 0) {
                    if(projects.at(get<1>(p)).days < 0) {
                        cout << "Math wrong" << "\n";
                    }
                    status = 2;
                    project_has_finished = true;
                }
                continue;
            }
            else {
                cout << "Status error" << "\n";
            }
        }

        // try to queue unfinished projects
        for (auto& p : projects_ordered) {
            // skip projects which are WIP
            if(get<2>(p) == 1) {
                continue;
            }

            // skip projects which are finished
            if(get<2>(p) == 2) {
                continue;
            }

            // skip project if contributor availability has not changed
            if ((!occ_has_changed_yesterday) && (!project_has_finished)) {
                continue;
            }

            // get roles for this project
            vector<pair<string, int>> p_roles;
            p_roles = projects.at(get<1>(p)).roles;

            // check if available contributors can cover the required roles
            bool not_found;
            vector<string> chosen_contr;
            for(auto& r : p_roles) {
                not_found = true;
                // get available contributors for this role
                vector<pair<string, bool>> avail_contr;
                for(auto& c : contributors_ordered) {
                    if(c.second<day) {
                        avail_contr.push_back(c);
                    }
                }
                // filter available contributors by needed skill and level
                for(vector<pair<string, bool>>::iterator it = avail_contr.begin(); it!=avail_contr.end();) {
                    int contr_role_level = -1;
                    try {
                        auto x = contributors.at(it->first);
                        contr_role_level = x.at(r.first);
                    }
                    catch(const std::exception& e) {
                    }
                    if(!(contr_role_level==-1 || contr_role_level<r.second)) {
                        // save first best fitting contributor to chosen_contr and set available=false
                        for(auto& c : contributors_ordered) {
                            if(c.first == it->first) {
                                c.second = day + projects.at(get<1>(p)).days - 1;
                                not_found = false;
                                chosen_contr.push_back(it->first);
                                break;
                            }
                        }
                    }
                    if(!not_found) {
                        break;
                    }
                    ++it;
                }

                if(not_found) {
                    break;
                }
            }
            if(not_found) {
                // update chosen contributor's availability if not every role is fulfilled
                for(auto& cc : chosen_contr) {
                    for(auto& c : contributors_ordered) {
                        if(c.first == cc) {
                            c.second = day - 1;
                        }
                    }
                }
                continue;
            }

            // set project to WIP
            get<2>(p) = 1;
            // add chosen_contr to solution
            solution.push_back(make_pair(get<1>(p), chosen_contr));
            // update that changes have happened
            occ_has_changed_today = true;
        }
        if(occ_has_changed_today) {
            occ_has_changed_yesterday = true;
        }            
        else{
            occ_has_changed_yesterday = false;
        }
        
        if(day%200==0) {
            cout << day << "\n";
        }
        ++day;
    }

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
