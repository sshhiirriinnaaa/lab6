#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <stack>
#include <algorithm>
#include <conio.h>
#include <limits>
#include <random>

using namespace std;

vector<vector<int>> generateAndPrintMatrix(int num_vertices, double edge_probability) {
	vector<vector<int>> adjacency_matrix(num_vertices + 1, vector<int>(num_vertices + 1, 0));
	int num_edges = 0;

	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = i + 1; j <= num_vertices; ++j) {
			if (static_cast<double>(rand()) / RAND_MAX < edge_probability) {
				adjacency_matrix[i][j] = 1;
				adjacency_matrix[j][i] = 1;
				num_edges++;
			}
		}
	}

	cout << "������� ���������:\n";
	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = 1; j <= num_vertices; ++j) {
			cout << adjacency_matrix[i][j] << ' ';
		}
		cout << endl;
	}
	return adjacency_matrix;
}

vector<vector<int>> matrixToList(const vector<vector<int>>& matrix) {
	int num_vertices = matrix.size() - 1;
	vector<vector<int>> adjacency_list(num_vertices + 1);

	for (int i = 1; i <= num_vertices; ++i) {
		for (int j = 1; j <= num_vertices; ++j) {
			if (matrix[i][j] == 1) {
				adjacency_list[i].push_back(j);
			}
		}
	}

	return adjacency_list;
}

void printList(const vector<vector<int>>& list) {
	cout << "������ ���������:\n";
	for (int i = 1; i < list.size(); ++i) {
		cout << i << ": ";
		for (const int& j : list[i]) {
			cout << j << ' ';
		}
		cout << endl;
	}
}

