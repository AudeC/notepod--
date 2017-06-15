#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fenAjout(new Ajout(this)),
    fenRel(new Relations(this)),
    fenCorbeille(new Corbeille(this))
{
    ui->setupUi(this);
    setWindowTitle("Notepod-- v0.1");

    // Barre de menu

    connect(ui->actionNvlleNote_2, SIGNAL(triggered()), fenAjout, SLOT(open()));
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(saveSlot()));
    ui->menuAffichage->addAction(ui->dockRelations_2->toggleViewAction());

    QAction *actionDenis = new QAction("&Denis", this);
    ui->menuSauvegarder->addAction(actionDenis);
    connect(actionDenis, SIGNAL(triggered()), this, SLOT(ouvrirDenis()));

    connect(ui->btnAjouter, SIGNAL(clicked()), fenAjout, SLOT(open()));
    connect(ui->btnCorbeille, SIGNAL(clicked()), fenCorbeille, SLOT(open()));

    connect(ui->visuEcheance, SIGNAL(toggled(bool)), this, SLOT(affEcheance(bool)));
    connect(ui->visuPrio, SIGNAL(toggled(bool)), this, SLOT(affPriorite(bool)));
    connect(ui->btnParcourir, SIGNAL(clicked(bool)), this, SLOT(chercherFichier()));
    connect(ui->btnSuppr, SIGNAL(clicked(bool)), this, SLOT(supprimer()));


    load();

    // Ajout des notes
    for(NOTES::Note* i : getNotes())
    {
        if (i->getClass()=="Tache")
        {
            NOTES::Tache* t= static_cast<NOTES::Tache*>(i);
            if (t->getStatut()==2)
            {
                /*QListWidgetItem* aInserer = new QListWidgetItem(i->getId());
                for(int row = 0; row < ui->listeTaches->count(); row++)
                {
                    NOTES::Tache* item = static_cast<NOTES::Tache*>(getNotePtr(ui->listeTaches->item(row)->text()));
                    if (item->getEcheance()>=t->getEcheance())
                    {
                        if (item->getPriorite()<t->getPriorite())
                        {
                            ui->listeTaches->insertItem(row, aInserer);
                            delete aInserer;
                            break;
                        }
                        else continue;
                    }
                 }
                if (ui->listeTaches->findItems(i->getId(), Qt::MatchExactly).size()==0)
                {*/
                    ui->listeTaches->addItem(i->getId());
                    /*delete aInserer;
                }*/
                    //trier listeTaches
            }
            else ui->listeNotes->addItem(i->getId());
        }
        else ui->listeNotes->addItem(i->getId());
    }
    connect(ui->listeNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));
    connect(ui->listeTaches, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));

    ui->visualisation->hide();
    connect(ui->btnSauver, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()));
    connect(ui->btnSauver, SIGNAL(clicked(bool)), this, SLOT(saveSlot()));
    ui->historique->hide();
    ui->btnHistorique->setCheckable(true);
    connect(ui->btnHistorique, SIGNAL(toggled(bool)), ui->historique, SLOT(setVisible(bool)));

    connect(ui->anciennesVersions, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(restaurerNote(QListWidgetItem*)));



    // Ajout des relations
    for(NOTES::Relation* r : relations)
    {
        ui->listeRelations->addItem(r->getTitre());
    }
    connect(ui->listeRelations, SIGNAL(itemDoubleClicked(QListWidgetItem*)), fenRel, SLOT(editerRel(QListWidgetItem*)));
    connect(ui->btnAjouterRel, SIGNAL(clicked(bool)), this, SLOT(ajouterRel())); //faire une fonction creerRel

}

