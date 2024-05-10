#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

struct incident
{
	int transition_number;	//вершина, куда идет ребро
	string x;	//помечающая функция 
	long double q;	//вероятность перехода
};

struct vertex
{
	int number; //номер вершины
	long double p_i;	//вероятность того, что она начальная
	long double e_i;	//вероятность того, что она конечная
	incident *edge;	//ребра вершины
	int count_incident;	//кол-во инциденций
};

int non_zero_elements(long double* P, int size, int i)	//поиск ненулевых элементов в матрицах
{
	int count = 0;
	for (int j = 0; j < size; j++)
		if (P[i * size + j] != 0)
			count += 1;
	return count;
}

void input_vertex(vertex* state, long double* p, long double* e, int size)
{
	for (int i = 0; i < size; i++)
	{
		state[i].number = i;
		state[i].p_i = p[i];
		state[i].e_i = e[i];
	}
}

void input_incident(int number, incident* edge, long double** P, int size_P, int count_P, string* X)
{
	int k = 0;
	for (int i = 0; i < count_P; i++)
	{
		for (int j = 0; j < size_P; j++)
		{
			if (P[i][number * size_P + j] != 0)
			{
				edge[k].q = P[i][number * size_P + j];
				edge[k].transition_number = j;
				edge[k].x = X[i];
				k++;
			}
		}
	}
}

void input_X(string* X, int size)
{
	ifstream fin("X.txt");
	string empty;
	cout << "\nВведите буквы входного алфавита (x1, x2 и тд.):\n";
	for (int i = 0; i < size; i++)
	{
		//cin >> X[i];
		fin >> X[i];
		cout << X[i] << endl;
		if (X[i] == empty)
		{
			cout << "!В файле недостаточно информации!";
			exit(0);
		}
	}
	fin.close();
}

void print_X(string* X, int size)
{
	cout << "\n\n";
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
			cout << "X = <" << X[i] << ", ";
		else if (i == size - 1)
			cout << X[i] << ">" << endl;
		else
			cout << X[i] << ", ";
	}
}

void input_p(long double* p, int size)	//ф-я заполнения вектора p
{
	for (int i = 0; i < size; i++)	
	{
		if (i == 0)
			p[i] = 1;
		else
			p[i] = 0;
		cout << "p[" << i << "] = " << p[i] << endl;

	}
}

void print_p(long double* p, int size)	//ф-я печати вектора p
{
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
			cout << "p = (" << p[i] << ", ";
		else if (i == size - 1)
			cout << p[i] << ")" << endl;
		else
			cout << p[i] << ", ";
			
	}
}

void input_e(long double* e, int size)	//ф-я заполнения вектора e
{
	long double sum = 0;
	ifstream fin("e.txt");
	while (sum != 1) 
	{
		sum = 0;
		for (int i = 0; i < size; i++)	
		{
			cout << "e[" << i << "] = ";
			fin >> e[i];
			cout << e[i] << endl;
			sum += e[i];
		}
		if (sum != 1)
		{
			cout << "!Неверно введённый вектор! Сумма значений всех элементов должна быть равна 1." << endl;
			cout << "Полученная сумма равна " << sum << endl;
			fin.close();
			exit(0);
		}
	}
	fin.close();
}

void print_e(long double* e, int size)	//ф-я печати вектора e
{
	for (int i = 0; i < size; i++)
	{
		if (i == 0)
			cout << "e = (" << e[i] << ", ";
		else if (i == size - 1)
			cout << e[i] << ")^T" << endl;
		else
			cout << e[i] << ", ";
	}
}