void printMatrix(const vector<vector<int>>& matrix) {
	cout << "������� ���������:\n";
	for (int i = 1; i < matrix.size(); i++) {
		for (int j = 1; j < matrix[i].size(); j++) {
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}
}

//�������������� ������
void identifyVertices(const vector < vector < int>>& originalMatrix, const vector < vector < int>>& originalList, int vertex1, int vertex2, vector < vector < int>>& resultMatrix, vector < vector < int>>& resultList) {
	// �������� ����� �������� ������� � ������ ���������
	resultMatrix = originalMatrix;
	resultList = originalList;

	int n = resultMatrix.size();

	// ���������� ����� ������� ���������
	for (int i = 0; i < n; ++i) {
		if (resultMatrix[vertex2][i]) {
			resultMatrix[vertex1][i] = 1;
			resultMatrix[i][vertex1] = 1;
		}
		resultMatrix[vertex2][i] = 0;
		resultMatrix[i][vertex2] = 0;
	}


	// ���������� ����� ������ ���������
	for (int neighbor : resultList[vertex2]) {
		if (find(resultList[vertex1].begin(), resultList[vertex1].end(), neighbor) == resultList[vertex1].end() && neighbor != vertex1) {
			resultList[vertex1].push_back(neighbor);
		}
	}

	// �������� vertex2 �� ���� ������� ��������� � �����
	for (auto& neighbors : resultList) {
		auto it = remove(neighbors.begin(), neighbors.end(), vertex2);
		neighbors.erase(it, neighbors.end());
		for (auto& neighbor : neighbors) {
			if (neighbor > vertex2) {
				--neighbor;
			}
		}
	}

	// �������� vertex2 �� ����� �������
	resultMatrix.erase(resultMatrix.begin() + vertex2);
	for (auto& row : resultMatrix) {
		row.erase(row.begin() + vertex2);
	}

	// �������� ������ ��������� ��� vertex2 � ���������� ����������� �������� � �����
	resultList.erase(resultList.begin() + vertex2);
}


//���������� �����
void contractEdge(const vector<vector<int>>& originalMatrix, const vector<vector<int>>& originalList, int vertex1, int vertex2, vector<vector<int>>&
	resultMatrix, vector<vector<int>>& resultList) {
	resultMatrix = originalMatrix;
	resultList = originalList;

	int n = resultMatrix.size();

	// �������� �� ������� ����� ����� vertex1 � vertex2
	if (!resultMatrix[vertex1][vertex2] && !resultMatrix[vertex2][vertex1]) {
		printf("��� ����� ����� %d � %d ��� ����������.\n", vertex1, vertex2);
		return;
	}

	// ����������� ������ � ����� ������� ���������
	for (int i = 0; i < n; ++i) {
		if (resultMatrix[vertex2][i]) {
			resultMatrix[vertex1][i] = 1;
			resultMatrix[i][vertex1] = 1;
		}
		resultMatrix[vertex2][i] = 0;
		resultMatrix[i][vertex2] = 0;
	}
	resultMatrix[vertex1][vertex1] = 0;

	// ����������� ������� ���������
	for (int neighbor : resultList[vertex2]) {
		if (find(resultList[vertex1].begin(), resultList[vertex1].end(), neighbor) == resultList[vertex1].end() && neighbor != vertex1) {
			resultList[vertex1].push_back(neighbor);
		}
	}

	// �������� vertex2 �� ���� ������� ��������� � �����
	for (auto& neighbors : resultList) {
		auto it = remove(neighbors.begin(), neighbors.end(), vertex2);
		neighbors.erase(it, neighbors.end());
	}

	// �������� vertex2 �� ����� ������� � ������ ���������
	resultMatrix.erase(resultMatrix.begin() + vertex2);
	for (auto& row : resultMatrix) {
		row.erase(row.begin() + vertex2);
	}
	resultList.erase(resultList.begin() + vertex2);
}

//����������� �������
void splitVertex(vector < vector < int>>& originalMatrix, vector < vector < int>>& originalList, int vertex, vector < vector < int>>& resultMatrix, vector < vector < int>>& resultList) {
	int n = originalMatrix.size();

	// ������� ����� ������� ��������� � ����� ������ ���������
	resultMatrix.resize(n + 1, vector<int>(n + 1, 0));
	resultList.resize(n + 1);

	// �������� �������� ������� ��������� � ������ ��������� � ����� ���������
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			resultMatrix[i][j] = originalMatrix[i][j];
		}
		resultList[i] = originalList[i];
	}

	// �������� �������, ������� �������������
	for (int i = 0; i < n; ++i) {
		resultMatrix[i][n] = originalMatrix[i][vertex];
		resultMatrix[n][i] = originalMatrix[vertex][i];
	}

	// ��������� ����� ������� � ������ ���������
	for (int i = 0; i < n; ++i) {
		if (i != vertex) {
			resultList[i].push_back(n);
		}
	}

	// ��������� ����� ������� � ����� ������ ���������
	for (int i = 0; i < originalList[vertex].size(); ++i) {
		resultList[n].push_back(originalList[vertex][i]);

	}
	resultList[n].push_back(vertex);
	resultList[vertex].push_back(n);
	resultMatrix[vertex][n] = 1;
	resultMatrix[n][vertex] = 1;
}

//�����������
vector < vector < int>> mergeGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int maxSize = max(graph1.size(), graph2.size());

	// �������� ����� ������� ��������� ��� ������������� �����
	vector < vector < int>> mergedGraph(maxSize, vector<int>(maxSize, 0));

	// ����������� ������ �� ������� �����
	for (int i = 0; i < graph1.size(); ++i) {
		for (int j = 0; j < graph1[i].size(); ++j) {
			mergedGraph[i][j] = graph1[i][j];
		}
	}

	// ���������� ��� ���������� ������ �� ������� �����
	for (int i = 0; i < graph2.size(); ++i) {
		for (int j = 0; j < graph2[i].size(); ++j) {
			if (graph2[i][j] > 0) {
				mergedGraph[i][j] = graph2[i][j];
			}
		}
	}

	return mergedGraph;
}

//�����������
vector < vector < int>> intersectGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int size = min(graph1.size(), graph2.size());
	vector < vector < int>> intersectionGraph(size, vector<int>(size, 0));

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			// ������� � ����� ������� ����� 1, ���� ��������������� �������� ����� ������ ����� 1
			intersectionGraph[i][j] = (graph1[i][j] && graph2[i][j]);
		}
	}

	return intersectionGraph;
}

//��������� �����
vector < vector < int>> adaptiveRingSumGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int maxSize = max(graph1.size(), graph2.size());
	vector < vector < int>> ringSumGraph(maxSize, vector<int>(maxSize, 0));

	for (int i = 0; i < maxSize; ++i) {
		for (int j = 0; j < maxSize; ++j) {
			int val1 = (i < graph1.size() && j < graph1.size()) ? graph1[i][j] : 0;
			int val2 = (i < graph2.size() && j < graph2.size()) ? graph2[i][j] : 0;
			ringSumGraph[i][j] = val1 != val2;
		}
	}

	return ringSumGraph;
}


