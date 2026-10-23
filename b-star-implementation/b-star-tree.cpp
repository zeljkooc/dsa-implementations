#include <string>
#include <queue>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>

using namespace std;

typedef struct customer {
	long long int id;
	string name;
	string lastName;
	string email;
	long long int adId;
}Customer;

typedef struct cvorStabla {
	Customer cus[10];
	int brojCus;
	struct cvorStabla* sinovi[10];
	struct cvorStabla* otac;
}CvorStabla;

void ispisStabla(CvorStabla* koren);


CvorStabla* prelivanjeSaDesnim(CvorStabla* koren, CvorStabla* tren, Customer noviCus, int id_na_tren, int m) {
	vector <Customer> nizPrelivanja;
	long long id = noviCus.id;
	int brojElemenata = 0;
	for (int i = 0;i < tren->brojCus;++i) {
		if (id < tren->cus[i].id && id > tren->cus[i - 1].id) {
			nizPrelivanja.push_back(noviCus);
		}
		nizPrelivanja.push_back(tren->cus[i]);
	}
	nizPrelivanja.push_back(tren->otac->cus[id_na_tren]);
	CvorStabla* brat = tren->otac->sinovi[id_na_tren + 1];
	for (int i = 0;i < brat->brojCus;++i) {
		nizPrelivanja.push_back(brat->cus[i]);
	}

	int indeks = floor(nizPrelivanja.size() / 2);
	tren->otac->cus[id_na_tren] = nizPrelivanja[indeks];
	for (int i = 0;i < indeks;++i)
		tren->cus[i] = nizPrelivanja[i];
	tren->brojCus = indeks;
	for (int i = indeks + 1;i < nizPrelivanja.size();++i)
		brat->cus[i - indeks - 1] = nizPrelivanja[i];
	brat->brojCus = indeks;
	return koren;
}

CvorStabla* prelivanjeSaLevim(CvorStabla* koren, CvorStabla* tren, Customer noviCus, int id_na_tren, int m) {
	vector <Customer> nizPrelivanja;
	long long int id = noviCus.id;
	int brojElemenata = 0;
	CvorStabla* brat = tren->otac->sinovi[id_na_tren - 1];
	for (int i = 0;i < brat->brojCus;++i) {
		nizPrelivanja.push_back(brat->cus[i]);
	}
	nizPrelivanja.push_back(tren->otac->cus[id_na_tren-1]);
	for (int i = 0;i < tren->brojCus;++i) {
		if (id < tren->cus[i].id && id > tren->cus[i - 1].id) {
			nizPrelivanja.push_back(noviCus);
		}
		nizPrelivanja.push_back(tren->cus[i]);
	}
	if (id > tren->cus[tren->brojCus - 1].id)
		nizPrelivanja.push_back(noviCus);


	int indeks = floor(nizPrelivanja.size() / 2);
	tren->otac->cus[id_na_tren-1] = nizPrelivanja[indeks];
	for (int i = 0;i < indeks;++i)
		brat->cus[i] = nizPrelivanja[i];
	tren->brojCus = indeks;
	for (int i = indeks + 1;i < nizPrelivanja.size();++i)
		tren->cus[i - indeks - 1] = nizPrelivanja[i];
	brat->brojCus = indeks;
	return koren;
}

