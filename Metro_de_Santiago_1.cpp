#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <locale.h>
#define INFINITO 10000000
#define MAXIMO 10000

using namespace std;
int Ruta[MAXIMO];

// Buscar el indicador de la estacion en relacion a su nombre
int Buscar (vector <string> Metro, string Estacion)
{
	int Indice;//aloja el indice correspondiente a la estacion
	bool InLista = false;
	for (int i=0; i< Metro.size() ; i++)//recorre la lista con los nombres de las estaciones
	{
		if(Metro[i] == Estacion)//si el la lista en la posicion i es igual al que se busca
		{
			Indice = i;
			InLista = true;//dice que es verdadero
		}
	}
	if(InLista == true)
	{
		return Indice;//y devuelve el indice
	}
	else
	{
		return -1;//sino devuelve -1
	}
}


class MapaMetro
{

private:
	// cantidad de vertices
	int Vertices; 
	// indicador de posiciones dentro de una lista de adyacencia
	list<pair<int, int> > * ListaAdyacencia;

public:
	// construye el grafo en relacion a la cantidad de vertices (matriz de nxn)
	MapaMetro(int Vertices)
	{
		this->Vertices = Vertices; 
		ListaAdyacencia = new list<pair<int, int> >[Vertices];
		
	}

	//crea arista para unir vertices y determinar todas las combinaciones posibles
	void Combinaciones(int VI, int VF, int peso)
	{
		ListaAdyacencia[VI].push_back(make_pair(VF, peso));//se agrega a la lista el valor de adyacencia desde el inicio hasta el fin
	}

	//muestra todas las estaciones recorridas hasta llegar a la uttima
	void MostrarRuta(int destino, vector<string> listaMetro)
	{
		if(Ruta[destino] != -1)//debe ser distinto a -1 para encontrarse dentro de la lista
		{
			MostrarRuta(Ruta[destino],listaMetro);//se repite el proceso hasta llegar a la ultima estacion
		}
		cout << listaMetro[destino] << endl;//muestra las estaciones
	}

	// proceso en el cual se encuentra el minimo de estaciones
	int dijkstra(int Inicio, int Fin, vector <string> listaMetro)
	{

		int Distancia[Vertices];//Vector de distancias
		
		int visitados[Vertices];//Visitados sirve en el caso de que ya se haya pasado por ese vertice

		// fila de prioridades(distancia, vértice)
		priority_queue < pair<int, int>,
					   vector<pair<int, int> >, greater<pair<int, int> > > pq;

		// inicia vector distancia y visitados
		for(int i = 0; i < Vertices; i++)
		{
			Distancia[i] = INFINITO;
			visitados[i] = false;
			Ruta[i] = -1;
		}

		Distancia[Inicio] = 0;// distancia parte en 0 porque no a recorrido nada

		pq.push(make_pair(Distancia[Inicio], Inicio)); //ETIQUETA 
		
		// loop del algoritmo
		while(!pq.empty())//mientras pq no este vacio
		{
			pair<int, int> p = pq.top();//ve el par de todo
			int u = p.second; //saca el vertice en relacion al par
			pq.pop(); //lo quita de pq
			
			//revisa si el vertice se movio
			if(visitados[u] == false)
			{
				//cuando es true es porque ya fue visitado
				visitados[u] = true;
				list<pair<int, int> >::iterator it;

				// Recorre los vertices adyacentes de u
				for(it = ListaAdyacencia[u].begin(); it != ListaAdyacencia[u].end(); it++)
				{
					//obtiene el vertice adyacente y el peso de la arista
					int v = it->first;
					int PesoArista = it->second;
					if(Distancia[v] > (Distancia[u] + PesoArista))
					{
						//actualiza la distancia entre vertices
						Distancia[v] = Distancia[u] + PesoArista;
						pq.push(make_pair(Distancia[v], v));
						Ruta[v]=u;//encuentra la estacion actual
					}
				}
				
			}
		}

		// devuelve distancia minima y ruta 
		cout << "\n=== RUTA MINIMA DE VIAJE ===\n" << endl;
		MostrarRuta(Fin,listaMetro);
		cout << "===============================" << endl;
		return Distancia[Fin];
	}
	
};

