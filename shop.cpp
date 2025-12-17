#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include "shop.h"
#include "data.h"
#include <cstring>

using namespace std;

Shop::Shop(Item* item_list, ItemType objtype) {
    head = nullptr;
    tail = nullptr;

    for (int i = 0; i < 10; i++) {
        lineItemIds[i] = -1;
    }
    int i = 0;

    while (true) {
        Item currentItem = item_list[i];

        if (currentItem.id != -1 && currentItem.type == objtype) {
            addShopItem(currentItem);
        }
        i++;

        if (currentItem.last) {
            break;
        }
    }
}

void Shop::addShopItem(Item item) {
    ShopItem* newItem = new ShopItem(item.id, item.name, item.damage, item.value);

    if (head == nullptr) {
        head = newItem;
        tail = newItem;
    } else {
        newItem->setPrev(tail);
        tail->setNext(newItem);
        tail = newItem;
    }
}
bool Shop::matchesSearch(const std::string& itemName, const std::string& searchTerm) {
    if (searchTerm.empty()) {
        return true;
    }
    string lowerItemName = itemName;
    string lowerSearchTerm = searchTerm;

    transform(lowerItemName.begin(), lowerItemName.end(), lowerItemName.begin(), ::tolower);
    transform(lowerSearchTerm.begin(), lowerSearchTerm.end(), lowerSearchTerm.begin(), ::tolower);

    return lowerItemName.find(lowerSearchTerm) != string::npos;
}

ShopItem* Shop::listItems(ShopItem* nextItemToShow, const std::string& searchTerm) {
    cout << left << setw(7) << "Item#" << setw(40) << "Name" << setw(10) << "Damage" << setw(5) << "Value" << endl;
    cout << left << setw(7) << "-----" << setw(40) << "--------------------" << setw(10) << "-----" << setw(5) << "-----" << endl;

    for (int i = 0; i < 10; i++) {
        lineItemIds[i] = -1;
    }

    ShopItem* current = nextItemToShow;
    int index = 1;
    int itemsDisplayed = 0;

    while (current != nullptr && itemsDisplayed < 10) {
        if (matchesSearch(current->getName(), searchTerm)) {
            cout << left << setw(7) << index
                 << setw(40) << current->getName()
                 << setw(10) << current->getDamage()
                 << setw(5) << current->getValue() << endl;

            lineItemIds[index - 1] = current->getId();
            itemsDisplayed++;
            index++;
        }
        current = current->getNext();
    }
    if (current != nullptr) {
        return current;
    } else {
        return nextItemToShow;
    }
}
ShopItem* Shop::removeItemById(int itemId) {
    ShopItem* current = head;

    while (current != nullptr) {
        if (current->getId() == itemId) {
            break;
        }
        current = current->getNext();
    }
    if (current == nullptr) {
        return nullptr;
    }

    ShopItem* prevItem = current->getPrev();
    ShopItem* nextItem = current->getNext();

    if (prevItem == nullptr && nextItem != nullptr) {
        head = nextItem;
        nextItem->setPrev(nullptr);
    }
    else if (prevItem != nullptr && nextItem == nullptr) {
        tail = prevItem;
        prevItem->setNext(nullptr);
    }
    else if (prevItem != nullptr && nextItem != nullptr) {
        prevItem->setNext(nextItem);
        nextItem->setPrev(prevItem);
    }
    else {
        head = nullptr;
        tail = nullptr;
    }

    return current;
}