CvorStabla* prelamanjeSaDesnimBratom(CvorStabla* koren, CvorStabla* tren, Customer noviCus,int id_na_tren,int m) {
	long long int id = noviCus.id;
	vector <Customer> nizPrelamanja;
	int brojElemenata = 0;
	for (int i = 0;i < tren->brojCus;++i) {
		if (id < tren->cus[i].id && id > tren->cus[i - 1].id) {
			nizPrelamanja.push_back(noviCus);
		}
		nizPrelamanja.push_back(tren->cus[i]);
	}

	nizPrelamanja.push_back(tren->otac->cus[id_na_tren]);
	CvorStabla* brat = tren->otac->sinovi[id_na_tren + 1];
	for (int i = 0;i < brat->brojCus;++i) {
		nizPrelamanja.push_back(brat->cus[i]);
	}
	int levi = floor((2 * m - 2) / 3);
	int srednji = floor((2 * m - 1) / 3);
	int desni = floor((2 * m) / 3);


	CvorStabla* noviBrat = new CvorStabla();
	noviBrat->otac = tren->otac;
	tren->otac->sinovi[id_na_tren + 2] = noviBrat;

	tren->otac->brojCus++;
	tren->brojCus = levi;
	brat->brojCus = srednji;
	noviBrat->brojCus = desni;
	for (int i = 0;i < levi;++i)
		tren->cus[i] = nizPrelamanja[i];
	for (int i = levi + 1;i < levi + srednji + 1;++i)
		brat->cus[i - levi - 1] = nizPrelamanja[i];
	for (int i = levi + srednji + 2;i < nizPrelamanja.size();++i)
		noviBrat->cus[i - levi - srednji - 2] = nizPrelamanja[i];
	tren->otac->cus[id_na_tren] = nizPrelamanja[levi];
	tren->otac->cus[id_na_tren + 1] = nizPrelamanja[levi + 1 + srednji];
	return koren;
}



CvorStabla* prelamanjeSaLevimBratom(CvorStabla* koren, CvorStabla* tren, Customer noviCus, int id_na_tren, int m,int imaNovog) {
	long long int id = noviCus.id;
	int maxCusKoren = 2 * floor((2 * m - 2) / 3);
	vector <Customer> nizPrelamanja;
	int brojElemenata = 0;
	CvorStabla* brat = tren->otac->sinovi[id_na_tren - 1];
	for (int i = 0;i < brat->brojCus;++i) {
		nizPrelamanja.push_back(brat->cus[i]);
	}
	nizPrelamanja.push_back(tren->otac->cus[id_na_tren-1]);
	for (int i = 0;i < tren->brojCus;++i) {
		if (imaNovog && id < tren->cus[i].id && id > tren->cus[i - 1].id) {
			nizPrelamanja.push_back(noviCus);
		}
		nizPrelamanja.push_back(tren->cus[i]);
	}
	if (imaNovog && id > tren->cus[tren->brojCus - 1].id)
		nizPrelamanja.push_back(noviCus);

	int levi = floor((2 * m - 2) / 3);
	int srednji = floor((2 * m - 1) / 3);
	int desni = floor((2 * m) / 3);

	CvorStabla* noviBrat = new CvorStabla();
	noviBrat->otac = tren->otac;
	tren->otac->sinovi[id_na_tren + 1] = noviBrat;

	tren->otac->brojCus++;
	tren->brojCus = levi;
	brat->brojCus = srednji;
	noviBrat->brojCus = desni;
	for (int i = 0;i < levi;++i)
		brat->cus[i] = nizPrelamanja[i];
	for (int i = levi + 1;i < levi + srednji + 1;++i)
		tren->cus[i - levi - 1] = nizPrelamanja[i];
	for (int i = levi + srednji + 2;i < nizPrelamanja.size();++i)
		noviBrat->cus[i - levi - srednji - 2] = nizPrelamanja[i];
	tren->otac->cus[id_na_tren-1] = nizPrelamanja[levi];
	tren->otac->cus[id_na_tren] = nizPrelamanja[levi + 1 + srednji];

	
	return koren;
}