void MainWindow::ajouterRel(){
    relations.push_back(new NOTES::Relation(ui->titreNouvelleRel->text(), QString()));
    ui->listeRelations->addItem(ui->titreNouvelleRel->text());
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

void MainWindow::ouvrirDenis()
{
    QMessageBox* ah = new QMessageBox;
    ah->setWindowTitle("Ah!");
    ah->setText("Ça veut dire que...?");
    ah->setStandardButtons(QMessageBox::Ok);
    ah->setDefaultButton(QMessageBox::Ok);
    QPixmap icon(QApplication::applicationDirPath()+"/ressources/denis.brogniart.ah.png");
    ah->setIconPixmap(icon);
    ah->setWindowIcon(QIcon(QApplication::applicationDirPath()+"/ressources/1495391974_cancel_16.png"));
    ah->show();
}

void MainWindow::affEcheance(bool b)
{
    if (b==true) ui->editEcheance->show();
    else
    {
        ui->editEcheance->hide();
        NOTES::Tache* t= (NOTES::Tache*)noteOuverte;
        t->setEcheance(QDateTime());
        noteOuverte->visualiser(ui);
    }
}

void MainWindow::affPriorite(bool b)
{
    if (b==true) ui->editPrio->show();
    else
    {
        ui->editPrio->hide();
        NOTES::Tache* t= (NOTES::Tache*)noteOuverte;
        t->setPriorite(0);
        noteOuverte->visualiser(ui);
    }
}

void MainWindow::chercherFichier()
{
    QString fichier = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"),
                                                    QDir::toNativeSeparators(QDir::currentPath()),
                                                    tr("Images (*.png *.gif *.jpg);;"
                                                       "Fichiers audio (*.mp3 *.wav *.wma);;"
                                                       "Fichiers vidéo (*.mp4 *.avi *.wmv)"));

    if (!fichier.isNull())
        ui->editMedia->setText(QDir::toNativeSeparators(fichier));
}

void MainWindow::demandeConfirmation()
{
   QMessageBox* conf = new QMessageBox();
   conf->setWindowTitle("Confirmation de la suppression");
   conf->setText("Voulez-vous vraiment supprimer cette note?");
   conf->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   conf->setDefaultButton(QMessageBox::Cancel);
   QPixmap icon(QApplication::applicationDirPath()+"/ressources/delete-512.png");
   conf->setIconPixmap(icon);
   conf->setWindowIcon(QIcon(QApplication::applicationDirPath()+"/ressources/warning-icon-png-276616.png"));
   connect(conf, SIGNAL(accepted()), this, SLOT(supprimer()));
   conf->show();
}

void MainWindow::sauvegarder(){

    vector<NOTES::MementoNote*>* v = &historique[noteOuverte->getId()];
    v->push_back(noteOuverte->creerMemento());
    ui->anciennesVersions->addItem(noteOuverte->creerMemento()->getModification().toString());
    noteOuverte->sauvegarder(ui);
}

void MainWindow::supprimer()
{
    corbeille.push_back(new NOTES::Note(*noteOuverte));
    fenCorbeille->ajouter(noteOuverte);



    if (ui->listeTaches->findItems(noteOuverte->getId(), Qt::MatchExactly).size()!=0)
    {
        for (int i = 0; i < ui->listeTaches->count(); i++)
        {
                if (ui->listeTaches->item(i)->text()==noteOuverte->getId())
                {
                    delete ui->listeTaches->item(i);
                    break;
                }
        }
        //retirer la note de la liste
    }
    else if (ui->listeNotes->findItems(noteOuverte->getId(), Qt::MatchExactly).size()!=0)
    {
        for (int i = 0; i < ui->listeNotes->count(); i++)
        {
                if (ui->listeNotes->item(i)->text()==noteOuverte->getId())
                {
                    delete ui->listeNotes->item(i);
                    break;
                }
        }
        //retirer la note de la liste
    }
    ui->visualisation->hide();
    fenCorbeille->open();
    try{
    vector<NOTES::Note*>::iterator position = std::find(notes.begin(), notes.end(), getNotePtr(noteOuverte->getId()));
    if (position != notes.end()) // == myVector.end() means the element was not found
        notes.erase(position);
    } catch(NOTES::NotesException& e){
        qDebug() << "erreur" ;
    }

    qDebug()<<"Je supprime"<<corbeille;
}

void MainWindow::visualiserNote(QListWidgetItem * i){

    // Cacher les parties spéciales
    ui->visuAction->hide();
    ui->visuCont->hide();
    ui->visuPrio->hide();
    ui->visuEcheance->hide();
    ui->visuStatut->hide();
    ui->editAction->hide();
    ui->editTexte->hide();
    ui->editPrio->hide();
    ui->editEcheance->hide();
    ui->editStatut->hide();
    ui->visuMedia->hide();
    ui->btnParcourir->hide();
    ui->editMedia->hide();

    NOTES::Note& a = getNote(i->text());
    noteOuverte = &a;

    a.visualiser(ui);

    /*ui->visuEcheance->setChecked(false);
    ui->visuPrio->setChecked(false);*/

    //décocher par défaut, puis recocher en fonction des attributs de i




    while(ui->anciennesVersions->count()>0)
    {
      ui->anciennesVersions->takeItem(0);//handle the item if you don't
                               //have a pointer to it elsewhere
    }
    for(NOTES::MementoNote * i : historique[a.getId()] ){
        ui->anciennesVersions->addItem(i->getModification().toString());
    }
    ui->editId->setText(a.getId());
    ui->visualisation->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
