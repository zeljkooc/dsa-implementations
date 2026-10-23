#include <iostream>
#include <stack>
#include <queue>

using namespace std;

typedef struct kontakt {
	string ime;
	string prezime;
	string broj;
}Kontakt;

struct CvorBST {
	Kontakt kont;
	struct CvorBST* otac;
	struct CvorBST* levi;
	struct CvorBST* desni;
};

typedef struct elemStek {
	CvorBST* cvor;
	int low;
	int high;
}ElemStek;

//Funkcija vraca broj kontakata
int kreiranjeNizaKontakta(FILE *dat, Kontakt niz[101]) {
	char a[101];

	char imep[50];
	char prezimep[50];
	char brojp[50];
	char c;
	while ((c = fgetc(dat)) != '\n') {
	}

	int brojKontakta=0;

	while (1) {
		char* ime = imep;
		char* prezime = prezimep;
		char* broj = brojp;
		while ((c = fgetc(dat)) != ',') {
			if (c == EOF) break;
			*prezime++ = c;
		}
		while ((c = fgetc(dat)) != ',') {
			if (c == EOF) break;
			*ime++ = c;
		}
		while ((c = fgetc(dat)) != '\n') {
			if (c == EOF) break;
			*broj++ = c;
		}
		*ime = '\0';
		*prezime = '\0';
		*broj = '\0';
		if (ime[0] == '\n') break;
		niz[brojKontakta].ime = imep;
		niz[brojKontakta].prezime = prezimep;
		niz[brojKontakta].broj = brojp;
		++brojKontakta;
		if (c == EOF) break;
	}
	return brojKontakta-1;
}

//Kod sa prezentacije vezbi za kreiranje balansiranog stabla
CvorBST* createBST(Kontakt niz[101], int n) {
	int low = 0, high = n-1;
	stack<ElemStek> stek;
	int mid = (low + high) / 2;
	CvorBST* koren =new CvorBST();
	if (!koren)
		cout << "neuspesna alokacija";
	koren->kont.ime = niz[mid].ime;
	koren->kont.prezime = niz[mid].prezime;
	koren->kont.broj = niz[mid].broj;
	koren->otac = NULL;
	CvorBST* sled = koren;
	ElemStek elem;
	//Ubacivanje desne strane niza u stek
	elem.cvor = sled;
	elem.low = mid + 1;
	elem.high = high;
	stek.push(elem);
	high = mid - 1;
	
	while (low <= high) {
		sled->levi = new CvorBST();
		sled->levi->otac = sled;
		sled = sled->levi;
		
		int mid = (low + high) / 2;
		sled->kont.ime = niz[mid].ime;
		sled->kont.prezime = niz[mid].prezime;
		sled->kont.broj = niz[mid].broj;

		elem.cvor = sled;
		elem.low = mid + 1;
		elem.high = high;
		stek.push(elem);
		high = mid - 1;
	}
	sled->levi = NULL;
	while (!stek.empty()) {
		ElemStek p=stek.top();
		stek.pop();
		sled = p.cvor;
		low = p.low;
		high = p.high;
		if (low <= high) {
			sled->desni = new CvorBST();
			sled->desni->otac = sled;
			sled = sled->desni;
			int mid = (low + high) / 2;
			sled->kont.ime = niz[mid].ime;
			sled->kont.prezime = niz[mid].prezime;
			sled->kont.broj = niz[mid].broj;

			elem.cvor = sled;
			elem.low = mid + 1;
			elem.high = high;
			stek.push(elem);
			high = mid - 1;
			while (low <= high) {
				sled->levi = new CvorBST();
				sled->levi->otac = sled;
				sled = sled->levi;
				int mid = (low + high) / 2;
				sled->kont.ime = niz[mid].ime;
				sled->kont.prezime = niz[mid].prezime;
				sled->kont.broj = niz[mid].broj;

				elem.cvor = sled;
				elem.low = mid + 1;
				elem.high = high;
				stek.push(elem);
				high = mid - 1;
			}
			sled->levi = NULL;
		}
		else
			sled->desni = NULL;
	}
	return koren;
}

CvorBST* brisanjeStablaLevelOrder(CvorBST* koren) {
	queue<CvorBST*> sinovi;
	sinovi.push(koren);
	CvorBST* tren;
	while (!sinovi.empty()) {
		tren = sinovi.front();
		sinovi.pop();
		if (tren->levi) {
			sinovi.push(tren->levi);
		}
		if (tren->desni)
			sinovi.push(tren->desni);
		delete tren;
	}
	return NULL;
}

