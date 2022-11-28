#include "menu.h"
#include "QString"
Menu::Menu(QString n, QStringList list, Menu* p) {

    name = n;
    menuItems = list;
    parent = p;
}


Menu::~Menu() {

    for(int x = 0; x < subMenus.length(); x++) {
        delete subMenus[x];
    }
}


void Menu::addChildMenu(Menu* m) {

    subMenus.push_back(m);
}

void Menu::addToMenu(int index,QString value) {
    menuItems[index] = menuItems[index];
    menuItems[index] = menuItems[index] + " " + value;

}


// getters
QString Menu::getName() { return name; }
QStringList Menu::getMenuItems() { return menuItems; }
Menu* Menu::getParent() { return parent; }
int Menu::getPos() { return position; }
Menu* Menu::get(int i) { return subMenus[i]; }
