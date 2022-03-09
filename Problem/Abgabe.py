import os
import pandas as pd


def collectData(filename):
    with open(os.path.join(os.path.dirname(__file__), "input_data", filename), "r") as f:
        # read contributor and project count
        contributor_count, project_count = [int(s) for s in f.readline().strip().split(" ")]

        # create DataFrames holding the data
        contributor_data = pd.DataFrame(columns=['name', 'skill', 'level'])
        contributor_occ = pd.DataFrame(columns=['name', 'occupied'])
        project_data = pd.DataFrame(columns=['name', 'days', 'score', 'bestbefore', 'F', 'done'])
        project_req = pd.DataFrame(columns=['name', 'role', 'requiredlevel'])

        for _ in range(contributor_count):
            contributor_name, skill_count = f.readline().strip().split()
            contributor_occ.loc[len(contributor_occ)] = [contributor_name, -1]
            for _ in range(int(skill_count)):
                skill_name, skill_level = f.readline().strip().split()
                contributor_data.loc[len(contributor_data)] = [contributor_name, skill_name, int(skill_level)]

        for _ in range(project_count):
            project_name, project_days, project_score, project_bestbefore, project_reqskills_count = f.readline().strip().split()
            project_data.loc[len(project_data)] = [project_name, int(project_days), int(project_score), int(project_bestbefore),  0, 0]
            for _ in range(int(project_reqskills_count)):
                skill_name, skill_level = f.readline().strip().split()
                project_req.loc[len(project_req)] = [project_name, skill_name, int(skill_level)]

    # calculate secret F-score
    project_data['F'] = project_data['bestbefore'] - project_data['days']

    # sort project_data by F-score
    project_data = project_data.sort_values(by='F')

    return contributor_data, contributor_occ, project_data, project_req


def create_output_file(projects: list, submission: str) -> None:
    with open(os.path.join(os.path.dirname(__file__), "output", submission), "w") as f:
        solution = str(len(projects))
        for i in projects:
            for j in i:
                solution += "\n{}".format(j)

        f.write(solution)


def everything_ready(projects) -> bool:
    wip = False

    for _, project in projects.iterrows():

        if project["done"] == 1:
            wip = True
            break

    return wip


def give_solution_pls(contributor_data, contributor_occ, project_data, project_req) -> list:

    day = 0
    solution = []
    occ_has_changed_yesterday = True

    while (everything_ready(project_data)) or day == 0:
        occ_has_changed_today = False
        project_has_finished = False

        for _, project in project_data.iterrows():
            if project["done"] == 1:
                project_data.loc[project_data["name"] == project["name"], "days"] -= 1

                if project["days"] <= 0:
                    if project["days"] < 0:
                        print("Math rong")
                    project_data.loc[project_data["name"] == project["name"], "done"] = 2
                    project_has_finished = True
                continue

            if project["done"] != 0:
                continue

            if (not occ_has_changed_yesterday) and (not project_has_finished):
                continue

            requirements_df = project_req[project_req["name"] == project["name"]]
            requirements = []

            for _, x in requirements_df.iterrows():
                requirements.append((x["role"], x["requiredlevel"]))

            tmp_l = [project["name"]]
            tmp_string = ""
            occ_copy = contributor_occ.copy()

            for req in requirements:
                not_found = True
                possible_contr = contributor_data[(contributor_data["skill"] == req[0]) & (contributor_data["level"] >= req[1])]
                possible_contr = possible_contr.sort_values(by=['level'], ascending=True)

                # TODO double check
                for _, contr in possible_contr.iterrows():
                    if contr["name"] in occ_copy.loc[(occ_copy["occupied"] < day)].values:
                        #!!!Potential error
                        occ_copy.loc[occ_copy["name"] == contr["name"], "occupied"] = day + int(project["days"]) - 1
                        tmp_string += contr["name"] + " "
                        not_found = False
                        break
                    not_found = True

                if not_found:
                    break

            if not_found:
                continue
            
            project_data.loc[project_data["name"] == project["name"], "done"] = 1
            contributor_occ = occ_copy.copy()
            occ_has_changed_today = True
            tmp_l.append(tmp_string[:-1])
            solution.append(tmp_l)

        if occ_has_changed_today:
            occ_has_changed_yesterday = True
        else:
            occ_has_changed_yesterday = False
        day += 1
        if(day%100==0):
            print(day)
    return solution


def main():

    file_names = ["a_an_example.in.txt", "b_better_start_small.in.txt", "c_collaboration.in.txt",
                  "d_dense_schedule.in.txt", "e_exceptional_skills.in.txt", "f_find_great_mentors.in.txt"]
    for f in file_names:
        data = collectData(f)
        solution = give_solution_pls(data[0], data[1], data[2], data[3])
        create_output_file(solution, f)
        print(f)


if __name__ == "__main__":
    main()
