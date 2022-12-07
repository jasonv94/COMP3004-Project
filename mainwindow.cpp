#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   time = 10;
    ui->setupUi(this);
    db = new DBManager();
    powerStatus = false;
    currentUser = 1;
    //ui->menuWidget->setVisible(powerStatus);
    masterMenu = new Menu("MAIN MENU", {"USER","NEW SESSION","HISTORY"}, nullptr);
    sessionMenu = new Menu("SESSION INFO", {"TYPE: ","TIME: ","FREQUENCY: "}, masterMenu);
    therapyName = "N/A";
    sessionTime = "N/A";
    hz = "N/A";
    sessionKill = false;
    mainMenu = masterMenu;
    initMenu(masterMenu);
    currentMenu = ui->menuWidget;
    currentMenu->addItems(masterMenu->getMenuItems());
    ui->sessionLabel->setHidden(true);
    ui->frequencyLabel->setHidden(true);
    ui->timeLabel->setHidden(true);
    ui->batteryBox->setValue(100);
    ui->BatteryBar->setValue(100);
    //Retrieve recordings based on current user
    getRecordings(currentUser);
     // Account for device being "off" on sim start
    powerStatus = false;
    changePowerStatus();

    // if powerStatus is true device on then set current row to beggining
    if(powerStatus == true){
        currentMenu->setCurrentRow(0);
    }

    bool subMenu = false;
    /*uncommenting for now will most likely delete
    recordings = db->getRecordings(currentUser);
    for (int x = 0; x < recordings.size(); x++) {
        userRecordings += recordings[x]->string_record()+". "+recordings[x]->get_date()+ " " +recordings[x]->get_therapyName()+" "+recordings[x]->get_sessionTime()+" "+
                                         recordings[x]->get_frequency() + " " + recordings[x]->get_intensity();
    }
    */

    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerChange);
    connect(ui->batteryBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeBatteryLevel);
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->alphaButton, &QPushButton::pressed, this, &MainWindow::alphaPressed);
    connect(ui->betaButton, &QPushButton::pressed, this, &MainWindow::betaPressed);
    connect(ui->deltaButton, &QPushButton::pressed, this, &MainWindow::deltaPressed);
    connect(ui->thetaButton, &QPushButton::pressed, this, &MainWindow::thetaPressed);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::startSession);
    connect(ui->clearButton, &QPushButton::pressed, this, &MainWindow::clearHistory);



    QPixmap pix(":/img/images/device.PNG");
    int w = ui->rmb->width();
    int h = ui->rmb->height();
    ui->rmb->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));
//    ui->userMenu.setVisible(false);
//    ui->mainMenu.setVisible(false);
//    ui->groupMenu.setVisible(false);
}

void MainWindow::initMenu(Menu* x){

    Menu* timeMenu = new Menu("TIME SELECTION MENU", {"20:00","45:00","Custom Time"}, sessionMenu);
    Menu* userMenu = new Menu("USER SELECTION MENU", {"User 1","User 2","User 3"}, x);
    Menu* history = new Menu("HISTORY",{}, x);
    Menu* customTime = new Menu("CUSTOM TIME",{},timeMenu);

    x->addChildMenu(userMenu);
    x->addChildMenu(sessionMenu);
    x->addChildMenu(history);
    sessionMenu->addChildMenu(timeMenu);
    timeMenu->addChildMenu(customTime);
}

void MainWindow::navigateUpMenu(){
    if(sessionStarted == true){
        int value = ui->progressBar->value();
        value += 1;
        ui->progressBar->setValue(value);
    }else{
    int nextIndex = currentMenu->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = currentMenu->count() - 1;
    }

    currentMenu->setCurrentRow(nextIndex);
    }
}

//    if(masterMenu->getName()=="VIEW"){
//qDebug()<<nextIndex;
//qDebug()<<allRecordings[nextIndex][0];
//}
//for retrieving history we need to mock based on the previous
//in denas it goes based off view so we need to do the same
//add check to make sure index isnt greater than size of list
//we need to access directly from list of records
//test for empty list as well
void MainWindow::navigateDownMenu() {

    if(sessionStarted == true){
        int value = ui->progressBar->value();
        value -= 1;
        ui->progressBar->setValue(value);
    }else{
    int nextIndex = currentMenu->currentRow() + 1;

    if (nextIndex > currentMenu->count() - 1) {
        nextIndex = 0;
    }
    //qDebug()<<nextIndex;

    currentMenu->setCurrentRow(nextIndex);
    }
}

