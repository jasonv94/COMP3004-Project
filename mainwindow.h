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
#include "user.h"
#include "therapy.h"

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
    Menu* masterMenu;
    Menu* mainMenu;
    History *recordedSession;
    Menu* sessionMenu;
    QString therapyName;
    QString sessionTime;
    QString hz;
    bool sessionStarted = false;
    Therapy* currentSession;
    User* CurrentUser;
    void initMenu(Menu *);
    DBManager* db;
    void updateMenu(const QString selectedMenuItem, const QStringList menuItems);
    QListWidget *currentMenu;
    QVector<History*> recordings;
    QStringList userRecordings;
    int time;
    void initTimer(QTimer*);
    void updateTimer();
private slots:
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void alphaPressed();
    void betaPressed();
    void thetaPressed();
    void deltaPressed();
    void startSession();
    void clear();
};
#endif // MAINWINDOW_H
