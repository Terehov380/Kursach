#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
const char Menu[] = "Выберите пункт меню введя соответсвующую цифру:\n"
"1.Создание новой записи в базе данных\n"
"2.Чтение всех записей\n"
"3.Поиск по дате релиза\n"
"4.Cортировка записей по колличесву установок\n"
"5.Сохранение новых записей в файл\n"
"6.Изменение выбранной записи\n"
"7.Поиск по имени\n"
"8.Выход из программы\n";
struct Base {
	char name[30];
	char realise[12];
	int installs;
	float balance;
	char paid[50];
	double version;
}; typedef struct Base base_t;
struct Month
{
	char key[2];
	char value[20];
};
FILE* OpenFileForWriting(const char* filename); // Обьявление функции для проверки на запись файла
FILE* OpenFileForReading(const char* filename); // Обьявление функции для проверки на чтение файла
FILE* NewFileCreate(FILE* file, char name[]); // Обьявление функции для создания рабочего текстового файла с заданным именем
int PrintToConsole(base_t* Base_t, int SizeMassiv); // Обьявление функции для печати всех полей массива в определнном виде
int SaveFile(int* SizeMassiv, base_t* Base_t, char name[]); // Обьявление функции для записи изменений совершенных в базе данных в файл
int Compare(const base_t* Base1_t, const base_t* Base2_t); // Обьявление функциии компаратора для использования алгоритма сортировки qsort
int* SearchByDate(base_t* Base_t, int SizeMassiv, char DateForSearch[], int* SearchResults);// Обьявление функции для поиска индексов элементов массива по имени
base_t* ReadFromAFile(char name[], base_t* Base_t, int* SizeMassiv); // Обьявление функции для первоначального считывания данных из текстового файла
base_t* AddNewPole(base_t* Base_t, int* SizeMassiv); // Обьявление функции для добавления новой записи в массив
base_t* EditBasePole(base_t* Base_t, int EditNumber);//Обьявление функции для изменения выбранной записи в массив
int* SearchByName(base_t* Base_t, int SizeMassiv, char gameName[], int* SearchResults);
int* SearchByMonth(base_t* Base_t, int SizeMassiv, char month[], int* SearchResults);

int main() {
	setlocale(LC_ALL, 0);// Установка локализации консоли и ее очистка
	system("chcp 1251");
	system("cls");
	base_t* Base_t;
	struct Month months[12] = { {"01", "Январь"}, {"02", "Февраль"}, {"03", "Март"}, {"04", "Апрель"}, {"05", "Май"},
	{"06", "Июнь"},
	{"07", "Июль"},
	{"08", "Август"},
	{"09", "Сентябрь"},
	{"10", "Октябрь"},
	{"11", "Ноябрь"},
	{"12", "Декабрь"} };
	int SizeMassiv = 0;
	int ExitState = 0;
	int SwitchNum = 0;
	int numedit = 0;
	char name[] = "Games.txt";
	char* DateForSearch[12];
	char gameName[12];
	char month[12];
	Base_t = (base_t*)malloc(1 * sizeof(base_t));//Выделение первоначальной ячейки памяти под массив
	Base_t = ReadFromAFile(name, Base_t, &SizeMassiv);// Инициализация массива из файла
	printf("\t Курсовая работа студента \n");
	printf("#-------------------------------------------#\n");
	printf("# Имя студента: %-28s#\n", "Терехов Владислав");
	printf("# Группа: %-34s#\n", "бИСТ - 232");
	printf("# Тема курсовой работы: %-20s#\n", "Программа для работой с базой данных \"Игры в соц. сети\"");
	puts("");

	// Реализация пользователььского меню для взаимодействия с программой с помощью цикла while и оператора switch
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
			printf("Запись добавлена в базу\n");
			break;
		case 2:
			system("cls");
			PrintToConsole(Base_t, TempSize);
			getchar();
			break;
		case 3:
			system("cls");
			printf("введите дату релиза в формате дд.мм.гггг для поиска записей в базе\n");
			if (scanf("%s", &DateForSearch) != 1) return 1;	printf("по дате релиза %s найденны следующие записи\n", DateForSearch);
		SearchResults = SearchByDate(Base_t, TempSize, DateForSearch,
				SearchResults);
			for (int i = 0; i < TempSize; i++) {
				int foundindex = *SearchResults++;
				if (foundindex >= 0 && foundindex <= TempSize) {
					printf("имя: %s\nдата релиза: %s\nколличество установок: %d\nбаланс %f\nметод оплаты: %s\nверсия: %lf\n", Base_t[i].name, Base_t[i].realise, Base_t[i].installs, Base_t[i].balance, Base_t[i].paid, Base_t[i].version);
				}
			}
			SearchResults = (int*)calloc(TempSize, sizeof(int));
			break;
		case 4:
			system("cls");
			qsort(Base_t, TempSize, sizeof(base_t), Compare);
			PrintToConsole(Base_t, TempSize);
			break;
		case 5:
			SaveFile(&SizeMassiv, Base_t, name);
			printf("Внесенные изменения были сохранены\n");
			break;
		case 6:
			printf("Выбрана функция изменения изменения поля\n");
			PrintToConsole(Base_t, TempSize);
			scanf("%d", &numedit);
			Base_t = EditBasePole(Base_t, numedit);
			break;
		case 7:
			system("cls");
			printf("введите название игры: ");
			if (scanf("%s", &gameName) != 1) return 1;
			printf("по имени %s найденны следующие записи\n", gameName);
			SearchByName(Base_t, TempSize, gameName, SearchResults);
			break;
		case 8:
			free(Base_t);
			free(SearchResults);
			exit(0);
			break;
		case 9:
			system("cls");
			printf("введите месяц игры: ");
			if (scanf("%s", &month) != 1) return 1;
			printf("по месяцу %s найдены следующие записи\n", month);
			SearchByMonth(Base_t, TempSize, month, SearchResults);
			break;
		default:
			break;
		}
	}
}

