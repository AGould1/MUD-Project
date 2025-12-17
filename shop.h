#ifndef SHOP_H
#define SHOP_H

#include <iostream>
#include <iomanip>
#include "data.h"
#include <string>

class ShopItem {
private:
    int id;
    std::string name;
    int damage;
    int value;
    ShopItem* next;
    ShopItem* prev;

public:
    ShopItem()
        : id(0), name(""), damage(0), value(0), next(nullptr), prev(nullptr) {}

    ShopItem(int id, const std::string& name, int damage, int value)
    : id(id), name(name), damage(damage), value(value), next(nullptr), prev(nullptr) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getDamage() const { return damage; }
    int getValue() const { return value; }

    ShopItem* getNext() const { return next; }
    ShopItem* getPrev() const { return prev; }

    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setDamage(int newDamage) { damage = newDamage; }
    void setValue(int newValue) { value = newValue; }

    void setNext(ShopItem* newNext) { next = newNext; }
    void setPrev(ShopItem* newPrev) { prev = newPrev; }
};

class Shop {
protected:
    ShopItem* head;
    ShopItem* tail;
    int lineItemIds[10];

    ShopItem* removeItemById(int itemId);
    bool matchesSearch(const std::string& itemName, const std::string& searchTerm);

public:
    Shop(Item* item_list, ItemType objtype);
    virtual ~Shop();

    void addShopItem(Item item);

    virtual ShopItem* listItems(ShopItem* nextItemToShow, const std::string& searchTerm);

    void enter(int* player_inventory, int* inventory_count);
};

class NonWeaponShop : public Shop {
public:
    NonWeaponShop(Item *item_list, ItemType objtype);
    ShopItem* listItems(ShopItem* nextItemToShow, const std::string& searchTerm) override;
};

#endif