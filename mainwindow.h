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
    //Menu* sessionMenu;
    void initMenu(Menu *);
    void updateMenu(const QString selectedMenuItem, const QStringList menuItems);
    QListWidget *currentMenu;
    void initTimer(QTimer*);
private slots:
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void alphaPressed();
    void betaPressed();
    void thetaPressed();
    void deltaPressed();
};
#endif // MAINWINDOW_H
