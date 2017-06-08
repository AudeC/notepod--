#if !defined(_NOTES_H)
#define _NOTES_H

#include "timing.h"
#include "main.h"
#include "databasemanager.h"

namespace NOTES {
    class NotesException {
    public:
        NotesException(const string& message) :info(message) {}
        string getInfo() const { return info; }
    private:
        string info;
    };

    class Note {
        QString id;
        QString title;
        TIME::Date creation;
        TIME::Date modification;
        bool actif;
        friend class NotesManager;
        friend class MainWindow;

    protected:
        //Note& operator=(const Note& a);
        //Note(const Note& a);
    public:
        Note(const QString& i, const QString& ti) : id(i), title(ti), creation(TIME::Date()), modification(TIME::Date()) {};

    public:

        QString getId() const { return id; }
        QString getTitle() const { return title; }
        TIME::Date getCreation() const { return creation; }
        TIME::Date getModification() const { return modification; }
        void setTitle(const QString& t);
        void archiver();
        virtual QString getClass() const { return "Note"; }
        virtual void visualiser(Ui::MainWindow * ui);

    };

    class Article : public Note
    {
        friend class NotesManager;
        friend class MainWindow;
        QString texte;


    public:
         Article(const QString& i, const QString& ti, const QString& te) : Note(i, ti), texte(te) {}
        QString getTexte() const { return texte; }
        void setTexte(const QString& s) { texte = s;  }
        QString getClass() const { return "Article"; }
          virtual void visualiser(Ui::MainWindow * ui);


    };
    class Media : public Note
    {

        QString texte;
        enum Mediatype type;

        friend class NotesManager;
        friend class MainWindow;
    public:
        QString getTexte() const { return texte; }
        enum Mediatype getType() const { return type; }
        void setTexte(const QString& t) { texte = t;  }
        void setType(enum Mediatype m) { type = m;  }
        QString getClass() const { return "Media"; }
        Media(const QString& i, const QString& ti, enum Mediatype m, const QString& te) : Note(i, ti), texte(te), type(m) {}
         virtual  void visualiser(Ui::MainWindow * ui);


    };
    class Tache: public Note
    {

        QString action;
        int priorite;
        TIME::Date echeance;

        friend class NotesManager;
        friend class MainWindow;
    public:
        Tache(const QString& i, const QString& ti, const QString& a, int p = 0, TIME::Date e = TIME::Date()):
            Note(i ,ti), action(a), priorite(p), echeance(e){}
        QString getAction() const{ return action; }
        int getPriorite() const{ return priorite; }
        TIME::Date getEcheance() const { return echeance; }
        void setAction(const QString& s) { action = s;  }
        void setPriorite(int i) { priorite = i; }
        void setEcheance(TIME::Date d) { echeance = d;  }
        QString getClass() const { return "Tache"; }
         virtual  void visualiser(Ui::MainWindow * ui);
    };



    class Relation
    {
    public:
        class Couple
        {
            Note* mb1;
            Note* mb2;
            QString label;
        public:
            Couple(Note* m1, Note* m2, const QString& la = "") : mb1(m1), mb2(m2), label(la) {}
            Couple(Note m1, Note m2, const QString& la = "") : mb1(&m1), mb2(&m2), label(la) {}
            QString getLabel() const { return label; }
            void setLabel(const QString& la) { label = la; }
            Note& getMb1() const { return *mb1;  }
            Note& getMb2() const { return *mb2;  }
            bool contient(Note* m) const
            {
                if (m == mb1 || m == mb2) return true;
                else return false;
            }
        };

    private:
        Relation(const QString& t, const QString& d, bool o = false) : titre(t), description(d), oriente(o) {}
        QString titre;
        QString description;
        bool oriente;
        vector<Couple> couples;
        friend class NotesManager;
    public:

        QString getTitre() const { return titre; }
        QString getDescription() const { return description; }
        vector<Couple> getCouples() const { return couples; }

        void ajouterCouple(Note* m1, Note* m2, const QString& la = "")
        {
            for(Couple c : couples)
            {
                if (c.contient(m1) && c.contient(m2)) throw NotesException("Couple d�j� existant");
            }
            couples.push_back(Couple(m1, m2, la));
        }

    };