void MainWindow::navigateSubMenu() {

   int index = currentMenu->currentRow();
   //code for retrieving based on history
   if(masterMenu->getName()=="HISTORY"){
       if(userRecordings.size() > 0){
            if(userRecordings[index]!=nullptr){
                int ridIndex = userRecordings[index].indexOf('.');
                int rid = userRecordings[index].mid(0,ridIndex).toInt();
                recordedSession = db->getRecord(currentUser,rid);
            }

            //setting master menu to session so proper menu is set so timecount can work
            masterMenu = sessionMenu;
            updateMenu(sessionMenu->getName(),sessionMenu->getMenuItems());
            therapyName = recordedSession->get_therapyName();
            sessionTime = recordedSession->get_frequency();
            hz = recordedSession->get_sessionTime();
            ui->sessionLabel->setText(recordedSession->get_therapyName());
            ui->sessionLabel->setHidden(false);
            ui->frequencyLabel->setText(recordedSession->get_frequency());
            ui->frequencyLabel->setHidden(false);
            ui->timeLabel->setText(recordedSession->get_sessionTime());
            ui->timeLabel->setHidden(false);
            ui->progressBar->setValue(recordedSession->get_intensity().toInt());
            return;


       }
       return;
   }


    qDebug()<< index;
    if (index < 0) return;
    qDebug()<<masterMenu->getMenuItems()[index];
    if(masterMenu->getMenuItems()[index] == "USER"){
        masterMenu = masterMenu->get(index);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "NEW SESSION"){
        masterMenu = masterMenu->get(index);
        //qDebug()<<masterMenu->getMenuItems()[1] + "something";
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "HISTORY"){
        masterMenu = masterMenu->get(index);
        //maybe change this menu name to recordings
        updateMenu(masterMenu->getName(),userRecordings);
    }else if(masterMenu->getMenuItems()[index] == "TIME: "){
        masterMenu = masterMenu->get(0);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 1"){
        currentUser = 1;//update current user
        getRecordings(currentUser);
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 2"){
        currentUser = 2;//update current user and get new recordings
        getRecordings(currentUser);
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 3"){
        currentUser = 3;//update current user
        getRecordings(currentUser);
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "20:00"){
        masterMenu = masterMenu->getParent();
        sessionTime = "20:00";
        ui->timeLabel->setHidden(false);
        ui->timeLabel->setText(sessionTime);
        //masterMenu->addToMenu(1,"20:00");
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "45:00"){
        masterMenu = masterMenu->getParent();
        sessionTime = "45:00";
        ui->timeLabel->setHidden(false);
        ui->timeLabel->setText(sessionTime);
        //masterMenu->addToMenu(1,"45:00");
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "Custom Time"){
        masterMenu = masterMenu->get(0);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getName() == "CUSTOM TIME"){

    }

}
//remeber to increment down
void MainWindow::alphaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    therapyName = "Alpha";
    hz = "9-11 Hz";
    ui->progressBar->setValue(0);
    ui->sessionLabel->setHidden(false);
    ui->sessionLabel->setText(therapyName);
    ui->frequencyLabel->setHidden(false);
    ui->frequencyLabel->setText(hz);
    //masterMenu->addToMenu(0,"Alpha Session");
    //masterMenu->addToMenu(2,"9-11 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];
}
void MainWindow::betaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    therapyName = "Beta 1";
    hz = "12-15 Hz";
    ui->progressBar->setValue(0);
    ui->sessionLabel->setHidden(false);
    ui->sessionLabel->setText(therapyName);
    ui->frequencyLabel->setHidden(false);
    ui->frequencyLabel->setText(hz);
    //masterMenu->addToMenu(0,"Beta 1 Session");
    //masterMenu->addToMenu(2,"12-15 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];

}
void MainWindow::deltaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    therapyName = "Delta";
    hz = "2.5-5 Hz";
    ui->progressBar->setValue(0);
    ui->sessionLabel->setHidden(false);
    ui->sessionLabel->setText(therapyName);
    ui->frequencyLabel->setHidden(false);
    ui->frequencyLabel->setText(hz);
    //masterMenu->addToMenu(0,"Delta Session");
    //masterMenu->addToMenu(2,"2.5-5 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];

}

