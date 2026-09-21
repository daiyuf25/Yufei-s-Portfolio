"""
Guess the Image
---------------
The program generates an image for the user(s) to play guess the image.
The user(s) are prompted to give their numbers, their names, how many hints they want, and how many pieces the image will consist of.
For each round, program uncovers one piece at a time and then prompts the user(s) to make a guess. The user(s) may choose to use a hint before their guess.
The game ends when one user has guessed the image correctly(the judge is Chat GPT) or when there is only one image left to uncover and none of the users have yet to guess correctly.
"""
import random
import math
from simpleimage import SimpleImage
from TypeImage import Image

def create_players():
    player_hints = {}
    players = input("How many players are there? ")
#makes sure that the user gives a valid input for number of players
    while players.isalpha() or players == "0" or players == "":
        players = input("Please enter a valid number of players. ")
#create a dictionary with the players' names as keys and the number of hints they will be receiving throughout the game as values
    for i in range(int(players)):
        player = input(f"What is player {i + 1}'s name? ")
        hint = input("How many hints should player one get through out the game? ")
#makes sure that the user enters a valid number of hints
        while hint.isalpha() or hint == "":
            hint = input("Please enter a valid amount of hints. ")
        player_hints[player] = int(hint)
    return player_hints

def get_puzzle_pieces():
    puzzle_pieces = input("How many pieces will the picture consist of? ")
#makes sure that the user enters a valid number of puzzle pieces
    while puzzle_pieces.isalpha() or int(puzzle_pieces) <= 1 or puzzle_pieces == "":
        puzzle_pieces = input("How many pieces will the picture consist of? ")
    return int(puzzle_pieces)

def create_puzzle_pieces(pieces_across_each_side, puzzle_pieces):
    puzzle_pieces_list = []
#get the coordinates for the upper right hand corner of each puzzle piece and append it as a tuple to puzzle_pieces_list
    if puzzle_pieces <= 3:
        for i in range(1, pieces_across_each_side + 1):
            puzzle_pieces_list.append((i, 1))
    else:
        for i in range(1, pieces_across_each_side + 1):
            for j in range(1, pieces_across_each_side + 1):
                puzzle_pieces_list.append((i, j))
    return puzzle_pieces_list

def get_player_guess(current_image, player, hints):
    guess = input(f"It is {player}'s turn. What do you think the image is? Enter hint if you would like a hint. You have {hints} hint(s) remaining: ")
    if guess == "hint" and hints != 0:
        #if guess equals to hint, use Chat GPT to generate a hint before asking the user for their guess
        guess = input(current_image.get_hint())
        hints -= 1
    return guess, hints

def give_chat_feedback(current_image):
    print("This program was made possible using the help of Chat GPT. Please give some feedback below on how Chat GPT did.")
    feedback = input("(e.g. Was there an answer that you gave that you thought was right but Chat thought was wrong? Where any of the hints too much of a give away?) Press enter if you have no feedback. ")
    if feedback != "":
        current_image.get_user_feedback(feedback)

def main():
#creates players
    players = create_players()
#gets number of pieces that image_to_guess will be split up into
    puzzle_pieces = get_puzzle_pieces()

    pictures = {
        "Pictures for Contest/Bellpepper.png": "bell pepper",
        "Pictures for Contest/Thanksgivingdayparade.png": "Thanksgiving Day Parade",
        "Pictures for Contest/Timessquare.png": "New Year's Eve Ball Drop",
        "Pictures for Contest/Tomatostem.png": "tomato stem",
        "Pictures for Contest/Wire.jpg": "wire",
        "Pictures for Contest/Melon.png": "cantaloupe",
        "Pictures for Contest/Pencil.png": "woodcase pencil",
        "Pictures for Contest/Mountrushmore.png": "mount rushmore",
        "Pictures for Contest/Goldengatebrigde.png": "golden gate bridge",
        "Pictures for Contest/coke.png": "coke",
        "Pictures for Contest/Circuitboard.png": "circuit board",
        "Pictures for Contest/Sourpatchkids.png": "Sour Patch Kids",
        "Pictures for Contest/Froglegs.png": "fried frog legs"
    }

    screen = "Pictures for Contest/block.png"
    screen = SimpleImage(screen)
    picture_options = []
    for key in pictures.keys():
        picture_options.append(key)
