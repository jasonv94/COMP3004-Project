#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DBManager();
    powerStatus = false;
    sessionStarted = false;
    currentUser = 1;
    tempLevel = 100.0;
    batteryHealth = 100;

    //Used to represent that the device should ast for an hour
    //This can be altered but for demo purposes, kept small to show drainage
    drainFactor = 1.0/3600.00;

    masterMenu = new Menu("MAIN MENU", {"USER","NEW SESSION","HISTORY"}, nullptr);
    sessionMenu = new Menu("SESSION INFO", {"TYPE: ","TIME: ","FREQUENCY: ","RECORD: "}, masterMenu);
    therapyName = "N/A";
    sessionTime = "N/A";
    hz = "N/A";

    mainMenu = masterMenu;
    initMenu(masterMenu);
    currentMenu = ui->menuWidget;
    currentMenu->addItems(masterMenu->getMenuItems());

    //Hide labels used to represent session info and default text
    ui->sessionLabel->setHidden(true);
    ui->frequencyLabel->setHidden(true);
    ui->timeLabel->setHidden(true);
    ui->customTimeLabel->setHidden(true);
    ui->customTimeLabel->setText("1:00");
    ui->recordLabel->setHidden(true);
    ui->recordLabel->setText("No");

    //set battery levels to 100 which represents full charge
    ui->batteryBox->setValue(100);
    ui->BatteryBar->setValue(100);
    //Retrieve recordings based on current user
    getRecordings(currentUser);

    //Account for device being "off" on sim start
    powerStatus = false;
    changePowerStatus();

    if(powerStatus == true){
        currentMenu->setCurrentRow(0);
    }


    connect(ui->powerButton, &QPushButton::released, this, &MainWindow::powerChange);
    connect(ui->batteryBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::changeBatteryHealth);
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->okButton, &QPushButton::pressed, this, &MainWindow::startSession);
    connect(ui->alphaButton, &QPushButton::pressed, this, &MainWindow::alphaPressed);
    connect(ui->betaButton, &QPushButton::pressed, this, &MainWindow::betaPressed);
    connect(ui->deltaButton, &QPushButton::pressed, this, &MainWindow::deltaPressed);
    connect(ui->thetaButton, &QPushButton::pressed, this, &MainWindow::thetaPressed);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->clearButton, &QPushButton::pressed, this, &MainWindow::clearHistory);



    QPixmap pix(":/img/images/device.PNG");
    int w = ui->rmb->width();
    int h = ui->rmb->height();
    ui->rmb->setPixmap(pix.scaled(w, h, Qt::KeepAspectRatio));

}

/*
 * Function: initMenu
 * Params: Menu *x
 *
 * Function is used to initialize menus and link them
 *
 * The last variable in each initialization represents its parent menu
 * If x that means parent is masterMenu
 */

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

//Up arrow button
/*
 * Function: navigateUpMenu
 *
 * Used to move up button on the device
 * Different usage between menus
 * If session is started the up arrow will move the progressBar
 * In the time menu the custom time value is altered
 * Otherwise used to move up menu screens on device
 *
 */
void MainWindow::navigateUpMenu(){
    if(sessionStarted == true){
        int value = ui->progressBar->value();//retrieve value from progressBar and update
        value += 1;
        ui->progressBar->setValue(value);
    }else if (masterMenu->getName() == "CUSTOM TIME"){
        int sepTime = ui->customTimeLabel->text().indexOf(':');//seperate time value to get actual time before colon
        int timeStart = ui->customTimeLabel->text().mid(0,sepTime).toInt(); // extracts actual time value
        if(timeStart >= 1){
            ui->customTimeLabel->setText(QString::number(timeStart+1) + ":00");
        }

    }else{

    int nextIndex = currentMenu->currentRow() - 1;//access the value of the menu

    if (nextIndex < 0) {
        nextIndex = currentMenu->count() - 1;
    }

    currentMenu->setCurrentRow(nextIndex); //set menu index
    }
}

//Up arrow button
/*
 * Function: navigateUpMenu
 *
 * Used to down up button on the device
 * Different usage between menus
 * If session is started the down arrow will move the progressBar
 * In the time menu the custom time value is altered
 * Otherwise used to move down menu screens on device
 *
 */