void MainWindow::thetaPressed(){
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }
    therapyName = "Theta";
    hz = "6-8 Hz";
    ui->progressBar->setValue(0);
    ui->sessionLabel->setHidden(false);
    ui->sessionLabel->setText(therapyName);
    ui->frequencyLabel->setHidden(false);
    ui->frequencyLabel->setText(hz);
    //masterMenu->addToMenu(0,"Theta Session");
    //masterMenu->addToMenu(2,"6-8 Hz");
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    qDebug()<<masterMenu->getMenuItems()[0];

}
//look on previous git history to review what was here before commented out everything else
void MainWindow::startSession(){
    qDebug()<<masterMenu->getName();
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    //change to add the flags in here
    //masterMenu->getMenuItems()[0].length() < 7 || masterMenu->getMenuItems()[1].length() < 7 || masterMenu->getMenuItems()[2].length() < 11
    }else if(therapyName == "N/A" || sessionTime == "N/A" || hz == "N/A"){
     return;
    }else{
        sessionStarted = true;
        int total_time = 60*ui->timeLabel->text().toInt();
        currentSession = new Therapy(therapyName,ui->progressBar->value(),hz,total_time,sessionTime);
        initTimer(currentSession->get_duration());
    }
}
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems){
    currentMenu->clear();
    currentMenu->addItems(menuItems);
    currentMenu->setCurrentRow(0);
    //mainMenu = sessionMenu;

    //ui->menuLabel->setText(selectedMenuItem);
}
void MainWindow::initTimer(QTimer* timer){
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->start(1000);
}
void MainWindow::updateTimer(){
    time-=1;
    currentSession->set_time(time);
    //currentSession->get_length() - 1
    //int timeLeft = currentSession->get_length();
    int timeLeft = currentSession->get_length();
    qDebug()<<timeLeft;
    if(timeLeft < 0){

    //db->addRecord(currentUser,currentSession->get_name(),currentSession->get_timestring(),currentSession->get_frequency(),ui->progressBar->value());
    currentSession->get_duration()->stop();
    currentSession->get_duration()->disconnect();
    currentSession = nullptr;
    //currentMenu->clear();
    therapyName = "N/A";
    sessionTime = "N/A";
    hz = "N/A";
    sessionStarted = false;
    ui->frequencyLabel->setHidden(true);
    ui->sessionLabel->setHidden(true);
    ui->timeLabel->setHidden(true);
    //possibly add if statement here to check menu name only if it is on the new session
    masterMenu = masterMenu->getParent();
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    time = 10;//testing time will need to uncomment out the above for actual time parameters
    }
    ui->timeLabel->setText(QString::number(timeLeft/60) + ((timeLeft%60 < 10) ? + ":0" + QString::number(timeLeft%60) : + ":" + QString::number(timeLeft%60)));
    //masterMenu->addToMenu(1,QString::number(timeLeft/60) + ((timeLeft%60 < 10) ? + ":0" + QString::number(timeLeft%60) : + ":" + QString::number(timeLeft%60)));
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
}

//this well need to be altered to handle battery level
void MainWindow::changeBatteryLevel(int newLevel) {
    if (newLevel == 0.0 && powerStatus == true) {
        powerChange();
    }

    ui->batteryBox->setValue(newLevel);
    ui->BatteryBar->setValue(newLevel);

    QString highBatteryHealth = "QProgressBar { selection-background-color: rgb(78, 154, 6); background-color: rgb(255, 255, 255); }";
    QString mediumBatteryHealth = "QProgressBar { selection-background-color: rgb(196, 160, 0); background-color: rgb(255, 255, 255); }";
    QString lowBatteryHealth = "QProgressBar { selection-background-color: rgb(164, 0, 0); background-color: rgb(255, 255, 255); }";

    if (newLevel >= 50) {
        ui->BatteryBar->setStyleSheet(highBatteryHealth);
    }
    else if (newLevel >= 20) {
        ui->BatteryBar->setStyleSheet(mediumBatteryHealth);
    }
    else {
        ui->BatteryBar->setStyleSheet(lowBatteryHealth);
    }
}

