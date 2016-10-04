#ifndef TEMP_H
#define TEMP_H
#include <QMainWindow>
#include <vector>

using namespace std;

namespace Ui {
class temp;
}

class temp : public QMainWindow
{
    Q_OBJECT

public:
    explicit temp(QWidget *parent = 0);
    ~temp();
    struct LocalData
    {
    int id;
    int ne;
    int nh;
    double S;
    double L;
    double k;
    double dL;
    };
    struct GlobalData
    {
    int ne;
    int nh;
    double L;
    double t_ot;
    double alfa;
    double q;
    };

    struct Element //struktura elementu
    {
        int id_e;
        int id_hs;
        int id_he;
        double dL;
        double C;
        double Hlok[2][2];
        double Plok[2];
    public: Element(int idE,int Ne,double L) //konstruktor do FemGrida
        {
            id_e=idE;
            id_hs=idE;
            id_he=idE+1;
            dL=L/Ne;
            C=0.0;
            Hlok[0][0]=0.0;
            Hlok[0][1]=0.0;
            Hlok[1][0]=0.0;
            Hlok[1][1]=0.0;
            Plok[1]=0.0;
            Plok[0]=0.0;
        }
    };

    struct node //struktura wezla
    {
        int id_n;
        int BC;
        double x_node;
        double t_node;
    public: node(int idN)
        {
            id_n=idN;
            BC=0;
            x_node=0;
            t_node=0;
        }
    };

    struct FemGrid { //struktura siatki
    vector <Element> tabEle;
    vector <node> tabNode;
    };

    struct OES { //struktura rownan
    vector<double> HG;
    vector<double> PG;
    vector<double> TG;

    };

    FemGrid BuildGrid(FemGrid FG,LocalData LD,GlobalData G);
    int SetBC();


private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_quit_clicked();

    void on_startButton_clicked();



    void on_printButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::temp *ui;

};



#endif // TEMP_H
