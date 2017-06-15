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
    connect(ui->btnRest, SIGNAL(clicked(bool)), this, SLOT(restaurer()));
    connect(this, SIGNAL(activerBtn(bool)), ui->btnVider, SLOT(setEnabled(bool)));
}

void Corbeille::ajouter(NOTES::Note* a)
{
    //if (ui->listeCorbeille->count()!=0)
        //ui->btnVider->setEnabled(false);
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
    const QString& s = ui->listeCorbeille->currentItem()->text();
}

Corbeille::~Corbeille()
{
    delete ui;
}
