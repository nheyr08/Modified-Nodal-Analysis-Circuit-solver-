//Created by Henry Betsaleel on 28/04/2022
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <cstring>
#define ket 2    
#define numbre 7  
using namespace std;
double Myvalue_d(int k)
{
return(3-0.01*exp((k^5)/0.04));
}

double C_source_val(double C,double h,double t, double v)
{
    return (C*v*(t-h)/h);
}
double str_d(string R)
{
  int x=0;
  double y=0;
  if(R!="0")
  {
    x = stoi(R);
    y = (double)x;
    return y;
  }
  else
    return 0;
}
int str_Int(string R)
{
  if(R!="0")
  {
    return stoi(R);
  }
  else
    return 0;
}

void G_max(vector<vector<double> > Retriche,vector<vector<double> > &MNA, vector<vector<double> > Q_Table)
{
  //vector
  double rSum;
  int nodal,nodre;
  // double vBE = 0.7,vBC = ;
  
  for(int key=1;key<=numbre;key++)
  {
    rSum=0;
    for(int i=0;i<Retriche.size();i++)
    {
      if((Retriche.at(i)).at(0) == key)//if node+ is key node
        rSum += 1/(Retriche.at(i)).at(2);
      else if((Retriche.at(i)).at(1) == key)//if node- is key node
        rSum += 1/(Retriche.at(i)).at(2);
    }
    (MNA.at(key-1)).at(key-1)=rSum;
  }
  for(int i=0;i<Retriche.size();i++)
  {
    
    nodal = (int)(Retriche.at(i)).at(0);
    nodre = (int)(Retriche.at(i)).at(1);
    if(nodal!=0 && nodre!=0) { 
      (MNA.at(nodal-1)).at(nodre-1) += -(1/(Retriche.at(i)).at(2));
      (MNA.at(nodre-1)).at(nodal-1) += -(1/(Retriche.at(i)).at(2));
    }
  }
}
void B_max(vector<vector<double> >V_Table,vector<vector<double> >&MNA)
{
  int nodal,nodre;
  for(int i=0;i<V_Table.size();i++)
  {  
    nodal = (int)(V_Table.at(i)).at(0);
    nodre = (int)(V_Table.at(i)).at(1);
        
    if(nodal!=0 && nodre!=0)
    {
      (MNA.at(numbre+i)).at(nodal-1) = 1;
      (MNA.at(numbre+i)).at(nodre-1) = -1;
      (MNA.at(nodal-1)).at(numbre+i) = 1;
      (MNA.at(nodre-1)).at(numbre+i) = -1;
    }
    else if(nodre==0)
    {
      (MNA.at(numbre+i)).at(nodal-1) = 1;
      (MNA.at(nodal-1)).at(numbre+i) = 1;        
    }
    else if(nodal==0)
    {
      (MNA.at(numbre+i)).at(nodre-1) = -1;
      (MNA.at(nodre-1)).at(numbre+i) = -1;
    }
  }
}

void construct_rhs(vector<vector<double> > V_Table, vector<double> &rhs)
{
  int x,y;
  for(int i=0;i<V_Table.size();i++)
    rhs.at(7+i) = (V_Table.at(i)).at(2);
}

void LU(vector<vector<double> >MNA, vector<vector<double> >&L_Matrix, vector<vector<double> >&U_Matrix, vector<double> &rhs)
{
  int t;
  double temp;

  for(int k=0;k<MNA.size();k++)
  {
    
    (L_Matrix.at(k)).at(k) = 1;
    double max;
    int reg,reg1=k;
    max=(MNA.at(k)).at(k);
    if(max<0)
      max=max*(-1);
    for(int i=k+1;i<MNA.size();i++)
    {  
      if((MNA.at(i)).at(k)>=0)//+
      {
        if((MNA.at(i)).at(k)>max)
        {
          max = (MNA.at(i)).at(k);
          reg1=i;
      }
        }
          
      else if ((MNA.at(i)).at(k)<0)//-
      {
        reg =(-1) *(MNA.at(i)).at(k);
        if(reg>max)
        {
          max = reg;
          reg1=i;
        }     
      }
    }
    t=reg1;
      for(int m=0;m<MNA.size();m++)
      {
        temp = (MNA.at(k)).at(m);
        (MNA.at(k)).at(m) = (MNA.at(t)).at(m);
        (MNA.at(t)).at(m) = temp;
      }
      for(int i=0;i<k;i++)
      {
        temp = (L_Matrix.at(k)).at(i);
        (L_Matrix.at(k)).at(i) = (L_Matrix.at(t)).at(i);
        (L_Matrix.at(t)).at(i) = temp;
      }

      temp=rhs.at(k);
      rhs.at(k)=rhs.at(t);
      rhs.at(t)=temp;
      
    for(int j=k;j<MNA.size();j++)
    {
      (U_Matrix.at(k)).at(j) = (MNA.at(k)).at(j);
    }
    for(int i=k+1;i<MNA.size();i++)
    {
      
      (L_Matrix.at(i)).at(k) = (MNA.at(i)).at(k)/(U_Matrix.at(k)).at(k);
    }
    for(int i=k+1;i<MNA.size();i++)
      for(int j=k+1;j<MNA.size();j++)
        (MNA.at(i)).at(j) = (MNA.at(i)).at(j) - (L_Matrix.at(i)).at(k)*(U_Matrix.at(k)).at(j);
  }
}

