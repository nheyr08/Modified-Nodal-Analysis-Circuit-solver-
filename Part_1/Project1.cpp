//Created by Henry Betsaleel on 28/04/2022
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iomanip> 
#include <iostream>
using namespace std;
/****************************************************************************************************************/
int M = 0;	// num of rows
int N = 0; // num of cols in each row
int p;
int n;
int kkkk=0;
int bb=0;  // iteration
vector<string> X;
vector<string> Xx;//mpoko konn
string content;//pral li fichyea
vector<string> Pr;//pral stok info mwen bezen yo de fichye a
vector<long double>B;
int tttt=0;//keeps track of resistors index in ResInd
long double value;
bool flag=false;
vector<int> ResInd;
vector<string> IInd;
vector<string> nodes;
bool flag2=false;
/****************************************************************************************************************/

int main(int argc, char *argv[])// std::cout << "There are " << argc << " arguments." << std::endl;
{  
   ifstream file;
   file.open(argv[1]);
   
   cout<<"These are the argv "<<argv[1]<<argv[2]<<argv[3]<<argv[4]<<endl;
   
   while(file >> content) //read file  content
    {   bb++;
       if(content[0]=='n'&&content[1]-48<10)
        { content.erase(std::remove(content.begin(), content.end(), 'n'), content.end()); 
          p=stoi(content);
          if(p>N)
          {
          N=p; 
          nodes.push_back("V(n"+content+")");
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
           content.erase(std::remove(content.begin(), content.end(), 'r'), content.end()); 
           flag=true;
           int  nm1=stoi(content);
           ResInd.push_back(nm1);
        }
        else if(content[0]=='V'&&content[1]-48<10||content[0]=='v'&&content[1]-48<10)
        {   flag2=false;
        M++;
        Pr.push_back(content);
        flag=true;
           content.erase(std::remove(content.begin(), content.end(), 'V'), content.end()); 
            content.erase(std::remove(content.begin(), content.end(), 'v'), content.end()); 
           int  nm1=stoi(content);
        //   VInd.push_back(nm1);
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
        else
        {
           bb--;
        }
   }
    int TruN=N;
    int TruN2=N;
    N=N+kkkk;    
    n=M+N;
    std::vector<long double> B(n,0);//inityalize 2d vecte saa 
    std::vector<std::vector<int> > C(M, std::vector<int>(N, 0.0));//inityalize 2d vecte saa 
    std::vector<std::vector<long double> > R(n, std::vector<long double>(n, 0.0));//inityalize 2d vecte saa 
    std::vector<std::vector<long double> > A(n, std::vector<long double>(n, 0.0));//inityalize 2d vecte saa 

//Xx for the second order resistors
for (int i = 0; i <Xx.size(); i++)
    {
      if(Xx[i][0]=='R')
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
         {	
         A[num1-1][num1-1] = A[num1-1][num1-1]+ (1.0/val);
			R[num1-1][num1-1] = R[num1-1][num1-1]+ (1.0/val);
			}
		   if(num2!=0)
		   {
         A[num2-1][num2-1] = A[num2-1][num2-1]+ (1.0/val);
			R[num2-1][num2-1] = R[num2-1][num2-1]+ (1.0/val);
			}
		   if(num1!=0 && num2!=0)
		   {  
		   A[num1-1][num2-1]  = A[num1-1][num2-1]-(1.0/val); 
         R[num1-1][num2-1]  = R[num1-1][num2-1]-(1.0/val);
         A[num2-1][num1-1]  = A[num2-1][num1-1]-(1.0/val); 
         R[num2-1][num1-1]  = R[num2-1][num1-1]-(1.0/val);
         } 
      }
      if(Pr[i][0]=='V'||Pr[i][0]=='v')
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
{          
   //[i].erase(std::remove(Pr[i].begin(), Pr[i].end(), 'I'), Pr[i].end()); 
   if(IInd[i][0]=='I'||IInd[i][0]=='i')
   {    int num1= stoi(IInd[i+1]);
        int num2= stoi(IInd[i+2]);
        long double val=stold(IInd[i+3]);
      if (num1>0)
         B[num1-1]=-val+B[num1-1];
      if (num2>0)
         B[num2-1]=val+B[num2-1]; 
   }  
}
// Write to the files
//___________________________________________________________________________________________________________*/
   ofstream outputMNA;
	ofstream outputXVec;
	ofstream outputRHS;
	outputMNA.open(argv[2]);
	outputXVec.open(argv[3]);
	outputRHS.open(argv[4]);


//ofstream outputXVec("output_xVector.txt");
if (outputXVec.is_open())
{
    for(int i=1;i<TruN2+1;i++)
    {
    outputXVec<<"V(n"<<i<<")"<<endl;
    }
    for(int i=0;i<Xx.size();i++)
    {
        if(Xx[i][0]=='R')
    outputXVec<<"I("<<Xx[i]<<")"<<endl;
    }
    for(int i=0;i<X.size();i++)
    {
    outputXVec<<X[i]<<endl;
    }
    outputXVec.close();
}

//ofstream ffout("output_mnaMatrix.txt");
  if (outputMNA.is_open())
  {
  //   outputMNA<<"\n\nA Matrix\n\n";
    for(int i=0;i<n;i++)
    {
    	for(int j=0;j<n;j++)
    	{
    		outputMNA<<A[i][j]<<"  ";
		}
//	outputMNA<<std::setprecision(4)<<"  "<<B[i]<<"\n";
   outputMNA<<endl;
	}
    outputMNA.close();
   }
//ofstream outputRHS("output_rhs.txt.");
if (outputRHS.is_open())
  {
    for(int i=0;i<n;i++)
    outputRHS<<B[i]<<"\n";
    outputRHS.close();
   }
return 0;
}