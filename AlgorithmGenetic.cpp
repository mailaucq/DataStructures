#include <stdlib.h>
#include <random>
#include <iostream>
#include <math.h>
#include <cmath>
#include <climits>
#include <string>
using namespace std;
struct Chrom{
	string bits;
	float fitness;
	float X[2];
	Chrom():bits(""),fitness(0.0f){};
	Chrom(string bts, float ftns): bits(bts), fitness(ftns){};
};
void inicializa(int *l, Chrom *poblacion, int SIZE_POBLACION, int NUM_VARIABLES, int * RIGHT, int *LEFT, int *PRECISION);
string codificacion(int *l, int NUM_VARIABLES, int *RIGHT, int *LEFT, int *PRECISION);
void decodificacion(int *l, Chrom *poblacion, int SIZE_POBLACION, int NUM_VARIABLES, int *RIGHT, int *LEFT);
float evalua(Chrom *poblacion, int SIZE_POBLACION,int NUM_VARIABLES);
void normalizacionLineal();
string ruleta(Chrom *poblacion, float bm, int SIZE_POBLACION);
void mutate(string &bits);
void crossover1Punto(string &bits1, string &bits2);
void crossover2Punto(string &bits1, string &bits2);
void crossoverUniforme(string &bits1, string &bits2);
//Puntero a funcion
void (*crossover[])(string &,string &)={crossover1Punto,crossover2Punto,crossoverUniforme};
float calculateFitness(float sumaCuadrados);
int binToDecimal(string bits);
string randomBin(int l);