FILE* OpenFileForWriting(const char* filename)
{
	FILE* file = fopen(filename, "w");
	if (!file)
	{
		printf("Ошибка при открытии файла %s для записи.\n", filename);
		return file;
	}
	return file;
}

FILE* OpenFileForReading(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		return NewFileCreate(file, filename);
	}
	return file;
}

//Определение функции для создания файла с базой в случае его отсуствия
FILE* NewFileCreate(FILE* file, char name[])
{
	file = OpenFileForWriting(name);
	return file;
}

//Определение функции для первоначальной инициализации базы из файла
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

//Определение функции для печати базы в заданном виде
int PrintToConsole(base_t* Base_t, int SizeMassiv) {
	for (int i = 0; i < SizeMassiv; i++) {
		printf("\n");
		printf("%d. Имя: %s\n", i + 1, &Base_t[i].name);
		printf("Дата релиза: %s\n", &Base_t[i].realise);
		printf("Колличество установок: %d\n", Base_t[i].installs);
		printf("Баланс: %f\t", Base_t[i].balance);
		printf("Допустимый метод полаты: %s\t", &Base_t[i].paid);
		printf("Версия: %lf\n\n", Base_t[i].version);
	}

}

//Определение функции для сохранения изменений в базе в текстовый файл
int SaveFile(int* SizeMassiv, base_t* Base_t, char name[]) {
	FILE* file;
	file = fopen(name, "w+");
	for (int i = 0; i < *SizeMassiv; i++) {
		fprintf(file, "%s %s %d %f %s %lf\n\n", Base_t[i].name, Base_t[i].realise, Base_t[i].installs, Base_t[i].balance, Base_t[i].paid, Base_t[i].version);
	}
	fclose(file);
}

//Определение функции компаратора
int Compare(const base_t* Base1_t, const base_t* Base2_t) {
	const struct Base* a = Base1_t, * b = Base2_t;
	int installs1, installs2;
	installs1 =
		a->installs;
	installs2 = b->installs;
	if (installs1 < installs2) return 1;
	if (installs1 > installs2) return -1;
	if (installs1 == installs2) return 0;
}

//Определение функции для поиска по дате
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

