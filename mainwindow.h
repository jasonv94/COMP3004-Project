#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSpinBox>
#include <QListWidget>
#include <QGraphicsView>
#include <QTime>
#include <QTimer>
#include <QStatusBar>
#include <QVector>
#include <QtGlobal>
#include <QMainWindow>
#include <QDebug>
#include "menu.h"
#include "history.h"
#include "dbmanager.h"
#include "therapy.h"
/*Class MainWindow
 * Purpose:
 * create and handle operations for the ui
 * Some functions were used and altered from previous COMP3004 Project denas Team 3
 *
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Menu* masterMenu; // Variable holds the current menu
    Menu* mainMenu; // Instantiate the main menu on the GUI
    History *recordedSession; // Used to retrieve a recorded session
    Menu* sessionMenu; // Set GUI to screen used for running a session
    QString therapyName;// Used to display therapy name on GUI
    QString sessionTime;// Used to display sessionTime on GUI
    QString hz;// Hold the frequency of the session and display on GUI
    int connection; //Represents whether device is connected to User
    bool sessionStarted; // If session is started or not
    bool powerStatus;// If device power is on
    bool therapySelected;// Check to ensure therapy selected before starting session
    Therapy* currentSession; // Used to hold variables of type therapy and run session
    void initMenu(Menu *);
    DBManager* db;// Database object to handle databse operations
    void updateMenu(const QString selectedMenuItem, const QStringList menuItems);//Update menu based on screen
    QListWidget *currentMenu;// Display all menu items
    QVector<History*> recordings;// Holds recordings from the db
    QStringList userRecordings;// retrieves recordings from recordings variables to display on GUI
    int currentUser;// represent current User by int value
    void initTimer(QTimer*); // Required to initialize and start time
    void updateTimer(); // Update timer based on interval in this case seconds
    void getRecordings(int currentUser); // Get recordings based on the User
    void drainBattery(); // Used to drain battery over time
    int batteryHealth; // Represent battery value
    double drainFactor;// value to drain battery based on max time allowed and intensity level
    int count;
    double tempLevel;//hold battery level value in double form which is subtracted to simulate battery level draining
    void changeBatteryLevel(double batteryLevel); // change battery level
private slots:
    void navigateDownMenu(); //move down menu
    void navigateUpMenu(); // move up menu
    void navigateSubMenu(); // move between menus
    void alphaPressed(); // represents session of type alpha
    void betaPressed();// represents session of type beta
    void thetaPressed();// represents session of type theta
    void deltaPressed();// represents session of type delta
    void startSession();// start session once all requirements met (type,time)
    void clearHistory();// removes history of recordings based on user
    void powerChange(); // used to toggle device powerstatus between on/off
    void changePowerStatus();// handles whether
    void applyToSkin(int checked); // sets connection variable to true to simulate connected to User
    void changeBatteryHealth(); //Used for battery box to increase/decrease battery level


};
#endif // MAINWINDOW_H
