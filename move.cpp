#include "move.h"

Move* Move::create(const json jd) {

    std::string name = jd["move_name"].string_value();
    std::string type = jd["move_type"].string_value();
    std::string category = jd["category"].string_value();
    int power = jd["power"].int_value();
    int accuracy = jd["accuracy"].int_value();
    int pp = jd["pp"].int_value();

    return new Move(name, type, category, power, accuracy, pp);
    // return new Move(); // REPLACE ME, just a placeholder
}

bool Move::usePP() {
    // Implement PP decrement logic
    /** CODE: insert here */
    if (pp > 0) {
        pp--;
        return true;
    }
    return false;
}

// Implement getter methods to provide read-only access to move attributes

string Move::getType() const {
    // Return the move's type
    /** CODE: insert here */
    return type;
}

string Move::getName() const {
    // Return the move's name
    /** CODE: insert here */
    return name;
}

string Move::getCategory() const {
    // Return the move's category
    /** CODE: insert here */
    return category;
}

int Move::getPower() const {
    // Return the move's power
    /** CODE: insert here */
    return power;
}

int Move::getAccuracy() const {
    // Return the move's accuracy
    /** CODE: insert here */
    return accuracy;
}

int Move::getPP() const {
    // Return the move's current PP
    /** CODE: insert here */
    return pp;
}