#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
int MAX_TIME = 300;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   time = 10;
    ui->setupUi(this);
    db = new DBManager();
    powerStatus = false;
    currentUser = 1;
    tempLevel = 100.0;
    batteryHealth = 100;
    drainFactor = 1.0/3600.00;
    //ui->menuWidget->setVisible(powerStatus);
    masterMenu = new Menu("MAIN MENU", {"USER","NEW SESSION","HISTORY"}, nullptr);
    sessionMenu = new Menu("SESSION INFO", {"TYPE: ","TIME: ","FREQUENCY: ","RECORD: "}, masterMenu);
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
    ui->customTimeLabel->setHidden(true);
    ui->customTimeLabel->setText("1:00");
    ui->recordLabel->setHidden(true);
    ui->recordLabel->setText("No");
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
    connect(ui->batteryBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeBatteryHealth);
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
    Menu* customTime = new Menu("CUSTOM TIME",{"Select time: "},timeMenu);

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
    }else if (masterMenu->getName() == "CUSTOM TIME"){
        int sepTime = ui->customTimeLabel->text().indexOf(':');
        int rid = ui->customTimeLabel->text().mid(0,sepTime).toInt();
        if(rid >= 1){
            ui->customTimeLabel->setText(QString::number(rid+1) + ":00");
        }

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
    }else if(masterMenu->getName() == "CUSTOM TIME"){
        int sepTime = ui->customTimeLabel->text().indexOf(':');
        int rid = ui->customTimeLabel->text().mid(0,sepTime).toInt();
        if(rid > 1){
            ui->customTimeLabel->setText(QString::number(rid-1) + ":00");
        }
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
            sessionTime = recordedSession->get_sessionTime();
            hz = recordedSession->get_frequency();
            ui->sessionLabel->setText(recordedSession->get_therapyName());
            ui->sessionLabel->setHidden(false);
            ui->frequencyLabel->setText(recordedSession->get_frequency());
            ui->frequencyLabel->setHidden(false);
            ui->timeLabel->setText(recordedSession->get_sessionTime());
            ui->timeLabel->setHidden(false);
            ui->recordLabel->setHidden(false);
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
        ui->recordLabel->setHidden(false);
        masterMenu = masterMenu->get(index);
        //qDebug()<<masterMenu->getMenuItems()[1] + "something";
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "HISTORY"){
        masterMenu = masterMenu->get(index);
        //maybe change this menu name to recordings
        getRecordings(currentUser);
        updateMenu(masterMenu->getName(),userRecordings);
    }else if(masterMenu->getMenuItems()[index] == "TIME: "){
        masterMenu = masterMenu->get(0);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
        ui->sessionLabel->setHidden(true);
        ui->frequencyLabel->setHidden(true);
        ui->timeLabel->setHidden(true);
        ui->recordLabel->setHidden(true);
    }else if(masterMenu->getMenuItems()[index] == "User 1"){
        currentUser = 1;//update current user
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 2"){
        currentUser = 2;//update current user and get new recordings
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "User 3"){
        currentUser = 3;//update current user
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "20:00"){
        masterMenu = masterMenu->getParent();
        sessionTime = "20:00";
        ui->timeLabel->setHidden(false);
        ui->timeLabel->setText(sessionTime);
        if(ui->sessionLabel->text() != "TextLabel"){
            ui->sessionLabel->setHidden(false);
            ui->frequencyLabel->setHidden(false);
        }
        ui->recordLabel->setHidden(false);
        //masterMenu->addToMenu(1,"20:00");
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "45:00"){
        masterMenu = masterMenu->getParent();
        sessionTime = "45:00";
        ui->timeLabel->setHidden(false);
        ui->timeLabel->setText(sessionTime);
        if(ui->sessionLabel->text() != "TextLabel"){
            ui->sessionLabel->setHidden(false);
            ui->frequencyLabel->setHidden(false);
        }
        ui->recordLabel->setHidden(false);
        //masterMenu->addToMenu(1,"45:00");
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "Custom Time"){
        ui->customTimeLabel->setHidden(false);
        masterMenu = masterMenu->get(0);
        qDebug()<<masterMenu->getName();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getName() == "CUSTOM TIME"){
        qDebug()<<"here";
        sessionTime = ui->customTimeLabel->text();
        ui->timeLabel->setHidden(false);
        ui->timeLabel->setText(sessionTime);
        if(ui->sessionLabel->text() != "TextLabel"){
            ui->sessionLabel->setHidden(false);
            ui->frequencyLabel->setHidden(false);
        }
        ui->recordLabel->setHidden(false);
        ui->customTimeLabel->setHidden(true);
        masterMenu = masterMenu->getParent()->getParent();
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "RECORD: "){
        if(ui->recordLabel->text() == "Yes"){
            ui->recordLabel->setText("No");
        }else{
            ui->recordLabel->setText("Yes");
        }
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
    //If battery is only at 2% then a session cant be run
    if(ui->BatteryBar->value()<=2){
        return;
    }
    if(sessionStarted == true){
        return;
    }
    qDebug()<<masterMenu->getName();
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    //change to add the flags in here
    //masterMenu->getMenuItems()[0].length() < 7 || masterMenu->getMenuItems()[1].length() < 7 || masterMenu->getMenuItems()[2].length() < 11
    }else if(therapyName == "N/A" || sessionTime == "N/A" || hz == "N/A"){
     return;
    }else{
        sessionStarted = true;
        int sepTime = ui->timeLabel->text().indexOf(':');
        int rid = ui->timeLabel->text().mid(0,sepTime).toInt();
        ui->customTimeLabel->setText(QString::number(rid) + ":00");
        int total_time = 60*rid;
        currentSession = new Therapy(therapyName,ui->progressBar->value(),hz,total_time,sessionTime);
        if(ui->recordLabel->text() == "Yes"){
            currentSession->set_record(true);
        }

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
    //time-=1;
    if(ui->BatteryBar->value() >= 5){
    drainBattery();
    }
    currentSession->set_time(currentSession->get_length() - 1);
    int timeLeft = currentSession->get_length();
    qDebug()<<timeLeft;
    if(timeLeft < 0 || ui->BatteryBar->value() <= 5){
    if(currentSession->get_record()){
        db->addRecord(currentUser,currentSession->get_name(),currentSession->get_timestring(),currentSession->get_frequency(),ui->progressBar->value());
    }
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
    ui->recordLabel->setHidden(true);
    //possibly add if statement here to check menu name only if it is on the new session
    masterMenu = masterMenu->getParent();
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    applyToSkin(false);
    }
    ui->timeLabel->setText(QString::number(timeLeft/60) + ((timeLeft%60 < 10) ? + ":0" + QString::number(timeLeft%60) : + ":" + QString::number(timeLeft%60)));
    //masterMenu->addToMenu(1,QString::number(timeLeft/60) + ((timeLeft%60 < 10) ? + ":0" + QString::number(timeLeft%60) : + ":" + QString::number(timeLeft%60)));
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
}


