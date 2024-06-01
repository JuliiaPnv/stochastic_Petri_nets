#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

struct incident
{
	int transition_number;	//the vertex where the edge goes
	string x;	//the value of the tagging function
	long double q;	//the probability of transition
};

struct vertex
{
	int number; //vertex number
	long double p_i;	//the probability that it is initial
	long double e_i;	//the probability that it is finite
	incident* edge;	//the edges of the vertex
	int count_incident;	//number of edges
};

int non_zero_elements(long double* P, int size, int i)	//search for non-zero elements in matrices
{
	int count = 0;
	for (int j = 0; j < size; j++)
		if (P[i * size + j] != 0)
			count += 1;
	return count;
}

void input_vertex(vertex* state, long double* p, long double* e, int size)	//initialization of vertex structures
{
	for (int i = 0; i < size; i++)
	{
		state[i].number = i;
		state[i].p_i = p[i];
		state[i].e_i = e[i];
	}
}

void input_incident(int number, incident* edge, long double** P, int size_P, int count_P, string* X)	//initializing vertex incidents
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

void input_X(string* X, int size)	//filling in the input alphabet
{
	ifstream fin("X.txt");
	string empty;
	cout << "\nLetters of the input alphabet (x1, x2, etc):\n";
	for (int i = 0; i < size; i++)
	{
		fin >> X[i];
		cout << X[i] << endl;
		if (X[i] == empty)
		{
			cout << "!There is not enough information in the file!";
			exit(0);
		}
	}
	fin.close();
}

void print_X(string* X, int size)	//output of the input alphabet
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

void input_p(long double* p, int size)	//filling in the vector p
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

void print_p(long double* p, int size)	//output of the vector p
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

void input_e(long double* e, int size)	//filling in the vector e
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
			cout << "!Incorrectly entered vector! The sum of the values of all elements must be equal to 1." << endl;
			cout << "The amount received is equal to " << sum << endl;
			fin.close();
			exit(0);
		}
	}
	fin.close();
}

void print_e(long double* e, int size)	//output of the vector e
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

void input_P(ifstream& fin, long double* P, int size, int k)	//filling in transition matrices
{
	long double sum;
	long double sum1 = 1.0;
	cout << "\nEntering the matrix P(" << k << "):" << endl;
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
			cout << "!Incorrectly entered matrix! The sum of the values of all the elements in each row must be equal to 1." << endl;
			cout << "In " << i << " row, the sum of the values of all its elements is equal to " << sum << endl;
			fin.close();
			exit(0);
		}
	}
}

void print_matrix(long double* P, int size)	//output of transition matrices
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i != size - 1)
				cout << setw(12) << P[i * size + j] << setw(12);
			else
				cout << setw(12) << P[i * size + j];
		}
		cout << endl;
	}
	return;
}

void input_word(string* word, string* X, int size_word, int size_X)	//entering a word
{
	int j;
	string empty_word = { "e" };
	cout << "Enter the letters included in the word sequentially:" << endl;
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
				cout << "There is no such letter, enter it again" << endl;
				i--;
				break;
			}

		}
	}
}

void print_word(string* word, int size)	//output of the entered word
{
	cout << "\nThe received word: w = ";
	for (int i = 0; i < size; i++)
	{
		cout << word[i];
	}
	cout << "\n";
}

void identity_matrix(long double* multiplied_matrices, int size)	//input the identity matrix
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

