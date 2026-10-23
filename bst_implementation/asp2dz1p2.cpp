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
int kreiranjeNizaKontakta(FILE* dat, Kontakt niz[101]) {
	char a[101];

	char imep[50];
	char prezimep[50];
	char brojp[50];
	char c;
	while ((c = fgetc(dat)) != '\n') {
	}

	int brojKontakta = 0;

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
		niz[brojKontakta].ime = imep;
		niz[brojKontakta].prezime = prezimep;
		niz[brojKontakta].broj = brojp;
		++brojKontakta;
		if (c == EOF) break;
	}
	return brojKontakta-1;
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
				cout << tren->kont.prezime + " " + tren->kont.ime + "-" + tren->kont.broj;
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

//Kod sa prezentacije vezbi za kreiranje balansiranog stabla
CvorBST* createBST(Kontakt niz[101], int n) {
	int low = 0, high = n - 1;
	stack<ElemStek> stek;
	int mid = (low + high) / 2;
	CvorBST* koren = new CvorBST();
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
		ElemStek p = stek.top();
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


int pretragaPoImenuiPrezimenu(CvorBST* koren, string prezime, string ime, CvorBST* nizNadjenih[101]) {
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


typedef struct elementListe {
	Kontakt knt;
	string poruke;
	struct elementListe* sledeci;
}ElementListe;

ElementListe* slanjePoruke(ElementListe* glavaIstorijaKomunikacije, Kontakt primalac, int n, int* brojKontakata) {
	int kontaktNadjen = 0;
	if (!glavaIstorijaKomunikacije) {
		glavaIstorijaKomunikacije = new ElementListe();
		glavaIstorijaKomunikacije->knt = primalac;
		glavaIstorijaKomunikacije->sledeci = NULL;
		*brojKontakata += 1;
	}
	else {
		ElementListe* tren = glavaIstorijaKomunikacije;
		ElementListe* pret = NULL;
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
	cout << "Istorija komunikacije: \n";
	while (p) {
		cout << p->knt.ime + " ";
		p = p->sledeci;
	}
	cout << "\n";
}

void deljenjeSadrzaja(Kontakt nizDeljenja[101], int* brojKontakataDeljenja, Kontakt primalac, int x, int* indeksUzastopne, int nizTranspozicija[101]) {
	int nadjenKontakt = 0;
	for (int i = 0;i < *brojKontakataDeljenja;++i) {
		if (nizDeljenja[i].broj == primalac.broj) {

			if (i == *indeksUzastopne)
				nizTranspozicija[i]++;
			else {
				nizTranspozicija[*indeksUzastopne] = 0;
				*indeksUzastopne = i;
				nizTranspozicija[*indeksUzastopne] = 1;
			}

			//Vracanje na pocetak niza
			if (nizTranspozicija[i] > x) {
				Kontakt q = nizDeljenja[i];
				for (int j = i - 1;j >= 0;--j) {
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
	cout << "Istorija deljenja sadrzaja: \n";
	for (int i = 0;i < brojKontakataDeljenja;++i) {
		cout << nizDeljenja[i].ime + " ";
	}
	cout << "\n";
}

int main() {
	FILE* dat;
	Kontakt niz[101];
	int brojNadjenih = 0;
	CvorBST* nizNadjenih[101];
	fopen_s(&dat, "Contacts20.txt", "r");
	int brojKontakta = kreiranjeNizaKontakta(dat, niz);
	fclose(dat);
	
	CvorBST* koren = createBST(niz, brojKontakta);

	ElementListe* glavaIK = NULL;
	int brojKontakata = 0;
	int n;
	int x;
	int nizTranspozicija[101];
	int indeksUzastopne = 0;
	int brojKontakataDeljenja = 0;
	Kontakt nizDeljenja[101];
	int id;

	cout << "Unesite broj kontakata koji se mogu cuvati u istoriji komunikacije: ";
	cin >> n;
	cout << "Unesite broj x koji odredjuje koliko puta uzastopce se mora deliti izmedju dva kontakta da bi se prebacio na pocetak: ";
	cin >> x;

	while (1) {
		printf("Meni\n");
		printf("1. Posaljite poruku\n");
		printf("2. Podelite sadrzaj\n");
		printf("3. Prikaz istorije komunikacije\n");
		printf("4. Prikaz istorije deljenja\n");
		printf("5. Prikaz svih kontakata\n");
		printf("0. Izlaz\n");
		int stavka;
		cout << "Unesite stavku menija:\n";
		cin >> stavka;
		string ime, prezime, broj;
		switch (stavka) {
		case 1:
			cout << "Unesite ime i prezime kontakta za slanje poruke: \n";
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
				cout << "Unesite redni broj kontakta kome saljete poruku: ";

				cin >> id;
			}
			else if (brojNadjenih == 1)
				id = 1;
			else {
				cout << "Kontakt ne postoji\n";
				break;
			}
			glavaIK = slanjePoruke(glavaIK, nizNadjenih[id - 1]->kont, n, &brojKontakata);
			break;
		case 2:
			cout << "Unesite ime i prezime kontakta za deljenje sadrzaja: \n";
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
				cout << "Unesite redni broj kontakta kome delite sadrzaj: ";

				cin >> id;
			}
			else if (brojNadjenih == 1)
				id = 1;
			else {
				cout << "Kontakt ne postoji\n";
				break;
			}

			deljenjeSadrzaja(nizDeljenja,&brojKontakataDeljenja, nizNadjenih[id - 1]->kont, x,&indeksUzastopne,nizTranspozicija);
			break;
		case 3: prikazIstorijeKomunikacije(glavaIK);
			break;
		case 4: prikazDeljenjaSadrzaja(nizDeljenja, brojKontakataDeljenja);
			break;
		case 5:
			for (int i = 0;i < brojKontakta;++i) {
				cout << niz[i].ime + " " + niz[i].prezime + " " + niz[i].broj + "\n";
			}
			break;
		default: cout << "Unesite ispravnu stavku menija\n";
		}
	}

	koren = brisanjeStablaLevelOrder(koren);
	return 0;
}