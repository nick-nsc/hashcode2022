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

for file in filelist_input:
    counter += 1
    with open(os.path.join(os.path.dirname(__file__), "input_data", file), "r") as f:
        # number of potential clients
        clients_count = int(f.readline().strip())
        # collect all clients' data
        clients_data = pd.DataFrame(columns=['client_ID', 'ingredient', 'like', 'dislike'])

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
        ingredients_selection = ingredients_data[ingredients_data.like_dislike_ratio >= 0].reset_index()
        ingredients_selection_count = len(ingredients_selection)

        # create output file
        with open(os.path.join(os.path.dirname(__file__), "output", file.replace("in.txt", "out.txt")), "w") as f:
            solution = str(ingredients_selection_count)
            for i in ingredients_selection['ingredient']:
                solution += " "
                solution += i
            f.write(solution)

    print("{} of {} finished.".format(counter, len(filelist_input)))

print("DONE")