#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Numarul usilor: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* vector = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < (*nrMasini); i++) {
		vector[i] = (*masini)[i];
	}
	vector[(*nrMasini)] = masinaNoua;
	// deep copy
	vector[(*nrMasini)].model = (char*)malloc(sizeof(char) * (strlen(masinaNoua.model) + 1));
	strcpy(vector[(*nrMasini)].model, masinaNoua.model);
	vector[(*nrMasini)].numeSofer = (char*)malloc(sizeof(char) * (strlen(masinaNoua.numeSofer) + 1));
	strcpy(vector[(*nrMasini)].numeSofer, masinaNoua.numeSofer);
	(*nrMasini)++;
	free(*masini);
	(*masini) = vector;
}

Masina citireMasinaFisier(FILE* file) {
	Masina masina;
	char buffer[100];
	char separator[] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		masina.id = -1; // Indica eroare
		return masina;
	}

	char* token = strtok(buffer, separator);
	masina.id = atoi(token);
	char* token1 = strtok(NULL, separator);
	masina.nrUsi = atoi(token1);
	char* token2 = strtok(NULL, separator);
	masina.pret = atof(token2);
	char* token3 = strtok(NULL, separator);
	masina.model = (char*)malloc(sizeof(char) * (strlen(token3) + 1));
	strcpy(masina.model, token3);
	char* token4 = strtok(NULL, separator);
	masina.numeSofer = (char*)malloc(sizeof(char) * (strlen(token4) + 1));
	strcpy(masina.numeSofer, token4);
	char* token5 = strtok(NULL, separator);
	masina.serie = token5[0];

	return masina;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	}
	Masina* masini = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaFisier(f);
		if (m.id == -1) break;
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

int main() {
	Masina* masini = NULL;
	int nrMasini = 0;
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	if (masini) {
		afisareVectorMasini(masini, nrMasini);
		dezalocareVectorMasini(&masini, &nrMasini);
	}
	return 0;
}