void ispisBST(CvorBST* koren) {
	if (!koren) return;
	queue<CvorBST*> trenutni;
	queue<CvorBST*> sinovi;
	trenutni.push(koren);
	CvorBST* tren = NULL;
	CvorBST* nivo = koren;
	int level = 0;
	int zadnjiRed = 0;
	int obidjeniSvi = 0;
	CvorBST* pret;

	cout << "\n";
	do {
		obidjeniSvi = 0;
		
		tren = NULL;
		while (!trenutni.empty()) {
			pret = tren;
			tren = trenutni.front();

			CvorBST* p = new CvorBST();
			p->kont.ime = "USER";
			p->kont.prezime = "USERLN";
			p->kont.broj = "ID";
			 
			if (pret != NULL)
				if (pret->otac->kont.broj != tren->otac->kont.broj) {
					cout << "| ";
				}
			
			if (tren->kont.ime != "USER")
				cout << tren->kont.prezime + " " + tren->kont.ime+ "-" + tren->kont.broj;
			else
				cout << "/";

			trenutni.pop();

			if (tren->levi) {
				sinovi.push(tren->levi);
				obidjeniSvi = 1;
			}
			else {
				p->otac = tren;
				sinovi.push(p);
				zadnjiRed = 1;
			}
			cout << "  ";
			if (tren->desni) {
				obidjeniSvi = 1;
				sinovi.push(tren->desni);
			}
			else {
				p->otac = tren;
				sinovi.push(p);
				zadnjiRed = 1;
			}
		}
		while (!sinovi.empty()) {
			trenutni.push(sinovi.front());
			sinovi.pop();
		}
		cout << "\n\n";
	} while (!trenutni.empty() && obidjeniSvi);
}

int pretragaPoPrezimenu(CvorBST* koren,string prezime, CvorBST* nizNadjenih[101]) {
	CvorBST* tren = koren;
	int brojNadjenih = 0;
	stack<CvorBST*> stek;
	//Knjiga
	while (tren && tren->kont.prezime != prezime) {
		if (prezime >tren->kont.prezime)
			tren = tren->desni;
		else
			tren = tren->levi;
	}
	if (!tren)
		return 0;
	stek.push(tren);
	
	int i = 1;
	while (!stek.empty()) {
		tren = stek.top();
		nizNadjenih[brojNadjenih] = tren;
		brojNadjenih++;
		stek.pop();
		CvorBST* p = tren;
		while (1) {
			if (p->levi) {
				if (p->levi->kont.prezime == prezime) {
					stek.push(p->levi);
					break;
				}
				else {
					p = p->levi;
					while (p->desni)
						p = p->desni;
				}
			}
			else {
				break;
			}
			if (p->kont.prezime == prezime)
				stek.push(p);
			else
				break;
		}
		p = tren;
		while (1) {
			if (p->desni) {
				if (p->desni->kont.prezime == prezime) {
					stek.push(p->desni);
					break;

				}
				else {
					p = p->desni;
					while (p->levi)
						p = p->levi;
					
				}
			}
			else {
				break;
			}
			if (p->kont.prezime == prezime)
				stek.push(p);
			else
				break;
		}
	}
	return brojNadjenih;
}

int pretragaPoImenuiPrezimenu(CvorBST* koren, string prezime,string ime,CvorBST* nizNadjenih[101]) {
	CvorBST* tren = koren;
	int brojNadjenih = 0;
	stack<CvorBST*> stek;
	//Knjiga
	while (tren && (tren->kont.prezime != prezime || tren->kont.ime != ime)) {
		if (prezime > tren->kont.prezime)
			tren = tren->desni;
		else if (prezime < tren->kont.prezime)
			tren = tren->levi;
		else if (ime > tren->kont.ime)
			tren = tren->desni;
		else
			tren = tren->levi;
	}
	if (!tren)
		return 0;

	CvorBST* p = tren;
	int i = 1;
	stek.push(tren);

	while (!stek.empty()) {
		tren = stek.top();
		nizNadjenih[brojNadjenih] = tren;
		brojNadjenih++;
		stek.pop();
		CvorBST* p = tren;
		while (1) {
			if (p->levi) {
				if (p->levi->kont.prezime == prezime && p->levi->kont.ime == ime) {
					stek.push(p->levi);
					break;
				}
				else {
					p = p->levi;
					while (p->desni)
						p = p->desni;
				}
			}
			else {
				break;
			}
			if (p->kont.prezime == prezime && p->kont.ime == ime)
				stek.push(p);
			else
				break;
		}
		p = tren;
		while (1) {
			if (p->desni) {
				if (p->desni->kont.prezime == prezime && p->desni->kont.ime == ime) {
					stek.push(p->desni);
					break;

				}
				else {
					p = p->desni;
					while (p->levi)
						p = p->levi;

				}
			}
			else {
				break;
			}
			if (p->kont.prezime == prezime && p->kont.ime == ime)
				stek.push(p);
			else
				break;
		}
	}
	return brojNadjenih;
}

