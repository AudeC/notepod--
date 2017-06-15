#if !defined(_NOTES_H)
#define _NOTES_H

#include "main.h"
#include "databasemanager.h"
#include <map>

namespace NOTES {
    class NotesException {
    public:
        NotesException(const string& message) :info(message) {}
        string getInfo() const { return info; }
    private:
        string info;
    };



    class MementoNote {
      QString title;
      QDateTime modification;

      public:
        QString getTitle() const { return title; }
        QDateTime getModification() const  { return modification; }
        MementoNote(const Note& n);
        MementoNote(const Note* n);
    };


    class Note {
        QString id;
        QString title;
        QDateTime creation;
        QDateTime modification;
        bool actif;
        friend class NotesManager;
        friend class MainWindow;
    public:
        Note(const QString& i, const QString& ti) : id(i), title(ti), creation(QDateTime::currentDateTime()), modification(QDateTime::currentDateTime()) {}
        QString getId() const { return id; }
        QString getTitle() const { return title; }
        QDateTime getCreation() const { return creation; }
        QDateTime getModification() const { return modification; }
        void setCreation(const QDateTime& c){ creation = c; }
        void setModification(const QDateTime& c){ modification = c; }
        void modifier(){ modification = QDateTime::currentDateTime();  }
        void setTitle(const QString& t);
        void archiver();
        virtual QString getClass() const { return "Note"; }
        virtual void visualiser(Ui::MainWindow * ui);
        virtual void sauvegarder(Ui::MainWindow * ui);
        virtual MementoNote* creerMemento() const;
        virtual void SetMemento(MementoNote *m);
    };

    class MementoArticle : public MementoNote {
        QString texte;
    public:
        QString getTexte() const { return texte; }
        MementoArticle(const Article& n);
        MementoArticle(const Article* n);

    };


    class Article : public Note
    {
        friend class NotesManager;
        friend class MainWindow;
        QString texte;


    public:
         Article(const QString& i, const QString& ti, const QString& te = "") : Note(i, ti), texte(te) {}
        QString getTexte() const { return texte; }
        void setTexte(const QString& s) { texte = s; modifier(); }
        QString getClass() const { return "Article"; }
        virtual void visualiser(Ui::MainWindow * ui);
        virtual void sauvegarder(Ui::MainWindow * ui);
        virtual MementoArticle* creerMemento() const;
        virtual void SetMemento(MementoNote *m);

    };

    class MementoMedia : public MementoNote {
        QString texte;
        enum Mediatype type;
        QString fichier;
    public:
        QString getTexte() const { return texte; }
        enum Mediatype getType() const { return type; }
        QString getFichier() const { return fichier; }
        MementoMedia(const Media& n);
         MementoMedia(const Media* n);
    };

    class Media : public Note
    {

        QString texte;
        enum Mediatype type;
        QString fichier;

        friend class NotesManager;
        friend class MainWindow;
    public:
        QString getTexte() const { return texte; }
        enum Mediatype getType() const { return type; }
        QString getFichier() const { return fichier; }
        void setTexte(const QString& t) { texte = t; modifier(); }
        void setType(enum Mediatype m) { type = m; modifier();  }
        void setFichier(const QString& f) { fichier = f; modifier(); }
        QString getClass() const { return "Media"; }
        Media(const QString& i, const QString& ti, enum Mediatype m = image, const QString& te = "", const QString& fi = "") : Note(i, ti), texte(te), type(m), fichier(fi) {}
        Media(const QString& i, const QString& ti, const QString& m, const QString& te, const QString& fi);
        virtual  void visualiser(Ui::MainWindow * ui);
        virtual void sauvegarder(Ui::MainWindow * ui);
         virtual MementoMedia* creerMemento() const;
        virtual void SetMemento(MementoNote* m);
    };


    class MementoTache : public MementoNote {
        QString action;
        int priorite;
        QDateTime echeance;
        int statut;
    public:
        QString getAction() const { return action; }
        int getPriorite() const{ return priorite; }
        QDateTime getEcheance() const { return echeance; }
        int getStatut() const {return statut;}
        MementoTache(const Tache& n);
        MementoTache(const Tache* n);
    };