void MainWindow::navigateDownMenu() {

    if(sessionStarted == true){
        int value = ui->progressBar->value();
        value -= 1;
        ui->progressBar->setValue(value);
    }else if(masterMenu->getName() == "CUSTOM TIME"){
        int sepTime = ui->customTimeLabel->text().indexOf(':');
        int timeStart = ui->customTimeLabel->text().mid(0,sepTime).toInt();
        if(timeStart > 1){
            ui->customTimeLabel->setText(QString::number(timeStart-1) + ":00");
        }
    }else{
    int nextIndex = currentMenu->currentRow() + 1;//access the value of the menu

    if (nextIndex > currentMenu->count() - 1) {
        nextIndex = 0;
    }

    currentMenu->setCurrentRow(nextIndex);//set menu index
    }
}

/*
 * Function: navigateSubMenu
 *
 * Move between menus on the device
 * If menu is History the selected recording will be loaded
 *
 * Otherwise used to move between menus
 *
 */
void MainWindow::navigateSubMenu() {

   int index = currentMenu->currentRow();
   //code for retrieving based on history
   if(masterMenu->getName()=="HISTORY"){
       if(userRecordings.size() > 0){
            if(userRecordings[index]!=nullptr){
                int ridIndex = userRecordings[index].indexOf('.'); //get index of the record id
                int rid = userRecordings[index].mid(0,ridIndex).toInt();// get recordid to be used to call database
                recordedSession = db->getRecord(currentUser,rid);//retrieve record based on user and record id
            }

            //setting master menu to session so proper menu is set so timecount can work
            //Update Menu and show the selected info for the session
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

    if (index < 0) return;

    //Used to handle the various menu items that can be clicked on
    //handle based on the name
    if(masterMenu->getMenuItems()[index] == "USER"){
        masterMenu = masterMenu->get(index);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "NEW SESSION"){
        ui->recordLabel->setHidden(false);
        masterMenu = masterMenu->get(index);
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getMenuItems()[index] == "HISTORY"){
        masterMenu = masterMenu->get(index);
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
        updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    }else if(masterMenu->getName() == "CUSTOM TIME"){
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


/*
 * Function: alphaPressed
 *
 * Slot for alphaButton
 *
 * Sets TYPE and FREQUENCY based on preset values
 * as shown in therapyName and hz variables
 *
 */
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
}

/*
 * Function: betaPressed
 *
 * Slot for betaButton
 *
 * Sets TYPE and FREQUENCY based on preset values
 * as shown in therapyName and hz variables
 *
 */
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
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());


}
/*
 * Function: deltaPressed
 *
 * Slot for deltaButton
 *
 * Sets TYPE and FREQUENCY based on preset values
 * as shown in therapyName and hz variables
 *
 */
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
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());


}

/*
 * Function: thetaPressed
 *
 * Slot for thetaButton
 *
 * Sets TYPE and FREQUENCY based on preset values
 * as shown in therapyName and hz variables
 *
 */
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
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());

}

/*
 * Function: startSession
 *
 * Slot for okButton
 *
 * Start the session, return if invalid options as per if statements
 *
 */

