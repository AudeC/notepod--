#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fenAjout(new Ajout(this)),
    fenRel(new Relations(this))
{
    ui->setupUi(this);

    // Barre de menu

    connect(ui->actionNvlleNote_2, SIGNAL(triggered()), fenAjout, SLOT(open()));
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(saveSlot()));
    ui->menuAffichage->addAction(ui->dockRelations_2->toggleViewAction());

    QAction *actionDenis = new QAction("&Denis", this);
    ui->menuSauvegarder->addAction(actionDenis);
    connect(actionDenis, SIGNAL(triggered()), this, SLOT(ouvrirDenis()));

    connect(ui->btnAjouter, SIGNAL(clicked()), fenAjout, SLOT(open()));

    load();



    // Ajout des notes
    for(NOTES::Note* i : getNotes())
    {
        ui->listeNotes->addItem(i->getId());
    }
    connect(ui->listeNotes, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(visualiserNote(QListWidgetItem*)));

    ui->visualisation->hide();
    connect(ui->btnSauver, SIGNAL(clicked(bool)), this, SLOT(sauvegarder()));
    connect(ui->btnSauver, SIGNAL(clicked(bool)), this, SLOT(saveSlot()));
    ui->historique->hide();
    ui->btnHistorique->setCheckable(true);
    connect(ui->btnHistorique, SIGNAL(toggled(bool)), ui->historique, SLOT(setVisible(bool)));

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

void MainWindow::ouvrirDenis()
{
    QMessageBox* ah = new QMessageBox;
    ah->setWindowTitle("Ah!");
    ah->setText("Ça veut dire que...?");
    ah->setStandardButtons(QMessageBox::Ok);
    ah->setDefaultButton(QMessageBox::Ok);
    QPixmap icon("C:/Users/SilverEye/notepod/denis.brogniart.ah.png");
    ah->setIconPixmap(icon);
    ah->setWindowIcon(QIcon("C:/Users/SilverEye/notepod/1495391974_cancel_16.png"));
    ah->show();
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

    connect(ui->visuEcheance, SIGNAL(toggled(bool)), ui->editEcheance, SLOT(setVisible(bool)));
    connect(ui->visuPrio, SIGNAL(toggled(bool)), ui->editPrio, SLOT(setVisible(bool)));
    connect(ui->btnParcourir, SIGNAL(clicked(bool)), this, SLOT(chercherFichier()));

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
    ui->editId->setText(a.getId());
    ui->visualisation->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
