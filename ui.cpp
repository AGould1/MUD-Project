#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "move.h"
#include "pokemon.h"
#include "json11.hpp"
#include "team.h"
#include "battle.h"
#include "load_pson.h"
#include "gym.h"

using namespace std;
using json = json11::Json;

// Function to get and validate user input.
int getInput() {
    int input;
    while (true) {
        cout << "Enter your choice: ";
        if (!(cin >> input)) {  // Check for end of input stream
            if (cin.eof()) {
                cout << "End of input reached.\n";
                exit(0);
            } else {
                cin.clear();    // Clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
                cout << "Invalid input. Please enter an integer.\n";
            }
        } else {
            cout<<endl;
            break;
        }
    }
    return input;
}

bool selectPokemonType(string& selectedType) {
    // Array of available Pokémon types for selection
    string types[] = {"", "Normal", "Fire", "Water", "Electric", "Grass", "Rock"};

    // Display the types to the user
    cout << "Choose Type:\n";
    for (int i = 1; i < 7; i++) {
        cout << i << ". " << types[i] << "\n";
    }

    /** CODE: Call the getInput() function to get the user's choice */
    int choice = getInput();

    /** CODE: Check if the choice is invalid return false (see for loop for valid range of values.) 
     *        Otherwise, update selectedType 
     */
    if (choice < 1 || choice > 6) {
        return false;
    }
    selectedType = types[choice];
    return true;
}

shared_ptr<Pokemon> selectPokemonFromType(const string& selectedType, vector<shared_ptr<Pokemon>>& pokemonList) {
    cout << "Select your " << selectedType << " type Pokemon:\n";

    /** CODE:   Display the pokemon of the selected Type for the user to select 
     *          The displayed list must start at 1
     *          The display when Grass is the selected type will start off with 
     *           1. Bulbasaur
     *           2. Ivysaur ...
     */    
    vector<shared_ptr<Pokemon>> filteredPokemon;
    int displayIndex = 1;

    for (auto& pokemon : pokemonList) {
        if (pokemon->getType() == selectedType) {
            cout << displayIndex << ". " << pokemon->getName() << "\n";
            filteredPokemon.push_back(pokemon);
            displayIndex++;
        }
    }
    if (filteredPokemon.empty()) {
        cout << "No Pokemon of type " << selectedType << " available.\n";
        return nullptr;
    }

    /** CODE: Call the getInput() function to get the user's choice **/
    int choice = getInput();

    /** CODE: return the chosen pokemon */
    if (choice < 1 || choice > static_cast<int>(filteredPokemon.size())) {
        return nullptr;
    }

    // If no valid choice was made, return nullptr
    return filteredPokemon[choice - 1];
}

void selectMovesForPokemon(shared_ptr<Pokemon>& selectedPokemon, const vector<Move>& movesList, const string& selectedType) {
    cout << "Choose up to 4 moves for " << selectedPokemon->getName() << " (enter 0 to exit early):\n";

    vector<Move> availableMoves;
    int displayIndex = 1;

    for (const auto& move : movesList) {
        if (move.getType() == selectedType || move.getType() == "Normal") {
            cout << displayIndex << ". " << move.getName() << "\n";
            availableMoves.push_back(move);
            displayIndex++;
        }
    }
    if (availableMoves.empty()) {
        cout << "No moves available for " << selectedPokemon->getName() << endl;
        return;
    }

    set<string> selectedMoveNames;
    int movesSelected = 0;

    while (movesSelected < 4) {
        int choice = getInput();

        if (choice == 0) {
            break;
        }
        if (choice < 1 || choice > static_cast<int>(availableMoves.size())) {
            cout << "Invalid move selected, please try again" << endl;
            continue;
        }
        Move& selectedMove = availableMoves[choice - 1];

    /** CODE:   If no moves are selected "No moves selected for " << selectedPokemon->getName() << endl */
        if (selectedMoveNames.find(selectedMove.getName()) != selectedMoveNames.end()) {
            cout << selectedMove.getName() << " move #" << choice << " is already selected, please select a different move" << endl;
            continue;
        }
        selectedPokemon->addMove(selectedMove);
        selectedMoveNames.insert(selectedMove.getName());
        movesSelected++;
        cout << "Your Pokemon, " << selectedPokemon->getName() << ", learned " << selectedMove.getName() << endl;
    }
    /** CODE:   If more than 1 move is selected then print  "Selection complete. " << selectedPokemon->getName() << " now knows " << NUM_MOVES_SELECTED << " move(s)." << endl */
    if (movesSelected == 0) {
        cout << "No moves selected for " << selectedPokemon->getName() << endl;
    }
    else if (movesSelected > 0) {
        cout << "Selection complete. " << selectedPokemon->getName() << " now knows " << movesSelected << " move(s)." << endl;
    }
}

