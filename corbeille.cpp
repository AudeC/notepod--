#include "ui_corbeille.h"
#include "mainwindow.h"


Corbeille::Corbeille(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Corbeille)
{
    ui->setupUi(this);
    setWindowTitle("Corbeille");
    setWindowIcon(QIcon("C:/Users/SilverEye/notepod/delete-51216.png"));
    ui->btnVider->setEnabled(false);
    ui->btnRest->setEnabled(false);


    connect(ui->listeCorbeille, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(demRest()));
    connect(ui->btnVider, SIGNAL(clicked(bool)), this, SLOT(viderCorbeille()));
}

void Corbeille::ajouter(NOTES::Note* a)
{
    if (ui->listeCorbeille->count()==0) ui->btnVider->setEnabled(true);
    ui->listeCorbeille->addItem(a->getId());
    qDebug() << "ajout d'un item";
}

void Corbeille::demRest()
{
    ui->btnRest->setEnabled(true);
}

void Corbeille::viderCorbeille()
{
    ui->listeCorbeille->clear();
    // le NotesManager contient un vecteur aSuppr
    //on veut supprimer tous les pointeurs qu'il contient
    m->viderCorbeille();


}

Corbeille::~Corbeille()
{
    delete ui;
}