void MainWindow::changePowerStatus(){

    qDebug()<<"pressed";
    ui->progressBar->setVisible(powerStatus);
    ui->BatteryBar->setVisible(powerStatus);
    //set background to black essentially turn it off

    // this is used to end session resetting time to 10 will remove based on what was there before
    if(sessionStarted == true){
        //qDebug()<<currentSession->get_name();
        //qDebug()<<currentSession->get_timestring();                                                                         //changed from currentSession->get_intensity();
        db->addRecord(currentUser,currentSession->get_name(),currentSession->get_timestring(),currentSession->get_frequency(),ui->progressBar->value());
        currentSession->get_duration()->stop();
        currentSession->get_duration()->disconnect();
        currentSession = nullptr;
        therapyName = "N/A";
        sessionTime = "N/A";
        hz = "N/A";
        time = 10;
    }
    if(!powerStatus){
       ui->menuWidget->setStyleSheet("background-color:black;");
       currentMenu->setCurrentRow(-1);
    }else{

        //set background to white when on

        ui->menuWidget->setStyleSheet("background-color:white;");

        //go back to main menu depending on nest level
        while(masterMenu->getName()!="MAIN MENU"){
         masterMenu = masterMenu->getParent();

        }
        qDebug()<<masterMenu->getName();
        qDebug()<<masterMenu->getMenuItems();

        //set menu back to home while loop gets us back to menu
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
        currentMenu->setCurrentRow(0);
    }

    //these are set up to disable or enable based on power status
    sessionStarted = false;
    ui->upButton->setEnabled(powerStatus);
    ui->downButton->setEnabled(powerStatus);
    ui->okButton->setEnabled(powerStatus);
    ui->alphaButton->setEnabled(powerStatus);
    ui->betaButton->setEnabled(powerStatus);
    ui->deltaButton->setEnabled(powerStatus);
    ui->thetaButton->setEnabled(powerStatus);
    ui->selectButton->setEnabled(powerStatus);
    ui->batteryBox->setEnabled(powerStatus);
    ui->connectionBox->setEnabled(powerStatus);
    ui->clearButton->setEnabled(powerStatus);
    ui->sessionLabel->setHidden(true);
    ui->frequencyLabel->setHidden(true);
    ui->timeLabel->setHidden(true);



    //ui->menuWidget->setVisible(powerStatus);
    //ui->menuWidget->setStyleSheet("background-color:black;");


}

// delete history based off the currentUser;
void MainWindow::clearHistory(){
db->deleteRecords(currentUser);
recordings.clear();
userRecordings.clear();

}



void MainWindow::powerChange() {
    powerStatus  = !powerStatus;
    changePowerStatus();

  /*
    if (currentTimerCount != -1) {
        //Save Record
        if (masterMenu->getParent()->getName() == "PROGRAMS") {
            recordings.last()->setDuration((currentTherapy->getTime())-currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addTherapyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }
        else {
            recordings.last()->setDuration(currentTimerCount);
            recordings.last()->setPowerLevel(maxPowerLevel);
            db->addFrequencyRecord(recordings.last()->getTreatment(),recordings.last()->getStartTime(),recordings.last()->getPowerLevel(),recordings.last()->getDuration());
        }

        allRecordings += recordings.last()->toString();
        //Stop therapy
        currentTimerCount = -1;
        currentTherapy->getTimer()->stop();
        currentTherapy->getTimer()->disconnect();
        applyToSkin(false);
    }*/
}
void MainWindow::getRecordings(int currentUser){
    //clear values from previous user and add from new
    recordings.clear();
    userRecordings.clear();
    recordings = db->getRecordings(currentUser);
    for (int x = 0; x < recordings.size(); x++) {
        userRecordings += recordings[x]->string_record()+". "+recordings[x]->get_date()+ " " +recordings[x]->get_therapyName()+" "+recordings[x]->get_sessionTime()+" "+
                                         recordings[x]->get_frequency() + " " + recordings[x]->get_intensity();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