void Forward_Sub(vector<vector<double> > L_Matrix, vector<double> &yVector, vector<double> rhs)
{
  for(int k=0;k<L_Matrix.size();k++)
  {
    yVector.at(k) = rhs.at(k)/(L_Matrix.at(k)).at(k);
    if(yVector.at(k) != 0)
    {
      for(int i=k+1;i<L_Matrix.size();i++)
        rhs.at(i) = rhs.at(i) - (L_Matrix.at(i)).at(k)*yVector.at(k);
    }
  }
}
void Backward_Sub(vector<vector<double> > U_Matrix, vector<double> &xVector, vector<double> yVector)
{
  for(int k=U_Matrix.size()-1;k>=0;k--)
  {
    xVector.at(k) = yVector.at(k)/(U_Matrix.at(k)).at(k);
    if(xVector.at(k)!=0)
      for(int i=k-1;i>=0;i--)
        yVector.at(i) = yVector.at(i) - (U_Matrix.at(i)).at(k)*xVector.at(k);
  }
}

void Iter_Newton (vector<vector<double> > MNA,vector<double> &rhs,vector<vector<double> >Q_Table,vector<vector<double> >M_Table,vector<vector<double> > &resp)
{
  vector<vector<double> > new_MNA (ket+numbre, vector<double> (ket+numbre));
  vector<double> new_rhs (9);
  vector<vector<double> >L_Matrix (ket+numbre, vector<double> (ket+numbre));
  vector<vector<double> >U_Matrix (ket+numbre, vector<double> (ket+numbre));
  vector<double> y_Vector (9);
  vector<double> x_Vector (9);
  double v1,v2,v3,v4,v5,v6,v7;
  int nodeE1,nodeC1,nodeB1
  ,nodeE2,nodeC2,nodeB2
  ,nodeD,nodeG,nodeS;
  double gee1,gec1,gce1,gcc1
  ,gee2,gec2,gce2,gcc2;
  double aF1,aR1,Ies1,Ics1;
  double aF2,aR2,Ies2,Ics2;
  double Vt,x,beta; 
  double VT = 0.026; 
  double Gds,gm;
  double Ie1,Ic1,Ie2,Ic2,Ieq;
  double id;
  nodeE1 = (int)(Q_Table.at(0)).at(2);
  nodeC1 = (int)(Q_Table.at(0)).at(0);
  nodeB1 = (int)(Q_Table.at(0)).at(1);
  aF1 = (Q_Table.at(0)).at(3);
  aR1 = (Q_Table.at(0)).at(4);
  Ies1 = (Q_Table.at(0)).at(5);
  Ics1 = (Q_Table.at(0)).at(6);
  nodeE2 = (int)(Q_Table.at(1)).at(2);
  nodeC2 = (int)(Q_Table.at(1)).at(0);
  nodeB2 = (int)(Q_Table.at(1)).at(1);
  aF2 = (Q_Table.at(1)).at(3);
  aR2 = (Q_Table.at(1)).at(4);
  Ies2 = (Q_Table.at(1)).at(5);
  Ics2 = (Q_Table.at(1)).at(6);
  nodeD = (int)(M_Table.at(0)).at(0);
  nodeG = (int)(M_Table.at(0)).at(1);
  nodeS = (int)(M_Table.at(0)).at(2);
  Vt = (M_Table.at(0)).at(3);
  x = (M_Table.at(0)).at(4);
  beta = (M_Table.at(0)).at(5);
for(int t=0;t<=300;t++)
{

  v1 = 0 + 0.01*t;
  v2=0 ;
  v3=0;
  v4=0;
  v5=0;
  v6=0;
  v7=0;
  rhs.at(7) = v1;
  for(int i=0;i<1000;i++)
  {

   for(int i=0;i<MNA.size();i++)
     for(int j=0;j<MNA.size();j++)
      (new_MNA.at(i)).at(j) = (MNA.at(i)).at(j);
    for(int i=0;i<rhs.size();i++)
      new_rhs.at(i) = rhs.at(i);
    double vBE1 = v2,vBC1 = v2 - v4;
    double vBE2 = v5 - v7,vBC2 = v5 - v6;
    double vGS = v2,vDS = v7;
    ///BJT1 
    gee1 = (Ies1/VT)*exp(vBE1/VT);
    gec1 = aR1*(Ics1/VT)*exp(vBC1/VT);
    gce1 = aF1*(Ies1/VT)*exp(vBE1/VT);
    gcc1 = (Ics1/VT)*exp(vBC1/VT);
    //BJT2 
    gee2 = (Ies2/VT)*exp(vBE2/VT);
    gec2 = aR2*(Ics2/VT)*exp(vBC2/VT);
    gce2 = aF2*(Ies2/VT)*exp(vBE2/VT);
    gcc2 = (Ics2/VT)*exp(vBC2/VT);
    //mosfet 
    if(vGS <= Vt)
    {
      Gds = 0;
      gm = 0;
      id = 0;
    }
    else if((vDS <= vGS-Vt)  && (vDS >= 0))
    {
      Gds = (beta/2)*x*vDS*vDS + beta*(vGS-Vt-vDS)*(1+2*x*vDS);
      gm = beta*vDS*(1+x*vDS);
      id = beta*((vGS-Vt)*vDS-0.5*vDS*vDS)*(1+x*vDS);
    }
    else if((vDS >= vGS-Vt) && (vGS-Vt>=0))
    {
      Gds = (beta/2)*x*(vGS-Vt)*(vGS-Vt);
      gm = beta*(vGS-Vt)*(1+x*vDS);
      id = (beta/2)*(vGS-Vt)*(vGS-Vt)*(i+x*vDS);
    }
    //rhs
    Ie1 = -Ies1*(exp(vBE1/VT)-1) + aR1*Ics1*(exp(vBC1/VT)-1) +gee1*vBE1-gec1*vBC1;
    Ic1 = aF1*Ies1*(exp(vBE1/VT)-1) - Ics1*(exp(vBC1/VT)-1) -gce1*vBE1 +gcc1*vBC1;
    Ie2 = -Ies2*(exp(vBE2/VT)-1) + aR2*Ics2*(exp(vBC2/VT)-1) +gee2*vBE2-gec2*vBC2;
    Ic2 = aF2*Ies2*(exp(vBE2/VT)-1) - Ics2*(exp(vBC2/VT)-1) -gce2*vBE2 +gcc2*vBC2;
    Ieq = id -Gds*vDS - gm*vGS;    
    (new_MNA.at(nodeC1-1)).at(nodeC1-1) = (MNA.at(nodeC1-1)).at(nodeC1-1)+gcc1;
    (new_MNA.at(nodeC1-1)).at(nodeB1-1) = (MNA.at(nodeC1-1)).at(nodeB1-1)+(gce1-gcc1);
    (new_MNA.at(nodeB1-1)).at(nodeC1-1) = (MNA.at(nodeB1-1)).at(nodeC1-1)+(gec1-gcc1);
    (new_MNA.at(nodeB1-1)).at(nodeB1-1) = (MNA.at(nodeB1-1)).at(nodeB1-1)+(gcc1+gee1-gce1-gec1);
    (new_MNA.at(nodeE2-1)).at(nodeE2-1) = (MNA.at(nodeE2-1)).at(nodeE2-1)+gee2;
    (new_MNA.at(nodeE2-1)).at(nodeC2-1) = (MNA.at(nodeE2-1)).at(nodeC1-1)-gec2;
    (new_MNA.at(nodeE2-1)).at(nodeB2-1) = (MNA.at(nodeE2-1)).at(nodeB1-1)+(gec2-gee2);
    (new_MNA.at(nodeC2-1)).at(nodeE2-1) = (MNA.at(nodeC2-1)).at(nodeE2-1)-gce2;
    (new_MNA.at(nodeC2-1)).at(nodeC2-1) = (MNA.at(nodeC2-1)).at(nodeC1-1)+gcc2;
    (new_MNA.at(nodeC2-1)).at(nodeB2-1) = (MNA.at(nodeC2-1)).at(nodeB1-1)+(gce2-gcc2);
    (new_MNA.at(nodeB2-1)).at(nodeE2-1) = (MNA.at(nodeB2-1)).at(nodeE2-1)+(gce2-gee2);
    (new_MNA.at(nodeB2-1)).at(nodeC2-1) = (MNA.at(nodeB2-1)).at(nodeC1-1)+(gec2-gcc2);
    (new_MNA.at(nodeB2-1)).at(nodeB2-1) = (MNA.at(nodeB2-1)).at(nodeB1-1)+(gcc2+gee2-gce2-gec2);
    (new_MNA.at(nodeD-1)).at(nodeD-1) = (MNA.at(nodeD-1)).at(nodeD-1)+Gds;
    (new_MNA.at(nodeD-1)).at(nodeG-1) = (MNA.at(nodeD-1)).at(nodeG-1)+gm;
    new_rhs.at(nodeC1-1) = rhs.at(nodeC1-1)-Ic1;
    new_rhs.at(nodeB1-1) = rhs.at(nodeB1-1)+Ie1+Ic1;
    new_rhs.at(nodeE2-1) = rhs.at(nodeE2-1)-Ie2;
    new_rhs.at(nodeC2-1) = rhs.at(nodeC2-1)-Ic2;
    new_rhs.at(nodeB2-1) = rhs.at(nodeB2-1)+Ie2+Ic2;
    new_rhs.at(nodeD-1) = rhs.at(nodeD-1)-Ieq; 
    LU(new_MNA,L_Matrix,U_Matrix,new_rhs);
    Forward_Sub(L_Matrix,y_Vector,new_rhs);
    Backward_Sub(U_Matrix,x_Vector,y_Vector);
    
    vector<double> sN (7);
    vector<double> s  (7);
    double a;
    double b;
    sN.at(1) = x_Vector.at(1) - v2;
    sN.at(2) = x_Vector.at(2) - v3;
    sN.at(3) = x_Vector.at(3) - v4;
    sN.at(4) = x_Vector.at(4) - v5;
    sN.at(5) = x_Vector.at(5) - v6;
    sN.at(6) = x_Vector.at(6) - v7;
    for(int i=0;i<7;i++)
    {
      if(sN.at(i) < 0)
      {
        a = -sN.at(i);
        b = -1;
      }
      else if(sN.at(i) > 0)
      {
        b = 1;
        a = sN.at(i);
      }
      else if(sN.at(i) == 0)
      {
        a = 0;
        b = 0;
      }
      s.at(i) = (1.3/16)*b*log(1 + 16*a);
    }
    v2 = v2 + s.at(1);
    v3 = v3 + s.at(2);
    v4 = v4 + s.at(3);
    v5 = v5 + s.at(4);
    v6 = v6 + s.at(5);
    v7 = v7 + s.at(6);
  }
  (resp.at(t)).at(0) = v1;
  (resp.at(t)).at(1) = v7;
} 

}
//using namespace std;
/****************************************************************************************************************/
//clock_t t_start, t_end; //for time