void Shop::enter(int* player_inventory, int* inventory_count) {
    ShopItem* nextItemToShow = head;
    string searchTerm = "";

    while (true) {
        ShopItem* result = listItems(nextItemToShow, searchTerm);

        cout << "Press Enter or 'n' to go forward, 'p' to go back, 'b' to buy, 's' to search, or 'e' to exit: ";

        char input[10];
        if (fgets(input, sizeof(input), stdin)) {
            input[strcspn(input, "\n")] = 0;

            if (input[0] == 'e' || input[0] == 'E') {
                break;
            }
            else if (input[0] == 'n' || input[0] == 'N' || input[0] == '\0') {
                if (result != nextItemToShow) {
                    nextItemToShow = result;
                }
            }
            else if (input[0] == 'p' || input[0] == 'P') {
                ShopItem* backtrack = nextItemToShow;
                int steps = 0;

                while (backtrack != nullptr && steps < 10) {
                    backtrack = backtrack->getPrev();
                    if (backtrack != nullptr && matchesSearch(backtrack->getName(), searchTerm)) {
                        steps++;
                    }
                }
                if (backtrack == nullptr) {
                    nextItemToShow = head;
                    while (nextItemToShow != nullptr && !matchesSearch(nextItemToShow->getName(), searchTerm)) {
                        nextItemToShow = nextItemToShow->getNext();
                    }
                } else {
                    nextItemToShow = backtrack;
                }
            }
            else if (input[0] == 's' || input[0] == 'S') {
                cout << "Enter search term: ";
                char searchInput[100];
                if (fgets(searchInput, sizeof(searchInput), stdin)) {
                    searchInput[strcspn(searchInput, "\n")] = 0;
                    searchTerm = searchInput;

                    nextItemToShow = head;
                    while (nextItemToShow != nullptr && !matchesSearch(nextItemToShow->getName(), searchTerm)) {
                        nextItemToShow = nextItemToShow->getNext();
                    }
                }
            }
            else if (input[0] == 'b' || input[0] == 'B') {
                cout << "Enter item number to buy: ";
                char buyInput[10];
                if (fgets(buyInput, sizeof(buyInput), stdin)) {
                    buyInput[strcspn(buyInput, "\n")] = 0;
                    int itemNumber = atoi(buyInput);

                    if (itemNumber >= 1 && itemNumber <= 10) {
                        int itemId = lineItemIds[itemNumber - 1];

                        if (itemId != -1) {
                            ShopItem* removedItem = removeItemById(itemId);

                            if (removedItem != nullptr) {
                                player_inventory[*inventory_count] = removedItem->getId();
                                (*inventory_count)++;

                                cout << "You bought " << removedItem->getName() << endl;

                                if (removedItem == nextItemToShow) {
                                    nextItemToShow = removedItem->getNext();
                                    while (nextItemToShow != nullptr && !matchesSearch(nextItemToShow->getName(), searchTerm)) {
                                        nextItemToShow = nextItemToShow->getNext();
                                    }
                                    if (nextItemToShow == nullptr) {
                                        nextItemToShow = head;
                                        while (nextItemToShow != nullptr && !matchesSearch(nextItemToShow->getName(), searchTerm)) {
                                            nextItemToShow = nextItemToShow->getNext();
                                        }  
                                    }
                                }
                                delete removedItem;
                            }
                        } else {
                            cout << "Error: Could not purchase item" << endl;
                        }
                    } else {
                        cout << "Invalid item number" << endl;
                    }
                }
            }
        }
    }
}
Shop::~Shop() {
    ShopItem* current = head;
    while (current != nullptr) {
        ShopItem* next = current->getNext();
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
}

NonWeaponShop::NonWeaponShop(Item* item_list, ItemType objtype) 
    : Shop(item_list, objtype) {
}

ShopItem* NonWeaponShop::listItems(ShopItem* nextItemToShow, const std::string& searchTerm) {
    cout << left << setw(7) << "Item#" << setw(40) << "Name" << setw(5) << "Value" << endl;
    cout << left << setw(7) << "-----" << setw(40) << "--------------------" << setw(5) << "-----" << endl;

    for (int i = 0; i < 10; i++) {
        lineItemIds[i] = -1;
    }

    ShopItem* current = nextItemToShow;
    int index = 1;
    int itemsDisplayed = 0;

    while (current != nullptr && itemsDisplayed < 10) {
        if (matchesSearch(current->getName(), searchTerm)) {
            cout << left << setw(7) << index
                 << setw(40) << current->getName()
                 << setw(5) << current->getValue() << endl;

            lineItemIds[index - 1] = current->getId();
            itemsDisplayed++;
            index++;
        }
        current = current->getNext();
    }

    if (current != nullptr) {
        return current;
    } else {
        return nextItemToShow;
    }
}