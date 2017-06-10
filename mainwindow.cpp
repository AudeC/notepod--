#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
     fenAjout(new Ajout(this)),
     fenRel(new Relations(this))
{
    ui->setupUi(this);

    // Menu - Ajout d'une note
    QAction *actionNouveau = new QAction("&Ajouter note", this);
    ui->menuAjout->addAction(actionNouveau);
    connect(actionNouveau, SIGNAL(triggered()), fenAjout, SLOT(open()));

    QAction *actionSave = new QAction("&Sauvegarder", this);
    ui->menuSauvegarder->addAction(actionSave);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(saveSlot()));

    load();



    // Ajout des notes
    for(NOTES::Note* i : getNotes())
    {
        ui->listeNotes->addItem(i->getId());
    }
    connect(ui->listeNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));

    ui->visualisation->hide();
    connect(ui->btnSauver, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()));

    connect(ui->anciennesVersions, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(restaurerNote(QListWidgetItem*)));

    addRelation(new NOTES::Relation("test", "descri"));
    getRelation("test").ajouterCouple(new NOTES::Note("coucou", "yolo"), new NOTES::Note("fleur", "hihihi"), "hey !");
    addRelation(new NOTES::Relation("test2", "descri2"));

    // Ajout des relations
    for(NOTES::Relation r : relations)
    {
        ui->listeRelations->addItem(r.getTitre());
    }
    connect(ui->listeRelations, SIGNAL(itemDoubleClicked(QListWidgetItem*)), fenRel, SLOT(editerRel(QListWidgetItem*)));

}

void MainWindow::restaurerNote(QListWidgetItem * a ){

    QString date = a->text();

    for(NOTES::MementoNote * i : historique[noteOuverte->getId()]){
        if(date == i->getModification().toString()){
            qDebug() << "j'ai la note" << i->getTitle();
            noteOuverte->SetMemento(i);
            noteOuverte->visualiser(ui);
            return;
        } else {
            qDebug() << "je supprime";
           // historique[noteOuverte->getId()].pop_back();
        }
    }
}

void MainWindow::ajouterNote(NOTES::Note *a){
    addNote(a);
    ui->listeNotes->addItem(a->getId());
}

void MainWindow::sauvegarder(){

    vector<NOTES::MementoNote*> * v = &historique[noteOuverte->getId()];
    v->push_back(noteOuverte->creerMemento());
    ui->anciennesVersions->addItem(noteOuverte->creerMemento()->getModification().toString());
    noteOuverte->sauvegarder(ui);
}

void MainWindow::visualiserNote(QListWidgetItem * i){

    // Cacher les parties spéciales
    ui->visuAction->hide();
    ui->visuCont->hide();
    ui->visuPrio->hide();
    ui->editAction->hide();
    ui->editTexte->hide();
    ui->editPrio->hide();

    NOTES::Note& a = getNote(i->text());
    noteOuverte = &a;

     a.visualiser(ui);

     while(ui->anciennesVersions->count()>0)
     {
       ui->anciennesVersions->takeItem(0);//handle the item if you don't
                               //have a pointer to it elsewhere
     }
     for(NOTES::MementoNote * i : historique[a.getId()] ){
         ui->anciennesVersions->addItem(i->getModification().toString());
     }
    ui->visualisation->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
