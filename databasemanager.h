#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include "main.h"

class DatabaseManager
{

public:
    DatabaseManager(const QString& path);
    void load(NOTES::NotesManager *m);
    void save(vector<NOTES::Note*> notes);
    void save(vector<NOTES::Relation*>);


    bool isOpen() const
    {
        return m_db.isOpen();
    }
   // void load();
private:
    QSqlDatabase m_db;
    bool createTable();

    void clearNotes();
    void clearRelations();
    void clearEtat();
    void insert(NOTES::Tache *);
    void insert(NOTES::Media *);
    void insert(NOTES::Article *);
    void insert(NOTES::Note *);

};
#endif // DATABASEMANAGER_H