/****************************************************************************************************************/

int main(int argc, char *argv[])// std::cout << "There are " << argc << " arguments." << std::endl;
{ //t_start=clock();

 //Declaration  
int M = 0;	// num of rows
int N = 0; // num of cols in each row
int p;
int n;
int kkkk=0;
int bb=0;  // iteration
std::vector<std::string> X;
std::vector<std::string> Xo;
std::vector<std::string> Xx;//mpoko konn
std::string content;//pral li fichyea
std::vector<std::string> Pr;//pral stok info mwen bezen yo de fichye a
int tttt=0;//keeps track of resistors index in ResInd
long double value;
bool flag=false;
std::vector<std::string> IInd;
bool flag2=false;
//___________________________________________________________________________________________________________*/
if(str_Int(argv[1])==1)
{

std::ifstream file;
file.open(argv[5]);
// Write to the files
//___________________________________________________________________________________________________________*/
   std::ofstream FILE_OUT;
	FILE_OUT.open(argv[6]);
    int pivot_index, order_temp;
   long double pivot_num;
    std::vector<long double> row_temp;
//___________________________________________________________________________________________________________*/   
   while(file >> content) //read file  content
    {   bb++;
       if(content[0]=='n'&&content[1]-48<10)
        { content.erase(std::remove(content.begin(), content.end(), 'n'), content.end()); 
          p=stoi(content);
          if(p>N)
          {
          N=p; 
      //    nodes.push_back("V(n"+content+")");
          }
         Pr.push_back(content);
          if (flag2){
              IInd.push_back(content);
          }
        }
        else if((content[0]=='R'||content[0]=='r')&&content[1]-48<10)
        {    flag2=false;
         //    t++;
           Pr.push_back(content);
           content.erase(std::remove(content.begin(), content.end(), 'R'), content.end()); 
          // content.erase(std::remove(content.begin(), content.end(), 'r'), content.end()); 
           flag=true;
           int  nm1=stoi(content);
       //    ResInd.push_back(nm1);
        }
        else if(content[0]=='V'&&content[1]-48<10||content[0]=='v'&&content[1]-48<10)
        {   flag2=false;
        M++;
        Pr.push_back(content);
        flag=true;
           content.erase(std::remove(content.begin(), content.end(), 'V'), content.end()); 
            content.erase(std::remove(content.begin(), content.end(), 'v'), content.end()); 
           int  nm1=stoi(content);
            X.push_back("I(V"+content+")");
        }
        else if(content[0]=='I'&&content[1]-48<10||content[0]=='i'&&content[1]-48<10)
        {   flag2=true;
            Pr.push_back(content);
            IInd.push_back(content);
            flag=true;
            content.erase(std::remove(content.begin(), content.end(), 'I'), content.end()); 
            content.erase(std::remove(content.begin(), content.end(), 'i'), content.end()); 
            int  nm1=stoi(content);
        }
        else if(bb%4==0&&flag==true)
        { 
         Pr.push_back(content);
         long double k=stold(content);
         flag=false;
       //  t=0;
         if (flag2){
             IInd.push_back(content);
         }
        }
        else if(content[0]=='0'&&flag==true)
        {
          Pr.push_back(content);
           p++;
             if (flag2)
             {
             IInd.push_back(content);
             }
        }
        else if(content[0]=='G'&&content[1]=='2'&&flag==false)
        {   flag2=false;
            int s=(bb/3)*2;
            Xx.push_back(Pr[bb-5]);
            Xx.push_back(Pr[bb-4]);
            Xx.push_back(Pr[bb-3]);
            Xx.push_back(Pr[bb-2]);
            Pr.pop_back();
            Pr.pop_back();
            Pr.pop_back();
            Pr.pop_back();
            bb=bb-5;
            kkkk++;
        }
        else if(content[0]=='C'&&content[1]-48<10)
        {
            Pr.push_back(content);
            flag=true;
        }
         else if(content[0]=='L'&&content[1]-48<10)
        {
            Pr.push_back(content);
             flag=true;
        }
        else if( content[0]=='Q')
        {
            Pr.push_back(content);
            flag=true;
        }
        else
        {
           bb--;
        }
   }
    int TruN=N;
    int TruN2=N;
    N+=kkkk;    
    n=M+N;
    std::vector<long double> B(n,0);//inityalize 2d vecte saa 
    //std::vector<std::vector<int> > C(M, std::vector<int>(N, 0.0));//inityalize 2d vecte saa 
  //  std::vector<std::vector<long double> > R(n, std::vector<long double>(n, 0.0));//inityalize 2d vecte saa 
    std::vector<std::vector<long double> > A(n, std::vector<long double>(n, 0.0));//inityalize 2d vecte saa 
//Xx for the second order resistors
for (int i = 0; i <Xx.size(); i+=4)
    { 
      int num1=stoi(Xx[i+1]);
      int num2=stoi(Xx[i+2]);
      long double val=stold(Xx[i+3]);
      
      A[TruN][TruN] =  A[TruN][TruN]-(val);
          
           if(num1>0)
           {
            A[TruN][num1-1] = 1; 
            A[num1-1][TruN] = 1; 
           }
	       if(num2>0)
            {
             A[TruN][num2-1] = -1; 
             A[num2-1][TruN] = -1;
            }
	      TruN = TruN + 1;
   }
// Other elements 
flag=false;

for(int i=0;i<bb;i++)
   {	// Resistance row
	 if(Pr[i][0]=='R')
	   { Pr[i].erase(std::remove(Pr[i].begin(), Pr[i].end(), 'R'), Pr[i].end()); 
         int num1=stoi(Pr[i+1]);
         int num2=stoi(Pr[i+2]);
         long double val=stold(Pr[i+3]);
        if(num1!=0)         	
             A[num1-1][num1-1] = A[num1-1][num1-1]+ (1.0/val);
		if(num2!=0)
           A[num2-1][num2-1] = A[num2-1][num2-1]+ (1.0/val);
		if(num1!=0 && num2!=0)
		   {  
		   A[num1-1][num2-1]  = A[num1-1][num2-1]-(1.0/val); 
           A[num2-1][num1-1]  = A[num2-1][num1-1]-(1.0/val); 
           } 
        } 
    if(Pr[i][0]=='C')
	   { //Pr[i].erase(std::remove(Pr[i].begin(), Pr[i].end(), 'R'), Pr[i].end()); 
         int num1=stoi(Pr[i+1]);
         int num2=stoi(Pr[i+2]);
         long double val=stold(Pr[i+3]);
        if(num1!=0)         	
            A[num1-1][num1-1] = A[num2-1][num1-1]+val/0.1;
		if(num2!=0)
          A[num2-1][num2-1] = A[num2-1][num1-1]+val/0.1;
		if(num1!=0 && num2!=0)
		   {  
		   A[num1-1][num2-1]  = A[num2-1][num1-1]-val/0.1; 
           A[num2-1][num1-1]  =  A[num2-1][num1-1]-val/0.1; 
           } 
           double pete=0;
           for (int i=0;i<100;i++){
            pete=C_source_val(val,0.01,i,pete);
           }
           B[num1]=B[num1]+ pete;
          
        } 
      else if(Pr[i][0]=='V'||Pr[i][0]=='v')
	   {
      int num1=stoi(Pr[i+1]);
      int num2=stoi(Pr[i+2]);
      long double val=stold(Pr[i+3]);
   
       if(num1>0)
        {
            A[N][num1-1] = 1; 
            A[num1-1][N] = 1; 
         }
	      if(num2>0)
         {
            A[N][num2-1] = -1; 
            A[num2-1][N] = -1;
         }
	      B[N] = B[N] + val;
	      N = N+1;
	   }
   }

/*_____________________________________________________________________________________________________________*/
//B matrix RHS contribution
bool flag3=true;
for(int i=0;i<IInd.size();i++)        
   if(IInd[i][0]=='I'||IInd[i][0]=='i')
   {    int num1= stoi(IInd[i+1]);
        int num2= stoi(IInd[i+2]);
        long double val=stold(IInd[i+3]);
      if (num1>0)
         B[num1-1]=-val+B[num1-1];
      if (num2>0)
         B[num2-1]=val+B[num2-1]; 
   }  

//FILE_OUT<<"MNA_matrix = \n";
//for(int i=0; i<n; i++){
//for(int j=0; j<n; j++){
        //FILE_OUT<<A[i][j]<<"  ";
//}
//FILE_OUT<<"\n";
//}
    for(int i=1;i<TruN2+1;i++)
    Xo.push_back("V(n"+std::to_string(i)+")");
    for(int i = 0; i <Xx.size(); i+=4)
    Xo.push_back("I("+Xx[i]+")");
    Xo.insert(Xo.end(), X.begin(), X.end());

//FILE_OUT<<"\nMNA_b = \n";
//for(int j=0; j<n; j++)
    //FILE_OUT<<B[j]<<"\n";

for(int k=0; k<n; k++)
   {
    pivot_num = 0;      
    for(int i=k; i<n; i++)
        if(fabs(A[i][k]) > fabs(pivot_num))
        {
            pivot_num = A[i][k];
            pivot_index = i;
        }
    
    if(pivot_num != 0){
        std::swap( A[pivot_index], A[k]);  
        std::swap( B[pivot_index], B[k]); 
     // std::swap(Xo[pivot_index],X[k]); 
        for(int i=k+1; i<n; i++)
        A[i][k] = A[i][k]/A[k][k];
        for(int i=k+1; i<n; i++)
        for(int j=k+1; j<n; j++)
            A[i][j] -= A[i][k] * A[k][j];    
        }
    }

    for(int i=1; i<n; i++)
        for(int j=0; j<i; j++)
           B.at(i) -=(B.at(j) * A[i][j]);
    B.at(n-1) /= A[n-1][n-1];
    for(int i=n-2; i>=0; i--)
    {
        for(int j=n-1; j>i; j--)
           B.at(i) -=B.at(j) * A[i][j];
        B.at(i) /= A[i][i];
    }
   
    //FILE_OUT << "\nsolution : \n";
    for(int i=0; i<n; i++)
    //FILE_OUT << Xo[i] << " = " <<B.at(i) <<"\n";
//t_end = clock();
//double k1=double(t_end - t_start);
////FILE_OUT<<"\nTime: "<< k1 / CLOCKS_PER_SEC<<" secs. \n";
  FILE_OUT.close();
}
else if(str_Int(argv[1])==2)
{

std::ifstream file;
file.open(argv[5]);
// Write to the files
//___________________________________________________________________________________________________________*/
   std::ofstream FILE_OUT;
	FILE_OUT.open(argv[6]);
    int pivot_index, order_temp;
   long double pivot_num;
    std::vector<long double> row_temp;
     double values;
    for(int i=0;i<100;i++){
      double R=0.744/i;
     values =Myvalue_d(R);
     FILE_OUT<< R <<" "<<values<<endl ;
    } 
//___________________________________________________________________________________________________________*/   
   while(file >> content) //read file  content
    {   bb++;
       if(content[0]=='n'&&content[1]-48<10)
        { content.erase(std::remove(content.begin(), content.end(), 'n'), content.end()); 
          p=stoi(content);
          if(p>N)
          {
          N=p; 
      //    nodes.push_back("V(n"+content+")");
          }
         Pr.push_back(content);
          if (flag2){
              IInd.push_back(content);
          }
        }
        else if((content[0]=='R'||content[0]=='r')&&content[1]-48<10)
        {    flag2=false;
         //    t++;
           Pr.push_back(content);
           content.erase(std::remove(content.begin(), content.end(), 'R'), content.end()); 
          // content.erase(std::remove(content.begin(), content.end(), 'r'), content.end()); 
           flag=true;
           int  nm1=stoi(content);
       //    ResInd.push_back(nm1);
        }
        else if(content[0]=='V'&&content[1]-48<10||content[0]=='v'&&content[1]-48<10)
        {   flag2=false;
        M++;
        Pr.push_back(content);
        flag=true;
           content.erase(std::remove(content.begin(), content.end(), 'V'), content.end()); 
            content.erase(std::remove(content.begin(), content.end(), 'v'), content.end()); 
           int  nm1=stoi(content);
            X.push_back("I(V"+content+")");
        }
        else if(content[0]=='I'&&content[1]-48<10||content[0]=='i'&&content[1]-48<10)
        {   flag2=true;
            Pr.push_back(content);
            IInd.push_back(content);
            flag=true;
            content.erase(std::remove(content.begin(), content.end(), 'I'), content.end()); 
            content.erase(std::remove(content.begin(), content.end(), 'i'), content.end()); 
            int  nm1=stoi(content);
        }
        else if(bb%4==0&&flag==true)
        { 
         Pr.push_back(content);
         long double k=stold(content);
         flag=false;
       //  t=0;
         if (flag2){
             IInd.push_back(content);
         }
        }
        else if(content[0]=='0'&&flag==true)
        {
          Pr.push_back(content);
           p++;
             if (flag2)
             {
             IInd.push_back(content);
             }
        }
        else if(content[0]=='G'&&content[1]=='2'&&flag==false)
        {   flag2=false;
            int s=(bb/3)*2;
            Xx.push_back(Pr[bb-5]);
            Xx.push_back(Pr[bb-4]);
            Xx.push_back(Pr[bb-3]);
            Xx.push_back(Pr[bb-2]);
            Pr.pop_back();
            Pr.pop_back();
            Pr.pop_back();
            Pr.pop_back();
            bb=bb-5;
            kkkk++;
        }
        else if(content[0]=='C'&&content[1]-48<10)
        {
            Pr.push_back(content);
            flag=true;
        }
         else if(content[0]=='L'&&content[1]-48<10)
        {
            Pr.push_back(content);
             flag=true;
        }
        else if( content[0]=='Q')
        {
            Pr.push_back(content);
            flag=true;
        }
        else
        {
           bb--;
        }
   }
    int TruN=N;
    int TruN2=N;
    N+=kkkk;    
    n=M+N;
    std::vector<long double> B(n,0);//inityalize 2d vecte saa 
    //std::vector<std::vector<int> > C(M, std::vector<int>(N, 0.0));//inityalize 2d vecte saa 
  //  std::vector<std::vector<long double> > R(n, std::vector<long double>(n, 0.0));//inityalize 2d vecte saa 
    std::vector<std::vector<long double> > A(n, std::vector<long double>(n, 0.0));//inityalize 2d vecte saa 
//Xx for the second order resistors
for (int i = 0; i <Xx.size(); i+=4)
    { 
      int num1=stoi(Xx[i+1]);
      int num2=stoi(Xx[i+2]);
      long double val=stold(Xx[i+3]);
      
      A[TruN][TruN] =  A[TruN][TruN]-(val);
          
           if(num1>0)
           {
            A[TruN][num1-1] = 1; 
            A[num1-1][TruN] = 1; 
           }
	       if(num2>0)
            {
             A[TruN][num2-1] = -1; 
             A[num2-1][TruN] = -1;
            }
	      TruN = TruN + 1;
   }
// Other elements 
flag=false;

for(int i=0;i<bb;i++)
   {	// Resistance row
	 if(Pr[i][0]=='R')
	   { Pr[i].erase(std::remove(Pr[i].begin(), Pr[i].end(), 'R'), Pr[i].end()); 
         int num1=stoi(Pr[i+1]);
         int num2=stoi(Pr[i+2]);
         long double val=stold(Pr[i+3]);
        if(num1!=0)         	
             A[num1-1][num1-1] = A[num1-1][num1-1]+ (1.0/val);
		if(num2!=0)
           A[num2-1][num2-1] = A[num2-1][num2-1]+ (1.0/val);
		if(num1!=0 && num2!=0)
		   {  
		   A[num1-1][num2-1]  = A[num1-1][num2-1]-(1.0/val); 
           A[num2-1][num1-1]  = A[num2-1][num1-1]-(1.0/val); 
           } 
        } 
    if(Pr[i][0]=='C')
	   { //Pr[i].erase(std::remove(Pr[i].begin(), Pr[i].end(), 'R'), Pr[i].end()); 
         int num1=stoi(Pr[i+1]);
         int num2=stoi(Pr[i+2]);
         long double val=stold(Pr[i+3]);
        if(num1!=0)         	
            A[num1-1][num1-1] = A[num2-1][num1-1]+val/0.1;
		if(num2!=0)
          A[num2-1][num2-1] = A[num2-1][num1-1]+val/0.1;
		if(num1!=0 && num2!=0)
		   {  
		   A[num1-1][num2-1]  = A[num2-1][num1-1]-val/0.1; 
           A[num2-1][num1-1]  =  A[num2-1][num1-1]-val/0.1; 
           } 
           double pete=0;
           for (int i=0;i<100;i++){
            pete=C_source_val(val,0.01,i,pete);
           }
           B[num1]=B[num1]+ pete;
          
        } 
      else if(Pr[i][0]=='V'||Pr[i][0]=='v')
	   {
      int num1=stoi(Pr[i+1]);
      int num2=stoi(Pr[i+2]);
      long double val=stold(Pr[i+3]);
   
       if(num1>0)
        {
            A[N][num1-1] = 1; 
            A[num1-1][N] = 1; 
         }
	      if(num2>0)
         {
            A[N][num2-1] = -1; 
            A[num2-1][N] = -1;
         }
	      B[N] = B[N] + val;
	      N = N+1;
	   }
   }
bool flag3=true;
for(int i=0;i<IInd.size();i++)        
   if(IInd[i][0]=='I'||IInd[i][0]=='i')
   {    int num1= stoi(IInd[i+1]);
        int num2= stoi(IInd[i+2]);
        long double val=stold(IInd[i+3]);
      if (num1>0)
         B[num1-1]=-val+B[num1-1];
      if (num2>0)
         B[num2-1]=val+B[num2-1]; 
   }  

//FILE_OUT<<"MNA_matrix = \n";
//for(int i=0; i<n; i++){
//for(int j=0; j<n; j++){
        //FILE_OUT<<A[i][j]<<"  ";
//}
//FILE_OUT<<"\n";
//}
    for(int i=1;i<TruN2+1;i++)
    Xo.push_back("V(n"+std::to_string(i)+")");
    for(int i = 0; i <Xx.size(); i+=4)
    Xo.push_back("I("+Xx[i]+")");
    Xo.insert(Xo.end(), X.begin(), X.end());

//FILE_OUT<<"\nMNA_b = \n";
//for(int j=0; j<n; j++)
    //FILE_OUT<<B[j]<<"\n";

for(int k=0; k<n; k++)
   {
    pivot_num = 0;      
    for(int i=k; i<n; i++)
        if(fabs(A[i][k]) > fabs(pivot_num))
        {
            pivot_num = A[i][k];
            pivot_index = i;
        }
    
    if(pivot_num != 0){
        std::swap( A[pivot_index], A[k]);  
        std::swap( B[pivot_index], B[k]); 
     // std::swap(Xo[pivot_index],X[k]); 
        for(int i=k+1; i<n; i++)
        A[i][k] = A[i][k]/A[k][k];
        for(int i=k+1; i<n; i++)
        for(int j=k+1; j<n; j++)
            A[i][j] -= A[i][k] * A[k][j];    
        }
    }

    for(int i=1; i<n; i++)
        for(int j=0; j<i; j++)
           B.at(i) -=(B.at(j) * A[i][j]);
    B.at(n-1) /= A[n-1][n-1];
    for(int i=n-2; i>=0; i--)
    {
        for(int j=n-1; j>i; j--)
           B.at(i) -=B.at(j) * A[i][j];
        B.at(i) /= A[i][i];
    }
   
    //FILE_OUT << "\nsolution : \n";
    for(int i=0; i<n; i++)
    //FILE_OUT << Xo[i] << " = " <<B.at(i) <<"\n";
//t_end = clock();
//double k1=double(t_end - t_start);
////FILE_OUT<<"\nTime: "<< k1 / CLOCKS_PER_SEC<<" secs. \n";
  FILE_OUT.close();


}

return 0;
}



                                                                                                                                //cout<<endl<<"With randomized quicksort"<<endl;
                                                                                                                                //j.printArray(B, n);
  //  t_start = clock();
  //      j.quickSort(A2, 0, n - 1);
   // t_end = clock();
  //   double k2=double(t_end - t_start);
   //     cout<<" Sorted array in quicksort(Normal) time: "<<k2/ CLOCKS_PER_SEC<<" secs. "<<endl;
     //   cout<<"the difference is: "<< abs(k2-k1)/ CLOCKS_PER_SEC <<"secs. "<<endl<<endl; 