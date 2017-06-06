#ifndef MAIN_H
#define MAIN_H
#include <QString>
#include <iostream>
#include <vector>

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
}

#endif // MAIN_H
