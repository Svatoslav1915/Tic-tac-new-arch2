#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

struct Tree {
	int NWL = 0;//ничья, поеда крестиков, победа ноликов
	vector<Tree*> branches;
	int turn = 1;
	int depth;
	int minmax;
	char **cells=new char*;
	Tree* mother;//указатель на материнский элемент
	int globaldepth;
};

//Принимает поле и его размер, возвращает Х если победили крестики, О если победили нолики, - если ничья
char WinLose(Tree*& branch, int n) {
											   //Итак нам нужен вектор в который будем закидывать значения сначала из каждой горизонтали, потом по вертикали и диагонали.
	vector<char> example_X, example_O, example;//Поверочные векторы, где все Х, О и наборный из дерева.
	for (int a = 0; a < n; a++) {
		example_X.push_back('X');
		example_O.push_back('O');
	}
		//Для горизонтали.
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (branch->cells[i][j] != '-') { example.push_back(branch->cells[i][j]); }
		}
		if (example.size() == n) {//если в экзампл поступили столько значений, каков размер поля, то они все не пустые, теперь проверим на совпадения
			if (example == example_X) { return 'X'; }
			if (example == example_O) { return 'O'; }
		}
		example.clear();//отчистим вектор чтобы дальше сравнивать с ним по горизонтали
	}
		//Для вертикали.
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (branch->cells[j][i] != '-') { example.push_back(branch->cells[j][i]); }
		}
		if (example.size() == n) {//если в экзампл поступили столько значений, каков размер поля, то они все не пустые, теперь проверим на совпадения
			if (example == example_X) { return 'X'; }
			if (example == example_O) { return 'O'; }
		}
		example.clear();//отчистим вектор чтобы дальше сравнивать с ним по вертикали
	}
		//Для главной диагонали.
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < n; j++) {
			if (branch->cells[j][j] != '-') { example.push_back(branch->cells[j][j]);}
		}
		if (example.size() == n) {//если в экзампл поступили столько значений, каков размер поля, то они все не пустые, теперь проверим на совпадения
			if (example == example_X) { return 'X'; }
			if (example == example_O) { return 'O'; }
		}
		example.clear();//отчистим вектор чтобы дальше сравнивать с ним по вертикали
	}
		//Для побочной диагонали.
	int j = n-1;
	for (int i = 0; i < n; i++) {
		if (branch->cells[j][i] != '-') { example.push_back(branch->cells[j][i]); j--; }
		}
		if (example.size() == n) {//если в экзампл поступили столько значений, каков размер поля, то они все не пустые, теперь проверим на совпадения
			if (example == example_X) { return 'X'; }
			if (example == example_O) { return 'O'; }
		}
		example.clear();//отчистим вектор чтобы дальше сравнивать с ним по вертикали
	return '-';// Если до сих пор функция не вернула X или O то значит ничья 
}

//Принимает поле, его размер, выводит все клетки с поля
void showcells(Tree*& branch, int n) {
	system("cls");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j == n - 1) { cout << '-' << i <<' ' << j << '-' << endl; }
			else { cout << '-' << i <<' ' << j << '-' << '|'; }
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j == n-1 ) { cout << '-' << branch->cells[i][j] << '-'<<endl;}
			else { cout << '-' << branch->cells[i][j] << '-'<<'|'; }
		}
	}
}

//Ставит метку в зависимости от текущего хода(Х/О) на незанятую позицию, если указана занятая, то просит переставить. 
//Для компьютера принимает значеня сразу, для человека просит ввести.
void makemove(Tree* branch,int index, int jindex) {
	if (index >=0||jindex>=0) {//если передадим отрицательные значения, то ходит человек
		if (branch->cells[index][jindex] == '-') {
			if (branch->turn == 0) { branch->cells[index][jindex] = 'O'; }
			if (branch->turn == 1) { branch->cells[index][jindex] = 'X'; }
		}
	}
	if (index < 0 || jindex < 0) {//для человека
		cout <<endl<<"Input number of cell you want to move: "<<endl ;
		int ind, jind;
		cin >> ind >> jind;
		if (branch->cells[ind][jind] != '-') {
			cout << endl << "This cell is already occupied, chose another one";
			makemove(branch, -1, -2);
		}
		if (branch->turn == 0) { branch->cells[ind][jind] = 'O'; }
		if (branch->turn == 1) { branch->cells[ind][jind] = 'X'; }
	}
}

