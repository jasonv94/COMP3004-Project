#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>
#include <QVector>


/* Purpose:
 * Menu class serves as an object that will be displayed and interacted with on the main window
 * Derived from COMP 3004 project denas team 3
 *
 */

class Menu {

public:
    explicit Menu(QString, QStringList, Menu*);
    ~Menu();

    QString getName(); //get menu name
    QStringList getMenuItems();//get menu items
    Menu* getParent();//get parent menu
    int getPos();//get position on menu
    Menu* get(int);// get menu
    void addChildMenu(Menu*);//add to child menu
    void addToMenu(int,QString);// adds to menu

private:
    QString name;// variable for menu ame
    QStringList menuItems;//used to show menuitems on UI
    int position; //get position in the menu
    QVector<Menu*> subMenus; // submenus of master menu
    Menu* parent;//parent of the menu


};

#endif // MENU_H