CvorStabla* levoPrelamanjeSaProverom(CvorStabla* koren, CvorStabla* tren, Customer noviCus, int id_na_tren, int m) {
	long long int id = noviCus.id;
	int maxCusKoren = 2 * floor((2 * m - 2) / 3);
	int maxCus = m - 1;

	koren = prelamanjeSaLevimBratom(koren, tren, noviCus, id_na_tren, m,1);
	
	CvorStabla* iter = tren;
	while (iter) {
		//Deljenje korena
		if (tren->otac->otac == NULL && tren->otac->brojCus > maxCusKoren) {
			int indeks = floor(koren->brojCus / 2);
			CvorStabla* noviKoren = new cvorStabla();
			CvorStabla* desni = new cvorStabla();
			noviKoren->otac = NULL;
			noviKoren->brojCus = 1;
			noviKoren->cus[0] = koren->cus[indeks];
			noviKoren->sinovi[0] = koren;
			noviKoren->sinovi[1] = desni;

			desni->brojCus = koren->brojCus - indeks - 1;

			koren->otac = noviKoren;
			desni->otac = noviKoren;

			for (int i = 0;i < desni->brojCus;++i)
				desni->cus[i] = koren->cus[i + indeks + 1];
			for (int i = 0;i < koren->brojCus - indeks;++i) {
				desni->sinovi[i] = koren->sinovi[i + indeks + 1];
				koren->sinovi[i + indeks + 1]->otac = desni;
				koren->sinovi[i + indeks + 1] = NULL;

			}

			koren->brojCus = indeks;
			return noviKoren;
		}
		else if (tren->otac != NULL && tren->otac->brojCus > maxCus) {			
			//Ako levi brat ima mesta
			if (tren->otac->otac != NULL && tren->otac->otac->sinovi[0]->brojCus < maxCus) {
				tren->otac->otac->sinovi[0]->sinovi[tren->otac->otac->sinovi[0]->brojCus] = tren->otac->sinovi[0];
				tren->otac->otac->sinovi[0]->cus[tren->otac->otac->sinovi[0]->brojCus] = tren->otac->otac->cus[0];
				tren->otac->otac->cus[0]= tren->otac->cus[0];
				for (int i = 0;i < tren->otac->brojCus;++i) {
					tren->otac->cus[i] = tren->otac->cus[i + 1];
					tren->otac->sinovi[i] = tren->otac->sinovi[i + 1];
				}
				tren->otac->otac->sinovi[0]->brojCus++;
				tren->otac->brojCus--;
			}
			else if (tren->otac->otac != NULL && tren->otac->otac->sinovi[0]->brojCus == maxCus) {
				prelamanjeSaLevimBratom(koren, tren->otac, noviCus, 1, m, 0);
				int levi = floor((2 * m - 2) / 3);
				int srednji = floor((2 * m - 1) / 3);
				int desni = floor((2 * m) / 3);

				//sinovi iz levog se prelancavaju u srednji
				for (int i = 0;i < maxCus - koren->sinovi[0]->brojCus;++i) {
					koren->sinovi[1]->sinovi[0] = tren->otac->otac->sinovi[0]->sinovi[maxCus-i];
				}
				//sinovi iz densog
				for (int i = 0;i < maxCus - desni;++i) {
					koren->sinovi[1]->sinovi[i+1]=tren->otac->otac->sinovi[2]->sinovi[i];
				}
				for (int i = 0;i < m- maxCus + desni;++i) {
					tren->otac->otac->sinovi[2]->sinovi[i]= tren->otac->otac->sinovi[2]->sinovi[i+maxCus - desni];

				}
			}
		}
		break;
	}
	return koren;
}