//Создали дерево глубиной в 4 для конкретной позиции или в 8 для 3 на 3.
void BRANCH(Tree*& branch, int n) {
	if (WinLose(branch, n) == '-') {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (branch->cells[i][j] == '-') {
					Tree* vetka= new Tree;
					vetka->mother = branch;
					vetka->cells = new char* [n];
					vetka->NWL = 0;
					vetka->minmax = 0;
					vetka->depth = branch->depth + 1;
					vetka->globaldepth = branch->globaldepth + 1;
					//Зададим размерность массива
					for (int j = 0; j < n; j++) {
						vetka->cells[j] = new char[n];
					}
					//Заполним массив
					for (int i = 0; i < n; i++) {
						for (int j = 0; j < n; j++) {
							vetka->cells[i][j] = branch->cells[i][j];
						}
					}
					//Работаем с массивом
					if (branch->turn == 1) {
						vetka->turn = 0;
						vetka->cells[i][j] = 'X';
						if (WinLose(vetka, n) == 'X') { vetka->minmax = 10 - vetka->depth; vetka->NWL = 1; }
					}
					if (branch->turn == 0) {
						vetka->turn = 1;
						vetka->cells[i][j] = 'O';
						if (WinLose(vetka, n) == 'O') { vetka->minmax = -10 + vetka->depth; vetka->NWL = 2; }
					}
					branch->branches.push_back(vetka);
				}
			}
		}
		for (int i = 0; i < branch->branches.size(); i++) {
			if (n == 3) {
				if (branch->depth == 7) { break; }
			}
			else { if (branch->depth == 3) { break; } }
			BRANCH(branch->branches[i], n);
		}
	}
}

//Выставляет минимальные и максимальные значения для хода снизу вверх дерева.
void MIMAX(Tree*& branch) {
	for (int i = 0; i < branch->branches.size(); i++) {
		if (branch->branches.size() >= 0) { MIMAX(branch->branches[i]); }//заходим вглубь
	}
	if (branch->turn == 1) {//если походили нолики? то для хода ноликов ищем минимальные значения
		if (branch->minmax <= branch->mother->minmax) { 
			int min = branch->minmax;
			branch->mother->minmax = min; }
		}
	if (branch->turn == 0) {//если сюда ходили крестики, то для того хода ищем максимальное значение
		if (branch->minmax >= branch->mother->minmax) {
			int max=branch->minmax;
			branch->mother->minmax = max; }
	}
}