/*
 *     if (newLevel >= 0.0 && newLevel <= 100.0) {
        if (newLevel == 0.0 && powerStatus == true) {
            powerChange();
            profile->setBLvl(0);
        }else{
            profile->setBLvl(newLevel);
        }

        ui->batteryLevelAdminSpinBox->setValue(newLevel);
        int newLevelInt = int(newLevel);
        ui->batteryLevelBar->setValue(newLevelInt);
 */

//this well need to be altered to handle battery level


void MainWindow::changeBatteryLevel(double newLevel) {

    if(newLevel <= 0.0 && powerStatus == true) {
            powerChange();
    }
    qDebug()<<"new level templevel";
    qDebug()<<newLevel;

    int batteryLevel = (int) newLevel;
    qDebug()<<"round down check";
    qDebug()<<batteryLevel;
    ui->batteryBox->setValue(batteryLevel);
    ui->BatteryBar->setValue(batteryLevel);
    //tempLevel = newLevel;
    /*
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
    */
}

void MainWindow::changePowerStatus(){

    qDebug()<<"pressed";

    ui->progressBar->setVisible(powerStatus);

    ui->BatteryBar->setVisible(powerStatus);
    //set background to black essentially turn it off

    // this is used to end session resetting time to 10 will remove based on what was there before
    if(sessionStarted == true){
        //qDebug()<<currentSession->get_name();
        //qDebug()<<currentSession->get_timestring();
        //changed from currentSession->get_intensity();
        if(currentSession->get_record()){
            db->addRecord(currentUser,currentSession->get_name(),currentSession->get_timestring(),currentSession->get_frequency(),ui->progressBar->value());
        }
        currentSession->get_duration()->stop();
        currentSession->get_duration()->disconnect();
        currentSession = nullptr;
        therapyName = "N/A";
        sessionTime = "N/A";
        hz = "N/A";
    }

    if(!powerStatus){
       ui->menuWidget->setStyleSheet("background-color:black;");
       applyToSkin(false);
       currentMenu->setCurrentRow(-1);
    }else{

        //set background to white when on

        ui->menuWidget->setStyleSheet("background-color:white;");

        //go back to main menu depending on nest level
        while(masterMenu->getName()!="MAIN MENU"){
         masterMenu = masterMenu->getParent();

        }
        masterMenu = masterMenu->get(0);
        qDebug()<<masterMenu->getName();
        qDebug()<<masterMenu->getMenuItems();
        int i;
        if(powerStatus && ui->batteryBox->value()>=80){
            i=8;
        }else if(powerStatus && ui->batteryBox->value()>60 && ui->batteryBox->value()<80){
            i=6;
        }else if(powerStatus && ui->batteryBox->value()>40 && ui->batteryBox->value()<60){
            i=4;
        }else {
            i=2;
        }
        ui->progressBar->setValue(i);
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
    ui->connectionBox->setEnabled(powerStatus);
    ui->clearButton->setEnabled(powerStatus);
    ui->sessionLabel->setHidden(true);
    ui->frequencyLabel->setHidden(true);
    ui->timeLabel->setHidden(true);
    ui->customTimeLabel->setHidden(true);
    ui->recordLabel->setHidden(true);
    ui->frequencyLabel->setText("");
    ui->sessionLabel->setText("");



    //ui->menuWidget->setVisible(powerStatus);
    //ui->menuWidget->setStyleSheet("background-color:black;");


}

// delete history based off the currentUser;
void MainWindow::clearHistory(){
if(masterMenu->getName() == "HISTORY"){
    db->deleteRecords(currentUser);
    recordings.clear();
    userRecordings.clear();
    masterMenu = masterMenu->getParent();
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
}
}



void MainWindow::powerChange() {
    powerStatus  = !powerStatus;
    changePowerStatus();

}
void MainWindow::getRecordings(int currentUser){
    //clear values from previous user and add from new
    recordings.clear();
    userRecordings.clear();
    recordings = db->getRecordings(currentUser);
    for (int x = 0; x < recordings.size(); x++) {
        userRecordings += recordings[x]->string_record()+". "+recordings[x]->get_date()+ " " +recordings[x]->get_therapyName()+" "+recordings[x]->get_sessionTime()+" "+
                                         recordings[x]->get_frequency() + " Intensity: " + recordings[x]->get_intensity();
    }
}

void MainWindow::applyToSkin(int checked) {

    ui->connectionBox->setCurrentIndex(checked ? 1 : 0);
    connection = checked;
}


void MainWindow::changeBatteryHealth() {
qDebug()<<"Called";
int batteryLevel = ui->batteryBox->value();
if(batteryLevel <= 0 && powerStatus == true) {
        powerChange();
}
if(!sessionStarted){
tempLevel = (double) batteryLevel;
}
ui->BatteryBar->setValue(batteryLevel);

}

void MainWindow::drainBattery() {
    //double drainFactor = 1.0/3600.0;
    double other = (double) ui->BatteryBar->value();
    qDebug()<<"Battery level real";
    qDebug()<<other;
    qDebug()<<"TEMPORARY LEVEL";
    qDebug()<<tempLevel;

    //double battery = batteryHealth - (ui->progressBar->value() * 0.05);
    double drainValue = tempLevel - ( (ui->progressBar->value()+1) * drainFactor);
    tempLevel = drainValue;
    //int battery = (int) drainValue;
    /*
    double batteryHealth = tempLevel;
    double intensity = (double) ui->progressBar->value();
    double drainValue = batteryHealth-((ui->progressBar->value()+1.0)*10.0)/3600.0;
    */
    //qDebug()<<ui->progressBar->value();
    //qDebug()<<drainFactor;
    qDebug()<<drainValue;
    qDebug()<<"_____________";
    //qDebug()<<battery;
    //int batteryLevel = (currentSession->get_intensity() == 0) ? ui->BatteryBar->value() - 1: ui->BatteryBar->value() - currentSession->get_intensity()/10;
    changeBatteryLevel(drainValue);
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i = 0; i < recordings.size(); i++){
       delete recordings[i];
     }
     delete db;


}
