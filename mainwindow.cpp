#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    masterMenu = new Menu("MAIN MENU", {"USER","NEW SESSION","HISTORY"}, nullptr);
    mainMenuOG = masterMenu;
    currentMenu = ui->menuWidget;
    currentMenu->addItems(masterMenu->getMenuItems());
    currentMenu->setCurrentRow(0);

    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
}


void MainWindow::navigateUpMenu() {

    int nextIndex = currentMenu->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = currentMenu->count() - 1;
    }

    currentMenu->setCurrentRow(nextIndex);
}


void MainWindow::navigateDownMenu() {

    int nextIndex = currentMenu->currentRow() + 1;

    if (nextIndex > currentMenu->count() - 1) {
        nextIndex = 0;
    }

    currentMenu->setCurrentRow(nextIndex);
}
MainWindow::~MainWindow()
{
    delete ui;
}

