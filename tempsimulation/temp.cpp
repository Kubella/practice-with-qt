#include "temp.h"
#include "ui_temp.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <QFile>
#include <QImage>
using namespace std;

temp::temp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::temp)
{
    ui->setupUi(this);
}

temp::~temp()
{
    delete ui;
}

void temp::on_addButton_clicked()
{
    ui->data->insertRow(ui->data->rowCount());
}

void temp::on_deleteButton_clicked()
{
    ui->data->removeRow(ui->data->rowCount()-1);
}

void temp::on_quit_clicked()
{
    qApp->quit();
}

double ResultPlot[100][2];
double AxisPlot[20];

int BCcond=1;
double MinTemp,MaxTemp;
int NLG,Gne;

void temp::on_startButton_clicked()
{
    ui->console->setText("");
        LocalData LD;

    NLG=ui->data->rowCount();
    LocalData LD1={0,0,0,0.0,0.0,0.0,0.0};
    GlobalData G1={0,	0,	0.0, 0.0, 0.0, 	0.0};
    vector<LocalData> tabLG;
    for(int i=0;i<NLG;i++)
        tabLG.push_back(LD1);


    for(int i=0;i<NLG;i++)
    {

        LD.id=i;
        LD.k= (temp::ui->data->item(i,3))->text().toDouble();
        LD.L= (temp::ui->data->item(i,2))->text().toDouble();
        LD.S= (temp::ui->data->item(i,1))->text().toDouble();
        LD.ne= (temp::ui->data->item(i,0))->text().toInt();
        LD.nh=LD.ne+1;
        LD.dL=LD.L/LD.ne;
        tabLG[i]=LD;
        //temp::ui->console->append("Dane segmentu "+QString::number(LD.id+1)+":\nk="+QString::number(LD.dL)+" L="+QString::number(LD.L)+" Ne="+QString::number(LD.ne));
    }
    G1.alfa=(temp::ui->konwdata->toPlainText()).toDouble();
    G1.t_ot=(temp::ui->tempdata->toPlainText()).toDouble();
    G1.q=(temp::ui->strumdata->toPlainText()).toDouble();
    for(int i=0;i<NLG;i++)
    {
        G1.ne=G1.ne+tabLG[i].ne;
        G1.nh=G1.nh+tabLG[i].nh;
        if(i!=0)
            G1.nh--;
    }
Gne=G1.ne;
   // temp::ui->console->append("Dane globalne:\n alfa="+QString::number(G1.alfa)+" temperatura ot="+QString::number(G1.t_ot)+" q="+QString::number(G1.q)+" ne="+QString::number(G1.ne)+" nh="+QString::number(G1.nh));

    FemGrid FG;
    vector<FemGrid> tabFG; //Tablica siatek
    for(int i=0;i<NLG;i++)
        tabFG.push_back(FG);

    for(int i=0;i<NLG;i++)
    {
        for(int j=0;j<tabLG[i].ne;j++)
        {
            tabFG[i].tabEle.push_back(Element(j+1,tabLG[i].ne,tabLG[i].L));
            cout<<tabLG[j].dL;
        }
            for(int j=0;j<tabLG[i].nh;j++)
        {
            tabFG[i].tabNode.push_back(node(j+1));
        }

    }

    int tmp =ui->listWidget->selectionModel()->currentIndex().row();

    if(tmp==2)
    {
     BCcond=0;
    }
    else if(tmp==1)
    {
    BCcond=2;
    }
   else
    BCcond=1;

   if(BCcond==1)
    {
        tabFG[0].tabNode[0].BC=1;
        tabFG[NLG-1].tabNode[tabLG[NLG-1].nh-1].BC=2;
    }
    else if(BCcond==2)
    {
        tabFG[0].tabNode[0].BC=2;
        tabFG[NLG-1].tabNode[tabLG[NLG-1].nh-1].BC=1;
    }
   else if(BCcond==0)
   {
       tabFG[0].tabNode[0].BC=0;
       tabFG[NLG-1].tabNode[tabLG[NLG-1].nh-1].BC=0;
   }
//   cout<<"\n================================================================================\n\n";
//   for(int j=0;j<NLG;j++)
//   {
//   cout<<"PARAMETRY SEGMENTU NR: "<<(j+1)<<endl;
//   cout<<"UTWORZONE ELEMENTY :\nNr wezla\tWezel pocz\tWezel konc\tDlugosc odcinka\n";
//   for(int i=0;i<tabLG[j].ne;i++)
//       cout<<tabFG[j].tabEle[i].id_e<<"\t\t"<<tabFG[j].tabEle[i].id_hs<<"\t\t"<<tabFG[j].tabEle[i].id_he<<"\t\t"<<tabFG[j].tabEle[i].dL<<endl;
//   cout<<"\n--------------------------------------------------------------------------------\n\n";
//   cout<<"UTWORZONE WEZLY Z BC:\nNr wezla\tBC\n";
//   for(int i=0;i<tabLG[j].nh;i++)
//   cout<<tabFG[j].tabNode[i].id_n<<"\t\t"<<tabFG[j].tabNode[i].BC<<endl;
//   cout<<"\n================================================================================\n\n";
//   }


for(int j=0;j<NLG;j++)
{
       for(int i=0;i<tabLG[j].ne;i++)
       {
           (tabFG[j].tabEle[i].C)=(tabLG[j].S)*(tabLG[j].k)/(tabFG[j].tabEle[i].dL);
           tabFG[j].tabEle[i].Hlok[0][0]=tabFG[j].tabEle[i].C;
           tabFG[j].tabEle[i].Hlok[1][1]=tabFG[j].tabEle[i].C;
           tabFG[j].tabEle[i].Hlok[0][1]=-tabFG[j].tabEle[i].C;
           tabFG[j].tabEle[i].Hlok[1][0]=-tabFG[j].tabEle[i].C;
       }

   if(NLG==1)
   {
   if(tabFG[j].tabNode[0].BC==1)
           {
               tabFG[j].tabEle[0].Hlok[0][0]+=G1.alfa*tabLG[j].S;
               tabFG[j].tabEle[0].Plok[0]+=-G1.alfa*G1.t_ot*tabLG[j].S;
               tabFG[j].tabEle[tabLG[j].ne-1].Plok[1]+=G1.q*tabLG[j].S;
           }
   else if(tabFG[j].tabNode[tabLG[j].nh-1].BC==1)
           {
               tabFG[j].tabEle[tabLG[j].ne-1].Hlok[1][1]+=G1.alfa*tabLG[j].S;
               tabFG[j].tabEle[tabLG[j].ne-1].Plok[1]+=-G1.alfa*G1.t_ot*tabLG[j].S;
               tabFG[j].tabEle[0].Plok[0]+=G1.q*tabLG[j].S;
           }
//   else
//   {
//       tabFG[j].tabEle[tabLG[j].ne-1].Hlok[1][1]+=0;
//       tabFG[j].tabEle[tabLG[j].ne-1].Plok[1]+=0;
//       tabFG[j].tabEle[0].Plok[0]+=0;
//   }
   }
   else
   {
   if(tabFG[j].tabNode[0].BC==1)
           {
               tabFG[j].tabEle[0].Hlok[0][0]+=G1.alfa*tabLG[j].S;
               tabFG[j].tabEle[0].Plok[0]+=-G1.alfa*G1.t_ot*tabLG[j].S;
           }
   else if(tabFG[j].tabNode[0].BC==2)
   {
               tabFG[j].tabEle[0].Plok[0]+=G1.q*tabLG[j].S;
   }
   else if(tabFG[j].tabNode[tabLG[j].nh-1].BC==1)
           {
               tabFG[j].tabEle[tabLG[j].ne-1].Hlok[1][1]+=G1.alfa*tabLG[j].S;
               tabFG[j].tabEle[tabLG[j].ne-1].Plok[1]+=-G1.alfa*G1.t_ot*tabLG[j].S;
           }
   else if(tabFG[j].tabNode[tabLG[j].nh-1].BC==2)
           {
               tabFG[j].tabEle[tabLG[j].ne-1].Plok[1]+=G1.q*tabLG[j].S;
           }
//   else
//   {
//       tabFG[j].tabEle[tabLG[j].ne-1].Hlok[1][1]+=0;
//       tabFG[j].tabEle[tabLG[j].ne-1].Plok[1]+=0;
//       tabFG[j].tabEle[0].Plok[0]+=0;
//   }
   }
}
//cout<<"LOKALNE TABLICE CIEZKOSCI I TABLICE OBCIAZEN\n\n";
//for(int j=0;j<NLG;j++)
//{
//    cout<<"TABLICA CIEZKOSCI I TABLICA OBCIAZEN DLA SEGMENTU NR: "<<(j+1)<<endl;
//for(int i=0;i<tabLG[j].ne;i++)
//{
//    cout<<"ELEMENT NR "<<(i+1)<<":\n";
//    cout<<tabFG[j].tabEle[i].Hlok[0][0]<<"  "<<tabFG[j].tabEle[i].Hlok[1][0]<<"\t\t"<<tabFG[j].tabEle[i].Plok[0]<<endl;
//    cout<<tabFG[j].tabEle[i].Hlok[0][1]<<"  "<<tabFG[j].tabEle[i].Hlok[1][1]<<"\t\t"<<tabFG[j].tabEle[i].Plok[1]<<endl;
//    cout<<endl;
//    cout<<"______________________________________________"<<endl;
//}
//}
//temp::ui->console->append("Dane globalne:\n=" + QString::number(tabFG[0].tabEle[7].Hlok[0][1])+" "+ QString::number(tabFG[1].tabEle[0].Hlok[1][0]) + " BC set:" + QString::number(tabFG[0].tabEle[0].Plok[0]));

OES MATRIX;

for(int i=0;(i<G1.nh*G1.nh);i++)
        MATRIX.HG.push_back(0);
for(int i=0;(i<G1.nh);i++)
{
        MATRIX.PG.push_back(0);
        MATRIX.TG.push_back(0);
}


int StartIndex=0;
 for(int k=0;k<NLG;k++)
 {
    for(int i=0; i<tabLG[k].ne;i++)
{
    for(int j=0; j<tabLG[k].ne;j++)
    {
        if(i==j)
        {
            MATRIX.HG[(i+StartIndex)*G1.nh+j+StartIndex]+=tabFG[k].tabEle[i].Hlok[0][0];
            MATRIX.HG[(i+StartIndex)*G1.nh+j+StartIndex+1]+=tabFG[k].tabEle[i].Hlok[1][0];
            MATRIX.HG[(i+StartIndex)*G1.nh+j+StartIndex+G1.nh]+=tabFG[k].tabEle[i].Hlok[0][1];
            MATRIX.HG[(i+StartIndex)*G1.nh+j+StartIndex+G1.nh+1]+=tabFG[k].tabEle[i].Hlok[1][1];
        }
    }

}
    if(NLG==1)
    {
        MATRIX.PG[0]=tabFG[k].tabEle[0].Plok[0];
        MATRIX.PG[G1.nh-1]=tabFG[k].tabEle[tabLG[k].ne-1].Plok[1];
    }
    else
    {
if(StartIndex==0)
{
    MATRIX.PG[0]=tabFG[k].tabEle[0].Plok[0];
}
if(G1.ne==StartIndex+tabLG[k].ne)
{
MATRIX.PG[G1.nh-1]=tabFG[k].tabEle[tabLG[k].ne-1].Plok[1];
}

}
StartIndex+=tabLG[k].ne;
}

// temp::ui->console->append("start index"+QString::number(StartIndex)+"PG "  + QString::number(MATRIX.PG[0]) + " PG" + QString::number(MATRIX.PG[G1.nh-1]));

 for(int i=0;(i<G1.nh);i++)
 {
     QString local_temp="";

     for(int j=0;(j<G1.nh);j++)
     {
     local_temp+=(QString::number(MATRIX.HG[i*G1.nh+j]) + " ");
     }
     //temp::ui->console->append(local_temp);
    // temp::ui->console->append("\n");
 }

// cout<<"RÓWNANIA DO ROZWIAZANIA:"<<endl;
// cout<<"\t     HG\t\t\t\t\tPG\t\tTG"<<endl;
// for(int i=0;i<G1.nh;i++)
// {
// for(int j=0;j<G1.nh;j++)
// {
//     cout<<MATRIX.HG[i*G1.nh+j]<<"\t";
// }
// cout<<"\t\t"<<MATRIX.PG[i]<<"\t\t"<<MATRIX.TG[i]<<endl;
// }



 double w;
 for(int i=0;i<(G1.nh-1);i++)
 {
     for(int j=i+1;j<G1.nh;j++)
     {
         w=MATRIX.HG[j*G1.nh+i]/MATRIX.HG[i*G1.nh+i];
         MATRIX.PG[j]-=MATRIX.PG[i]*w;
         for(int k=i;k<G1.nh;k++)
         {
             MATRIX.HG[j*G1.nh+k]-=MATRIX.HG[i*G1.nh+k]*w;
         }
     }
 }



 MinTemp=G1.t_ot;
 MaxTemp=G1.t_ot;


 for(int i=(G1.nh-1);i>=0;i--)
 {
     int j=i+1;
     while(j<G1.nh)
     {
         MATRIX.TG[i]+=MATRIX.HG[i*G1.nh+j]*MATRIX.TG[j];
         j++;
     }
     MATRIX.TG[i]=(-MATRIX.PG[i]-MATRIX.TG[i])/MATRIX.HG[i*G1.nh+i];
      if(MaxTemp<MATRIX.TG[i])
      {
          MaxTemp=MATRIX.TG[i];
      }

}

// for(int i=0;i<G1.nh;i++)
//     {
//         for(int j=0;j<G1.nh;j++)
//         {
//             cout<<MATRIX.HG[i*G1.nh+j]<<"\t";
//         }
//         cout<<"\t"<<MATRIX.PG[i]<<"\t"<<MATRIX.TG[i]<<endl;
//     cout<<endl;
// }

QString result="";
result+="Wektor rowiązań:\n";
double DL=0.0;
int T=0;
result+=(QString::number(T+1)+"\t"+QString::number(DL)+"\t"+QString::number(MATRIX.TG[T])+"\n");
ResultPlot[T][0]=DL;
ResultPlot[T][1]=MATRIX.TG[T];

for(int j=0;j<NLG;j++)
{
        for(int i=0;i<tabLG[j].ne;i++)
        {
        T++;
        DL+=tabFG[j].tabEle[i].dL;
        result+=(QString::number(T+1)+"\t"+QString::number(DL)+"\t"+QString::number(MATRIX.TG[T])+"\n");
        ResultPlot[T][0]=DL;
        ResultPlot[T][1]=MATRIX.TG[T];
         }
}
result+=("Minimalna temperatura: "+QString::number(MinTemp)+"\nMaksymalna Tempertatura: "+QString::number(MaxTemp)+"\n");
double DLSUM=0;

for(int i=0;i<NLG;i++)
{
   DLSUM+=tabLG[i].L;
   AxisPlot[i]=DLSUM;
}

temp::ui->console->append(result);


ui->printButton->setEnabled(true);

}


