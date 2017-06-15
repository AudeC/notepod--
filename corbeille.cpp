#include "ui_corbeille.h"
#include "mainwindow.h"


Corbeille::Corbeille(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Corbeille)
{
    ui->setupUi(this);
    setWindowTitle("Corbeille");
    setWindowIcon(QIcon(QApplication::applicationDirPath()+"/ressources/delete-51216.png"));
    ui->btnVider->setEnabled(false);
    ui->btnRest->setEnabled(false);


    connect(ui->listeCorbeille, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(demRest()));
    connect(ui->btnVider, SIGNAL(clicked(bool)), this, SLOT(viderCorbeille()));
    connect(ui->btnRest, SIGNAL(clicked(bool)), this, SLOT(restaurer()));
    connect(this, SIGNAL(activerBtn(bool)), ui->btnVider, SLOT(setEnabled(bool)));
}

void Corbeille::ajouter(NOTES::Note* a)
{
    ui->listeCorbeille->addItem(a->getId());
    qDebug() << "ajout d'un item";
}

void Corbeille::demRest()
{
    ui->btnRest->setEnabled(true);
    emit activerBtn(true);
}

void Corbeille::viderCorbeille()
{
    ui->listeCorbeille->clear();
    m->viderCorbeille();
}

void Corbeille::restaurer()
{
    QString id = ui->listeCorbeille->currentItem()->text();
    if (m->getNote(id).getClass()=="Tache")
    {
        NOTES::Tache* aCopier = new NOTES::Tache(static_cast<NOTES::Tache&>(m->getNote(id)));
        m->addNote(aCopier);
    }
    else if (m->getNote(id).getClass()=="Media")
    {
        NOTES::Media* aCopier = new NOTES::Media(static_cast<NOTES::Media&>(m->getNote(id)));
        m->addNote(aCopier);
    }
    else
    {
        NOTES::Article* aCopier = new NOTES::Article(static_cast<NOTES::Article&>(m->getNote(id)));
        m->addNote(aCopier);
    }
    QListWidgetItem* it = ui->listeCorbeille->takeItem(ui->listeCorbeille->currentRow());
    delete it;


}

Corbeille::~Corbeille()
{
    delete ui;
}