    class Tache: public Note
    {

        QString action;
        int priorite;
        QDateTime echeance;
        int statut;

        friend class NotesManager;
        friend class MainWindow;
    public:
        Tache(const QString& i, const QString& ti, const QString& a = "", int p = 0, QDateTime e = QDateTime(), int s = 1):
        Note(i ,ti), action(a), priorite(p), echeance(e), statut(s){}
        QString getAction() const{ return action; }
        int getPriorite() const{ return priorite; }
        QDateTime getEcheance() const { return echeance; }
        int getStatut() const {return statut;}
        void setAction(const QString& s) { action = s; modifier();  }
        void setPriorite(int i) { priorite = i;  modifier(); }
        void setEcheance(QDateTime d) { echeance = d;    modifier();}
        void setStatut(int s) { statut=s; modifier();}
        QString getClass() const { return "Tache"; }
        virtual  void visualiser(Ui::MainWindow * ui);
        virtual void sauvegarder(Ui::MainWindow * ui);
        virtual MementoTache* creerMemento() const;
        virtual void SetMemento(MementoNote* m);
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
                return m == mb1 || m == mb2;
            }
        };

    private:

        QString titre;
        QString description;
        bool oriente;
        vector<Couple> couples;
    public:
        Relation(const QString& t, const QString& d, bool o = false) : titre(t), description(d), oriente(o) {}

        QString getTitre() const { return titre; }
        QString getDescription() const { return description; }
        vector<Couple> getCouples() const { return couples; }
        bool getOrientation()const { return oriente; }

        void setTitre(const QString& t) { titre = t; }
        void setDescription(const QString& t) { description = t; }
        void setOrientation(bool b) { oriente = b; }

        void ajouterCouple(Note* m1, Note* m2, const QString& la = "")
        {
            for(Couple c : couples)
            {
                if (c.contient(m1) && c.contient(m2)) throw NotesException("Couple d�j� existant");
            }
            couples.push_back(Couple(m1, m2, la));
        }


        void enleverCouple(Note* m1, Note* m2)
        {
            for(vector<Couple>::const_iterator i = couples.cbegin(); i != couples.cend(); i++)
            {
                if ((*i).contient(m1) && (*i).contient(m2)){
                    couples.erase(i);
                }
            }
        }

        void enleverCouple(Note* m1)
        {
            for(vector<Couple>::const_iterator i = couples.cbegin(); i != couples.cend(); i++)
            {
                if ((*i).contient(m1)){
                    couples.erase(i);
                }
            }
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

    protected:
        vector<Note*> notes;
        vector<Relation*> relations;
        vector<Note*> corbeille;
        map<QString, vector<MementoNote*>> historique; // tableau associatif de vecteurs de Memento

        QString filename;
        DatabaseManager db;



        static Handler handler;


        //NotesManager& operator=(const NotesManager& m);
        //NotesManager(): filename("notes.json"){}
        //NotesManager(const NotesManager& m);

        friend struct Handler;
    public:
        void addNote(Note* a);
        void addRelation(Relation* r);

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
        Note* getNotePtr(const QString& id){ return &getNote(id); }
        //Article& getNewArticle(const QString& id);
        //Tache& getNewTache(const QString& id);
        //Media& getNewMedia(const QString& id, enum Mediatype m = image);
        Relation& getNewRelation(const QString& n, const QString& d = "", bool o = false);
        Relation* getRelation(const QString& n);
        vector<Note*> getNotes(){ return notes; }
        void viderCorbeille() { corbeille.clear(); }

        void load();
        void save();
        virtual ~NotesManager()
        {
            db.save(notes);
            db.save(relations);
            notes.clear();
            relations.clear();
            corbeille.clear();
            filename = "";

        }


    };



}

ostream& operator<<(ostream& f, const NOTES::Note& a);
ostream& operator<<(ostream& f, const NOTES::Relation& a);
ostream& operator<<(ostream& f, const NOTES::Relation::Couple& a);
#endif
