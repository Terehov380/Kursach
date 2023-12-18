#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

const char Menu[] = "�������� ����� ���� ����� �������������� �����:\n"
"1.�������� ����� ������ � ���� ������\n"
"2.������ ���� �������\n"
"3.����� �� ���� ������\n"
"4.C��������� ������� �� ���������� ���������\n"
"5.���������� ����� ������� � ����\n"
"6.��������� ��������� ������\n"
"7.����� �� ���������\n";


struct Base {
	char name[30];
	char realise[12];
	int installs;
	float balance;
	char paid[50];
	double version;
}; typedef struct Base base_t;

FILE* OpenFileForWriting(const char* filename); // ���������� ������� ��� �������� �� ������ �����
FILE* OpenFileForReading(const char* filename); // ���������� ������� ��� �������� �� ������ �����
int NewFileCreate(FILE* file, char name[]); // ���������� ������� ��� �������� �������� ���������� ����� � �������� ������
int PrintToConsole(base_t* Base_t, int SizeMassiv); // ���������� ������� ��� ������ ���� ����� ������� � ����������� ����
int SaveFile(int* SizeMassiv, base_t* Base_t, char name[]); // ���������� ������� ��� ������ ��������� ����������� � ���� ������ � ����
int Compare(const base_t* Base1_t, const base_t* Base2_t); // ���������� �������� ����������� ��� ������������� ��������� ���������� qsort
int* SearchByDate(base_t* Base_t, int SizeMassiv, char DateForSearch[], int* SearchResults);// ���������� ������� ��� ������ �������� ��������� ������� �� �����
base_t* ReadFromAFile(char name[], base_t* Base_t, int* SizeMassiv); // ���������� ������� ��� ��������������� ���������� ������ �� ���������� �����
base_t* AddNewPole(base_t* Base_t, int* SizeMassiv); // ���������� ������� ��� ���������� ����� ������ � ������
base_t* EditBasePole(base_t* Base_t, int EditNumber);//���������� ������� ��� ��������� ��������� ������ � ������


int main() {
	setlocale(LC_ALL, 0);// ��������� ����������� ������� � �� �������
	system("chcp 1251");
	system("cls");


	base_t* Base_t;

	int SizeMassiv = 0;
	int ExitState = 0;
	int SwitchNum = 0;
	int numedit = 0;
	char name[] = "Games.txt";
	char* DateForSearch[12];


	Base_t = (base_t*)malloc(1 * sizeof(base_t));//��������� �������������� ������ ������ ��� ������
	Base_t = ReadFromAFile(name, Base_t, &SizeMassiv);// ������������� ������� �� �����

	printf("\t   �������� ������ �������� \n");
	printf("#-------------------------------------------#\n");
	printf("# ��� ��������: %-28s#\n", "������� ���������");
	printf("# ������: %-34s#\n", "���� - 232");
	printf("# ���� �������� ������: %-20s#\n", "��������� ��� ������� � ����� ������ \"���� � ���. ����\"");
	puts("");

	// ���������� ������������������ ���� ��� �������������� � ���������� � ������� ����� while � ��������� switch
	while (ExitState != 1) {
		printf(Menu);
		int TempSize = SizeMassiv;
		int* SearchResults = (int*)calloc(TempSize, sizeof(int));
		scanf("%d", &SwitchNum);
		switch (SwitchNum)
		{
		case 1:
			system("cls");
			Base_t = AddNewPole(Base_t, &SizeMassiv);
			printf("������ ��������� � ����\n");
			break;
		case 2:
			system("cls");
			PrintToConsole(Base_t, TempSize);
			getchar();
			break;
		case 3:
			system("cls");
			printf("������� ���� ������ � ������� ��.��.���� ��� ������ ������� � ���� � ����\n");
			if (scanf("%s", &DateForSearch) != 1) return 1;
			printf("�� ���� ������ %s �������� ��������� ������\n", DateForSearch);
			SearchResults = SearchByDate(Base_t, TempSize, DateForSearch, SearchResults);
			for (int i = 0; i < TempSize; i++) {
				int foundindex = *SearchResults++;
				if (foundindex >= 0 && foundindex <= TempSize) {
					printf("���: %s\n���� ������: %s\n����������� ���������: %d\n������ %f\n����� ������: %s\n������: %lf\n", Base_t[i].name, Base_t[i].realise, Base_t[i].installs, Base_t[i].balance, Base_t[i].paid, Base_t[i].version);
				}
			}
			break;
		case 4:
			system("cls");
			qsort(Base_t, TempSize, sizeof(base_t), Compare);
			PrintToConsole(Base_t, TempSize);
			break;
		case 5:
			SaveFile(&SizeMassiv, Base_t, name);
			printf("��������� ��������� ���� ���������\n");
			break;
		case 6:
			printf("������� ������� ��������� ��������� ����\n");
			PrintToConsole(Base_t, TempSize);
			scanf("%d", &numedit);
			Base_t = EditBasePole(Base_t, numedit);
			break;
		case 7:
			free(Base_t);
			free(SearchResults);
			exit(0);
			break;
		default:
			break;
		}
	}
}

//����������� ������� ��� �������� ����� � ����� � ������ ��� ���������
int NewFileCreate(FILE* file, char name[]) {
	file = OpenFileForWriting(name);
	fprintf(file, "%s %s %d %f %s %lf\n\n", "MyGame", "10.10.2023", "153", "253000", "Kiwi", "0.1");
	fclose(file);
	return 0;
}