//Получает на вход текущее положение, выбирает по минмакс самый ценный ход(просто посмотреть значения и сравнить в branch->branches->minmax), затем делает ход, показывает позиции на экран, передаёт ход
//ещё нам нужно поискать значение клетки куда именно ходить боту согласно наилучшему переданному минмакс.
void game(Tree*& branch, int n, int first) {//n=size of field
	char ex[1];
	ex[0] = WinLose(branch, n);
	if (ex[0] == 'X') { cout << endl << "X has WON the game "; }
	if (ex[0] == 'O') { cout << endl << "O has WON the game "; }
	if (ex[0] == '-') {
		if (first == 1) {//если фирст=1, то ходит перый ходит компьютер крестиками, если фирст=0 то первый ходи человек крестиками
			if (branch->turn == 1) {//ход компьютера максимизирующий
				BRANCH(branch,n);
				MIMAX(branch);
				vector<int> miax;
				int index_of_max=0;
				for (int i = 0; i < branch->branches.size(); i++) {
					miax.push_back(branch->branches[i]->minmax);
				}
				index_of_max = distance(miax.begin(), (max_element(miax.begin(), miax.end())));//индекс хода в branches с максимальным значением
				for (int i = 0; i < n ; i++) {
					for (int j = 0; j < n ; j++) {
						if (branch->cells[i][j] != branch->branches[index_of_max]->cells[i][j]) {//ищем какие именно i и j нужно передать в makemove
							makemove(branch, i, j);
							cout << endl;
							showcells(branch, n);
							break;
						}
					}
				}
				Tree* vetka = new Tree;
				vetka->mother = vetka;
				vetka->cells = new char* [n];
				vetka->NWL = 0;
				vetka->minmax = 0;
				vetka->depth = 0;
				vetka->globaldepth = branch->globaldepth + 1;
				vetka->turn = 0;
				//Зададим размерность массива
				for (int j = 0; j < n; j++) {
					vetka->cells[j] = new char[n];
				}
				//Заполним массив
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n; j++) {
						vetka->cells[i][j] = branch->cells[i][j];
					}
				}
				game(vetka, n,1);//нью ван селл передаст текущий ход игроку и состояние поля 
			}
			if (branch->turn == 0) {
				makemove(branch, -3, -3);
				cout << endl;
				showcells(branch, n);
				Tree* vetka = new Tree;
				vetka->mother = vetka;
				vetka->cells = new char* [n];
				vetka->NWL = 0;
				vetka->minmax = 0;
				vetka->depth = 0;
				vetka->globaldepth = branch->globaldepth + 1;
				vetka->turn = 1;
				//Зададим размерность массива
				for (int j = 0; j < n; j++) {
					vetka->cells[j] = new char[n];
				}
				//Заполним массив
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n; j++) {
						vetka->cells[i][j] = branch->cells[i][j];
					}
				}
				game(vetka, n, 1);
			}
		}
		if (first == 0) {//фирст=0 то первый ходит человек крестиками, компьютер ноликами
			if (branch->turn == 1) {
				makemove(branch, -3, -3);
				cout << endl;
				showcells(branch, n);
				Tree* vetka = new Tree;
				vetka->mother = vetka;
				vetka->cells = new char* [n];
				vetka->NWL = 0;
				vetka->minmax = 0;
				vetka->depth = 0;
				vetka->globaldepth = branch->globaldepth + 1;
				vetka->turn = 0;
				//Зададим размерность массива
				for (int j = 0; j < n; j++) {
					vetka->cells[j] = new char[n];
				}
				//Заполним массив
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n; j++) {
						vetka->cells[i][j] = branch->cells[i][j];
					}
				}
				game(vetka, n, 0);
			}
			if (branch->turn == 0) {//компьютер-минимизирующий
				BRANCH(branch, n);
				MIMAX(branch);
				vector<int> miax;
				int index_of_min;
				for (int i = 0; i < branch->branches.size(); i++) {
					miax.push_back(branch->branches[i]->minmax);
				}
				index_of_min = distance(miax.begin(), (min_element(miax.begin(), miax.end())));//индекс хода в branches с минимальным значением
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n ; j++) {
						if (branch->cells[i][j] != branch->branches[index_of_min]->cells[i][j]) {//ищем какие именно i и j нужно передать в makemove
							makemove(branch, i, j);
							cout << endl;
							showcells(branch, n);
						}
					}
				}
				Tree* vetka = new Tree;
				vetka->mother = vetka;
				vetka->cells = new char* [n];
				vetka->NWL = 0;
				vetka->minmax = 0;
				vetka->depth = 0;
				vetka->globaldepth = branch->globaldepth + 1;
				vetka->turn = 1;
				//Зададим размерность массива
				for (int j = 0; j < n; j++) {
					vetka->cells[j] = new char[n];
				}
				//Заполним массив
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n; j++) {
						vetka->cells[i][j] = branch->cells[i][j];
					}
				}
				game(vetka, n, 0);
			}
		}
	}
}

