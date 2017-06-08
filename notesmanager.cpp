
#include "notesmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
string qtostd(QString o){
    return o.toLocal8Bit().constData();
                        }


using namespace TIME;

namespace NOTES {
    void Note::setTitle(const QString& t) {
        title = t;
    }

    void Note::archiver()
    {
        actif = false;
    }

    void Note::visualiser(Ui::MainWindow * ui){
        ui->editTitre->setText(title);
    }

    void Article::visualiser(Ui::MainWindow * ui){
        ui->editTitre->setText("getTitle()");
        ui->editTexte->setText(texte);
         ui->visuCont->show();
          ui->editTexte->show();
    }

    void Media::visualiser(Ui::MainWindow * ui){
        ui->editTitre->setText(getTitle());
    }

    void Tache::visualiser(Ui::MainWindow * ui){
        ui->editTitre->setText(getTitle());
        ui->editAction->show();
        ui->editAction->setText(action);
        ui->editPrio->show();
        ui->visuPrio->show();
    }

    //NotesManager* NotesManager::instanceUnique= nullptr;
    NotesManager::Handler NotesManager::handler = NotesManager::Handler();

    NotesManager& NotesManager::getInstance() {
        if (NotesManager::handler.instance == nullptr)
            NotesManager::handler.instance = new NotesManager;
        return *NotesManager::handler.instance;
    }

    void NotesManager::freeInstance() {
        delete NotesManager::handler.instance;
        NotesManager::handler.instance = nullptr;

    }



    void NotesManager::addNote(Note* a) {

        for (Note* i : notes) {
            if (i->getId() == a->getId()) throw NotesException("error, creation of an already existent note");
        }
        notes.push_back(a);
        std::cout<< "Nouvelle note insérée avec succès";

    }

    void NotesManager::addRelation(Relation* r)
    {
        relations.push_back(*r);
    }

    Note& NotesManager::getNote(const QString& id) {
        int j = 0;
        for (Note* i : notes) {
            if (i->getId() == id) {
                std::cout<< qtostd(notes[j]->getClass());
                return *notes[j];
            }
            j++;
        }
        throw NotesException("error, nonexistent note");

    }

    Relation& NotesManager::getNewRelation(const QString& n, const QString& d, bool o)
    {
        Relation *r = new Relation(n, d, o);
        addRelation(r);
        return *r;
    }

    Relation& NotesManager::getRelation(const QString& n)
    {
        int j = 0;
        for(Relation i: relations)
        {
            if (i.getTitre() == n) return relations[j];
            j++;
        }
        throw new NotesException("La relation n'existe pas");
    }
    Note& NotesManager::getNewNote(const QString& id) {
        Note* a = new Note(id, "");
        addNote(a);
        return *a;
    }
    Article& NotesManager::getNewArticle(const QString& id) {
        Article* a = new Article(id, "", "");
        addNote(a);
        return *a;
    }
    Tache& NotesManager::getNewTache(const QString& id) {
        Tache* a = new Tache(id, "", "");
        addNote(a);
        return *a;
    }
    Media& NotesManager::getNewMedia(const QString& id, enum Mediatype m) {
        Media* a = new Media(id, "", m, "");
        addNote(a);
        return *a;
    }

    QTextStream& operator<<(QTextStream& flot, const TIME::Date& date) {
        flot << date.getAnnee() << "-" << date.getMois() << "-" << date.getJour();
        return flot;
    }




    void NotesManager::save() {
        db.save(notes);
    }

 void NotesManager::load(){
    db.load(this);

}

ostream& operator<<(ostream& f, const NOTES::Note& a) {
    f << a.getId().toLocal8Bit().constData() << "\n";
    f << a.getTitle().toLocal8Bit().constData() << "\n";
    return f;
}

ostream& operator<<(ostream& f, const NOTES::Relation::Couple& a) {
    f << "(" << a.getMb1() << ",";
    f << a.getMb2() << ")";
    return f;
}
ostream& operator<<(ostream& f, const NOTES::Relation& a) {
    f << a.getTitre().toLocal8Bit().constData() << " : relation impliquant " << a.getCouples().size() << " couples\n";
    for(NOTES::Relation::Couple c : a.getCouples() )
    {
        f << c << " / ";
    }
    f << "\n";
    return f;
}

}


