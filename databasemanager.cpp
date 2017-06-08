#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include "notesmanager.h"

DatabaseManager::DatabaseManager(const QString& path)
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(path);

   if (!m_db.open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

void DatabaseManager::clear(){
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM notes");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all notes failed: " << removeQuery.lastError();
    }


}


void DatabaseManager::insert(NOTES::Tache * n){
      QSqlQuery s;
      s.prepare("INSERT INTO notes(id, titre, type, texte) VALUES(:id, :titre, :type, :text)");
      s.bindValue(":id", n->getId());
      s.bindValue(":titre", n->getTitle());
      s.bindValue(":type", "Tache");
      s.bindValue(":text", n->getAction());

      if(s.exec()) qDebug() << "sa marche";
      else qDebug() << s.lastError();
}

void DatabaseManager::insert(NOTES::Media * n){
  QSqlQuery s;
  s.prepare("INSERT INTO notes(id, titre, type, texte, media) VALUES(:id, :titre, :type, :text, :med)");
  s.bindValue(":id", n->getId());
  s.bindValue(":titre", n->getTitle());
  s.bindValue(":type", "Media");
  s.bindValue(":text", n->getTexte());
  s.bindValue(":media", n->getType());
  if(s.exec()) qDebug() << "sa marche";
  else qDebug() << s.lastError();
}

void DatabaseManager::insert(NOTES::Article * n){
      QSqlQuery s;
      s.prepare("INSERT INTO notes(id, titre, type, texte) VALUES(:id, :titre, :type, :text)");
      s.bindValue(":id", n->getId());
      s.bindValue(":titre", n->getTitle());
      s.bindValue(":type", "Article");
      s.bindValue(":text", n->getTexte());

      if(s.exec()) qDebug() << "sa marche";
      else qDebug() << s.lastError();

}

void DatabaseManager::insert(NOTES::Note * n){
      QSqlQuery s;
      s.prepare("INSERT INTO notes(id, titre, type) VALUES(:id, :titre, :type)");
      s.bindValue(":id", n->getId());
      s.bindValue(":titre", n->getTitle());
      s.bindValue(":type", QString("Note"));


     if(s.exec()) qDebug() << "sa marche";
     else qDebug() << s.lastError();
}



void DatabaseManager::save(const vector<NOTES::Note *> notes){

   clear();
    for(NOTES::Note* i : notes){

        insert(i);



    }

}

void DatabaseManager::load(NOTES::NotesManager* m){
    QSqlQuery query;
    query.prepare("SELECT id, titre, type, texte, priorite, echeance, fichier, media FROM notes");

       qDebug() << "Je me loade";
    if (query.exec())
    {
       while (query.next())
       {
          // it exists
           qDebug() << "Une note trouvee !!";
            QString i = query.value(0).toString();
            QString ti = query.value(1).toString();
            QString ty = query.value(2).toString();
            if(ty == "Article"){
                m->addNote(new NOTES::Article(i, ti, query.value(3).toString()));
            } else if(ty == "Note"){
                 m->addNote(new NOTES::Note(i, ti));
            }

       }
    }

}

bool DatabaseManager::createTable()
{
    bool success = true;
    QSqlQuery query;
    query.prepare("CREATE TABLE notes(id TEXT PRIMARY KEY, titre TEXT, type TEXT, texte TEXT, priorite INTEGER, echeance DATE, fichier TEXT, media TEXT);");

    if (!query.exec())
    {
        qDebug() << "Couldn't create the table 'notes': one might already exist.";
        success = false;
    }

    return success;
}