void MainWindow::startSession(){
    //If battery is only at 2% then a session cant be run
    if(ui->BatteryBar->value()<=2){
        return;
    }
    //prevent from starting multiple sessions
    if(sessionStarted == true){
        return;
    }
    if(masterMenu->getName() != "SESSION INFO"){
        return;
    }else if(therapyName == "N/A" || sessionTime == "N/A" || hz == "N/A"){
     return;
    }else if(ui->connectionBox->currentText() == "False"){
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
/*
 * Function: updateMenu
 * Update menu screens on the device
 * Params: selectedMenuItem used for the menu name based on whats selected
 *         menuItems used to load the new menu items on the menu screen for the device based
 *                   on its name
 */
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems){
    currentMenu->clear();
    currentMenu->addItems(menuItems);
    currentMenu->setCurrentRow(0);
}
/*
 * Function: initTimer
 * Initialize timer used for timing a session
 * Params: *timer  used to initialize timer of
 * QTimer class
 */
void MainWindow::initTimer(QTimer* timer){
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer->start(1000);
}

/*
 * Function: updateTimer
 * Update timer based on timer->start value in seconds
 *
 *
 */
void MainWindow::updateTimer(){
    // if device is not connected pause or do not allow timer to run
    if(ui->connectionBox->currentText() == "False"){
        return;
    }
    //continue to run and drain battery if battery level is above 5
    if(ui->BatteryBar->value() >= 5){
    drainBattery();
    }
    currentSession->set_time(currentSession->get_length() - 1);

    int timeLeft = currentSession->get_length();// get the time left

    //end session if time is up or battery level is too low
    if(timeLeft < 0 || ui->BatteryBar->value() <= 5){

    // if record option selected save to db
    //stop timer and disconnect
    // reset values to default/hidden
    if(currentSession->get_record()){
        db->addRecord(currentUser,currentSession->get_name(),currentSession->get_timestring(),currentSession->get_frequency(),ui->progressBar->value());
    }
    currentSession->get_duration()->stop();
    currentSession->get_duration()->disconnect();
    currentSession = nullptr;
    therapyName = "N/A";
    sessionTime = "N/A";
    hz = "N/A";
    sessionStarted = false;
    ui->frequencyLabel->setHidden(true);
    ui->sessionLabel->setHidden(true);
    ui->timeLabel->setHidden(true);
    ui->recordLabel->setHidden(true);

    masterMenu = masterMenu->getParent();
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
    applyToSkin(false);
    }

    ui->timeLabel->setText(QString::number(timeLeft/60) + ((timeLeft%60 < 10) ? + ":0" + QString::number(timeLeft%60) : + ":" + QString::number(timeLeft%60)));
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
}

/*
 * Function: changeBatteryLevel
 * Update timer based on timer->start value in seconds
 * Params: newLevel take new battery level from change
 *
 *
 */
void MainWindow::changeBatteryLevel(double newLevel) {

    if(newLevel <= 0.0 && powerStatus == true) {
            powerChange();
    }

    int batteryLevel = (int) newLevel;
    ui->batteryBox->setValue(batteryLevel);
    ui->BatteryBar->setValue(batteryLevel);
}

/*
 * Function: changePowerStatus
 * Used to toggle between on and off screens
 * Used with powerChange slot
 *
 */
void MainWindow::changePowerStatus(){

    ui->progressBar->setVisible(powerStatus);

    ui->BatteryBar->setVisible(powerStatus);

    //if power
    if(sessionStarted == true){
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
        //set background to black essentially turn it off
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

}

/*
 * Function: clearHistory
 * Clear history based on USER
 *
 */
void MainWindow::clearHistory(){
if(masterMenu->getName() == "HISTORY"){
    db->deleteRecords(currentUser);
    recordings.clear();
    userRecordings.clear();
    masterMenu = masterMenu->getParent();
    updateMenu(masterMenu->getName(),masterMenu->getMenuItems());
}
}


/*
 * Function: powerChange
 * slot used to handle powerButton pressed
 * toggles powerStatus which enables/disables features
 * calls changePowerStatus to handle showing device off
 *
 */
void MainWindow::powerChange() {
    powerStatus  = !powerStatus;
    changePowerStatus();

}

/*
 * Function: getRecordings
 * Params: currentUser user to retrieve recordings from
 *
 */
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

/*
 * Function: applyToSkin
 * Params: checked 1 if applied 0 if not
 *
 */
void MainWindow::applyToSkin(int checked) {

    ui->connectionBox->setCurrentIndex(checked ? 1 : 0);
    connection = checked;
}


/*
 * Function: changeBatteryHealth
 * slot used to change the batteryBar health as based on the batteryBox
 * or drained amount
 *
 */
void MainWindow::changeBatteryHealth() {
int batteryLevel = ui->batteryBox->value();
if(batteryLevel <= 0 && powerStatus == true) {
        powerChange();
}
if(!sessionStarted){
tempLevel = (double) batteryLevel;
}
ui->BatteryBar->setValue(batteryLevel);

}

/*
 * Function: drainBattery
 * Used to drain battery based on drainFactor and progressBar value
 *
 */
void MainWindow::drainBattery() {

    //Show that battery level is draining since it will be slower showing for proof
    qDebug()<<"BATTERY LEVEL";
    qDebug()<<tempLevel;

    double drainValue = tempLevel - ( (ui->progressBar->value()+1) * drainFactor);
    tempLevel = drainValue;


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