void input_P(ifstream &fin,long double* P, int size, int k)	//ф-я заполнения матрицы и проверки, что в строке сумма значений = 1
{
	long double sum;
	long double sum1 = 1.0;
	cout << "\nВвод матрицы P(" << k << "):" << endl;
	for (int i = 0; i < size; i++)
	{
		sum = 0;
		for (int j = 0; j < size; j++)
		{
			cout << "P(" << k << ")[" << i << "][" << j << "] = ";
			fin >> P[i * size + j];
			cout << P[i * size + j] << endl;
			sum += P[i * size + j];
		}
		if (sum != (long double)1.0)
		{ 
			cout << "!Неверно введённая матрица! Сумма значений всех элементов в каждой строке должна быть равна 1" << endl;
			cout << "В " << i << " строке сумма значений всех её элементов равна " << sum << endl;
			fin.close();
			exit(0);
		}
	}
}

void print_matrix(long double* P, int size)	//ф-я печати матрицы переходов
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if(i != size-1)
				cout << setw(12) << P[i * size + j] << setw(12);
			else 
				cout << setw(12) << P[i * size + j];
		}
		cout << endl;
	}
	return;
}

void input_word(string* word, string* X, int size_word, int size_X)
{
	int j;
	string empty_word = { "e" };
	cout << "Введите последовательно буквы, входящие в слово:" << endl;
	for (int i = 0; i < size_word; i++) {
		cin >> word[i];
		j = 0;
		while (word[i] != X[j])
		{
			if (word[i] == empty_word)
				break;
			j++;
			if ((word[i] != X[j]) && (j == size_X - 1))
			{
				cout << "Такой буквы нет, введите ещё раз" << endl;
				i--;
				break;
			}

		}
	}
}

void print_word(string* word, int size)
{
	cout << "\nПолученное слово: w = ";
	for (int i = 0; i < size; i++)
	{
		cout << word[i];
	}
	cout << "\n";
}

void identity_matrix(long double* multiplied_matrices, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j)
				multiplied_matrices[i * size + j] = 1;
			else
				multiplied_matrices[i * size + j] = 0;
		}
	}
}

long double* multi_matrix(long double* P, long double* multiplied_matrices, int size)
{
	long double* result{ new long double[size*size] };	
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++) 
		{
			result[i * size + j] = 0;
			for (int k = 0; k < size; k++)
				result[i * size + j] += multiplied_matrices[i * size + k] * P[k * size + j];
		}
	}
	return result;
}

long double multi(long double* multiplied_matrices, long double* p, long double* e, int size)
{
	int i, j, k;
	long double* row{ new long double[size] };	//результат умножения строки p на матрицу
	long double result = 0;	//результат умножения полученной строки row на столбец e
	for (j = 0; j < size; j++)
	{
		row[j] = 0;
		for (k = 0; k < size; k++)
		{
			row[j] += p[k] * multiplied_matrices[k * size + j];
		}
	}
	cout << "\nРезультат умножения вектора p на полученную матрицу P(w):" << endl;
	for (i = 0; i < size; i++)
	{
		if (i == 0)
			cout << "(" << row[i] << ", ";
		else if (i == size - 1)
			cout << row[i] << ")" << endl;
		else
			cout << row[i] << ", ";
	}
	for (i = 0; i < size; i++)
	{
		result += row[i] * e[i];
	}
	delete[] row;
	return result;
}

void print_vertex(vertex* state, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << endl << state[i].number << " вершина\n{\nначальная вероятность " << state[i].p_i << ", конечная вероятность " << state[i].e_i << ", количество инциденций " << state[i].count_incident << ":" << endl;
		for (int j = 0; j < state[i].count_incident; j++)
		{
			cout << "по букве " << state[i].edge[j].x << " с вероятностью " << state[i].edge[j].q << " переходим в вершину " << state[i].edge[j].transition_number << endl;
		}
		cout << "}" << endl;
	}
}

void reset(long double** matrix, int size)
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = 0;
			if ((matrix[1][j + 1] == 0) && (j + 1 != size))
				break;
		}
}