long double* multi_matrix(long double* P, long double* multiplied_matrices, int size)	//matrix multiplication
{
	long double* result{ new long double[size * size] };
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

long double word_weight_in_machine(long double* multiplied_matrices, long double* p, long double* e, int size)	//matrix multiplication by vectors and counting the probability of a word
{
	int i, j, k;
	long double* row{ new long double[size] };	//the result of multiplying the vector p by the matrix
	long double result = 0;	//the result of multiplying the resulting row vector by the e vector
	for (j = 0; j < size; j++)
	{
		row[j] = 0;
		for (k = 0; k < size; k++)
		{
			row[j] += p[k] * multiplied_matrices[k * size + j];
		}
	}
	cout << "\nThe result of multiplying the vector p by the resulting matrix P(w):" << endl;
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

void print_vertex(vertex* state, int size)	//vertex output
{
	for (int i = 0; i < size; i++)
	{
		cout << endl << state[i].number << " vertex\n{\ninitial probability " << state[i].p_i << ", final probability " << state[i].e_i << ", number of incidents " << state[i].count_incident << ":" << endl;
		for (int j = 0; j < state[i].count_incident; j++)
		{
			cout << "by letter " << state[i].edge[j].x << " with probability " << state[i].edge[j].q << " moving to the vertex " << state[i].edge[j].transition_number << endl;
		}
		cout << "}" << endl;
	}
}

void reset(long double** matrix, int size)	//zeroing the matrix
{
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = 0;
			if ((matrix[1][j + 1] == 0) && (j + 1 != size))
				break;
		}
}

void input_markup(vertex* state, long double** matrix, long double** duplicate_matrix, int size, string X)	//filling in the markup of a stochastic Petri net
{
	int j, i, m, pos_duplicate = 0;
	for (i = 0; i < size; i++) //let's run through the vector of states of the stochastic Petri net
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

	for (i = 0; i < 2; i++)	//equating a duplicate of the current states
	{
		for (j = 0; j < size; j++)
		{
			matrix[i][j] = duplicate_matrix[i][j];
			if ((duplicate_matrix[1][j + 1] == 0) && (j + 1 != size))
				break;
		}
	}
}

void print_markup(long double** matrix, int size)	//output of the markup of a stochastic Petri net
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
			else if (j == size - 1)
				cout << setw(12) << matrix[i][j];
			else
				cout << setw(12) << matrix[i][j] << setw(12);
		}
		cout << endl;
	}
	return;
}

long double** sum_of_probabilities(long double** matrix, int l, int size)	//summing up the probabilities of tricks in the same position
{
	int i, j, k = 0, m = 0;
	for (i = 0; i < l; i++)
	{
		{
			if ((matrix[1][i] != 0))
				for (j = i + 1; j < l; j++)
					if ((matrix[0][i] == matrix[0][j]) && (matrix[1][j] != 0))
					{
						matrix[1][i] += matrix[1][j];
						matrix[1][j] = matrix[0][j] = 0;
					}
		}
	}

	for (i = 0; i < l; i++)
		if ((matrix[1][i] != 0))
			k++;

	long double** duplicate_matrix = new long double* [2];
	for (i = 0; i < 2; i++)
	{
		duplicate_matrix[i] = new long double[size];
	}
	for (i = 0; i < k; i++)
	{
		while ((m < l) && (matrix[1][m] == 0))
			m++;
		duplicate_matrix[0][i] = matrix[0][m];
		duplicate_matrix[1][i] = matrix[1][m];
		m++;
	}
	for (i = k; i < size; i++)
		duplicate_matrix[0][i] = duplicate_matrix[1][i] = 0;
	return duplicate_matrix;
}

long double word_weight_in_Petri(long double** matrix, vertex* state, int size)	//counting the probability of a word
{
	long double result = 0;
	int number_vertex, i = 0;
	cout << "m(w) = 0";
	while ((matrix[1][i] != 0) && (i < size))
	{
		number_vertex = matrix[0][i];
		if (state[number_vertex].e_i != 0)
		{
			result += state[number_vertex].e_i * matrix[1][i];
			cout << " + " << state[number_vertex].e_i << "*" << matrix[1][i];
		}
		i++;
	}
	cout << endl;
	return result;
}

