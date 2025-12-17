#include "team.h"
#include <algorithm>
#include <cctype>

using namespace std;
using json = json11::Json;

bool Team::addPokemon(const shared_ptr<Pokemon>& pokemon) {
    if (isFull()) {        
        return false;
    }
    /** CODE: insert here and return true*/
    team.push_back(pokemon);
    return true;
}

bool Team::removePokemon(size_t index) {    
    if (index < team.size()) {
        // If the index is valid, erase the Pokémon at that index from the team and return true.
        /** CODE: insert here*/
        team.erase(team.begin() + index);
        return true;
    }
    // Return false if the index is invalid, meaning no Pokémon is removed.
    return false;
}

bool Team::isFull() const {
    // Check if the current team size equals the MAX_TEAM_SIZE.
    /** CODE: insert here*/
    return team.size() >= MAX_TEAM_SIZE;
    // return false; // Placeholder return statement. Replace with the actual condition check.
}

shared_ptr<Pokemon> Team::getPokemon(size_t index) {
    // If the index is within the bounds of the team array, return a pointer to the Pokémon at that index.
    /** CODE: insert here*/
    if (index < team.size()) {
        return team[index];
    }
    return nullptr; // Return nullptr if the index is out of bounds.
}

int Team::getSize() const {
    // Return the number of Pokémon in the team.
    /** CODE: insert here*/
    return team.size();
}

// Displays the stats of each Pokémon in the team, numbered in the order they were added.
void Team::printTeam() const {
    if (team.size() == 0) printf("You have no Pokemon in your team");
    for (size_t i = 0; i < team.size(); i++) {
        cout << i+1 << ". ";
        team[i]->displayStats();
    }
    printf("\n");
}

shared_ptr<Pokemon> Team::findPokemonByName(vector<shared_ptr<Pokemon>>& pokemonList, string name) {
    // Loop through the given list to find a Pokémon by name and return the pointer to it if found.
    /** CODE: insert here*/
    for (auto& pokemon : pokemonList) {
        if (pokemon && pokemon->getName() == name) {
            return pokemon;
        }
    }
    return nullptr; // Return nullptr if the Pokémon is not found.
}

Move* Team::getMove(const vector<Move>& movesList, string name) {
    /** CODE: Iterate through the list of moves if found, use the copy constructor to create a pointer to a new Move
     *          Move * newMove = new Move(currentMove)
     *          return newMove
     */
    for (const auto& currentMove : movesList) {
        if (currentMove.getName() == name) {
            Move* newMove = new Move(currentMove);
            return newMove;
        }
    }
    return nullptr; // Return nullptr if the Move is not found.
}

bool Team::loadTeam(const string& filename, vector<shared_ptr<Pokemon>>& pokemonList, const vector<Move>& movesList) {
    string err;
    string fileContents; // You need to load the file content into this string.
    // Example of reading a file into a string (pseudo-code):
    // std::ifstream ifs(filename);
    // std::string fileContents((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    /** CODE: read from the JSON file and load into a string */
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        cout << "loadTeam in team.cpp has failed to open file " << filename << endl;
        return false;
    }
    fileContents = string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    ifs.close();

    // Parse the JSON content
    auto jsonCBs = json::parse(fileContents, err);
    if (!err.empty()) {
        cout << "loadTeam in team.cpp has failed to parse the JSON of " << filename << " because of " << err << endl;
        return false;
    }

    /** CODE: uncomment for loop */
    // Process each Pokémon configuration in the JSON array
    for (const auto& pm : jsonCBs.array_items()) {
        // Example of accessing a Pokémon's name and moves from the JSON object:
        string name = pm["name"].string_value();
        auto movesJson = pm["moves"].array_items();
        shared_ptr<Pokemon> pokemon;
        /** CODE:   Find the Pokémon by name using 'findPokemonByName' and set pokemon to the returned value (using the pokemon variable is required)
         *          For each move in 'movesJson', find the move using 'getMove' and add it to the Pokémon
         *          If any Pokémon or moves are not found, print an error message and return false
         */
        pokemon = findPokemonByName(pokemonList, name);
        if (!pokemon) {
            cout << "Error: Pokemon '" << name << "' not found in pokemonList" << endl;
            return false;
        }
        for (const auto& moveJson : movesJson) {
            string moveName = moveJson.string_value();
            Move* move = getMove(movesList, moveName);
            if (!move) {
                cout << "Error: Move '" << moveName << "' not found in movesList" << endl;
                return false;
            }
            pokemon->addMove(*move);
        }

        // After processing each Pokémon and its moves, add it to the team
        /** CODE: insert here*/
        addPokemon(pokemon);
    }

    return true; // Return true if all Pokémon and their moves are successfully processed
}

void Team::writeTeam(const string& filename) const {
    vector<json> teamJson;

    // Iterates through each team member to create a JSON representation
    for (const auto& member : team) {
        vector<string> moves;
        // Converts each move of the member to a string and stores in a vector
        for (const auto& move : member->getMoves()) {
            moves.push_back(move.getName()); // Gets the name of the move
        }

        // Creates a JSON object for the team member and adds it to the teamJson vector
        teamJson.push_back(json::object {
                {"name", member->getName()}, // Adds the member's name
                {"moves", json(moves)} // Adds the member's moves
        });
    }

    ofstream outFile(filename); // Creates an ofstream to write to the file
    if (outFile.is_open()) {
        json teamArray = json::array(teamJson); // Converts the team vector into a JSON array
        outFile << teamArray.dump(); // Writes the JSON array to the file
        outFile.close();
    } else {
        cerr << "Failed to open " << filename << " for writing.\n";
    }
}