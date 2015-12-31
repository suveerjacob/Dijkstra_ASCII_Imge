/*
	Author : Suveer Jacob
	Description : The following program is an implementation of the Dijkstra's Algorithm.
				  It takes input from a graph.dat file and is of the following format: 

				  node# 1st-arc-to-node# weight-for-previous-arc 2nd-arc-to-node# weight-for-previous-arc 3rd-arc-to-node# weight-for-previous-arc ...

				  As an example:
				  1 2 10 3 5 4 100
				  2 1 5
				  4 3 50 2 10

				  In the above example, node 1 has an arc with weight 10 to node 2, weight 5 to node 3, and weight 100 to node 4. 
				  Node 2 has an arc with weight 5 to node 1. 
				  Node 4 has an arc to node 3 with weight 50 and to node 2 with weight 10.
				  Note that in the above example, there is a node 3 despite not being specifically declared on a line. 
				  Any node that is either explicitly defined on its own line, or into which an arc goes, must be considered to exist.

				  It displays the shortest path from each node of the graph to every other node.  

				  It also displays an ASCII art image of the graph provided as the input.
				  For convenience all nodes of the graph are diaplayed vertically and are connected to each other as given in the input file.


	**** Note : If the graph is not displayed properly on Console, then expand the CMD promt horizontally.
				### ASCII art image can also be viewed in "graph_output.dat" file. ###

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include <iomanip>
#include <regex>

using namespace std;

// struct for finding shortest path
struct node {
	int vertex;
	double weight;
	node(int _vertex, double _weight): vertex(_vertex), weight(_weight) { }
	node() {}
};

// Structure for drawing an ASCII art Image
struct vertex {
	int node;
	vertex(int _node) : node(_node) {
	}
};

// Data Structures for holding the shortest path results
map<int, list<node> > adj_list;
map<int, double> min_distance;
map<int, int> previous;

// Variables and Data Structures for displaying ASCII art image
vector<vector<string>> displayMatrix; 
vector<vertex> vertices;

// initial width ofthe display matrix. Width is dynamically increased as the number of edges increases.
int width = 3; 

// following variable is the vertical distance between each nodes
const int nodeGap = 9;
int leftNodes = 0;
int rightNodes = 0;


/// Comparator for comparing values of set
template <typename T1, typename T2>
struct pair_first_less
{
	bool operator()(pair<T1, T2> p1, pair<T1, T2> p2)
	{
		return p1.first < p2.first;
	}
};

// Function that calculates the shortest path and minimum cost between source and destination node
// Logic Referred from following links: 
// http://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B
// http://www.cplusplus.com/forum/general/124853/

void Dijkstra(int source)
{
	for (map<int, list<node> >::iterator vertex_iter = adj_list.begin(); vertex_iter != adj_list.end(); vertex_iter++)
	{
		int v = vertex_iter->first;
		min_distance[v] = numeric_limits< double >::infinity();
	}
	min_distance[source] = 0;
	set< pair<double, int>, pair_first_less<double, int> > vertex_queue;
	for (map<int, list<node> >::iterator vertex_iter = adj_list.begin(); vertex_iter != adj_list.end(); vertex_iter++)
	{
		int v = vertex_iter->first;
		vertex_queue.insert(pair<double, int>(min_distance[v], v));
	}

	while (!vertex_queue.empty()) {
		int u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());
		for (list<node>::iterator node_iter = adj_list[u].begin(); node_iter != adj_list[u].end(); node_iter++)
		{
			int v = node_iter->vertex;
			double weight = node_iter->weight;
			double distance_through_u = min_distance[u] + weight;
			if (distance_through_u < min_distance[v]) {
				vertex_queue.erase(pair<double, int>(min_distance[v], v));
				min_distance[v] = distance_through_u;
				previous[v] = u;
				vertex_queue.insert(pair<double, int>(min_distance[v], v));
			}
		}
	}
}

// Method returns the shortest path between nodes
list<int> shortestPathTo(int _vertex)
{
	list<int> path;
	map<int, int>::iterator prev;
	int vertex = _vertex;
	path.push_front(vertex);
	while ((prev = previous.find(vertex)) != previous.end())
	{
		vertex = prev->second;
		path.push_front(vertex);
	}
	return path;
}

// convert int to string
string itos(int i) 
{
	stringstream s;
	s << i;
	return s.str();
}

/// Functions to display shortest path result on Console
void printResult() {
	string pathString = "";
	stringstream ss;
	map<int, list<node> >::iterator vertex_iter = adj_list.begin();
	for (int j = 0 ; vertex_iter != adj_list.end(); j++, vertex_iter++)
	{
		
		int v = vertex_iter->first;
		if (j == 0) {
			cout << left << setw(12) << v;
		}
		else {
			cout << left << setw(12) << " "<< left << setw(12) <<v;
		}
		list<int> path = shortestPathTo(v);
		list<int>::iterator path_iter = path.begin();
		for (int i = 0; path_iter != path.end(); i++, path_iter++)
		{
			ss << *path_iter;
			if (i == path.size() - 1) {
				pathString += itos(*path_iter);
			}
			else {
				pathString += (itos(*path_iter) + "->");
			}
		}
		if (min_distance[v] == INFINITY) {
			cout << left << setw(25) << "inf" << setw(12) << min_distance[v];
		}
		else {
			cout << left << setw(25) << pathString << setw(12) << min_distance[v];
		}
		pathString = "";
		cout << endl;
	}
}


// Function to show the adjacency list.
void showData() {
	int index;
	for (map<int, list<node> >::iterator it = adj_list.begin(); it != adj_list.end(); ++it) {
		index = it->first;
		list<node> nodelist = adj_list[index];
		for (list<node>::iterator list_iter = nodelist.begin(); list_iter != nodelist.end(); ++list_iter) {
			cout << it->first << " " << list_iter->vertex << " " << list_iter->weight << " -- ";
		}
		cout << endl;
	}
}

// Reading graph from "graph.dat" file //
void readData() {
	string line;
	ifstream readFile("graph.dat");
	int startNode;
	int otherNode;
	int weight;
	while (getline(readFile, line))
	{
		istringstream iss(line);
		iss >> startNode;
		if (adj_list.find(startNode) == adj_list.end()) { // node does not exists
			adj_list[startNode].push_back(node(startNode, 0)); // self loop with weight 0
		}

		while ((iss >> otherNode >> weight)) {
			if (adj_list.find(otherNode) == adj_list.end()) { // node does not exists
				adj_list[otherNode].push_back(node(otherNode, 0)); // self loop with weight 0
			}
			adj_list[startNode].push_back(node(otherNode, weight));
		}
	}
	readFile.close();
}

// Following function expands the displayMatrix horizontally dynamically as the number of nodes and edges increases.
void expandDisplayMatrix() {
	for (int i = 0; i < displayMatrix.size(); i++) {
		for (int j = 0; j < 2; j++) {
			displayMatrix[i].insert(displayMatrix[i].begin(), "");
			displayMatrix[i].insert(displayMatrix[i].end(), "");
		}
	}
	width = displayMatrix[0].size();
	
}

/// Create edges between Nodes ///
void createPathBetweenNodes(int source, int destination, int weight) {
	expandDisplayMatrix();
	bool topBottom = false;
	bool bottomTop = false;
	int currentX = 0;
	int currentY = 0;
	int tempCounter = 0; // Used to display Weight at 3rd position on edge
	if (source < destination) {
		topBottom = true;
	}
	else {
		bottomTop = true;
	}

	int i;
	if (topBottom) {
		rightNodes++;
		// Going Left to right
		currentX = (width / 2);
		currentY = (nodeGap / 2 + nodeGap * source);
		for (i = 0; i < rightNodes * 2; i++) {
			if (!(displayMatrix[currentY][(width / 2) + (i + 1)].compare("+") == 0)) {
				if (displayMatrix[currentY][(width / 2) + (i + 1)].compare("|") == 0) {
					displayMatrix[currentY][(width / 2) + (i + 1)] = "=";
				}
				else {
					displayMatrix[currentY][(width / 2) + (i + 1)] = "-";
				}
			}
			currentX = (width / 2) + (i + 1);
		}
		displayMatrix[currentY][currentX] = "+";
		// Going Down
		for (i = (nodeGap / 2 + nodeGap * source); i < (nodeGap / 2 + nodeGap * destination) - 3; i++) {
			tempCounter++;
			if (displayMatrix[currentY + 1][currentX].compare("-") == 0) {
				displayMatrix[currentY + 1][currentX] = "=";
			}
			else if (!(displayMatrix[currentY + 1][currentX].compare("+") == 0)) {
				if (tempCounter == 3) {
					displayMatrix[currentY + 1][currentX] = itos(weight);
				}
				else {
					displayMatrix[currentY + 1][currentX] = "|";
				}

			}
			currentY++;
		}
		tempCounter = 0;
		// Going left 
		displayMatrix[currentY][currentX] = "+";
		currentX--;
		for (i = currentX; i != (width / 2); i--) {
			if (displayMatrix[currentY][i].compare("|") == 0) {
				displayMatrix[currentY][i] = "=";
			}
			else if (!(displayMatrix[currentY][i].compare("+") == 0)) {
				displayMatrix[currentY][i] = "-";
			}
			currentX--;
		}
		// Going Down
		displayMatrix[currentY][currentX] = "+";
		currentY++;
		for (i = currentY; i != (nodeGap / 2 + nodeGap * destination); i++) {
			displayMatrix[i][currentX] = "|";
			currentY++;
		}
		displayMatrix[currentY - 1][currentX] = "V";
	}
	else if(bottomTop){
		leftNodes++;
		currentX = (width / 2);
		currentY = (nodeGap / 2 + nodeGap * source);
		// Going from Right to Left
		for (i = 0; i < leftNodes * 2; i++) {
			if (!(displayMatrix[currentY][(width / 2) - (i + 1)].compare("+") == 0)) {
				if (displayMatrix[currentY][(width / 2) - (i + 1)].compare("|") == 0) {
					displayMatrix[currentY][(width / 2) - (i + 1)] = "-";
				}
				else {
					displayMatrix[currentY][(width / 2) - (i + 1)] = "-";
				}
			}
			currentX = (width / 2) - (i + 1);
		}
		displayMatrix[currentY][currentX] = "+";
		// Going up
		for (i = currentY; i > (nodeGap / 2 + nodeGap * destination) + 3; i--) {
			tempCounter++;
			if (displayMatrix[currentY - 1][currentX].compare("-") == 0) {
				displayMatrix[currentY - 1][currentX] = "=";
			}
			else if (!(displayMatrix[currentY - 1][currentX].compare("+") == 0)) {
				if (tempCounter == 3) {
					displayMatrix[currentY - 1][currentX] = itos(weight);
				}
				else {
					displayMatrix[currentY - 1][currentX] = "|";
				}
			}
			currentY--;
		}
		tempCounter = 0;
		// Going right 
		displayMatrix[currentY][currentX] = "+";
		currentX++;
		for (i = currentX; i != (width / 2); i++) {
			if (displayMatrix[currentY][i].compare("|") == 0) {
				displayMatrix[currentY][i] = "=";
			}
			else if (!(displayMatrix[currentY][i].compare("+") == 0)) {
				displayMatrix[currentY][i] = "-";
			}
			currentX++;
		}
		// Going Up
		displayMatrix[currentY][currentX] = "+";
		currentY--;
		for (i = currentY; i != (nodeGap / 2 + nodeGap * destination); i--) {
			displayMatrix[i][currentX] = "|";
			currentY--;
		}
		displayMatrix[currentY + 1][currentX] = "^";
	}
}


map<int, int> vertexMap;
/// The Following function is responsible for displaying image on console as well as writing image on a file ///
void createImage() {
	//cout << "In create image function" << endl;

	/// Storing all nodes in vertex ///
	int vertexNumber;
	int counter = 0;
	for (map<int, list<node> >::iterator it = adj_list.begin(); it != adj_list.end(); ++it) {
		vertexNumber = it->first;
		vertices.push_back(vertex(vertexNumber));
		vertexMap[vertexNumber] = counter;
		counter++;
	}
	/// Storing all nodes in vertex ///


	/// Resizing and initializing vector with default values ///
	int height = vertices.size() * nodeGap;
	displayMatrix.resize(height);
	for (int i = 0; i < height; i++) {
		displayMatrix[i].swap(vector<string>(width, ""));
	}
	/// Resizing and initializing vector with default values ///

	/// Inserting nodes in Display Matrix ///
	for (int i = 0; i < vertices.size(); i++) {
		displayMatrix[nodeGap / 2 + nodeGap * i][width / 2] = "[" + itos(vertices[i].node) + "]";
	}
	/// Inserting nodes in Display Matrix ///

	/// Creating Paths ///
	int source = -1; 
	int destination = -1;
	int weight = -1;
	for (map<int, list<node> >::iterator it_2 = adj_list.begin(); it_2 != adj_list.end(); ++it_2) {
		source = vertexMap[it_2->first];

		list<node> nodelist = adj_list[it_2->first];
		for (list<node>::iterator list_iter = nodelist.begin(); list_iter != nodelist.end(); ++list_iter) {
			destination = vertexMap[list_iter->vertex];
			weight = list_iter->weight;
			//cout << source << " -- " << destination << " -- " << weight;
			if (source != destination) {
				//cout << "About to create path" << endl;
				createPathBetweenNodes(source, destination, weight);
			}
		}
	}
	/// Creating Paths ///
	
	/// Displaying the output and also wirtes the output to a file ///
	ofstream outfile("graph_output.dat");
	if (outfile.fail()) {
		cout << "Unable to write in file " << endl;
	}
	else
	{
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < width; k++) {

				if (displayMatrix[j][k].compare("-") == 0) {
					cout << right << setw(5) << "-----";
					outfile << right << setw(5) << "-----";
				}
				else if (displayMatrix[j][k].compare("+") == 0 && displayMatrix[j][k - 1].compare("") != 0) {
					cout << right << setw(5) << "----+";
					outfile << right << setw(5) << "----+";
				}
				else if (displayMatrix[j][k].compare("=") == 0) {
					cout << right << setw(5) << "----=";
					outfile << right << setw(5) << "----=";
				}
				else {
					cout << right << setw(5) << displayMatrix[j][k];
					outfile << right << setw(5) << displayMatrix[j][k];
				}
			}
			cout << endl;
			outfile << endl;
		}
	}

	outfile.close();
	/// Displaying the output ///
}


int main()
{
	readData();
	// Uncomment the function below to display the adjacency List
	//showData();
	
	cout << left <<setw(12) << "START NODE" << setw(12) << "END NODE" << setw(25) << "MIN PATH" << setw(12) << "MIN DISTANCE" << endl;
	cout << left <<setw(12) << "==========" << setw(12) << "========" << setw(25) << "========" << setw(12) << "============" << endl;

	//// Printing All Results ///
	for (map<int, list<node> >::iterator it = adj_list.begin(); it != adj_list.end(); ++it) {
		cout << setw(12) << it->first;
		Dijkstra(it->first);
		printResult();
		min_distance.clear();
		previous.clear();
		cout << "\n";
	}
	//// Printing All Results ///

	cout << "\n*********  GRAPH IMAGE DRAWN BELOW  *********";
	cout << "\n*********  TO SEE BETTER IMAGE, EXPAND OUTPUT WINDOW HORIZONTALLY   *********";
	cout << "\n*********  IF IMAGE IS NOT DISPLAYED PROPERLY, REFER THE \"GRAPH_OUTPUT.DAT\" FILE.  *********"<<endl;
	
	/// Function call to create and print Graph ///
	createImage();
	cin.get();
	return 0;
}