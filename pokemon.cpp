#include "pokemon.h"

using namespace std;
using json = json11::Json;

// Static map defining how effective a Pokémon's move is against another Pokémon based on their types
map<string, map<string, double>> Pokemon::typeMultipliers = {
        {"Normal",   {{"Normal", 1.0}, {"Fire", 1.0}, {"Water", 1.0}, {"Electric", 1.0}, {"Grass", 1.0}, {"Rock", 0.5}}},
        {"Fire",     {{"Normal", 1.0}, {"Fire", 0.5}, {"Water", 0.5}, {"Electric", 1.0}, {"Grass", 2.0}, {"Rock", 0.5}}},
        {"Water",    {{"Normal", 1.0}, {"Fire", 2.0}, {"Water", 0.5}, {"Electric", 1.0}, {"Grass", 0.5}, {"Rock", 2.0}}},
        {"Electric", {{"Normal", 1.0}, {"Fire", 1.0}, {"Water", 2.0}, {"Electric", 0.5}, {"Grass", 0.5}, {"Rock", 1.0}}},
        {"Grass",    {{"Normal", 1.0}, {"Fire", 0.5}, {"Water", 2.0}, {"Electric", 1.0}, {"Grass", 0.5}, {"Rock", 2.0}}},
        {"Rock",     {{"Normal", 1.0}, {"Fire", 2.0}, {"Water", 1.0}, {"Electric", 1.0}, {"Grass", 1.0}, {"Rock", 1.0}}}
};

float Pokemon::getDamageMultiplier(const string& opponentMoveType) const {
    auto outerIt = typeMultipliers.find(opponentMoveType);
    if (outerIt != typeMultipliers.end()) {
        auto innerIt = outerIt->second.find(getType());
        if (innerIt != outerIt->second.end()) {
            return innerIt->second;
        }
    }
    return 1.0f;
}

shared_ptr<Pokemon> pokemonFactory(const json jd) {
    // Extract the Pokémon type from JSON data
    string pmType = jd["type"].string_value();
    // Extract base information for the Pokémon
    json baseInfo = jd["base"];

    // Example case for Normal type Pokémon. Implement cases for other types similarly.
    if (pmType == "Normal") {
        return make_shared<NormalPokemon>(
                jd["name"].string_value(),
                baseInfo["HP"].int_value(),
                baseInfo["Attack"].int_value(),
                baseInfo["Defense"].int_value(),
                baseInfo["Sp. Attack"].int_value(),
                baseInfo["Sp. Defense"].int_value(),
                baseInfo["Speed"].int_value()
        );
    }
    // Add similar blocks for other Pokémon types: Fire, Water, Electric, Grass, Rock,
    // ensuring the project is limited to these six types for simplicity.
    /** CODE: complete method */
    else if (pmType == "Fire") {
        return make_shared<FirePokemon>(
                jd["name"].string_value(),
                baseInfo["HP"].int_value(),
                baseInfo["Attack"].int_value(),
                baseInfo["Defense"].int_value(),
                baseInfo["Sp. Attack"].int_value(),
                baseInfo["Sp. Defense"].int_value(),
                baseInfo["Speed"].int_value()
        );
    }
    else if (pmType == "Water") {
        return make_shared<WaterPokemon>(
                jd["name"].string_value(),
                baseInfo["HP"].int_value(),
                baseInfo["Attack"].int_value(),
                baseInfo["Defense"].int_value(),
                baseInfo["Sp. Attack"].int_value(),
                baseInfo["Sp. Defense"].int_value(),
                baseInfo["Speed"].int_value()
        );
    }
    else if (pmType == "Electric") {
        return make_shared<ElectricPokemon>(
                jd["name"].string_value(),
                baseInfo["HP"].int_value(),
                baseInfo["Attack"].int_value(),
                baseInfo["Defense"].int_value(),
                baseInfo["Sp. Attack"].int_value(),
                baseInfo["Sp. Defense"].int_value(),
                baseInfo["Speed"].int_value()
        );
    }
    else if (pmType == "Grass") {
        return make_shared<GrassPokemon>(
                jd["name"].string_value(),
                baseInfo["HP"].int_value(),
                baseInfo["Attack"].int_value(),
                baseInfo["Defense"].int_value(),
                baseInfo["Sp. Attack"].int_value(),
                baseInfo["Sp. Defense"].int_value(),
                baseInfo["Speed"].int_value()
        );
    }
    else if (pmType == "Rock") {
        return make_shared<RockPokemon>(
                jd["name"].string_value(),
                baseInfo["HP"].int_value(),
                baseInfo["Attack"].int_value(),
                baseInfo["Defense"].int_value(),
                baseInfo["Sp. Attack"].int_value(),
                baseInfo["Sp. Defense"].int_value(),
                baseInfo["Speed"].int_value()
        );
    }
    // If no matching type is found, return nullptr
    return nullptr;
}

void Pokemon::displayStats() const {
    cout << left << setw(10) << name
         << " (HP: " << hp << "/" << base_hp << ")\n";
}

void Pokemon::displayMoves() const {
    cout << left;
    for (size_t i = 0; i < moves.size(); ++i) {
        cout << i + 1 << ": "
             << setw(20) << moves[i].getName()
             << "pp:" << setw(4) << moves[i].getPP()
             << endl;
    }
}

// Implement the getter and setter methods:

string Pokemon::getName() const {
    /** CODE: complete method */
    return name;
}

int Pokemon::getHP() const {
    /** CODE: complete method */
    return hp;
}

void Pokemon::resetHP() {
    /** CODE: complete method */
    hp = base_hp;
}

void Pokemon::setHP(int new_hp) {
    /** CODE: complete method */
    hp = new_hp;
}

int Pokemon::getAttack() const {
    /** CODE: complete method */
    return attack;
}

int Pokemon::getDefense() const {
    /** CODE: complete method */
    return defense;
}

int Pokemon::getSpAttack() const {
    /** CODE: complete method */
    return sp_attack;
}

int Pokemon::getSpDefense() const {
    /** CODE: complete method */
    return sp_defense;
}

int Pokemon::getSpeed() const {
    /** CODE: complete method */
    return speed;
}

vector<Move> Pokemon::getMoves() const {
    /** CODE: complete method */
    return moves;
}

Move& Pokemon::getMove(int index) {
    return moves.at(index);
}