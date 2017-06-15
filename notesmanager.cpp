
#include "notesmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
string qtostd(QString o){
    return o.toLocal8Bit().constData();
}



namespace NOTES {

     enum Mediatype toMedia(const QString& q){
        if(q.toLower() == "image") return image;
        if(q.toLower() == "son") return son;
        return video;
     }

    /*
     * Note
     */
    void Note::setTitle(const QString& t) {title = t;}

    void Note::archiver(){actif = false; }

    void Note::visualiser(Ui::MainWindow * ui){ui->editTitre->setText(title);}

    void Note::SetMemento(MementoNote* m)
    {
        title = m->getTitle();
        modification = m->getModification();

    }

    MementoNote* Note::creerMemento() const { return new MementoNote(this); }
    MementoNote::MementoNote(const Note& n): title(n.getTitle()), modification(n.getModification()){}
    MementoNote::MementoNote(const Note* n): title(n->getTitle()), modification(n->getModification()){}

    /*
     * Articles
     */

    void Article::SetMemento(MementoNote *m)
    {
        Note::SetMemento(m);
        texte = ((MementoArticle*) m)->getTexte();
    }

    void Article::visualiser(Ui::MainWindow * ui){
        ui->editTitre->setText(getTitle());
        ui->editTexte->setText(texte);
        ui->visuCont->show();
        ui->editTexte->show();


    }

      MementoArticle* Article::creerMemento() const {
          return new MementoArticle(this); }

      MementoArticle::MementoArticle(const Article& n): MementoNote(n), texte(n.getTexte()){}
      MementoArticle::MementoArticle(const Article* n): MementoNote(n), texte(n->getTexte()){}

      /*
       * Media
       */

    Media::Media(const QString& i, const QString& ti, const QString& m, const QString& te = "", const QString& fi = "") : Note(i, ti), texte(te), type(toMedia(m)), fichier(fi) {}

    void Media::visualiser(Ui::MainWindow * ui){
        ui->editTitre->setText(getTitle());
        ui->editMedia->setText(getFichier());
        ui->visuMedia->show();
        ui->btnParcourir->show();
        ui->editMedia->show();
    }

    void Media::SetMemento(MementoNote *m){
       Note::SetMemento(m);
       texte = ((MementoMedia*) m)->getTexte();
       type = ((MementoMedia*) m)->getType();
       fichier = ((MementoMedia*) m)->getFichier();
   }

      MementoMedia* Media::creerMemento() const { return new MementoMedia(this); }

      MementoMedia::MementoMedia(const Media& n): MementoNote(n), texte(n.getTexte()), type(n.getType()), fichier(n.getFichier()){}
      MementoMedia::MementoMedia(const Media* n): MementoNote(n), texte(n->getTexte()), type(n->getType()), fichier(n->getFichier()){}



      /*
       * Taches
       */

      void Tache::visualiser(Ui::MainWindow * ui){
        ui->editTitre->setText(getTitle());
        ui->editAction->show();
        ui->editAction->setText(action);
        ui->editStatut->show();
        ui->visuEcheance->show();
        /*ui->visuEcheance->setChecked(false);
        ui->visuPrio->setChecked(false);*/


        if (getEcheance()!=QDateTime())
        {
            ui->visuEcheance->setChecked(true);
            ui->editEcheance->setDateTime(getEcheance());
            ui->editEcheance->show();
            ui->visuEcheance->show();

        }
        else
        {
            ui->visuEcheance->setChecked(false);
            ui->editEcheance->setDateTime(QDateTime::currentDateTime());
        }
        ui->visuPrio->show();
        if (getPriorite()!=0)
        {
            ui->visuPrio->setChecked(true);
            int index = ui->editPrio->findText(QString::number(getPriorite()));
            ui->editPrio->setCurrentIndex(index);
            ui->visuPrio->show();
            ui->editPrio->show();

        }
        else
        {
            ui->visuPrio->setChecked(false);
            ui->editPrio->setCurrentIndex(0);
        }
        ui->visuAction->show();
        ui->visuStatut->show();
        if (getStatut()==2) ui->statutEnCours->setChecked(true);
        else if (getStatut()==3) ui->statutTermine->setChecked(true);
        else ui->statutEnAttente->setChecked(true);

    }

    void Tache::SetMemento(MementoNote* m)
            {
                Note::SetMemento(m);
                action = ((MementoTache*) m)->getAction();
                priorite = ((MementoTache*) m)->getPriorite();
                echeance = ((MementoTache*) m)->getEcheance();
                statut = ((MementoTache*) m)->getStatut();
            }

      MementoTache* Tache::creerMemento() const { return new MementoTache(this); }
      MementoTache::MementoTache(const Tache& n): MementoNote(n), action(n.getAction()), priorite(n.getPriorite()), echeance(n.getEcheance()), statut(n.getStatut()){}
      MementoTache::MementoTache(const Tache* n): MementoNote(n), action(n->getAction()), priorite(n->getPriorite()), echeance(n->getEcheance()), statut(n->getStatut()){}


    void Note::sauvegarder(Ui::MainWindow * ui){
        setTitle(ui->editTitre->text());
    }

    void Article::sauvegarder(Ui::MainWindow * ui){
        Note::sauvegarder(ui);
        setTexte(ui->editTexte->toPlainText());
    }

    void Media::sauvegarder(Ui::MainWindow * ui){
        Note::sauvegarder(ui);
        setTexte(ui->editTexte->toPlainText());
        setFichier(ui->editMedia->text());
    }

    void Tache::sauvegarder(Ui::MainWindow * ui){
        Note::sauvegarder(ui);
        setAction(ui->editAction->text());
        if (ui->visuPrio->isChecked()==true) setPriorite(ui->editPrio->currentText().toInt());
        if (ui->visuEcheance->isChecked()==true) setEcheance(ui->editEcheance->dateTime());
        if (ui->statutEnCours->isChecked()==true)
        {
            if (ui->listeTaches->findItems(getId(), Qt::MatchExactly).size()==0)
            {
                ui->listeTaches->addItem(getId());
                for (int i = 0; i < ui->listeNotes->count(); i++)
                {
                        if (ui->listeNotes->item(i)->text()==getId())
                        {
                            delete ui->listeNotes->item(i);
                            break;
                        }
                }
            }
            setStatut(2);
        }
        else
        {
            if (ui->listeNotes->findItems(getId(), Qt::MatchExactly).size()==0)
            {
                ui->listeNotes->addItem(getId());
                for (int i = 0; i < ui->listeTaches->count(); i++)
                {
                        if (ui->listeTaches->item(i)->text()==getId())
                        {
                            delete ui->listeTaches->item(i);
                            break;
                        }
                }
            }
            if  (ui->statutTermine->isChecked()==true||ui->editEcheance->dateTime()<QDateTime::currentDateTime()) setStatut(3);
            else setStatut(1);
        }
    }

    /*
     * NotesManager
     */

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
        qDebug() << "Nouvelle note insérée avec succès";

    }

    void NotesManager::addRelation(Relation* r)
    {
        relations.push_back(r);
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

    Relation* NotesManager::getRelation(const QString& n)
    {
        int j = 0;
        for(Relation* i: relations)
        {
            if (i->getTitre() == n) return relations[j];
            j++;
        }
        throw new NotesException("La relation n'existe pas");
    }

    Note& NotesManager::getNewNote(const QString& id) {
        Note* a = new Note(id, "");
        addNote(a);
        return *a;
    }


    void NotesManager::save() {
        db.save(notes);
        db.save(relations);
        qDebug() << "Sauvegarde effectuée";
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