bool addPokemonToTeam(Team* team, vector<shared_ptr<Pokemon>>& pokemonList, const vector<Move>& movesList) {

    if (team->isFull()) {
        cout << "Team is full. Cannot add more Pokémon.\nTry removing a Pokemon.\n\n";
        return false;
    }

    string selectedType;
    if (!selectPokemonType(selectedType)) {
        return false;
    }

    shared_ptr<Pokemon> selectedPokemon = selectPokemonFromType(selectedType, pokemonList);
    if (!selectedPokemon) {
        return false;
    }

    selectMovesForPokemon(selectedPokemon, movesList, selectedType);
    if (team->addPokemon(selectedPokemon)) {
        cout << "Successfully added " << selectedPokemon->getName() << " to your team!" << endl;
        return true;
    }

    return false;
}

bool removePokemonFromTeam(Team* team) {

    /** CODE: if the team has 0 members, print the error mesage "No Pokemon to remove" and return false */
    if (team->getSize() == 0) {
        cout << "No Pokemon to remove" << endl;
        return false;
    }

    cout << "Choose a Pokemon to Remove:\n";
    /** CODE: Print your team. (HINT: team has a printTeam function) */
    team->printTeam();

    /** CODE:   Prompt for input to select a Pokémon to remove by calling the getInput() function.
     *          subtract 1 from choice to line up with 0-based list
     *          Validate the chosen number is within the valid range
     */
    int choice = getInput();
    /** CODE:   Attempt to remove the selected Pokémon from the team using the relevant method.
     *          If removal is successful, print "Successfully removed {pokemon name}" and return true.
     *          If removal fails, print "Failed to remove the Pokémon." and return false.
     */
    choice = choice - 1;
    if (choice < 0 || choice >= team->getSize()) {
        cout << "Failed to remove the Pokémon." << endl;
        return false;
    }

    shared_ptr<Pokemon> pokemonToRemove = team->getPokemon(choice);
    string pokemonName = pokemonToRemove->getName();

    if (team->removePokemon(choice)) {
        cout << "Successfully removed " << pokemonName << endl;
        return true;
    } else {
        cout << "Failed to remove the Pokémon." << endl;
        return false;
    }

    return false;
}

void pokemonMenu() {
    vector<Move> movesList = loadMoves("moves.json");
    vector<shared_ptr<Pokemon>> pokemonList = loadPokemon("pokemon.json");

    // Instantiate a team object and load tem
    string filename = "team.json";
    Team *team = new Team();
    team->loadTeam(filename, pokemonList, movesList);

    // Instantiate a battle object to fight a random Pokémon
    Battle *battle = new Battle();

    int choice;
    do {

        cout << "---------\n" << "MAIN MENU\n" << "---------\n";
        cout << "1. Show Pokemon\n";
        cout << "2. Choose Pokemon\n";
        cout << "3. Remove Pokemon\n";
        cout << "4. Battle Random\n";

        cout << "6. Exit\n";

        choice = getInput();

        switch(choice) {
            case 1:
                // Print your team
                cout << "Your Pokemon:" << endl;
                team->printTeam();
                break;
            case 2:
                // Add Pokémon to team
                if (addPokemonToTeam(team, pokemonList, movesList)) {
                    team->writeTeam(filename);
                }
                break;
            case 3:
                // Remove Pokémon from team
                if (removePokemonFromTeam(team)) {
                    team->writeTeam(filename);
                }
                break;
            case 4:
                // Enter the battle loop with a random Pokémon.
                battle->start(team, pokemonList, movesList);
                break;

            case 6:
                // Quit
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while(choice!=6);

    delete(team);
    delete(battle);

}