CvorStabla* dodavanjeCvora(CvorStabla* koren, Customer noviCus,int maxCus,int m) {
	int maxCusKoren=2*floor((2*m-2)/3);
	long long int id = noviCus.id;
	if (!koren)
		koren->cus[0] = noviCus;
	CvorStabla *tren = koren;
	int id_na_tren;
	int list=0;
	while (!list) {
		if (tren->sinovi[0] == NULL)
			list=1;
		for (int i = 0;i < tren->brojCus;++i) {
			if (!list && id < tren->cus[i].id) {
				tren = tren->sinovi[i];
				id_na_tren = i;
				break;
			}
			else if (tren->cus[i].id == id)
				return koren;
			else if (!list && id > tren->cus[i].id && i == tren->brojCus - 1) {
				tren = tren->sinovi[i + 1];
				id_na_tren = i+1;
				break;
			}
			
		}
	}
	//0.slucaj - puni se koren
	if(tren->otac==NULL && tren->brojCus==maxCusKoren){
		vector <Customer> nizRazlamanja;
		for (int i = 0;i < tren->brojCus;++i) {
			if (id < tren->cus[i].id && id > tren->cus[i - 1].id) {
				nizRazlamanja.push_back(noviCus);
			}
			
			nizRazlamanja.push_back(tren->cus[i]);
		}
		if (id > tren->cus[tren->brojCus - 1].id)
			nizRazlamanja.push_back(noviCus);

		int indeks = floor(nizRazlamanja.size() / 2);
		tren->brojCus = 1;
		tren->cus[0] = nizRazlamanja[indeks];
		tren->sinovi[0] = new CvorStabla();
		tren->sinovi[1] = new CvorStabla();
		tren->sinovi[0]->otac = tren;
		tren->sinovi[1]->otac = tren;
		tren->sinovi[0]->brojCus= 0;
		tren->sinovi[1]->brojCus = 0;
		for (int i = 0;i < indeks;++i) {
			tren->sinovi[0]->cus[i] = nizRazlamanja[i];
			tren->sinovi[0]->brojCus++;
		}
		for (int i = indeks+1;i < nizRazlamanja.size();++i) {
			tren->sinovi[1]->cus[i-indeks-1] = nizRazlamanja[i];
			tren->sinovi[1]->brojCus++;
		}

	}
	//1. slucaj - ima mesta u listu
	//Proverava se da li ima mesta u proizvoljnom cvoru i posebno za koren
	else if (tren->brojCus < maxCus || (tren->otac==NULL && tren->brojCus<maxCusKoren)) {
		//Dodavanje u niz trenutnog cvora na odgovarajucu poziciju
		int it=0;
		Customer pret=noviCus;
		while (it<tren->brojCus && id > tren->cus[it].id) it++;
		for (int i = it;i < tren->brojCus + 1;++i) {
			Customer p = pret;
			pret = tren->cus[i];
			tren->cus[i]=p;
		}
		tren->brojCus++;
	}
	//Pozajmica od brata
	//Ima desnog brata i 
	//Ako on ima mesta za prelivanje
	else if (tren->otac->sinovi[id_na_tren + 1] != NULL && tren->otac->sinovi[id_na_tren + 1]->brojCus < maxCus) {
		//Radi se prelivanje sa prvim desnim bratom
		koren = prelivanjeSaDesnim(koren, tren, noviCus, id_na_tren, m);
	}
	//Prvi levi
	else if (id_na_tren>0 && tren->otac->sinovi[id_na_tren - 1] != NULL && tren->otac->sinovi[id_na_tren - 1]->brojCus < maxCus) {
		koren = prelivanjeSaLevim(koren, tren, noviCus, id_na_tren, m);
	}
	//Ako ima desnog brata sada se radi prelamanje
	else if(tren->otac->sinovi[id_na_tren + 1] != NULL){
		cout << "a";
		koren = prelamanjeSaDesnimBratom(koren, tren, noviCus, id_na_tren, m);
	}
	else if (id_na_tren > 0 && tren->otac->sinovi[id_na_tren - 1] != NULL) {
		koren = levoPrelamanjeSaProverom(koren, tren, noviCus, id_na_tren, m);
	}
	return koren;
}

Customer pretraga(CvorStabla* koren, long long int id,int *koraci) {
	
	CvorStabla* tren = koren;
	while (tren) {
		for (int i = 0;i < tren->brojCus;++i) {
			++*koraci;
			if (tren->cus[i].id == id)
				return tren->cus[i];
			else if (id < tren->cus[i].id) {
				tren = tren->sinovi[i];
				break;
			}
		}
		if (id > tren->cus[tren->brojCus - 1].id)
			tren = tren->sinovi[tren->brojCus];
		else if (id == tren->cus[tren->brojCus - 1].id)
			return tren->cus[tren->brojCus-1];
	}
}

void ispisStabla(CvorStabla* koren) {
	CvorStabla* tren;
	queue <CvorStabla*> red;
	queue <CvorStabla*> sinovi;
	CvorStabla* pretOtac = NULL;
	CvorStabla* pretDeda = NULL;
	red.push(koren);
	int i = 0;
	while (!sinovi.empty() || !red.empty()) {
		if (red.empty()) {
			cout << "\n";
			while (!sinovi.empty()) {
				red.push(sinovi.front());
				sinovi.pop();
			}
		}
		tren = red.front();
		red.pop();
		pretOtac = tren->otac;
		for (int i = 0;i < tren->brojCus;++i) {
			cout << " " << tren->cus[i].id << " ";
			if (tren->sinovi[i] != NULL)
				sinovi.push(tren->sinovi[i]);
		}
		if (tren->sinovi[tren->brojCus] != NULL)
			sinovi.push(tren->sinovi[tren->brojCus]);
		cout << " | ";
	}
	cout << endl;
	cout << endl;
}