//��������� ������������
vector < vector < int>> cartesianProductGraphs(const vector < vector < int>>& graph1, const vector < vector < int>>& graph2) {
	int n1 = graph1.size() - 1; // ���������, ��� ������� ������ ���������� � 1
	int n2 = graph2.size() - 1;
	int productSize = n1 * n2;
	vector < vector < int>> productGraph(productSize + 1, vector<int>(productSize + 1, 0)); // +1 ��� ����������� �������

	for (int i1 = 1; i1 <= n1; ++i1) {
		for (int i2 = 1; i2 <= n2; ++i2) {
			for (int j1 = 1; j1 <= n1; ++j1) {
				for (int j2 = 1; j2 <= n2; ++j2) {
					int v1 = (i1 - 1) * n2 + i2; // ������������� �������
					int v2 = (j1 - 1) * n2 + j2;
					if (i1 == j1 && graph2[i2][j2]) {
						productGraph[v1][v2] = 1;
					}
					if (i2 == j2 && graph1[i1][j1]) {
						productGraph[v1][v2] = 1;
					}
				}
			}
		}
	}

	return productGraph;
}

void printDistances(const vector<vector<int>>& distances) {
	int n = distances.size();
	int minSum = INT_MAX;
	vector<int> centers(n);
	for (int i = 0; i < n; i++) {
		int Sum = 0;
		for (int j = 0; j < n; j++) {
			cout << "���������� �� ������� " << i + 1 << " �� ������� " << j + 1 << " = ";
			if (distances[i][j] == 0) {
				cout << "0";
			}
			else if (distances[i][j] == numeric_limits<int>::max()) {
				cout << "-1";
			}
			else {
				cout << distances[i][j];
				Sum += distances[i][j];
			}
			cout << endl;
		}
		cout << "����� ���������� �� ������� " << i + 1 << "=" << Sum;
		cout << endl;
		if (Sum <= minSum && Sum != 0) {
			minSum = Sum;
			centers[i] = i + 1;

		}
	}
		cout << "����� �������: ";
		for (int i = 0; i < n; i++) {
			if (centers[i] != 0) {
				cout << centers[i]<<' ';
			}
			else cout << ' ';
		}
		cout << endl;

}

void findAllPairsShortestPath(const vector<vector<int>>& adjacencyMatrix) {
	int n = adjacencyMatrix.size();
	vector< vector<int>> distances(n, vector<int>(n, 0));

	// ������������� ������� ���������� ����������
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			distances[i][j] = adjacencyMatrix[i][j];
			if (i != j && distances[i][j] == 0) {
				distances[i][j] = std::numeric_limits<int>::max();
			}
		}
	}

	// ������� ���������� ����������
	for (int k = 0; k < n; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (distances[i][k] != std::numeric_limits<int>::max()
					&& distances[k][j] != std::numeric_limits<int>::max()
					&& distances[i][k] + distances[k][j] < distances[i][j]) {
					distances[i][j] = distances[i][k] + distances[k][j];
				}
			}
		}
	}

	// ������� ����������
	cout << "������� ���������:" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << adjacencyMatrix[i][j] << " ";
		}
		cout << endl;
	}

	printDistances(distances);
}

