#ifndef MAIN_H
#define MAIN_H
#include <QString>
#include <iostream>
#include <vector>
#include <QDebug>
#include <QSqlQuery>
#include <QDateTime>
using namespace std;


enum Mediatype { image, son, video };

namespace Ui {
class MainWindow;
class Ajout;
}

class MainWindow;
class Ajout;

namespace NOTES {
    class NotesException;
    class NotesManager;

    class Note;
    class Article;
    class Media;
    class Tache;
    class Relation;

   class MementoNote;
   class MementoMedia;
   class MementoArticle;
   class MementoTache;

}
#endif // MAIN_H
