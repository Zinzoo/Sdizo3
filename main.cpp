#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <math.h>
#include <string>
#include <iomanip>
#include <queue>
#include <vector>
#include <Windows.h>
using namespace std;

//const int MAXINT = 2147483647;
double PCFreq = 0.0;
__int64 licznik = 0;


int n, m, v0, d, dh, sptr, shptr, wybor;
int limit, maxWartosc = 0;
bool **A;                   
int **W;                   
int *S, *Sh;                     
bool *visited;
bool poczatek = true;

void czasStart()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "B³¹d!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	licznik = li.QuadPart;
}

void pobierzCzas()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	cout << endl;
	cout << "Operacja zajela: " << (li.QuadPart - licznik) / PCFreq << " milisekund" << endl;
	licznik = 0;
}
							
void Pakuj(int i, int *waga, int *wartosc, bool *rzecz, bool *wynik,
	int obecnaWartosc, int obecnaWaga)
{
	if (i == n)                               //jezeli jestesmy na jednym z koncow
	{
		if (obecnaWartosc>maxWartosc)   //porownaj wartosc plecaka 
		{
			maxWartosc = obecnaWartosc;
			for (int j = 0; j<n; j++)
				wynik[j] = rzecz[j];
		}
	}
	else
	{
		if (obecnaWaga + waga[i] <= limit)     //sprawdz czy element sie zmiesci
		{
			rzecz[i] = 1;             //dodaj go i sprawdzaj dalej
			obecnaWaga += waga[i];
			obecnaWartosc += wartosc[i];
			Pakuj(i + 1, waga, wartosc, rzecz, wynik, obecnaWartosc, obecnaWaga);
			rzecz[i] = 0;             //odejmij go i sprawdz inne mozliwosci
			obecnaWaga -= waga[i];
			obecnaWartosc -= wartosc[i];
		}
		Pakuj(i + 1, waga, wartosc, rzecz, wynik, obecnaWartosc, obecnaWaga);  //idz dalej
	}
}
							
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
	int *waga, *wartosc, wartoscPlecaka = 0, wagaPlecaka = 0;
	double *stosunek;
	bool *wynik, *rzecz;


	while (poczatek)
	{
		cout << "Wybierz problem" << endl << "1. Problem komiwojazera" << endl << "2. Problem plecakowy" << endl;
		cin >> wybor;
		system("cls");
		switch (wybor)
		{
		case 1:
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

			d = 2147483647;
			dh = v0 = 0;
			czasStart();
			TSP(v0);
			pobierzCzas();
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

			system("pause");
		}break;
		case 2:
		{
			cout << "Stworz plecak" << endl << "1. Wczytaj z pliku" << endl << "2. Wprowadz recznie" << endl;
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
					plik >> limit >> n;

					waga = new int[n];                //wagi przedmiotow
					wartosc = new int[n];             //wartosci przedmiotow
					stosunek = new double[n];         //do posortowania
					wynik = new bool[n];              //do zapisania wynikow
					rzecz = new bool[n];              //do zapamietania obecnych elementow

					for (i = 0; i < n; i++)
					{
						plik >> waga[i] >> wartosc[i];

						if (waga[i] > limit)      //jezeli waga przedmiotu przekracza limit
						{
							i--;  //nie uwzgledniaj go
							n--;
						}
						else
							stosunek[i] = (double)wartosc[i] / waga[i];
					}

					for (i = 0; i < n - 1; i++)
					{
						for (j = 0; j < n - i - 1; j++)
						{
							if (stosunek[j] < stosunek[j + 1])
							{
								swap(stosunek[j], stosunek[j + 1]);
								swap(waga[j], waga[j + 1]);
								swap(wartosc[j], wartosc[j + 1]);
							}
						}
					}

					delete(stosunek);

					/* Wywolanie wlasciwego algorytmu */
					czasStart();
					Pakuj(0, waga, wartosc, rzecz, wynik, 0, 0);
					pobierzCzas();
					delete(rzecz);

					/* Wypisanie wynikow */

					cout << endl << endl << "\tRzeczy w plecaku: " << endl;
					for (i = 0; i < n; i++)
						if (wynik[i])
						{
							wagaPlecaka += waga[i];
							wartoscPlecaka += wartosc[i];
							cout << "\tPrzedmiot o wadze " << waga[i];
							cout << " i o wartosci " << wartosc[i] << "." << endl;
						}
					cout << endl << "\tWaga plecaka: " << wagaPlecaka << endl;
					cout << "\tWartosc plecaka: " << wartoscPlecaka << "\n\n\t";

					delete(waga);
					delete(wartosc);
					delete(wynik);

					system("pause");

				}
			}break;

			case 2:
			{
				cout << endl << endl << "\tPodaj limit plecaka: ";
				cin >> limit;
				cout << "\tPodaj ilosc przedmiotow: ";
				cin >> n;
				cout << "\n\n";

				waga = new int[n];                //wagi przedmiotow
				wartosc = new int[n];             //wartosci przedmiotow
				stosunek = new double[n];         //do posortowania
				wynik = new bool[n];              //do zapisania wynikow
				rzecz = new bool[n];              //do zapamietania obecnych elementow

				for (i = 0; i<n; i++)
				{
					cout << "\tPodaj wage przedmiotu " << i << " : ";
					cin >> waga[i];
					cout << "\tPodaj wartosc przedmiotu: ";
					cin >> wartosc[i];
					cout << "\n\n";
					if (waga[i]>limit)      //jezeli waga przedmiotu przekracza limit
					{
						i--;  //nie uwzgledniaj go
						n--;
					}
					else
						stosunek[i] = (double)wartosc[i] / waga[i];
				}

				/* Posortowanie danych malejaco wzgledem stosunku wartosci do wagi */
				/* Sortowanie babelkowe */

				for (i = 0; i<n - 1; i++)
				{
					for (j = 0; j<n - i - 1; j++)
					{
						if (stosunek[j]<stosunek[j + 1])
						{
							swap(stosunek[j], stosunek[j + 1]);
							swap(waga[j], waga[j + 1]);
							swap(wartosc[j], wartosc[j + 1]);
						}
					}
				}

				delete(stosunek);

				/* Wywolanie wlasciwego algorytmu */
				czasStart();
				Pakuj(0, waga, wartosc, rzecz, wynik, 0, 0);
				pobierzCzas();
				delete(rzecz);

				/* Wypisanie wynikow */

				cout << endl << endl << "\tRzeczy w plecaku: " << endl;
				for (i = 0; i<n; i++)
					if (wynik[i])
					{
						wagaPlecaka += waga[i];
						wartoscPlecaka += wartosc[i];
						cout << "\tPrzedmiot o wadze " << waga[i];
						cout << " i o wartosci " << wartosc[i] << "." << endl;
					}
				cout << endl << "\tWaga plecaka: " << wagaPlecaka << endl;
				cout << "\tWartosc plecaka: " << wartoscPlecaka << "\n\n\t";

				delete(waga);
				delete(wartosc);
				delete(wynik);

				system("pause");
			}break;
			}
			
		}break;
		}
	}
}