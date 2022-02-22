import pandas as pd
import os.path

filelist_input = [
    "a_an_example.in.txt",
    "b_basic.in.txt",
    "c_coarse.in.txt",
    "d_difficult.in.txt",
    "e_elaborate.in.txt",
    ]

counter = 0
final_score = 0

for file in filelist_input:
    counter += 1
    with open(os.path.join(os.path.dirname(__file__), "input_data", file), "r") as f:
        # number of potential clients
        clients_count = int(f.readline().strip())
        # create a DataFrame holding the clients' preferences
        clients_data = pd.DataFrame(columns=['client_ID', 'ingredient', 'like', 'dislike'])

        # collect all clients' preferences
        for client in range(clients_count):
            likes = f.readline().strip()
            likes = likes.split()
            for liked_ingredient in range(int(likes[0])):
                clients_data.loc[len(clients_data)] = [client, likes[liked_ingredient+1], 1, 0]

            dislikes = f.readline().strip()
            dislikes = dislikes.split()
            for disliked_ingredient in range(int(dislikes[0])):
                clients_data.loc[len(clients_data)] = [client, dislikes[disliked_ingredient+1], 0, 1]

        # count of likes and dislikes per ingredient
        ingredients_data = clients_data.groupby('ingredient').agg({'like': ['sum'], 'dislike': ['sum']}).reset_index()

        # add like/dislike ratio
        ingredients_data['like_dislike_ratio'] = ingredients_data['like']['sum'] - ingredients_data['dislike']['sum']

        # select ingredients based on their like_dislike_ratio
        ingredients_selected = ingredients_data[ingredients_data.like_dislike_ratio >= 0].reset_index()
        ingredients_notselected = ingredients_data[ingredients_data.like_dislike_ratio < 0].reset_index()
        ingredients_selected_count = len(ingredients_selected)

        # calculate expected score
        clients_selected = clients_data
        # get every client who dislikes at least one of the selected ingredients and exclude them from the selected clients
        for ingredient in ingredients_selected['ingredient']:
            indizes_to_drop = clients_selected[(clients_selected['ingredient'] == ingredient) & (clients_selected['dislike'] == 1)]['client_ID']
            clients_selected = clients_selected[~clients_selected['client_ID'].isin(indizes_to_drop)]
        # get every client who likes at least one of the not selected ingredients and exclude them from the selected clients
        for ingredient in ingredients_notselected['ingredient']:
            indizes_to_drop = clients_selected[(clients_selected['ingredient'] == ingredient) & (clients_selected['like'] == 1)]['client_ID']
            clients_selected = clients_selected[~clients_selected['client_ID'].isin(indizes_to_drop)]
        
        clients_selected_count = len(clients_selected.groupby('client_ID'))
        final_score += clients_selected_count
        print("Expected score: {}".format(clients_selected_count))

        # create output file
        with open(os.path.join(os.path.dirname(__file__), "output", file.replace("in.txt", "out.txt")), "w") as f:
            solution = str(ingredients_selected_count)
            for i in ingredients_selected['ingredient']:
                solution += " "
                solution += i
            f.write(solution)

    print("{} of {} finished.".format(counter, len(filelist_input)))

print("-------------------------------")
print("Final Score: {}".format(final_score))
print("DONE")