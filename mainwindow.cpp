#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    masterMenu = new Menu("MAIN MENU", {"USER","NEW SESSION","HISTORY"}, nullptr);
    sessionMenu = new Menu("SESSION INFO", {"TYPE: ","TIME: ","FREQUENCY: "}, nullptr);
    mainMenuOG = masterMenu;
    currentMenu = ui->menuWidget;
    currentMenu->addItems(masterMenu->getMenuItems());
    currentMenu->setCurrentRow(0);

    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
}



void MainWindow::navigateUpMenu() {

    int nextIndex = currentMenu->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = currentMenu->count() - 1;
    }
    qDebug()<<masterMenu->getMenuItems()[nextIndex];
    currentMenu->setCurrentRow(nextIndex);
}


void MainWindow::navigateDownMenu() {

    int nextIndex = currentMenu->currentRow() + 1;

    if (nextIndex > currentMenu->count() - 1) {
        nextIndex = 0;
    }
    //qDebug()<<masterMenu->getMenuItems()[nextIndex];

    currentMenu->setCurrentRow(nextIndex);
}

void MainWindow::navigateSubMenu() {

    int index = currentMenu->currentRow();

    if (index < 0) return;
    qDebug()<<masterMenu->getMenuItems()[index];
    if(masterMenu->getMenuItems()[index] == "NEW SESSION"){
        qDebug()<<"REACHED";
        updateMenu(sessionMenu->getName(),sessionMenu->getMenuItems());
    }

    /*
    // Prevent crash if ok button is selected in view
    if (masterMenu->getName() == "VIEW") {
        return;
    }

    //Logic for when the menu is the delete menu.
    if (masterMenu->getName() == "CLEAR") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            db->deleteRecords();
            allRecordings.clear();

            for (int x = 0; x < recordings.size(); x++) {
                delete recordings[x];
            }

            recordings.clear();
            navigateBack();
            return;
        }
        else {
            navigateBack();
            return;
        }
    }

    //If the menu is a parent and clicking on it should display more menus.
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());


    }
    //If the menu is not a parent and clicking on it should start a therapy
    else if (masterMenu->get(index)->getMenuItems().length() == 0 && (masterMenu->getName() == "FREQUENCIES" || masterMenu->getName() == "PROGRAMS")) {
        if (masterMenu->getName() == "PROGRAMS") {
            //Update new menu info
            masterMenu = masterMenu->get(index);
            MainWindow::updateMenu(programs[index]->getName(), {});
            MainWindow::beginTherapy(programs[index]);
        }
        else if (masterMenu->getName() == "FREQUENCIES") {
            masterMenu = masterMenu->get(index);
            MainWindow::updateMenu(frequencies[index]->getName(), {});
            MainWindow::beginTherapy(frequencies[index]);
        }
    }
    //If the button pressed should display the device's recordings.
    else if (masterMenu->get(index)->getName() == "VIEW") {
        masterMenu = masterMenu->get(index);
        MainWindow::updateMenu("RECORDINGS", allRecordings);
    }
    */
}
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    currentMenu->clear();
    currentMenu->addItems(menuItems);
    currentMenu->setCurrentRow(0);

    //ui->menuLabel->setText(selectedMenuItem);
}
MainWindow::~MainWindow()
{
    delete ui;
}