//����������� ������� ��� �������������� ������������� ���� �� �����
base_t* ReadFromAFile(char name1[], base_t* Base_t, int* SizeMassiv) {
	FILE* file = OpenFileForReading(name1);
	int i = 0;
	fseek(file, 0, SEEK_END);
	long pos = ftell(file);
	if (pos > 0)
	{
		fseek(file, 0, SEEK_SET);
		while (!feof(file)) {
			*SizeMassiv += 1;
			Base_t = (base_t*)realloc(Base_t, *SizeMassiv * sizeof(base_t));
			fscanf(file, "%30s %12s %d %f %50s %lf\n", &Base_t[i].name, &Base_t[*SizeMassiv - 1].realise, &Base_t[*SizeMassiv - 1].installs, &Base_t[*SizeMassiv - 1].balance, &Base_t[*SizeMassiv - 1].paid, &Base_t[*SizeMassiv - 1].version);
			i++;
		}
	}

	fclose(file);
	return Base_t;
}

//����������� ������� ��� ������ ���� � �������� ����
int PrintToConsole(base_t* Base_t, int SizeMassiv) {
	for (int i = 0; i < SizeMassiv; i++) {
		printf("\n");
		printf("%d. ���: %s\n", i + 1, &Base_t[i].name);
		printf("���� ������: %s\n", &Base_t[i].realise);
		printf("����������� ���������: %d\n", Base_t[i].installs);
		printf("������: %f\t", Base_t[i].balance);
		printf("���������� ����� ������: %s\t", &Base_t[i].paid);
		printf("������: %lf\n\n", Base_t[i].version);
	}

}

//����������� ������� ��� ���������� ��������� � ���� � ��������� ����
int SaveFile(int* SizeMassiv, base_t* Base_t, char name[]) {
	FILE* file;
	file = fopen(name, "w+");
	for (int i = 0; i < *SizeMassiv; i++) {
		fprintf(file, "%s %s %d %f %s %lf\n\n", Base_t[i].name, Base_t[i].realise, Base_t[i].installs, Base_t[i].balance, Base_t[i].paid, Base_t[i].version);
	}
	fclose(file);
}

//����������� ������� �����������
int Compare(const base_t* Base1_t, const base_t* Base2_t) {
	const struct Base* a = Base1_t, * b = Base2_t;
	int installs1, installs2;
	installs1 = a->installs;
	installs2 = b->installs;
	if (installs1 < installs2) return 1;
	if (installs1 > installs2) return -1;
	if (installs1 == installs2) return 0;
}

//����������� ������� ��� ������ �� ����
int* SearchByDate(base_t* Base_t, int SizeMassiv, char DateForSearch[], int* SearchResults) {
	int n = 0;
	for (int i = 0; i < SizeMassiv; i++) {
		if (strcmp(Base_t[i].realise, DateForSearch) == 0) {
			SearchResults[n++] = i;
		}
		else SearchResults[n++] = SizeMassiv + 10;
	}
	return SearchResults;
}

//����������� ������� ��� ���������� � ������ ����� ������
base_t* AddNewPole(base_t* Base_t, int* SizeMassiv) {
	*SizeMassiv += 1;
	Base_t = (base_t*)realloc(Base_t, *SizeMassiv * sizeof(base_t));
	if (Base_t == NULL) {
		printf("\a ������ ��� ���������� ������� �������\n");
		return Base_t;
	}
	printf("������� �������� ����\n");
	if (scanf("%s", &Base_t[*SizeMassiv - 1].name) != 1) return 1;

	printf("������� ���� ������ � ������� ��.��.����\n");
	if (scanf("%s", &Base_t[*SizeMassiv - 1].realise) != 1) return 1;

	printf("������� ����������� ��������� � ���� \n");;
	if (scanf("%d", &Base_t[*SizeMassiv - 1].installs) != 1) return 1;

	printf("������� ������ ����\n");
	if (scanf("%f", &Base_t[*SizeMassiv - 1].balance) != 1) return 1;

	printf("������� ��������� ����� ������\n");
	if (scanf("%s", &Base_t[*SizeMassiv - 1].paid) != 1) return 1;

	printf("������� ������\n");
	if (scanf("%lf", &Base_t[*SizeMassiv - 1].version) != 1) return 1;
	return Base_t;
}

base_t* EditBasePole(base_t* Base_t, int EditNumber) {
	printf("��������� ������ ��������� ��� ��������������\t ���: %s\n���� ������: %s\n����������� ���������: %d\n������ %f\n����� ������: %s\n ������: %lf\n",
		Base_t[EditNumber].name, Base_t[EditNumber].realise, Base_t[EditNumber].installs, Base_t[EditNumber].balance, Base_t[EditNumber].paid, Base_t[EditNumber].version);
	printf("������� �������� ����\n");
	if (scanf("%s", &Base_t[EditNumber].name) != 1) return 1;

	printf("������� ���� ������ � ������� ��.��.����\n");
	if (scanf("%s", &Base_t[EditNumber].realise) != 1) return 1;

	printf("������� ����������� ��������� � ���� \n");;
	if (scanf("%d", &Base_t[EditNumber].installs) != 1) return 1;

	printf("������� ������ ����\n");
	if (scanf("%f", &Base_t[EditNumber].balance) != 1) return 1;

	printf("������� ��������� ����� ������\n");
	if (scanf("%s", &Base_t[EditNumber].paid) != 1) return 1;

	printf("������� ������\n");
	if (scanf("%lf", &Base_t[EditNumber].version) != 1) return 1;
	return Base_t;
}
FILE* OpenFileForWriting(const char* filename)	
{
	FILE* file = fopen(filename, "w");
	if (!file)
	{
		printf("������ ��� �������� ����� %s ��� ������.\n", filename);
	}
	return file;
}
FILE* OpenFileForReading(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("������ ��� �������� ����� %s ��� ������.\n", filename);
	}
	return file;
}