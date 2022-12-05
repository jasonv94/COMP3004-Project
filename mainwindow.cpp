#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_9->setHidden(true);
    db = new DBManager();
    timer = new QTimer(this);
    int profile = 1;
    QString word = "hello";
    masterMenu = new Menu("MAIN MENU", {"USER","NEW SESSION","HISTORY"}, nullptr);
    sessionMenu = new Menu("SESSION INFO", {"TYPE: ","TIME: ","FREQUENCY: "}, nullptr);
    historyMenu = new Menu("HISTORY",{"RECORDS MAN"},nullptr);
    mainMenuOG = masterMenu;
    currentTimerCount = 1000;
    //ui->menuWidget->setDisabled(true);
    currentMenu = ui->menuWidget;
    currentMenu->addItems(masterMenu->getMenuItems());
    currentMenu->addItem(QString::number(currentTimerCount));
    currentMenu->setCurrentRow(0);

    bool subMenu = false;
    //db->addRecord(1,"Theta","25mins","10 Hz",4);
    recordings = db->getRecordings();

    for (int x = 0; x < recordings.size(); x++) {
        qDebug()<<recordings[x]->string_record();
        userRecordings += recordings[x]->string_record()+recordings[x]->get_date()+ " " +recordings[x]->get_therapyName()+" "+recordings[x]->get_sessionTime()+" "+
                                 recordings[x]->get_frequency() + " " + recordings[x]->get_intensity();
    }
    //db->addRecord(1,"Alpha")
    connect(timer, &QTimer::timeout, this,&MainWindow::updateTimer);
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::sessionWindow);
    connect(ui->alphaButton, &QPushButton::pressed, this, &MainWindow::alphaPressed);
    connect(ui->betaButton, &QPushButton::pressed, this, &MainWindow::betaPressed);
    connect(ui->deltaButton, &QPushButton::pressed, this, &MainWindow::deltaPressed);
    connect(ui->thetaButton, &QPushButton::pressed, this, &MainWindow::thetaPressed);
    connect(ui->deleteButton, &QPushButton::pressed, this, &MainWindow::deleteData);


    QPixmap pix("/home/student/Desktop/COMP3004-Project/images/device.PNG");
    int w = ui->rmb->width();
    int h = ui->rmb->height();
    ui->rmb->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
//    ui->userMenu.setVisible(false);
//    ui->mainMenu.setVisible(false);
//    ui->groupMenu.setVisible(false);
}


void MainWindow::initializeTimer(QTimer* t) {

    connect(t, &QTimer::timeout, this, &MainWindow::updateTimer);

    t->start(1000);


}



void MainWindow::navigateUpMenu() {

    int nextIndex = currentMenu->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = currentMenu->count() - 1;
    }

    qDebug()<<"Index from the up"+QString::number(nextIndex);
    currentMenu->setCurrentRow(nextIndex);

}


void MainWindow::navigateDownMenu() {

    int nextIndex = currentMenu->currentRow() + 1;

    if (nextIndex > currentMenu->count() - 1) {
        nextIndex = 0;
    }
    //qDebug()<<masterMenu->getMenuItems()[nextIndex];
    //qDebug()<<userRecordings.size();
    qDebug()<<"Index from the down"+QString::number(nextIndex);
    currentMenu->setCurrentRow(nextIndex);
}

void MainWindow::navigateSubMenu() {

    int index = currentMenu->currentRow();
    //qDebug()<< currentMenu->index
    if (index < 0) return;
    //qDebug()<<masterMenu->getMenuItems()[index];
    if(masterMenu->getMenuItems()[index] == "NEW SESSION"){
        qDebug()<<sessionMenu->getMenuItems()[1] + "something";
        updateMenu(sessionMenu->getName(),sessionMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "HISTORY"){
        MainWindow::updateMenu("RECORDINGS",userRecordings);
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
//remeber to increment down
void MainWindow::alphaPressed(){

    ui->progressBar->setValue(2);
    sessionMenu->addToMenu(0,"Alpha Test");
    updateMenu(sessionMenu->getName(),sessionMenu->getMenuItems());
    qDebug()<<sessionMenu->getMenuItems()[0];
}
void MainWindow::betaPressed(){
    ui->progressBar->setValue(4);
    sessionMenu->addToMenu(0,"Beta Test");
    updateMenu(sessionMenu->getName(),sessionMenu->getMenuItems());


}
void MainWindow::deltaPressed(){
    ui->progressBar->setValue(6);
    sessionMenu->addToMenu(0,"Delta Test");
    updateMenu(sessionMenu->getName(),sessionMenu->getMenuItems());


}

void MainWindow::thetaPressed(){
    ui->progressBar->setValue(8);
    sessionMenu->addToMenu(0,"Theta Test");
    updateMenu(sessionMenu->getName(),sessionMenu->getMenuItems());

}

void MainWindow::sessionWindow(){
    currentMenu->addItem(QString::number(currentTimerCount));
    timer->start(1000);


    if(testSelect==true && timeSelect == true){
        sessionStart = true;

    }else{
        qDebug()<<"SELECT THERAPY AND SESSION FIRST";
    }
    bool testSelect = false;
    bool timeSelect = false;
    bool sessionStart = false;
}

void MainWindow::updateTimer() {
    qDebug()<<"HERE WE GO";
    timeString = QString::number(currentTimerCount/60) + ((currentTimerCount%60 < 10) ? + ":0" + QString::number(currentTimerCount%60) : + ":" + QString::number(currentTimerCount%60));
    currentTimerCount -= 1;
    //currentMenu->clear();
    //currentMenu->addItem(QString::number(currentTimerCount));
    ui->label_9->setHidden(false);
    ui->label_9->setText(QString::number(currentTimerCount));
    qDebug()<<QString::number(currentTimerCount);
}

void MainWindow::deleteData(){
qDebug()<<"Data is deleted";
//userRecordings.clear();
db->deleteRecords();

}
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    currentMenu->clear();
    currentMenu->addItems(menuItems);
    currentMenu->setCurrentRow(0);
    mainMenuOG = sessionMenu;

    //ui->menuLabel->setText(selectedMenuItem);
}
MainWindow::~MainWindow()
{
    delete ui;
}