int main()
{
	//lectura de archivos txt 
	setlocale(LC_ALL, "Spanish");
	ifstream EstMetro, ID_Estaciones;
	string Estaciones, ID, EstacionA, EstacionB, Seguir;
	bool Condicion;
	int EstA,EstB;
	vector <string> ListaMetro;
	vector <int> ListaID;
	
	EstMetro.open("Metro.txt",ios::in); 
	
	int ValorID;
	ID_Estaciones.open("ID.txt",ios::in); 

	if(EstMetro.fail())
	{
		cout << "No se puede leer 'Metro.txt''" << endl;
	}

	if(ID_Estaciones.fail())
	{
		cout << "No se puede leer 'ID.txt'" << endl;
	}

	while(!EstMetro.eof())
	{ 
		getline(EstMetro,Estaciones);
		ListaMetro.push_back(Estaciones);
	}

	while(!ID_Estaciones.eof())
	{ 
		getline(ID_Estaciones,ID);
		ValorID = atoi(ID.c_str());
		ListaID.push_back(ValorID);
	}
	
	vector <string> ListMetro = ListaMetro;
	vector <int> ListID = ListaID;
	
	EstMetro.close();
	ID_Estaciones.close();
	
	MapaMetro cant(119);// se asignan la cantidad de vertices perteneciente al grafo
	
	//agrega las aristas
	for (int i = 0; i < ListID.size() ;i++ )
	{
		int A = ListID[i];
		int B = ListID[i+1];
		
		if( A == -1 || B == -1)//si el dato es menos uno no lo agrega porque es un cambio de linea
		{
			continue;
		}
		
		else
		{
			//agrega las conecciones entre lineas
			cant.Combinaciones(A, B, 1);
			cant.Combinaciones(B, A, 1);
		}
	}
	
	Condicion = true;
	
	//Interfaz para el usuario
	while(Condicion)
	{
		cout << endl;
		cout << "========= BIENVENIDO AL METRO DE SANTIAGO ==========\n" 
		<< "====================================================\n" 
		<< "PD: \nSi una estacion de metro tiene una letra ñ en \nsu nombre reemplacela por una n y omita tildes,\nademás, cada palabra del nombre de la estación \ninicia con maysucula excepto los conectores de y la.\n" 
		<< "====================================================\n" 
		<< "EJ: \n\t Ines de Suarez \n\t Plaza de Puente Alto \n\t Lo Valledor \n\t Bellavista de la Florida \n\t Nunoa\n"
		<< "====================================================" << endl;
		cout << "Ingrese su estacion de partida: ";
		getline(cin,EstacionA);
		EstA = Buscar(ListMetro, EstacionA);
		if(EstA == -1)
		{
			cout << "Vuelva a intentar," << "'" << EstacionA << "'" << ", no existente o mal escrita"<< endl;
			cout << endl;
		}
		
		else
		{
			cout << "Ingrese su estacion de destino: ";
			getline(cin,EstacionB);
			EstB = Buscar(ListMetro,EstacionB);
			if(EstB == -1)
			{
				cout << "Vuelva a intentar," << "'" << EstacionB << "'" << ", no existente o mal escrita"<< endl;
				cout << endl;
			}
			else
			{
				cout <<"La cantidad  de estaciones entre " << EstacionA << " y " << EstacionB << " es de " 
				<< cant.dijkstra(EstA, EstB, ListMetro) << "\n" << endl;
				cout << endl;
				
				cout << "¿Desea continuar?(s/n)" << endl;
				cin >> Seguir;
				if(Seguir == "s")
				{
					Condicion = true;
				}
				else
				{
					Condicion = false;
				}
			}	
		}
	}
	exit(1);
	return 0;
}