int* SearchByName(base_t* Base_t, int SizeMassiv, char gameName[], int* SearchResults) {
	int n = 0;
	for (int i = 0; i < SizeMassiv; i++) {
		if (strcmp(Base_t[i].name, gameName) == 0) {
			printf("\n");
			printf("Имя: %s\n", &Base_t[i].name);
			printf("Дата релиза: %s\n", &Base_t[i].realise);
			printf("Колличество установок: %d\n", Base_t[i].installs);
			printf("Баланс: %f\t", Base_t[i].balance);
			printf("Допустимый метод полаты: %s\t", &Base_t[i].paid);
			printf("Версия: %lf\n\n", Base_t[i].version);
		}
	}
	return SearchResults;
}

int* SearchByMonth(base_t* Base_t, int SizeMassiv, char month[], int* SearchResults) {
	int n = 0;
	char monthOfGame[3];
	for (int i = 0; i < SizeMassiv; i++) {
		sscanf(Base_t[i].realise, "%*c%*c.%[^.]", monthOfGame);
		if (strcmp(monthOfGame, month) == 0) {
			printf("\n");
			printf("Имя: %s\n", &Base_t[i].name);
			printf("Дата релиза: %s\n", &Base_t[i].realise);
			printf("Колличество установок: %d\n", Base_t[i].installs);
			printf("Баланс: %f\t", Base_t[i].balance);
			printf("Допустимый метод полаты: %s\t", &Base_t[i].paid);
			printf("Версия: %lf\n\n", Base_t[i].version);
		}
	}
	return SearchResults;
}

//Определение функции для добавления в массив новой записи
base_t* AddNewPole(base_t* Base_t, int* SizeMassiv) {
	*SizeMassiv += 1;
	Base_t = (base_t*)realloc(Base_t, *SizeMassiv * sizeof(base_t));
	if (Base_t == NULL) {
		printf("\a Ошибка при увелечении размера массива\n");
		return Base_t;
	}
	printf("Введите название игры\n");
	if (scanf("%s", &Base_t[*SizeMassiv - 1].name) != 1) return 1;

	printf("Введите дату релиза в формате ДД.ММ.ГГГГ\n");
	if (scanf("%s", &Base_t[*SizeMassiv - 1].realise) != 1) return 1;

	printf("Введите колличество установок у игры \n");;
	if (scanf("%d", &Base_t[*SizeMassiv - 1].installs) != 1) return 1;

	printf("Введите баланс игры\n");
	if (scanf("%f", &Base_t[*SizeMassiv - 1].balance) != 1) return 1;

	printf("Введите возможный метод оплаты\n");
	if (scanf("%s", &Base_t[*SizeMassiv - 1].paid) != 1) return 1;

	printf("Введите версию\n");
	if (scanf("%lf", &Base_t[*SizeMassiv - 1].version) != 1) return 1;
	return Base_t;
}

base_t* EditBasePole(base_t* Base_t, int EditNumber) {
	printf("Начальная запись выбранная для редактирования\t Имя: %s\nДата релиза: %s\nКолличество установок: %d\nБаланс %f\nМетод оплаты: %s\n Версия: %lf\n",
		Base_t[EditNumber].name, Base_t[EditNumber].realise, Base_t[EditNumber].installs, Base_t[EditNumber].balance, Base_t[EditNumber].paid, Base_t[EditNumber].version);
	printf("Введите название игры\n");
	if (scanf("%s", &Base_t[EditNumber].name) != 1) return 1;

	printf("Введите дату релиза в формате ДД.ММ.ГГГГ\n");
	if (scanf("%s", &Base_t[EditNumber].realise) != 1) return 1;

	printf("Введите колличество установок у игры \n");;
	if (scanf("%d", &Base_t[EditNumber].installs) != 1) return 1;

	printf("Введите баланс игры\n");
	if (scanf("%f", &Base_t[EditNumber].balance) != 1) return 1;

	printf("Введите возможный метод оплаты\n");
	if (scanf("%s", &Base_t[EditNumber].paid) != 1) return 1;

	printf("Введите версию\n");
	if (scanf("%lf", &Base_t[EditNumber].version) != 1) return 1;
	return Base_t;
}