int main(){

	int SIZE_POBLACION = 100;
	int NUM_VARIABLES = 2;
	int RIGHT[2] = {50,50};
	int LEFT[2] ={-50,-50};
	int PRECISION[2] = {8,8};
	int l[NUM_VARIABLES];
	int MAX_GEN = 40;
	int MAX_EXPERIMENTOS = 20;
	int TYPE_CROSSOVER = 0;
	double PROB_CROSSOVER = 0.65;
	double PROB_MUTATION = 0.08;
	int FLAG_ELITISMO = 0;
	int FLAG_NORMALIZACION = 0;
	//cout<<"Tamanho poblacion: "<<endl;
	//cin>>SIZE_POBLACION;
	//cout<<"Numero de Generaciones: "<<endl;
	//cin>>MAX_GEN;
	//cout<<"Numero de Experimentos: "<<endl;
	//cin>>MAX_EXPERIMENTOS;
	//cout<<"Tipo de Cruce: Cruce 1: 0, Cruce 2: 1, Cruce Uniforme: 2"<<endl;
	//cin>>TYPE_CROSSOVER;
	//cout<<"Probabilidad de Cruce, valor entre 0 y 1"<<endl;
	//cin>>PROB_CROSSOVER;
	//cout<<"Probabilidad de Mutacion, valor entre 0 y 1"<<endl;
	//cin>>PROB_MUTATION;
	//cout<<"Elitismo, Si: 1, No: 0"<<endl;
	//cin>>FLAG_ELITISMO;
	//cout<<"Normalizacion Lineal, Si: 1, No: 0"<<endl;
	//cin>>FLAG_NORMALIZACION;

	Chrom poblacion[SIZE_POBLACION];
	float totalFitness;
	int t=0;
	inicializa(l, poblacion, SIZE_POBLACION,NUM_VARIABLES,RIGHT,LEFT,PRECISION);
	decodificacion(l,poblacion, SIZE_POBLACION, NUM_VARIABLES, RIGHT, LEFT);
	totalFitness = evalua(poblacion, SIZE_POBLACION,NUM_VARIABLES);
	float bestFitness[MAX_GEN];
	while(t < MAX_GEN){
		t=t+1;
		//guardar el mejor fitness de t-1
		float bestFitnessTmp=-INT_MAX;
		for(int i=0;i<SIZE_POBLACION;i++){
			if(bestFitnessTmp<poblacion[i].fitness){
				bestFitnessTmp = poblacion[i].fitness;
			}
		}
		bestFitness[t-1] = bestFitnessTmp;
		cout<<t-1<<" "<<bestFitness[t-1]<<endl;
		//
		Chrom poblacionTmp[SIZE_POBLACION];
		int contIndividuo=0;
		while(contIndividuo < SIZE_POBLACION){
			//seleccionar
			string bits1 = ruleta(poblacion,totalFitness,SIZE_POBLACION);
			string bits2 = ruleta(poblacion,totalFitness,SIZE_POBLACION);
			//operar
			crossover[TYPE_CROSSOVER](bits1,bits2);
			mutate(bits1);
			mutate(bits2);
			poblacionTmp[contIndividuo++]=Chrom(bits1,0.0f);
			poblacionTmp[contIndividuo++]=Chrom(bits2,0.0f);
		}
		for(int i=0;i<SIZE_POBLACION;i++){
			poblacion[i]=poblacionTmp[i];
		}
		decodificacion(l,poblacion, SIZE_POBLACION, NUM_VARIABLES, RIGHT, LEFT);
		totalFitness = evalua(poblacion, SIZE_POBLACION,NUM_VARIABLES);
		cout<<"TotalFitness"<<totalFitness<<endl;
	}
}
void inicializa(int *l, Chrom *poblacion, int SIZE_POBLACION, int NUM_VARIABLES, int * RIGHT, int *LEFT, int *PRECISION){
	for(int i=0;i<SIZE_POBLACION;i++){
		poblacion[i].bits = codificacion(l,NUM_VARIABLES,RIGHT,LEFT,PRECISION);
		poblacion[i].fitness = 0.0f;
	}
}
string codificacion(int *l, int NUM_VARIABLES, int *RIGHT, int *LEFT, int *PRECISION){
	string chromosoma;
	for (int i=0;i<NUM_VARIABLES;i++){
		l[i]=log(RIGHT[i]-LEFT[i]*pow(10,PRECISION[i]));
		chromosoma += randomBin(l[i]);
	}
	return chromosoma;
}
void decodificacion(int *l, Chrom *poblacion, int SIZE_POBLACION, int NUM_VARIABLES, int *RIGHT, int *LEFT){
	for (int k=0; k<SIZE_POBLACION; k++){
		for(int i=0;i<NUM_VARIABLES;i++){
			float suma = binToDecimal(poblacion[k].bits.substr(i*l[i], l[i]));
			float value = LEFT[i]+suma*(RIGHT[i]-LEFT[i])/(pow(2,l[i])-1);
			poblacion[k].X[i]=value;
		}
	}
}
float evalua(Chrom *poblacion, int SIZE_POBLACION,int NUM_VARIABLES){
	float totalFitness = 0.0f;
	for (int i=0; i<SIZE_POBLACION; i++){
		float sumaCuadrados=0.0f;
		for(int i=0;i <NUM_VARIABLES;i++){
			sumaCuadrados += std::pow(poblacion[i].X[i],2);
		}
		poblacion[i].fitness = calculateFitness(sumaCuadrados);
        totalFitness += poblacion[i].fitness;
    }
    return totalFitness;
}
string ruleta(Chrom *poblacion, float bm, int SIZE_POBLACION){
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, bm);
    float rangeInfFitness = 0.0f;
    int j=0;
    double uj = dis(gen);
    while(uj > rangeInfFitness && j < SIZE_POBLACION)
    {
    	rangeInfFitness += poblacion[j].fitness;
        j++;
    }
    return poblacion[j].bits;
}
//crossover 1 punto de cruce
void crossover1Punto(string &bits1, string &bits2)
{
	int lengthBits = bits1.length();
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, lengthBits);
  	int crossover = (int) (dis(gen));
    string t1 = bits1.substr(0, crossover) + bits2.substr(crossover, lengthBits);
    string t2 = bits2.substr(0, crossover) + bits1.substr(crossover, lengthBits);
    bits1 = t1; bits2 = t2;
}
void crossover2Punto(string &bits1, string &bits2){

}
void crossoverUniforme(string &bits1, string &bits2){

}
//mutacion un bit
void mutate(string &bits){
	int lengthBits = bits.length();
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, lengthBits);
	int mutate = (int) (dis(gen));
	if (mutate < lengthBits){
        if (bits.at(mutate) == '1'){
            bits.at(mutate) = '0';
        } else{
            bits.at(mutate) = '1';
        }
    }
    return;
}
float calculateFitness(float sumaCuadrados){
	float result = 0.5f-(std::pow(std::sin(std::sqrt(sumaCuadrados)),2)-0.5f)/(std::pow((1.0f+0.001f*(sumaCuadrados)),2));
	return result;
}
int binToDecimal(string bits)
{
    int val = 0;
    int base = 1;
    for (int i = bits.length(); i > 0; i--)
    {
        if (bits.at(i-1) == '1'){
        	val+=base;
        }
 	    base *= 2;

    }
    return val;
}
string randomBin(int length){
	string bits;
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
	for(int i=0;i<length;i++){
		double randomNum=dis(gen);
		if(randomNum>0.5){
			bits+="1";
		} else {
			bits+="0";
		}
	}
	return bits;
}
