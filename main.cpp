#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
//#include <windows.h>
#include <math.h>
#include <string>
#include <iomanip>
#include <queue>
#include <vector>
using namespace std;

const int MAXINT = 2147483647;


int n, m, v0, d, dh, sptr, shptr, wybor;
bool **A;                   
int **W;                   
int *S, *Sh;                     
bool *visited;
bool poczatek = true;

							
						
							
void TSP(int v)
{
	int u;

	Sh[shptr++] = v;            

	if (shptr < n)             
	{
		visited[v] = true;         
		for (u = 0; u < n; u++)      
			if (A[v][u] && !visited[u])  
			{
				dh += W[v][u];          
				TSP(u);               
				dh -= W[v][u];         
			}
		visited[v] = false;      
	}
	else if (A[v0][v])            
	{
		dh += W[v][v0];            
		if (dh < d)                 
		{
			d = dh;                
			for (u = 0; u < shptr; u++) 
				S[u] = Sh[u];
			sptr = shptr;
		}
		dh -= W[v][v0];           
	}
	shptr--;                     
}


int main()
{
	int i, j, x, y, z;

	while (poczatek)
	{
		cout << "Stworz graf" << endl << "1. Wczytaj z pliku" << endl << "2. Wprowadz recznie" << endl;
		cin >> wybor;
		system("cls");
		switch (wybor)
		{
		case 1:
		{
			string nazwa;
			cout << "Podaj nazwe pliku" << endl;
			cin >> nazwa;
			nazwa = nazwa + ".txt";
			ifstream plik(nazwa);
			if (!plik)
			{
				cout << "Nie mozna otworzyc pliku" << endl;
				poczatek = true;
			}

			else
			{
				plik >> n >> m;

				S = new int[n];
				Sh = new int[n];
				visited = new bool[n];
				A = new bool *[n];
				W = new int *[n];
				for (i = 0; i < n; i++)
				{
					A[i] = new bool[n];
					W[i] = new int[n];
					for (j = 0; j < n; j++)
					{
						A[i][j] = false;
						W[i][j] = 0;
					}
					visited[i] = false;
				}
				sptr = shptr = 0;

				for (i = 0; i < m; i++)
				{
					plik >> x >> y >> z;
					A[x][y] = A[y][x] = true;
					W[x][y] = W[y][x] = z;
				}

				cout << endl;
			}
		}break;

		case 2:
		{
			cout << "Podaj liczbe wierzcholkow oraz krawedzi" << endl;

			cin >> n >> m;

			S = new int[n];
			Sh = new int[n];
			visited = new bool[n];
			A = new bool *[n];
			W = new int *[n];
			for (i = 0; i < n; i++)
			{
				A[i] = new bool[n];
				W[i] = new int[n];
				for (j = 0; j < n; j++)
				{
					A[i][j] = false;
					W[i][j] = 0;
				}
				visited[i] = false;
			}
			sptr = shptr = 0;

			cout << "Podaj wagi krawedzi" << endl;

			for (i = 0; i < m; i++)
			{
				cin >> x >> y >> z;
				A[x][y] = A[y][x] = true;
				W[x][y] = W[y][x] = z;
			}

			cout << endl;
		}break;
		}

		d = MAXINT;
		dh = v0 = 0;
		TSP(v0);
		if (sptr)
		{
			for (i = 0; i < sptr; i++) cout << S[i] << " ";
			cout << v0 << endl;
			cout << "d = " << d << endl;
		}
		else cout << "NO HAMILTONIAN CYCLE" << endl;

		cout << endl;



		delete[] S;
		delete[] Sh;
		delete[] visited;

		for (i = 0; i < n; i++)
		{
			delete[] A[i];
			delete[] W[i];
		}

		delete[] A;
		delete[] W;

		return 0;
	}
}