void input_matrix_stochastic(vertex* state, long double** matrix, long double** duplicate_matrix, int size, string X)
{
	int j, i, m, pos_duplicate = 0;
	for (i = 0; i < size; i++) //пробегаемся по вектору состояний ссп
	{
		if (matrix[1][i] != 0)
		{
			j = 0;
			while (matrix[0][i] != state[j].number)
				j++;
			for (m = 0; m < state[j].count_incident; m++)
				if (X == state[j].edge[m].x)
				{
					duplicate_matrix[0][pos_duplicate] = state[j].edge[m].transition_number;
					duplicate_matrix[1][pos_duplicate] = matrix[1][i] * state[j].edge[m].q;
					pos_duplicate++;
				}
		}
	}
	for (i = 0; i < 2; i++)	//приравнивание дубликата текущих состояний
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = duplicate_matrix[i][j];
			if ((duplicate_matrix[1][j + 1] == 0) && (j + 1 != size))
				break;
		}
	}
}

void print_matrix_stochastic(long double** matrix, int size)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((matrix[1][j + 1] == 0) && (j + 1 != size))
			{
				cout << setw(12) << matrix[i][j];
				break;
			}
			else if (j == size-1)
				cout << setw(12) << matrix[i][j];
			else
				cout << setw(12) << matrix[i][j] << setw(12);
		}
		cout << endl;
	}
	return;
}

long double sum(long double** matrix, vertex* state, int size)
{
	long double result = 0;
	int number_vertex, i = 0;
	cout << "m(w) = 0";
	while ((matrix[1][i] != 0) && (i < size)) //пробегаемся по вектору состояний ссп
	{
		number_vertex = matrix[0][i];
		if (state[number_vertex].e_i != 0)
		{
			result += state[number_vertex].e_i * matrix[1][i];
			cout <<" + " << state[number_vertex].e_i << "*" << matrix[1][i];
		}
		i++;
	}
	cout << endl;
	return result;
}