int kreiranjeNizaK(long long int nizK[100]) {
	int k;
	cout << "Unesite broj k: ";
	cin >> k;
	cout << "Unesite "<<k<<" primarnih kljuceva za pretragu : \n";
	long long int broj;
	for (int i = 0;i < k;++i) {
		cin>>nizK[i];
	}
	return k;
}


//Delimicno preuzeto iz knjige
void inOrder(CvorStabla* koren) {

	stack <Customer> stek;
	stack <CvorStabla*> stekCvor;
	CvorStabla* tren = koren;
	Customer trenCus;
	int s = 0;
	while (1) {
		
		while (tren->sinovi[0]) {
			stek.push(tren->cus[0]);
			stekCvor.push(tren);
			tren = tren->sinovi[0];
		}

		/*cout << tren->cus[0].id << "   ";
		while (tren) {
			tren = tren->otac;
			for (int i = 0;i < tren->brojCus+1;++i) {
				for (int j = 0;j < tren->sinovi[i]->brojCus;++j)
					cout << tren->sinovi[i]->cus[j].id<<" ";
				cout << tren->cus[i].id<<" ";
			}
		}*/
		if (!stek.empty()) {
			trenCus = stek.top();
			stek.pop();
			cout << trenCus.id << " ";
			for (int i = 1;i < tren->brojCus;++i)
				stek.push(tren->sinovi[i]->cus[0]);
		}



		for (int j = 0;j < tren->brojCus;++j)
			cout << tren->cus[j].id << " ";

		tren = tren->otac->otac;
		s++;
		cout << tren->cus[s - 1].id;
		tren = tren->sinovi[s];
		while (tren->sinovi[0])
			tren = tren->sinovi[0];
		while (tren) {

			for (int i = 0;i < tren->brojCus + 1;++i) {
				for (int j = 0;j < tren->sinovi[i]->brojCus;++j)
					cout << tren->sinovi[i]->cus[j].id << " ";
				cout << tren->cus[i].id << " ";
			}
		}
	}

}




void inOrder3(CvorStabla* koren, long long int k[100]) {
	CvorStabla* tren = koren;
	stack <CvorStabla*> stek;
	stek.push(koren);
	int s = 0;
	while (1) {
		while (tren) {
			stek.push(tren);
			tren = tren->sinovi[0];
		}
		if (!stek.empty()) {
			tren = stek.top();
			stek.pop();
			cout << tren->cus[s].id << " ";
			tren = tren->sinovi[s];
			s++;
			//for (int i = 1;i < tren->brojCus;++i)
				//stek.push(tren->sinovi[i]);
		}
		else
			return;
	}
}

int j = 0;

void inOrder2(CvorStabla* koren,int k,long long int nizK[100],ofstream &output) {
	if (!koren || j>k)
		return;
	for (int i = 0;i < koren->brojCus+1;++i) {
		inOrder2(koren->sinovi[i],k,nizK,output);
		if(i!=koren->brojCus)
			if (nizK[j] == koren->cus[i].id) {
				output << koren->cus[i].id << "|";
				output << koren->cus[i].name << "|";
				output << koren->cus[i].lastName << "|";
				output << koren->cus[i].email << "|";
				output << koren->cus[i].adId << "\n";
				j++;
			}
	}
}

void pretragaPoImenu(CvorStabla* koren,string ime) {
	int brojKoraka = 0;
	CvorStabla* tren;
	queue <CvorStabla*> red;
	red.push(koren);
	int i = 0;
	while (!red.empty()) {
		tren = red.front();
		red.pop();
		for (int i = 0;i < tren->brojCus;++i) {
			brojKoraka++;
			if (tren->cus[i].name==ime) {
				cout << "Pretraga po imenu je izvrsena za " << brojKoraka << " koraka\n\n";
				cout << "Podaci o nadjenom redu:\n";
				cout << tren->cus[i].id<<"|";
				cout << tren->cus[i].name << "|";
				cout << tren->cus[i].lastName << "|";
				cout << tren->cus[i].email << "|";
				cout << tren->cus[i].adId << "\n";
				return;
			}
			if (tren->sinovi[i] != NULL)
				red.push(tren->sinovi[i]);
		}
		if (tren->sinovi[tren->brojCus] != NULL)
			red.push(tren->sinovi[tren->brojCus]);
	}
	cout << "Osoba sa tim imenom nije nadjena.\n";
}

