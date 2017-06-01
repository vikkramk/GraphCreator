#include <vector>
#include <queue>
#include <set>
#include <iostream>

#define SIZE 20

using namespace std;

//Node in a path containing a vertex index and parent node
struct pathNode {
	pathNode() {
		parent = NULL;
	}
	
	pathNode(int index) {
		this->index = index;
		parent = NULL;
	}
	
	bool operator<(const pathNode& node) {
		return index < node.index;
	}
	
	int index;
	pathNode* parent;
};

//Print the path from a goal to the start backwards from pathNodes
//Returns the total dist
int printPath(pathNode* node, string verts[], int adjtable[SIZE][SIZE]) {
	if (node->parent != NULL) {
		int pathlen = 0;
		pathlen += printPath(node->parent, verts, adjtable);
		cout << verts[node->index] << " " << flush;
		pathlen += adjtable[node->parent->index][node->index];
		
		return pathlen;
 	}
	else {
		cout << verts[node->index] << " " << flush;
		return 0;
	}
	
}

int getIndex(string s, string verts[]) {
	for (int i = 0; i < SIZE; i++) {
		if (verts[i] == s)
			return i;
	}
	
	return -1;
}

int main() {
	string verts[SIZE];	//Names of vertices in order
	int adjtable[SIZE][SIZE];	//Adjacency table with weights (-1 is no edge)
	int nextvert = 0;	//Index of next spot for a vertice
	
	//Clear the adjacency table
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			adjtable[i][j] = -1;
		}
	}
	
	bool done = false;
	while (!done) {
		string command;
		cout << "Enter command: addvert, rmvert, addedge, rmedge, path, print, or quit: " << endl;
		cin >> command;
		
		//Add a vertex
		if (command == "addvert") {
			//Check if space
			if (nextvert > SIZE) {
				cout << "The graph is full" << endl;
				continue;
			}
			
			string label;
			cout << "Enter the label for the new vertex: " << flush;
			cin >> label;
			
			//Check if already have the label the graph
			bool ingraph = false;
			for (int i = 0; i < SIZE; i++) {
				if (verts[i] == label) {
					ingraph = true;
				}
			}
			if (ingraph) {
				cout << "Node already in graph" << endl;
				continue;
			}
			
			//Put in the label
			verts[nextvert] = label;
			
			//Get the next vertex position
			nextvert = 0;
			while (nextvert < SIZE && verts[nextvert] != "")
				nextvert++;
			
			cout << "Added vertex" << endl;
		}
		
		//Remove a vertex
		else if (command == "rmvert") {
			string label;
			cout << "Enter the vertex label: " << flush;
			cin >> label;
			
			//Find the vertex
			int index = getIndex(label, verts);
			
			//Not found
			if (index ==  -1) {
				cout << "No such vertex" << endl;
				continue;
			}
			
			//Remove the label and references in adjacency table
			verts[index] = "";
			for (int i = 0; i < SIZE; i++) {
				adjtable[index][i] = -1; 	//Clear connetions to others
				adjtable[i][index] = -1;	//Clear connections from others
			}
			
			//Set the spot as available to put another vertex
			nextvert = index;
			
			cout << "Removed vertex" << endl;
		}
		
		//Add an edge
		else if (command == "addedge") {
			string label1, label2;
			int weight;
			cout << "Enter the label for the first vertex: " << flush;
			cin >> label1;
			cout << "Enter the label for the second vertex: " << flush;
			cin >> label2;
			cout << "Enter the weight of the edge: " << flush;
			cin >> weight;
			
			//Find the vertices
			int index1 = getIndex(label1, verts);
			int index2 = getIndex(label2, verts);
			
			//Not found
			if (index1 == -1 || index2 == -1) {
				cout << "No such vertices" << endl;
				continue;
			}
			
			//Check if valid weight
			if (weight < 0) {
				cout << "Weight cannot be negative" << endl;
				continue;
			}
			
			//Add to adjacency table
			adjtable[index1][index2] = weight;
			
			cout << "Added edge" << endl;
		}
		
		//Remove an edge
		else if (command == "rmedge") {
			string label1, label2;
			cout << "Enter the label for the first vertex: " << flush;
			cin >> label1;
			cout << "Enter the label for the second vertex: " << flush;
			cin >> label2;
			
			//Find the vertices
			int index1 = getIndex(label1, verts);
			int index2 = getIndex(label2, verts);
			
			//Not found
			if (index1 == -1 || index2 == -1) {
				cout << "No such vertices" << endl;
				continue;
			}
			
			//No such edge
			if (adjtable[index1][index2] < 0) {
				cout << "No such edge" << endl;
				continue;
			}
			
			//No self edge
			if (index1 == index2) {
				cout << "Edges must be between 2 different nodes" << endl;
				continue;
			}
			
			//Remove adjtable entry
			adjtable[index1][index2] = -1;
			
			cout << "Removed edge" << endl;
		}
		
		//Find the shortest path by breath first search
		else if (command == "path") {
			string label1, label2;
			cout << "Enter the label for the start vertex: " << flush;
			cin >> label1;
			cout << "Enter the label for the end vertex: " << flush;
			cin >> label2;
			
			//Find the vertices
			int index1 = getIndex(label1, verts);
			int index2 = getIndex(label2, verts);
			
			//Not found
			if (index1 == -1 || index2 == -1) {
				cout << "No such vertices" << endl;
				continue;
			}
			
			//Find the minimum distance path
			pathNode* goal = NULL;
			set<pathNode*> nodes;
			queue<pathNode*> connections;
			
			connections.push(new pathNode(index1));
			
			//Keep on looking through connections while haven't found end
			while (!connections.empty()) {
				pathNode* current = connections.front();
				connections.pop();
				
				//Found goal
				if (current->index == index2) {
					goal = current;
					break;
				}
				
				//Else add connections
				for (int i = 0; i < SIZE; i++) {
					if (adjtable[current->index][i] > -1) {
						bool newconnection = true;
						for (set<pathNode*>::iterator it = nodes.begin(); it != nodes.end(); it++) {
							if ((*it)->index == i)
								newconnection = false;
						}
						if (newconnection) {
							pathNode* connection = new pathNode(i);
							nodes.insert(connection);
							connection->parent = current;
							connections.push(connection);
						}
					}
				}
			}
			
			//Not found
			if (goal == NULL) {
				cout << "No possible path" << endl;
				continue;
			}
			
			cout << "Path: " << flush;
			int pathlen = printPath(goal, verts, adjtable);
			cout << pathlen << endl;
		}
		
		else if (command == "print") {
			
			//Print vertex labels
			cout << " " << flush;
			for (int i = 0; i < SIZE; i++) {
				if (verts[i] != "")
					cout << " " << verts[i] << flush;
			}
			
			cout << endl;
			
			for (int vert = 0; vert < SIZE; vert++) {
				if (verts[vert] != "") {
					cout << verts[vert] << flush;
					for (int dest = 0; dest < SIZE; dest++) {
						if (verts[dest] != "") {
							if (adjtable[vert][dest] == -1)
								cout << " -" << flush;
							else
								cout << " " << adjtable[vert][dest] << flush;
						}
					}
					cout << endl;
				}
			}
		}
	
		//No such command
		else if (command == "quit")  {
			done = true;
		}
		
		//No such command
		else {
			cout << "Invalid Command" << endl;
		}
	}
}