int main() {
	setlocale(LC_ALL, "Rus");
	int n, N, i,j, k, end, l, pos_duplicate;
	string empty_word = { "e" };
	long double result_machine, result_Petri;

	cout << "Введите количество букв входного алфавита\nn = ";
	cin >> n;
	cout << "\nВведите количество принимаемых автоматом состояний\nN = ";
	cin >> N;
	string* X{ new string[n] };
	input_X(X, n);

	long double* p{ new long double[N] };	//вектор начальных состояний p
	cout << "\nЗначения вектора начальных состояний\n";
	input_p(p, N);
	
	long double* e{ new long double[N] };	//вектор конечных состояний e
	cout << "\nЗначения вектора конечных состояний\n";
	input_e(e, N);
	
	ifstream fin("P.txt");
	long double **P = new long double* [n];	//матрица указателей на матрицы входов и выходов
	for (i = 0; i < n; i++)
	{
		P[i] = new long double [N * N];
		input_P(fin, P[i], N, i);
	}
	fin.close();

	//вывод полученных векторов и матриц вероятностного автомата на экран
	print_X(X, n);
	print_p(p, N);	
	print_e(e, N);
	for (i = 0; i < n; i++)
	{
		cout << "P(" << i << "):" << endl;
		print_matrix(P[i], N);
	}

	//создаем ССП
	vertex* state{ new vertex[N] };	//создаем N вершин (сколько и состояний)
	int count_edge;	//счетчик ненулевых элементов в матрице
	for (i = 0; i < N; i++)
	{
		count_edge = 0;
		for (j = 0; j < n; j++)
			count_edge += non_zero_elements(P[j], N, i);
		state[i].edge = new incident[count_edge];	//создаем массив структур инцинденций у каждой вершины
		state[i].count_incident = count_edge;
	}
	input_vertex(state, p, e, N);	//заполняем для каждой вершины её номер, начальную и конечную вероятность
	
	for (i = 0; i < N; i++)
		input_incident(state[i].number, state[i].edge, P, N, n, X);	//заполняем инциденции

	long int all_incident = 0;
	for (i = 0; i < N; i++)	//считаем все инциденции для матриц текущих состояний
		all_incident += state[i].count_incident;
	all_incident *= 100;

	//вывод вершин 
	cout << endl << "Вершины ССП и их инциденции:" << endl;
	print_vertex(state, N);

	while (1)
	{
		cout << "\nВведите размер слова = ";
		cin >> k;
		string* word{ new string[k] };	//слово из k букв
		input_word(word, X, k, n);
		print_word(word, k);

		long double* multiplied_matrices{ new long double[N * N] };	//перемноженные матрицы
		long double** vertex_Petri = new long double* [2];	//вектор состояний, в которых мы находимся с какой-то вероятностью
		for (i = 0; i < 2; i++)
		{
			vertex_Petri[i] = new long double[all_incident];
		}
		reset(vertex_Petri, all_incident);	//обнуляем матрицу текущих состояний
		vertex_Petri[1][0] = 1;

		long double** duplicate = new long double* [2];	
		for (i = 0; i < 2; i++)
		{
			duplicate[i] = new long double[all_incident];
		}
		reset(duplicate, all_incident);	//обнуляем дубликат текущих состояний
		
		cout << "\nПромежуточные вычисления в ССП:" << endl;

		cout << endl << "Матрица вероятностей нахождения в вершинах ССП в нулевой момент времени:" << endl;
		print_matrix_stochastic(vertex_Petri, all_incident);

		if (word[0] == empty_word)
		{
			identity_matrix(multiplied_matrices, N);	//единичная матрица для пустого слова e
		}
		else
		{
			for (i = 0; i < k; i++)
			{
				j = 0;
				while ((word[i] != X[j]) && j < n)
				{
					j++;
				}
				//вычисление в вероятностном автомате
				if (i == 0)
				{
					identity_matrix(multiplied_matrices, N);
					multiplied_matrices = multi_matrix(P[j], multiplied_matrices, N);
				}
				else
				{
					multiplied_matrices = multi_matrix(P[j], multiplied_matrices, N);
				}

				//вычисление в ссп
				pos_duplicate = 0;
				input_matrix_stochastic(state, vertex_Petri, duplicate, all_incident, word[i]);	//заполняем матрицу текущих состояний

				cout << endl << "Матрица вероятностей нахождения в вершинах ССП при поступлении буквы " << X[j] << endl;
				print_matrix_stochastic(vertex_Petri, all_incident);

				reset(duplicate, all_incident);	//обнуляем дубликат текущих состояний
			}
		}
		cout << "\nВычисление веса слова в ССП:" << endl;
		result_Petri = sum(vertex_Petri, state, all_incident);
		cout << "\nВес слова в ССП: m(w) = " << result_Petri << endl;

		cout << "\nПромежуточные вычисления в вероятностном автомате:" << endl;
		cout << "P(w):" << endl;
		print_matrix(multiplied_matrices, N);
		print_p(p, N);
		result_machine = multi(multiplied_matrices, p, e, N);
		cout << "\nВес слова в вероятностном автомате: m(w) = " << result_machine << endl;

		cout << "\nВвести новое слово? (1 - да, 0 - завершить работу программы)" << endl;
		cin >> end;
		if (end == 0)
		{
			delete[] multiplied_matrices;
			delete[] word;
			for (i = 0; i < 2; i++)
			{
				delete[] vertex_Petri[i];
				delete[] duplicate[i];
			}
			delete[] vertex_Petri;
			delete[] duplicate;
			break;
		}
		delete[] multiplied_matrices;
		delete[] word;
		for (i = 0; i < 2; i++)
		{
			delete[] vertex_Petri[i];
		}
		delete[] vertex_Petri;
	}

	delete[] X;
	delete[] p;
	delete[] e;
	for (i = 0; i < n; i++)
	{
		delete[] P[i];
	}
	delete[] P;
	for (i = 0; i < N; i++)
			delete[] state[i].edge;
	delete[] state;
	return 1;
}