int main() {
	//setlocale(LC_ALL, "Rus");
	int n, N, i, j, k, end, l, m1, m2;
	string empty_word = { "e" };
	long double result_machine, result_Petri;

	cout << "Enter the number of letters of the input alphabet\nn = ";
	cin >> n;
	cout << "\nEnter the number of states accepted by the machine\nN = ";
	cin >> N;
	string* X{ new string[n] };
	input_X(X, n);

	long double* p{ new long double[N] };	//vector of initial states p
	cout << "\nValues of the initial state vector\n";
	input_p(p, N);

	long double* e{ new long double[N] };	//vector of final states e
	cout << "\nValues of the finite state vector\n";
	input_e(e, N);

	ifstream fin("P.txt");
	long double** P = new long double* [n];	//a matrix of pointers to transition matrices
	for (i = 0; i < n; i++)
	{
		P[i] = new long double[N * N];
		input_P(fin, P[i], N, i);
	}
	fin.close();

	//output of the obtained vectors and matrices of the probabilistic automaton to the screen
	print_X(X, n);
	print_p(p, N);
	print_e(e, N);
	for (i = 0; i < n; i++)
	{
		cout << "P(" << i << "):" << endl;
		print_matrix(P[i], N);
	}

	//creating a stochastic Petri net
	vertex* state{ new vertex[N] };	//creating N vertex (how many states)
	int count_edge;	//counter of non-zero elements in the matrix

	input_vertex(state, p, e, N);	//for each vertex, its number, initial and final probability are filled in
	for (i = 0; i < N; i++)
	{
		count_edge = 0;
		for (j = 0; j < n; j++)
			count_edge += non_zero_elements(P[j], N, i);
		state[i].edge = new incident[count_edge];	//creating an array of incident structures at each vertex
		state[i].count_incident = count_edge;
	}

	for (i = 0; i < N; i++)
		input_incident(state[i].number, state[i].edge, P, N, n, X);	//filling out incidents

	long int all_incident = 0;
	for (i = 0; i < N; i++)
		all_incident += state[i].count_incident;
	all_incident *= 100;

	//vertex output
	cout << endl << "Vertices of the stochastic Petri net and their incidents:" << endl;
	print_vertex(state, N);

	long double* multiplied_matrices{ new long double[N * N] };
	long double** vertex_Petri = new long double* [2];	//marking up a stochastic Petri net
	for (i = 0; i < 2; i++)
		vertex_Petri[i] = new long double[all_incident];
	long double** duplicate = new long double* [2];
	for (i = 0; i < 2; i++)
		duplicate[i] = new long double[all_incident];

	while (1)
	{
		cout << "\nEnter the length of the word = ";
		cin >> k;
		string* word{ new string[k] };
		input_word(word, X, k, n);
		print_word(word, k);

		for (i = 0; i < N * N; i++)
			multiplied_matrices[i] = 0;

		reset(vertex_Petri, all_incident);	//zeroing out the matrix of current states
		vertex_Petri[1][0] = 1;

		reset(duplicate, all_incident);	//zeroing out the duplicate of current states

		cout << "\nIntermediate calculations in a stochastic Petri net:" << endl;

		cout << endl << "Marking up a stochastic Petri net at a zero point in time:" << endl;
		print_markup(vertex_Petri, all_incident);

		if (word[0] == empty_word)
		{
			identity_matrix(multiplied_matrices, N);	//the unit matrix for the empty word e
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
				//calculation in a probabilistic automaton
				if (i == 0)
				{
					identity_matrix(multiplied_matrices, N);
					multiplied_matrices = multi_matrix(P[j], multiplied_matrices, N);
				}
				else
				{
					multiplied_matrices = multi_matrix(P[j], multiplied_matrices, N);
				}

				//calculation in a stochastic Petri net
				input_markup(state, vertex_Petri, duplicate, all_incident, word[i]);	//filling in the markup

				l = 0;
				while (vertex_Petri[1][l] != 0)
					l++;
				vertex_Petri = sum_of_probabilities(vertex_Petri, l, all_incident);
				cout << endl << "Marking up a stochastic Petri net upon receipt of a letter " << X[j] << endl;
				print_markup(vertex_Petri, all_incident);

				reset(duplicate, all_incident);	//zeroing out the duplicate of current states
			}
		}
		cout << "\nCalculating the weight of a word in a stochastic Petri net:" << endl;
		result_Petri = word_weight_in_Petri(vertex_Petri, state, all_incident);
		cout << "\nThe weight of a word in a stochastic Petri net: m(w) = " << result_Petri << endl;

		cout << "\nIntermediate calculations in a probabilistic automaton:" << endl;
		cout << "P(w):" << endl;
		print_matrix(multiplied_matrices, N);
		print_p(p, N);
		result_machine = word_weight_in_machine(multiplied_matrices, p, e, N);
		cout << "\nThe weight of a word in a probabilistic automaton: m(w) = " << result_machine << endl;

		cout << "\nWould you like to introduce a new word?? (1 - yes, 0 - stop the program)" << endl;
		cin >> end;
		//deleting dynamic matrices at the end of the program
		if (end == 0)
		{
			delete[] word;

			break;
		}
		delete[] word;
	}

	delete[] multiplied_matrices;
	for (i = 0; i < 2; i++)
	{
		delete[] vertex_Petri[i];
		delete[] duplicate[i];
	}
	delete[] vertex_Petri;
	delete[] duplicate;
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