//Игра между компьютерами, как итог они играют одинаково ввиду своей идентичности.
void gamecomputer(Tree*& branch, int n) {
	BRANCH(branch, n);
	MIMAX(branch);
	vector<int> miax;
	int index_of_max = 0;
	for (int i = 0; i < branch->branches.size(); i++) {
		miax.push_back(branch->branches[i]->minmax);
	}
	index_of_max = distance(miax.begin(), (max_element(miax.begin(), miax.end())));//индекс хода в branches с максимальным значением
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (branch->cells[i][j] != branch->branches[index_of_max]->cells[i][j]) {//ищем какие именно i и j нужно передать в makemove
				makemove(branch, i, j);
				cout << endl;
				showcells(branch, n);
				break;
			}
		}
	}
	int tr = 0;
	if (branch->turn == 0) { tr = 1; }
	if (branch->turn == 1) { tr = 0; }
	Tree* vetka = new Tree;
	vetka->mother = vetka;
	vetka->cells = new char* [n];
	vetka->NWL = 0;
	vetka->minmax = 0;
	vetka->depth = 0;
	vetka->globaldepth = branch->globaldepth + 1;
	vetka->turn = tr;
	//Зададим размерность массива
	for (int j = 0; j < n; j++) {
		vetka->cells[j] = new char[n];
	}
	//Заполним массив
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			vetka->cells[i][j] = branch->cells[i][j];
		}
	}
	gamecomputer(vetka, n);//нью ван селл передаст текущий ход игроку и состояние поля 
}

//Функция для игры человек VS человек.
void gamehumanic(Tree*& root, int n) {
	char ex[1];
	ex[0] = WinLose(root, n);
	if (ex[0] == 'X') { cout << endl << "X has WON the game "; }
	if (ex[0] == 'O') { cout << endl << "O has WON the game "; }
	if (ex[0] == '-') {
		makemove(root, -3, -3);
		showcells(root, n);
		int tr = 0;
		if (root->turn == 0) { tr = 1; }
		if (root->turn == 1) { tr = 0; }
		Tree* vetka = new Tree;
		vetka->mother = vetka;
		vetka->cells = new char* [n];
		vetka->NWL = 0;
		vetka->minmax = 0;
		vetka->depth = 0;
		vetka->globaldepth = root->globaldepth + 1;
		vetka->turn = tr;
		//Зададим размерность массива
		for (int j = 0; j < n; j++) {
			vetka->cells[j] = new char[n];
		}
		//Заполним массив
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				vetka->cells[i][j] = root->cells[i][j];
			}
		}
		gamehumanic(vetka, n);
	}
}


int main() {
	int n = 0;
	while (n < 3 || n>5) {
		cout << endl << "Size of field(3-5): ";
		cin >> n;
	}
	Tree* root = new Tree;
	root->turn = 1;
	root->mother = root;
	root->cells = new char*[n];
	root->depth = 0;
	root->globaldepth = 0;
	root->minmax = 0;
	for (int j = 0; j < n; j++) {
		root->cells[j] = new char[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			root->cells[i][j] = '-';
		}
	}
	int turn = 0;
	int bot = 0;
	showcells(root, n);
	cout << endl;
	while (bot < 1 || bot>3) {
		cout << endl << "1) PLAYER VS PLAYER" << endl << "2) PLAYER VS COMPUTER" << endl << "3) COMPUTER vs COMPUTER(beta)" << endl;
		cin >> bot;
	}
	if(bot ==1){
		gamehumanic(root, n);
	}
	if (bot == 2) {
		cout << "Which side you want to play?" << endl << "0 to play first" << endl << "1 to play second";
		int side = 0;
		cin >> side;
		game(root, n,side);
	}
	if (bot == 3) {
		gamecomputer(root, n);
	}
	
	return 0;
}