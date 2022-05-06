#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

struct node;

// used to relate two nodes with a distance
struct path {
    node *dest;
    int dist;

    path(node *dest, const int &dist): dest(dest), dist(dist){}

    bool operator<(const path &rhs) const {
        return dist < rhs.dist;
    }
};

struct node {
    char name[4] = {};
    vector<path> paths;
    node *child;
    node *parent;
    int distToParent;

    node(char name[]): paths(), child(nullptr), parent(nullptr), distToParent(0) {
        strcpy(this->name, name);
    }

    node(char name[], node *dest, const int &dist): paths(), child(nullptr), parent(nullptr), distToParent(0) {
        strcpy(this->name, name);
        this->paths.emplace_back(dest, dist);
    }
};

// returns node with lowest distance from start
node *getLowestVector(vector<node *> &arr) {
    int lowest = 0;
    node *low = arr[lowest];
    arr.erase(arr.begin() + lowest);
    return low;
}

// checks if node is in vector
bool inVector(vector<node *> &arr, node *&key) {
    if (find(arr.begin(), arr.end(), key) != arr.end()) {
        return true;
    } else {
        return false;
    }
}

// takes in start node and destination node
// preforms dijkstra's search and returns shortest distance
int search(node *&start, node *&end) {
    vector<node *> open;
    vector<node *> closed;
    open.push_back(start);
    start->distToParent = 0;
    start->parent = nullptr;
    while (!open.empty()) {
        node *curr = getLowestVector(open);
        if (curr == end) {
            return curr->distToParent;
        }
        for (int i = 0; i < curr->paths.size(); i++) {
            if (inVector(closed, curr->paths[i].dest)) {
                continue;
            }

            if (inVector(open, curr->paths[i].dest)) {
                if (curr->paths[i].dist + curr->distToParent < curr->paths[i].dest->distToParent) {
                    curr->paths[i].dest->distToParent = curr->paths[i].dist + curr->distToParent;
                    curr->paths[i].dest->parent = curr;
                }
                continue;
            }

            curr->paths[i].dest->distToParent = curr->paths[i].dist + curr->distToParent;
            curr->paths[i].dest->parent = curr;

            int j;
            for (j = 0; j < open.size(); j++) {
                if (curr->paths[i].dest->distToParent < open[j]->distToParent) {
                    break;
                }
            }
            open.insert(open.begin() + j, curr->paths[i].dest);
        }
        closed.push_back(curr);
    }
    return -1;
}

// takes in graph data, start name and destination name and print results
void pathfinding(vector<node*> &graph, char start[4], char dest[4]) {
    node *startNode = nullptr;
    for (int i = 0; i < graph.size(); i++) {
        if (strncmp(graph[i]->name, start, 3) == 0) {
            startNode = graph[i];
        }
    }
    if (startNode == nullptr) {
        cout << "Not Possible" << endl << endl;
        return;
    }
    node *endNode = nullptr;
    for (int i = 0; i < graph.size(); i++) {
        if (strncmp(graph[i]->name, dest, 3) == 0) {
            endNode = graph[i];
        }
    }
    if (endNode == nullptr) {
        cout << "Not Possible" << endl << endl;
        return;
    }

    int dist = search(startNode, endNode);
    cout << startNode->name << " to " << endNode->name << " - ";
    if (dist == -1) {
        cout << "Not Possible" << endl << endl;
    } else {
        cout << "Distance is: " << dist << " - Hops: ";
        endNode->child = nullptr;
        node *i;
        int hops = 0;
        for (i = endNode; i->parent != nullptr; i = i->parent) {
            i->parent->child = i;
            hops++;
        }
        cout << hops << " -  Path: ";
        for (i = startNode; i->child != nullptr; i = i->child) {
            cout << i->name << " -> ";
        }
        cout << i->name;
        cout << endl << endl;
    }
}

int main() {

    // stores map data
    vector<node*> places;

    // load data from file
    ifstream mapFile("canadamap.txt");
    char line[64];

    // read file and store in vector
    while (mapFile.getline(line, 64)) {
        line[3] = '\0';
        line[7] = '\0';
        int containSource = -1;
        int containDest = -1;

        // checks if source or destination exists in vector
        for (int i = 0; i < places.size(); i++) {
            if (strncmp(line, places[i]->name, 3) == 0) {
                containSource = i;
                if (containDest != -1) {
                    break;
                }
            }
            if (strncmp(&line[4], places[i]->name, 3) == 0) {
                containDest = i;
                if (containSource != -1) {
                    break;
                }
            }
        }

        // stores path data and add new nodes as needed
        if (containSource != -1 && containDest != -1) {
            places[containSource]->paths.emplace_back(places[containDest], stoi(&line[8]));
            places[containDest]->paths.emplace_back(places[containSource], stoi(&line[8]));
        } else if (containSource != -1) {
            places.emplace_back(new node(&line[4], places[containSource], stoi(&line[8])));
            places[containSource]->paths.emplace_back(places[places.size() - 1], stoi(&line[8]));
        } else if (containDest != -1) {
            places.emplace_back(new node(line, places[containDest], stoi(&line[8])));
            places[containDest]->paths.emplace_back(places[places.size() - 1], stoi(&line[8]));
        } else {
            places.emplace_back(new node(&line[4]));
            places.emplace_back(new node(line, places[places.size() - 1], stoi(&line[8])));
            places[places.size() - 2]->paths.emplace_back(places[places.size() - 1], stoi(&line[8]));
        }
    }

// uncomment to display data loaded from map file
//
//    cout << "graph size: " << places.size() << endl;
//    for (int i = 0; i < places.size(); i++) {
//        cout << places[i]->name << endl;
//        sort(places[i]->paths.begin(), places[i]->paths.end());
//        for (int j = 0; j < places[i]->paths.size(); j++) {
//            cout << places[i]->paths[j].dest->name << ":" << places[i]->paths[j].dist << "   ";
//        }
//        cout << endl << endl;
//    }

// uncomment to display all possible paths in graph
//
//    int n = 0;
//    for (int i = 0; i < places.size(); i++) {
//        for (int j = i + 1; j < places.size(); j++) {
//            pathfinding(places, places[i]->name, places[j].name);
//            n++;
//        }
//    }
//    cout << n << endl << endl;

// display home to 3 different destinations take by traceroute
    char src[] = "YYC";
    char can1[] = "YYZ";
    char can2[] = "YUL";
    char can3[] = "YVR";

    cout << "Traceroute comparison: \n" << endl;

    // calgary to toronto
    pathfinding(places, src, can1);
    // calgary to montreal
    pathfinding(places, src, can2);
    // calgary to vancouver
    pathfinding(places, src, can3);

    cout << "Menu:\n"
            "1 - Search\n"
            "2 - Quit\n";

    // main loop asks for input
    while(true){
        cout << "Select option: ";
        string input;
        getline(cin, input);
        cout << endl;
        string source;
        string dest;
        switch(input[0]){
            case '1':
                cout << "Enter source: ";
                getline(cin, source);
                cout << endl;

                cout << "Enter destination: ";
                getline(cin, dest);
                cout << endl;

                pathfinding(places, &source[0], &dest[0]);
                break;
            case '2':
                for (int i = 0; i < places.size(); i++) {
                    delete places[i];
                }
                cout << "Program Closed - BYE BYE" << endl;
                return 0;
            default:
                cout << "Invalid Input\n" << endl;
        }
    }
}
