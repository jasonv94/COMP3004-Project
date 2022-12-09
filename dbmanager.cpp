#include "dbmanager.h"



const QString DBManager::DATABASE_PATH = "/database/oasis.db";


/*
 * Create a new DBManager to a existing denas database, defined by DBManager::DATABASE_PATH
 *
 * Throws:
 *  If the database could not be opened
 */
DBManager::DBManager() {

    oasisDB = QSqlDatabase::addDatabase("QSQLITE");
    oasisDB.setDatabaseName("oasis.db");

    if (!oasisDB.open()) {
        throw "Error: Database could not be opened";
    }

    if (!DBInit()) {
        throw "Error: Database could not be initialized";
    }
}


bool DBManager::DBInit() {

    oasisDB.transaction();

    QSqlQuery query;
    //queries to reset records taable
    //query.exec("DELETE FROM RECORDS;");
    //query.exec("TRUNCATE TABLE RECORDS;");
    query.exec("CREATE TABLE IF NOT EXISTS profiles ( pid INTEGER NOT NULL UNIQUE PRIMARY KEY, username TEXT NOT NULL);");
    query.exec("CREATE TABLE IF NOT EXISTS records (rid INTEGER NOT NULL UNIQUE PRIMARY KEY AUTOINCREMENT,pid INTEGER NOT NULL,date TEXT NOT NULL,therapyname TEXT NOT NULL, sessiontime TEXT NOT NULL,frequency TEXT NOT NULL,intensity INTEGER NOT NULL);");
    // initialize device with sample entry
    //query.exec("INSERT OR REPLACE INTO records (pid,date,therapyname,sessiontime,frequency,intensity) VALUES (1,'2022-01-01','Alpha','45','12-15hz',1);");
    return oasisDB.commit();
}


History * DBManager::getRecord(int pid,int rid){
    History *record;
    int record_id;//this is session id
    QString date;
    QString therapyName;
    QString sessionTime;
    QString frequency;
    int intensity;
    oasisDB.transaction();
    oasisDB.transaction();
    //const QDateTime& time;
    QSqlQuery query;
    //query.exec

    query.prepare("SELECT * from records where :pid = pid and rid = :rid LIMIT 1;");
    query.bindValue(":pid", pid);
    query.bindValue(":rid",rid);
    query.exec();
    oasisDB.commit();
    qDebug()<<"Get record function";
    while(query.next()){
        record_id = query.value(0).toInt();//this is session id
        date = query.value(2).toString();
        therapyName = query.value(3).toString();
        sessionTime = query.value(4).toString();
        frequency = query.value(5).toString();
        intensity = query.value(6).toInt();

    }
    record = new History(date, therapyName, record_id,sessionTime, frequency,intensity);
    return record;





}


QVector<History*> DBManager::getRecordings(int pid) {
    //will have to add for other user next
    QSqlQuery query;
    QVector<History*> qvr;
    oasisDB.transaction();
    //remeber to add filter with id so look at preparing this properly
    query.prepare("SELECT * from records where :pid = pid;");
    query.bindValue(":pid", pid);
    query.exec();
    //query.exec();

    while (query.next()) {
        int record_id = query.value(0).toInt();//this is session id
        QString date = query.value(2).toString();
        QString therapyName = query.value(3).toString();
        QString sessionTime = query.value(4).toString();
        QString frequency = query.value(5).toString();
        int intensity = query.value(6).toInt();
        History *r = new History(date, therapyName, record_id,sessionTime, frequency,intensity);
        qvr.push_back(r);
    }
    return qvr;
}

bool DBManager::addRecord(int pid,QString therapyName,QString sessionTime,QString frequency, int intensity) {
    //add proper date
    oasisDB.transaction();
    //const QDateTime& time;
    QSqlQuery query;
    //query.exec("INSERT OR REPLACE INTO records (pid,date,therapyname,sessiontime,frequency,intensity) VALUES (1,'2022-01-02','Beta','25 mins','20hz',1);");
    qDebug()<<pid;
    qDebug()<<therapyName;
    qDebug()<<"EXPECTED TIME";
    qDebug()<<sessionTime;
    qDebug()<<"EXPECTED FREQ";
    qDebug()<<frequency;
    qDebug()<<intensity;
    query.prepare("INSERT INTO records (pid,date,therapyname,sessiontime,frequency,intensity) VALUES (:pid,:date,:therapyname,:sessiontime,:frequency,:intensity);");
    query.bindValue(":pid", pid);
    query.bindValue(":date", "2022-01-01");//add proper date
    query.bindValue(":therapyname", therapyName);
    query.bindValue(":sessiontime", sessionTime);
    query.bindValue(":frequency", frequency);
    query.bindValue(":intensity", intensity);
    query.exec();
    /*
    int rowid = query.lastInsertId().toInt();
    query.prepare("INSERT INTO " + tableName + "_records VALUES (:name, :id);");
    query.bindValue(":name", name);
    query.bindValue(":id", rowid);
    query.exec();
    */

    return oasisDB.commit();
}


/*
 * Type: Public
 * Adds a therapy record to the database, if the arguments are valid.
 *
 * Parameters:
 *  therapy - the name of the therapy to add to the records, must be capitalized and in the database already
 *  time - the time the therapy started in QDateTime format
 *  powerLevel - the maximum power level observed during a therapy
 *  duration - the amount of time in seconds that the therapy was in use
 *
 * Returns:
 *  True - If the record was successfully added to the database
 *  False - If the arguments were invalid, or if the record couldn't be added to the database
 */
/*
bool DBManager::addTherapyRecord(const QString& therapy, const QDateTime& time, int powerLevel, int duration) {

    if (!isValidRecord("therapy", time, powerLevel, duration)) {
        return false;
    }
    return addRecord("therapy", therapy, time, powerLevel, duration);
}

*/
/*
 * Type: Public
 * Adds a frequency record to the database, if the arguments are valid.
 *
 * Parameters:
 *  frequency - the name of the frequency to add to the records, must be in the database already
 *  time - the time the frequency treatment started in QDateTime format
 *  powerLevel - the maximum power level observed during a frequency treatment
 *  duration - the amount of time in seconds that the frequency treatment was in use
 *
 * Returns:
 *  True - If the record was successfully added to the database
 *  False - If the arguments were invalid, or if the record couldn't be added to the database
 */
/*
bool DBManager::addFrequencyRecord(const QString& frequency, const QDateTime& time, int powerLevel, int duration) {
    if (!isValidRecord("frequency", time, powerLevel, duration)) {
        return false;
    }
    return addRecord("frequency", frequency, time, powerLevel, duration);
}
*/

/*
 * Type: Public
 * Deletes all records from the database.
 *
 * Returns:
 *  True - If the records were successfully removed from the database
 *  False - If the records couldn't be deleted from the database
 */

bool DBManager::deleteRecords(int pid) {
    oasisDB.transaction();
    QSqlQuery query;
    query.prepare("DELETE FROM records where :pid = pid;");
    query.bindValue(":pid",pid);
    query.exec();
    query.prepare("DELETE FROM users;");
    query.exec();
    query.prepare("TRUNCATE TABLE records;");
    query.exec();
    //maybe change back to default
    return oasisDB.commit();
}