int main() {
	setlocale(LC_ALL, "RUS");
	int num_vertices1, num_vertices2, operation;
	int graph_choice, vertex1, vertex2;
	double edge_probability = 0.5;

	vector<vector<int>> matrix1, matrix2, matrixCopy;
	vector<vector<int>> list1, list2, listCopy;

	srand(static_cast<unsigned int>(time(nullptr)));

	while (true) {

		cout << "�������� ��������:\n 1 - �������������� � ����������\n 2 - ����������� �������\n 3 - �������� ��������\n 4 - ����� ������� ����� \n 0 - �����\n";

		operation = _getch() - '0'; // �������� ������ � ������������ ��� � �����

		if (operation == 0) {
			break;
		}

		switch (operation) {

		case 1: {
			cout << "������� ���������� ������ � �����: ";
		isk1:
			cin >> num_vertices1;
			if (num_vertices1 < 0) {
				cout << "������� �� ������������� �����: ";
				goto isk1;
			}
			cout << "����:\n";
			matrix1 = generateAndPrintMatrix(num_vertices1, edge_probability);
			list1 = matrixToList(matrix1);
			printList(list1);

			cout << "������� ������ ���� ������ (��������, 1 3): ";
			cin >> vertex1 >> vertex2;

			if (vertex1 > num_vertices1 || vertex2 > num_vertices1) {
				cout << "������������ ����� �������" << endl;
				break;
			}

			cout << "��������������:\n";
			identifyVertices(matrix1, list1, vertex1, vertex2, matrixCopy, listCopy);
			printMatrix(matrixCopy);
			printList(matrixToList(matrixCopy));

			cout << "����������:\n";
			contractEdge(matrix1, list1, vertex1, vertex2, matrixCopy, listCopy);
			printMatrix(matrixCopy);
			printList(matrixToList(matrixCopy));
			break;
		}

		case 2: {
			cout << "������� ���������� ������ � �����: ";
		isk2:
			cin >> num_vertices1;
			if (num_vertices1 < 0) {
				cout << "������� �� ������������� �����: ";
				goto isk2;
			}

			cout << "����:\n";
			matrix1 = generateAndPrintMatrix(num_vertices1, edge_probability);
			list1 = matrixToList(matrix1);
			printList(list1);

			cout << "������� ����� ������� ��� �����������: ";
			cin >> vertex1;

			if (vertex1 > num_vertices1) {
				cout << "������������ ����� �������" << endl;
				break;
			}

			splitVertex(matrix1, list1, vertex1, matrixCopy, listCopy);
			printMatrix(matrixCopy);
			printList(matrixToList(matrixCopy));
			break;
		}
		case 3: {
			cout << "������� ���������� ������ � ������ �����: ";
		isk3:
			cin >> num_vertices1;
			if (num_vertices1 < 0) {
				cout << "������� �� ������������� �����: ";
				goto isk3;
			}
			cout << "������ ����:\n";
			matrix1 = generateAndPrintMatrix(num_vertices1, edge_probability);
			list1 = matrixToList(matrix1);
			printList(list1);

			cout << "������� ���������� ������ �� ������ �����: ";
		isk4:
			cin >> num_vertices2;
			if (num_vertices2 < 0) {
				cout << "������� �� ������������� �����: ";
				goto isk4;
			}

			cout << "������ ����:\n";
			matrix2 = generateAndPrintMatrix(num_vertices2, edge_probability);
			list2 = matrixToList(matrix2);
			printList(list2);

			vector<vector<int>> mergedGraph = mergeGraphs(matrix1, matrix2);
			cout << "�����������: \n";
			printMatrix(mergedGraph);


			vector<vector<int>> intersectionGraph = intersectGraphs(matrix1, matrix2);
			cout << "�����������: \n";
			printMatrix(intersectionGraph);

			vector<vector<int>> ringSumGraph = adaptiveRingSumGraphs(matrix1, matrix2);
			cout << "��������� �����: \n";
			printMatrix(ringSumGraph);

			vector<vector<int>> cartesianProductGraph = cartesianProductGraphs(matrix1, matrix2);
			cout << "��������� ������������: \n";
			printMatrix(cartesianProductGraph);
			break;
		}
		case 4: {
			int matrixSize;
			do {
				cout << "������� ������ ������� ���������: ";
				cin >> matrixSize;
				if (matrixSize <= 0) {
					cout << "������� ������������� �����." << endl;
				}
			} while (matrixSize <= 0);

			// ���������� ��������� ������������ ������� ���������
			vector<vector<int>> graph(matrixSize, vector<int>(matrixSize, 0));
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dis(0, 1);

			for (int i = 0; i < matrixSize; i++) {
				for (int j = i + 1; j < matrixSize; j++) {
					int value = dis(gen);
					graph[i][j] = value;
					graph[j][i] = value;  // ��������������
				}
			}

			findAllPairsShortestPath(graph);

			  break;
		}
		default:
			cout << "�������� ����� ��������.\n";
			break;
		}
	}
	return 0;
}