CvorStabla* brisanjeStabla(CvorStabla* koren) {
	CvorStabla* tren;
	queue <CvorStabla*> red;
	red.push(koren);
	while (!red.empty()) {
		tren = red.front();
		red.pop();
		for (int i = 0;i < tren->brojCus;++i) {
			if (tren->sinovi[i] != NULL)
				red.push(tren->sinovi[i]);
		}
		if (tren->sinovi[tren->brojCus] != NULL)
			red.push(tren->sinovi[tren->brojCus]);
		delete tren;
	}
	return NULL;
}

int main() {
	int m;
	cout << "Unesite red stabla m: ";
	cin >> m;


	CvorStabla* koren = new CvorStabla();
	koren->otac = NULL;

	Customer cus[101];
	int brojCus = 0;
	ifstream dat;
	dat.open("customer20.txt");
	if (!dat.is_open()) cout << "Greska";
	string red;
	while (!dat.eof()) {
		dat >> red;
		int i = 0;
		string ids, name, lastName, email, adIds;
		stringstream str(red);
		getline(str, ids, '|');
		getline(str, name, '|');
		getline(str, lastName ,'|');
		getline(str, email ,'|');
		getline(str, adIds ,'|');

		long long int id = stoll(ids);
		long long int idA = stoll(adIds);
		cus[brojCus].id = id;
		cus[brojCus].name= name;
		cus[brojCus].lastName= lastName;
		cus[brojCus].email= email;
		cus[brojCus].adId= idA;
		koren = dodavanjeCvora(koren, cus[brojCus], m - 1, m);
		++brojCus;
	}
	dat.close();
	

	Customer noviCus;
	Customer cust;
	string s;
	long long int nizK[100];

	ofstream output;
	output.open("output.txt");
	if (!output.is_open()) cout << "Greska";
	string out;

	string ime;
	int k;
	while (1) {
			printf("-=Meni=-\n");
			printf("1.Ispis B* stabla\n");
			printf("2.Dodavanje novog zapisa u tabelu\n");
			printf("3.Pretraga podataka po primarnom kljucu\n");
			printf("4.Pretraga podataka po imenu\n");
			printf("5.Pretraga k podataka po primarnim kljucevima\n");
			printf("0.Izlaz\n\n");
			printf("Unesite stavku menija: ");
			int stavka;
			int koraci;
			long long int id;
			cin >> stavka;
			switch (stavka) {
			case 1:
				ispisStabla(koren);
				break;
			case 2:
				printf("Unesite podatke novog zapisa:\n ");
				printf("id: ");
				cin >> noviCus.id;
				printf("ime: ");
				cin >> noviCus.name;
				printf("prezime: ");
				cin >> noviCus.lastName;
				printf("email: ");
				cin >> noviCus.email;
				printf("Kljuc na tabelu adresa: ");
				cin >> noviCus.adId;
				koren = dodavanjeCvora(koren, noviCus, m - 1, m);
				break;
			case 3:
				cout << "Unesite primarni kljuc za pretragu: ";
				cin >> id;
				koraci = 0;
				 cust = pretraga(koren, id, &koraci);
				cout << "Podaci o nadjenom zapisu: \n";
				printf("Id: ");
				cout << cust.id;
				printf("\nIme: ");
				cout << cust.name;
				printf("\nPrezime: ");
				cout << cust.lastName;
				printf("\nEmail: ");
				cout << cust.email;
				printf("\nKljuc na tabelu adresa: ");
				cout << cust.adId<<"\n";
				printf("Ukupan broj koraka pretrage: %d\n\n",koraci);
				break;
			case 4:
				cout << "Unesite ime za pretragu: ";
				cin >> ime;
				pretragaPoImenu(koren, ime);
				break;
			case 5:
				k=kreiranjeNizaK(nizK);
				inOrder2(koren,k,nizK,output);
				output.close();

				cout << "\n";
				break;
			case 0:
				brisanjeStabla(koren);
				return 0;
			}
		}
		
		int koraci = 0;
		return 0;
	}