void temp::on_printButton_clicked()
{
    double a,b,c,d;
    QVector<double> x(Gne+1), y(Gne+1),XL(2),YL(2);

    for (int i=0; i<(Gne+1); i++)
    {
      x[i] = ResultPlot[i][0];
      y[i] = ResultPlot[i][1];
    }

    ui->Graph->addGraph();
    ui->Graph->graph(0)->setData(x, y);

    double AVGTEMP=(MaxTemp-MinTemp)/2;


    for(int i=0;i<NLG;i++)
    {
       ui->Graph->addGraph();
       XL[0]=AxisPlot[i];
       YL[0]=MinTemp-AVGTEMP;
       XL[1]=AxisPlot[i];
       YL[1]=MaxTemp+AVGTEMP;
       ui->Graph->graph(i+1)->setData(XL,YL);
    }


    ui->Graph->xAxis->setLabel("DL");
    ui->Graph->yAxis->setLabel("Temp");

    ui->Graph->xAxis->setRange(0,AxisPlot[NLG-1]+0.01*AxisPlot[NLG-1] );
    ui->Graph->yAxis->setRange(MinTemp-AVGTEMP,MaxTemp +AVGTEMP);
    ui->Graph->graph(0)->setPen(QPen(QColor(255, 0, 0), 2));
    ui->Graph->replot();
    ui->printButton->setEnabled(false);
}

void temp::on_saveButton_clicked()
{

    QFile file("out.txt");
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

    QTextStream out(&file);

    for (int i=0; i<(Gne+1); i++)
    {
      out <<QString::number(i+1)<<"\t"<< QString::number(ResultPlot[i][0])<<"\t"<<QString::number(ResultPlot[i][1])<<"\n";
    }

}
