
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

void DatabaseManager::clearNotes(){

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM notes");
    removeQuery.exec();

}

void DatabaseManager::clearRelations(){

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM relations");
    removeQuery.exec();

    removeQuery.prepare("DELETE FROM couples");
    removeQuery.exec();

}


void DatabaseManager::insert(NOTES::Tache * n){
      QSqlQuery s;
      s.prepare("INSERT INTO notes(id, titre, type, texte, echeance, priorite, statut) VALUES(:id, :titre, :type, :text, :echeance, :priorite, :statut)");
      s.bindValue(":id", n->getId());
      s.bindValue(":titre", n->getTitle());
      s.bindValue(":type", "Tache");
      s.bindValue(":text", n->getAction());
      s.bindValue(":echeance", n->getEcheance());
      s.bindValue(":priorite", n->getPriorite());
      s.bindValue(":statut", n->getStatut());

      if(s.exec()) qDebug() << "insertion tache";
      else qDebug() << s.lastError();
}

void DatabaseManager::insert(NOTES::Media * n){
  QSqlQuery s;
  s.prepare("INSERT INTO notes(id, titre, type, texte, media, fichier) VALUES(:id, :titre, :type, :text, :med, :fichier)");
  s.bindValue(":id", n->getId());
  s.bindValue(":titre", n->getTitle());
  s.bindValue(":type", "Media");
  s.bindValue(":text", n->getTexte());
  s.bindValue(":media", n->getType());
  s.bindValue(":fichier", n->getFichier());
  if(s.exec()) qDebug() << "insertion media";
  else qDebug() << s.lastError();
}

void DatabaseManager::insert(NOTES::Article * n){
      QSqlQuery s;
      s.prepare("INSERT INTO notes(id, titre, type, texte) VALUES(:id, :titre, :type, :text)");
      s.bindValue(":id", n->getId());
      s.bindValue(":titre", n->getTitle());
      s.bindValue(":type", "Article");
      s.bindValue(":text", n->getTexte());

      if(s.exec()) qDebug() << "insertion article";
      else qDebug() << s.lastError();

}



void DatabaseManager::insert(NOTES::Note * n){
      if(n->getClass() == "Article"){
           return insert((NOTES::Article*) n);
      } else if(n->getClass() == "Tache"){
          return insert((NOTES::Tache*) n);
     }if(n->getClass() == "Media"){
          return insert((NOTES::Media*) n);

     }

      QSqlQuery s;
      s.prepare("INSERT INTO notes(id, titre, type) VALUES(:id, :titre, :type)");
      s.bindValue(":id", n->getId());
      s.bindValue(":titre", n->getTitle());
      s.bindValue(":type", QString("Note"));


     if(s.exec()) qDebug() << "insertion note";
     else qDebug() << s.lastError();
}



void DatabaseManager::save(const vector<NOTES::Relation*> relations){

   clearRelations();
    for(NOTES::Relation* r : relations){
        QSqlQuery s;
        s.prepare("INSERT INTO relations(titre, description, oriente) VALUES(:titre, :desc, :o)");
        s.bindValue(":titre", r->getTitre());
        s.bindValue(":desc", r->getDescription());
        int o = r->getOrientation() ? 1 : 0;
        s.bindValue(":o", o);
        if(s.exec()) qDebug() << "insertion relation";

        for(NOTES::Relation::Couple c : r->getCouples()){

            s.prepare("INSERT INTO couples(relation, id1, id2, label) VALUES(:rel, :mb1, :mb2, :la)");
            s.bindValue(":rel", r->getTitre());
            s.bindValue(":mb1", c.getMb1().getId());
            s.bindValue(":mb2", c.getMb2().getId());
            s.bindValue(":la", c.getLabel());
            if(s.exec()) qDebug() << "insertion couple";
            else qDebug() << s.lastError();

        }

    }

}

void DatabaseManager::save(const vector<NOTES::Note *> notes){

   clearNotes();
    for(NOTES::Note* i : notes){
        insert(i);
    }

}

void DatabaseManager::load(NOTES::NotesManager* m){
    QSqlQuery query;

    // CHARGER NOTES
    query.prepare("SELECT id, titre, type, texte, priorite, echeance, fichier, media, statut FROM notes");

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
            } else if(ty == "Tache"){
                m->addNote(new NOTES::Tache(i, ti, query.value(3).toString(), query.value(4).toInt(), query.value(5).toDateTime(), query.value(8).toInt()));
            } else if(ty == "Media"){
                 m->addNote(new NOTES::Media(i, ti, query.value(7).toString(), query.value(3).toString(), query.value(6).toString()));
            } else if(ty == "Note"){
                 m->addNote(new NOTES::Note(i, ti));
            }

       }
    }

    // CHARGER RELATIONS
    query.prepare("SELECT titre, description, oriente FROM relations");

       qDebug() << "Je me loade";
    if (query.exec())
    {
       while (query.next())
       {
          // it exists
           qDebug() << "Une relation trouvee !!";
            QString titre = query.value(0).toString();
            QString de = query.value(1).toString();
            QString orie = query.value(2).toString();
            bool o = orie == "1" ? true : false;
            m->addRelation(new NOTES::Relation(titre, de, o));

       }
    }

    //CHARGER COUPLES
    query.prepare("SELECT relation, id1, id2, label FROM couples");

       qDebug() << "Je me loade";
    if (query.exec())
    {
       while (query.next())
       {
          // it exists
           qDebug() << "Un couple trouve !!";
            QString relation = query.value(0).toString();
            QString id1 = query.value(1).toString();
            QString id2 = query.value(2).toString();
            QString label = query.value(3).toString();

            m->getRelation(relation)->ajouterCouple(m->getNotePtr(id1), m->getNotePtr(id2), label);
       }
    }



}

bool DatabaseManager::createTable()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE notes(id TEXT PRIMARY KEY, titre TEXT, type TEXT, texte TEXT, priorite INTEGER, echeance DATE, fichier TEXT, media TEXT, statut INTEGER);");

    query.exec();

    query.prepare("CREATE TABLE relations(titre TEXT PRIMARY KEY, description TEXT, oriente INTEGER);");

    query.exec();

    query.prepare("CREATE TABLE couples(relation TEXT, id1 TEXT, id2 TEXT, label TEXT);");

    query.exec();



    return true;
}