//Kod je delimicno preuzet iz knjige
CvorBST* dodavanjeKontakta(CvorBST* koren, string prezime, string ime, string broj) {
	CvorBST* novi = new CvorBST();
	novi->kont.broj = broj;
	novi->kont.ime = ime;
	novi->kont.prezime = prezime;
	novi->desni = NULL;
	novi->levi= NULL;
	int prethodnik = 0;
	CvorBST* tren = koren;
	CvorBST* pret = NULL;
	while (tren) {
		pret = tren;
		if (novi->kont.prezime == tren->kont.prezime && novi->kont.ime == tren->kont.ime && novi->kont.broj == tren->kont.broj) {
			cout << "Kontakt vec postoji!\n";
			return koren;
		}
		else if (novi->kont.prezime == tren->kont.prezime && novi->kont.ime == tren->kont.ime) {
			if (tren->levi) {
				tren = tren->levi;
				while (tren->desni)
					tren = tren->desni;
			}
			else {
				prethodnik = 1;
				tren = tren->levi;
			}
		}
		else if (novi->kont.prezime < tren->kont.prezime)
			tren = tren->levi;
		else if(novi->kont.prezime > tren->kont.prezime)
			tren = tren->desni;
		else if(novi->kont.ime < tren->kont.ime)
			tren = tren->levi;
		else 
			tren = tren->desni;
	}
	if (pret == NULL)
		koren = novi;
	else {
		novi->otac = pret;
		if (novi->kont.prezime < pret->kont.prezime || prethodnik)
			pret->levi = novi;
		else if (novi->kont.prezime > pret->kont.prezime)
			pret->desni = novi;
		else if (novi->kont.ime < pret->kont.ime)
			pret->levi = novi;
		else
			pret->desni= novi;
	}
	return koren;
}

typedef struct elementListe {
	Kontakt knt;
	string poruke;
	struct elementListe* sledeci;
}ElementListe;

ElementListe *slanjePoruke(ElementListe* glavaIstorijaKomunikacije, Kontakt primalac, int n, int *brojKontakata) {
	int kontaktNadjen = 0;
	if (!glavaIstorijaKomunikacije) {
		glavaIstorijaKomunikacije = new ElementListe();
		glavaIstorijaKomunikacije->knt = primalac;
		glavaIstorijaKomunikacije->sledeci = NULL;
		*brojKontakata += 1;
	}
	else {
		ElementListe *tren = glavaIstorijaKomunikacije;
		ElementListe *pret=NULL;
		while (tren) {
			if (tren->knt.ime == primalac.ime && tren->knt.prezime == primalac.prezime && tren->knt.broj == primalac.broj) {
				pret->sledeci = tren->sledeci;
				tren->sledeci = glavaIstorijaKomunikacije;
				glavaIstorijaKomunikacije = tren;
				kontaktNadjen = 1;
			}
			pret = tren;
			tren = tren->sledeci;
		}
		if (!kontaktNadjen) {
			ElementListe* novi = new ElementListe();
			novi->sledeci = glavaIstorijaKomunikacije;
			glavaIstorijaKomunikacije = novi;
			novi->knt = primalac;
			*brojKontakata += 1;
		}
	}
	
	if (*brojKontakata > n) {
			ElementListe* p = glavaIstorijaKomunikacije;
			while (p->sledeci->sledeci) {
				p = p->sledeci;
			}
			delete p->sledeci;
			p->sledeci = NULL;
			*brojKontakata -= 1;
		}
	return glavaIstorijaKomunikacije;
}

void prikazIstorijeKomunikacije(ElementListe* glavaIstorijaKomunikacije) {
	ElementListe* p = glavaIstorijaKomunikacije;
	while (p) {
		cout << p->knt.ime+" ";
		p = p->sledeci;
	}
	cout << "\n";
}