#randomly choose a picture for the user to guess
    name_image_to_guess = random.choice(picture_options)
    image_to_guess = SimpleImage(name_image_to_guess)
    current_image = Image(pictures[name_image_to_guess])


    width_image = image_to_guess.width
    height_image = image_to_guess.height
    width_block = screen.width
    height_block = screen.height
    if puzzle_pieces == 2 or puzzle_pieces == 3:
#get the dimensions of each puzzle piece
        x_step = width_image/puzzle_pieces
        y_step = height_image
#the function has to have puzzle_pieces as the second parameter because pieces_across_each_side could be the same for some values of puzzle_pieces
#for example pieces_across_each_side for when puzzle_pieces is equal to 9 and when puzzle_pieces is equal to 3 is the same
        puzzle_pieces_list = create_puzzle_pieces(puzzle_pieces, puzzle_pieces)
    else:
        pieces_across_each_side = int(math.sqrt(puzzle_pieces))
#get the dimensions of each puzzle piece
        x_step = width_image/pieces_across_each_side
        y_step = height_image/pieces_across_each_side
        puzzle_pieces_list = create_puzzle_pieces(pieces_across_each_side, puzzle_pieces)
#choose the upper left hand corner of the image so that image_to_guess will be centered on the screen
    x_start = (width_block - width_image)/2
    y_start = (height_block - height_image)/2

    guess = "nothing"
#only enter this while loop if there is more than one puzzle piece left to show and the user(s) have not guessed the image
    while len(puzzle_pieces_list)!= 1 and current_image.get_answer_validation(guess) == False:
# get a random puzzle piece
        piece_to_show = random.choice(puzzle_pieces_list)
#unpack the x and y coordinates for the upper right hand corner of the piece
        piece_to_show_x, piece_to_show_y = piece_to_show
#when one user has guessed the image correctly, the program will break out of the for loop and it will go back to to the top of the while loop
#this condition will prevent the code in the while loop from running when the game is technically over
        if current_image.get_answer_validation(guess) == False:
            for pixel in image_to_guess:
#if the pixel is within the piece_to_show get the x and y coordinates for the piece
                if (piece_to_show_x - 1) * x_step <= pixel.x <= piece_to_show_x * x_step and (piece_to_show_y - 1) * y_step <= pixel.y <= piece_to_show_y * y_step:
                    x = pixel.x
                    y = pixel.y
#map the pixel onto the screen
                    screen.set_pixel(x_start + x, y_start + y, image_to_guess.get_pixel(x, y))
#remove the piece_to_show from puzzle_pieces_list
            puzzle_pieces_list.remove(piece_to_show)
            screen.show()

            for player, hints in players.items():
                guess, hints = get_player_guess(current_image, player, hints)
                players[player] = hints
                if current_image.get_answer_validation(guess):
#if chat gpt thinks that the user's answer is valid, print that the user wins and show the entire image
                    print(f"Congratulations. {player} wins.")
                    image_to_guess.show()
                    break
#print some blank lines so that if they're multiple players, the players don't see each others' guesses
                for i in range(12):
                    print()
#if there are no more puzzle pieces to show besides the last remaining one and the user(s) have not gotten the answer correct, then show the entire image and print the correct answer
    if current_image.get_answer_validation(guess) == False:
        image_to_guess.show()
        print(f"The correct answer is: {pictures[name_image_to_guess]}")

    give_chat_feedback(current_image)


if __name__ == "__main__":
    main()