    class NotesManager {
    public:
        // handler
       struct Handler {
            NotesManager* instance;
            Handler() :instance(nullptr) {}
            ~Handler() {
                delete instance;
                instance = nullptr;
            }

        };

    private:
        vector<Note*> notes;
        vector<Relation> relations;
        QString filename;
        DatabaseManager db;
        static Handler handler;

        //NotesManager& operator=(const NotesManager& m);
        //NotesManager(): filename("notes.json"){}
        //NotesManager(const NotesManager& m);

        friend struct Handler;
    public:
        void addNote(Note* a);
    private:
        void addRelation(Relation* r);

    public:
        static NotesManager& getInstance();
        // donne le seul NotesManager du programme
        static void freeInstance(); // le supprime

        static Handler getHandler()
        {
            return handler;
        }

        NotesManager(): db( DatabaseManager("notes.db")){

                if (db.isOpen())
                {
                    db.createTable();   // Creates a table if it doens't exist. Otherwise, it will use existing table.

                    qDebug() << "End";
                }
                else
                {
                    qDebug() << "Database is not open!";
            }

        }

        Note& getNewNote(const QString& id);
        Note& getNote(const QString& id);
        Article& getNewArticle(const QString& id);
        Tache& getNewTache(const QString& id);
        Media& getNewMedia(const QString& id, enum Mediatype m = image);
        Relation& getNewRelation(const QString& n, const QString& d = "", bool o = false);
        Relation& getRelation(const QString& n);
        vector<Note*> getNotes(){ return notes; }

        void load();
        void save();
        virtual ~NotesManager()
        {
            db.save(notes);
            notes.clear();
            relations.clear();
            filename = "";

        }

        /*
        class iterator {
            int ptr;
            int max;
            Note** notes;
            iterator(Note** a, int m, int c = 0) {
                notes = a;
                ptr = c;
                max = m;
            }
            friend class NotesManager;
        public:
            void operator++() {
                if (ptr < max) ptr++;
                else throw NotesException("iterateur fini");
            }
            Note& operator*() const {
                return *notes[ptr];
            }
            bool operator!=(const iterator a) const {
                return notes != a.notes || ptr != a.ptr;
            }

        };
        NotesManager::iterator begin() const { return NotesManager::iterator(notes, nbNotes); }
        NotesManager::iterator end() const { return NotesManager::iterator(notes, nbNotes, nbNotes); }

        class ConstIterator {
            int ptr;
            int max;
            Note** articles;
            friend class NotesManager;
            ConstIterator(Note** a, int m) {
                articles = a;
                ptr = 0;
                max = m;
            }
        public:
            int isDone() const {
                if (ptr == max)
                    return 1;
            }
            void next() {
                if (ptr < max) ptr++;
                else throw NotesException("iterateur fini");
            }
            const Note& current() const {
                return *articles[ptr];
            }
        };


        ConstIterator getIterator() const { return ConstIterator(notes, nbNotes); }
        ConstIterator getConstIterator() const { return ConstIterator(notes, nbNotes); }

        class SearchIterator {
            int ptr;
            int max;
            QString search;
            Note** notes;
            friend class NotesManager;
            SearchIterator(Note** a, int m, const QString& se = "") {
                notes = a;
                ptr = 0;
                max = m;
                search = se;
                do {
                    ptr++;
                } while (ptr < max && notes[ptr]->getTitle().find(search) == QString::npos);
            }
        public:
            bool isDone() const {
                return ptr == max;
            }
            void next() {
                if (ptr < max) {
                    do {
                        ptr++;
                    } while (ptr < max && notes[ptr]->getTitle().find(search) == QString::npos);
                }
                else throw NotesException("iterateur fini");
            }
            Note& current() const {
                return *notes[ptr];
            }
        };
        SearchIterator getSearchIterator(const QString& txt) const { return SearchIterator(notes, nbNotes, txt); }
        */

    };



}

ostream& operator<<(ostream& f, const NOTES::Note& a);
ostream& operator<<(ostream& f, const NOTES::Relation& a);
ostream& operator<<(ostream& f, const NOTES::Relation::Couple& a);
#endif