void deljenjeSadrzaja(Kontakt nizDeljenja[101],int *brojKontakataDeljenja,Kontakt primalac,int x,int *indeksUzastopne, int nizTranspozicija[101]) {
	int nadjenKontakt = 0;
	for (int i = 0;i < *brojKontakataDeljenja;++i) {
		if (nizDeljenja[i].broj == primalac.broj) {

			if(i==*indeksUzastopne)
				nizTranspozicija[i]++;
			else {
				nizTranspozicija[*indeksUzastopne] = 0;
				*indeksUzastopne = i;
				nizTranspozicija[*indeksUzastopne] = 1;
			}

			//Vracanje na pocetak niza
			if (nizTranspozicija[i] > x) {
				Kontakt q = nizDeljenja[i];
				for (int j = i-1;j >=0 ;--j) {
					nizDeljenja[j + 1] = nizDeljenja[j];
				}
				nizTranspozicija[*indeksUzastopne] = 0;
				*indeksUzastopne = 0;
				nizDeljenja[0] = q;
			}
			else if (i > 0) {
				Kontakt p = nizDeljenja[i];
				nizDeljenja[i] = nizDeljenja[i - 1];
				nizDeljenja[i - 1] = p;
			}
			nadjenKontakt = 1;
		}
	}
	if (!nadjenKontakt) {
		nizTranspozicija[*brojKontakataDeljenja] = 0;
		nizDeljenja[*brojKontakataDeljenja] = primalac;
		(*brojKontakataDeljenja)++;
	}
}

void prikazDeljenjaSadrzaja(Kontakt nizDeljenja[101], int brojKontakataDeljenja) {
	for (int i = 0;i < brojKontakataDeljenja;++i) {
		cout << nizDeljenja[i].ime+ " ";
	}
	cout << "\n";
}

CvorBST* brisanjeKontakta(CvorBST* koren,CvorBST* kontakt) {
	int levi = 0, desni = 0;
	if (kontakt != koren) {
		if (kontakt->otac->levi == kontakt)
			levi = 1;
		if (kontakt->otac->desni == kontakt)
			desni = 1;
	}
	
	if (kontakt->levi == NULL && kontakt->desni == NULL) {
		if (levi)
			kontakt->otac->levi = NULL;
		if (desni)
			kontakt->otac->desni = NULL;
		delete kontakt;
	}
	else if (kontakt->levi == NULL) {
		if (desni)
			kontakt->otac->desni = kontakt->desni;
		if (levi)
			kontakt->otac->levi = kontakt->desni;
		delete kontakt;
	}
	else if (kontakt->desni == NULL) {
		if (desni)
			kontakt->otac->desni = kontakt->levi;
		if (levi)
			kontakt->otac->levi = kontakt->levi;
		delete kontakt;
	}
	else {
		CvorBST* sledbenik = kontakt;
		sledbenik = sledbenik->desni;
		while (sledbenik->levi)
			sledbenik = sledbenik->levi;
		if (sledbenik->otac == kontakt)
			kontakt->desni = NULL;
		else
			sledbenik->otac->levi = sledbenik->desni;
		kontakt->kont = sledbenik->kont;
		delete sledbenik;
	}
	return koren;
}

void izmenaKontakta(CvorBST* koren,CvorBST* kontakt) {
	int stavka;
	string ime= kontakt->kont.ime, prezime = kontakt->kont.prezime, broj = kontakt->kont.broj;
	while (1) {
		printf("Izaberite stavku menija za izmenu:\n");
		printf("1. Ime\n");
		printf("2. Prezime\n");
		printf("3. Broj\n");
		printf("4. Izlaz\n");
		printf("0. Gotova izmena\n");
		cin >> stavka;
		switch (stavka) {
		case 1: cin >> ime; break;
			case 2: cin >> prezime; break;
			case 3: cin >> broj; break;
			case 4: return;
			case 0: break;
			default:cout << "Unesite ispravnu stavku menija!\n";
		}
		if (!stavka)
			break;
	}
	brisanjeKontakta(koren, kontakt);
	dodavanjeKontakta(koren, prezime, ime, broj);
}

int main() {
	FILE* dat;
	Kontakt niz[101];
	int id;
	int brojNadjenih = 0;
	CvorBST* nizNadjenih[101];
	try {
		fopen_s(&dat,"Contacts20.txt","r");
		int brojKontakta=kreiranjeNizaKontakta(dat, niz);
		fclose(dat);
		/*for (int i = 0;i < brojKontakta;++i) {
			cout << niz[i].ime+" "+ niz[i].prezime +" "+ niz[i].broj+"\n";
		}*/
		CvorBST *koren = createBST(niz, brojKontakta);
		while (1) {
			printf("Meni\n");
			printf("1. Pretraga svih kontakata po prezimenu\n");
			printf("2. Pretraga svih kontakata po prezimenu i imenu\n");
			printf("3. Dodavanje novog kontakta\n");
			printf("4. Formatiran ispis stabla\n");
			printf("5. Uklanjanje kontakta\n");
			printf("6. Izmena postojeceg kontakta\n");
			printf("7. Prikaz niza kontakata\n");
			printf("0. Izlaz\n");
			int stavka;
			cout << "Unesite stavku menija:\n";
			cin >> stavka;
			string ime, prezime, broj;
			switch (stavka) {
			case 1:
				cout << "Unesite prezime za pretragu: ";
				cin >> prezime;
				brojNadjenih = pretragaPoPrezimenu(koren, prezime, nizNadjenih);
				cout << "Svi kontakti koji se prezivaju " << prezime + ":\n";
				for (int i = 0;i < brojNadjenih;++i) {
					cout << i + 1 << ": " << nizNadjenih[i]->kont.ime + " " << nizNadjenih[i]->kont.prezime << " - " + nizNadjenih[i]->kont.broj << "\n";
				}
				break;
			case 2:
				cout << "Unesite podatke za pretragu: \n";
				cout << "Ime: ";
				cin >> ime;
				cout << "Prezime: ";
				cin >> prezime;
				brojNadjenih = pretragaPoImenuiPrezimenu(koren, prezime, ime, nizNadjenih);
				cout << "Svi kontakti koji se zovu " << ime + " " << prezime + ":\n";
				for (int i = 0;i < brojNadjenih;++i) {
					cout << i + 1 << ": " << nizNadjenih[i]->kont.ime + " " << nizNadjenih[i]->kont.prezime << " - " + nizNadjenih[i]->kont.broj << "\n";
				}
				break;
			case 3:
				cout << "Unesite podatke za novog kontakta: \n";
				cout << "Ime: ";
				cin >> ime;
				cout << "Prezime: ";
				cin >> prezime;
				cout << "Broj: ";
				cin >> broj;
				dodavanjeKontakta(koren, prezime, ime, broj);
				cout << "Uspesno dodavanje kontakta\n";
				break;
			case 4: ispisBST(koren);
				break;
			case 5:
				cout << "Unesite ime i prezime kontakta za brisanje: \n";
				cout << "Ime: ";
				cin >> ime;
				cout << "Prezime: ";
				cin >> prezime;
				brojNadjenih = pretragaPoImenuiPrezimenu(koren, prezime, ime, nizNadjenih);
				if (brojNadjenih > 1) {
					cout << "Svi kontakti koji se zovu " << ime + " " << prezime + ":\n";
					for (int i = 0;i < brojNadjenih;++i) {
						cout << i + 1 << ": " << nizNadjenih[i]->kont.ime + " " << nizNadjenih[i]->kont.prezime << " - " + nizNadjenih[i]->kont.broj << "\n";
					}
					cout << "Unesite redni broj kontakta kog zelite da obrisete: ";

					cin >> id;
				}
				else if (brojNadjenih == 1)
					id = 1;
				else {
					cout << "Kontakt ne postoji\n";
					break;
				}
				brisanjeKontakta(koren, nizNadjenih[id - 1]);
				break;
			case 6:
				cout << "Unesite ime i prezime kontakta koga zelite da menjate: \n";
				cout << "Ime: ";
				cin >> ime;
				cout << "Prezime: ";
				cin >> prezime;
				int id;
				brojNadjenih = pretragaPoImenuiPrezimenu(koren, prezime, ime, nizNadjenih);
				if (brojNadjenih > 1) {
					cout << "Svi kontakti koji se zovu " << ime + " " << prezime + ":\n";
					for (int i = 0;i < brojNadjenih;++i) {
						cout << i + 1 << ": " << nizNadjenih[i]->kont.ime + " " << nizNadjenih[i]->kont.prezime << " - " + nizNadjenih[i]->kont.broj << "\n";
					}
					cout << "Unesite redni broj kontakta kog zelite da menjate: ";

					cin >> id;
				}
				else if (brojNadjenih == 1)
					id = 1;
				else {
					cout << "Kontakt ne postoji\n";
					break;
				}

				izmenaKontakta(koren, nizNadjenih[id - 1]);
				cout << "Uspesno ste izmenili kontakt\n";
			case 7:
				for (int i = 0;i < brojKontakta;++i) {
					cout << niz[i].ime+" "+ niz[i].prezime +" "+ niz[i].broj+"\n";
				}
				break;
			case 0:return 0; break;
			default:cout << "Unesite ispravnu stavku menija\n";
			}
		}

		koren=brisanjeStablaLevelOrder(koren);
	}
	catch (...) {
		printf("greska");
	}
	
	
	return 0;
}	