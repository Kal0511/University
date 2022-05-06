#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

// used to store the results aggregated from polling tests
struct result {
    int idle;
    int success;
    int collision;
    int total;

    result() {
        idle = 0;
        success = 0;
        collision = 0;
        total = 0;
    }

    void display(const string& desc) const {
        printf("| %-6d| %-9d| %-11d| %-7d| %-8.2f| %-11.2f| %-13.2f| %s\n", idle, success, collision, total, 100.f*idle/total, 100.f*success/total, 100.f*collision/total, desc.c_str());
    }
};

class tree;

// node of binary tree
class node {
public:
    friend tree;

    node(const int &k, int l, int h) : leftChild(nullptr), rightChild(nullptr), key((h + l) / 2), numOfChild(0) {}

private:
    node *leftChild;
    node *rightChild;
    int key;
    int numOfChild;
};

// simple binary tree used for analysis
class tree {
public:
    tree(const int &kValue) : k(kValue), m((int) pow(2, k)), root(new node(k, 0, m - 1)), leaves(), loadEstimate(1) {
        buildTree(root, 0, 0, m - 1);
        leaves.reserve(m);
    }

    ~tree() {
        deleteTree(root);
    }

    void insert(int index) {
        insertRecursive(root, index);
    }

    void reset() {
        estimateLoad();
        resetTree(root);
    }

    void resizeTree(int kValue) {
        deleteTree(root);
        k = kValue;
        m = (int) pow(2, kValue);
        root = new node(k, 0, m - 1);
        leaves.clear();
        leaves.reserve(m);
        buildTree(root, 0, 0, m - 1);
        if (loadEstimate <= 0) {
            loadEstimate = m / 4;
        } else if (loadEstimate > m) {
            loadEstimate = 3 * m / 4;
        }
    }

    // start polling at root level
    void pollingRoot() {
        result res;
        pollingRecursive(root, res);
        res.display("Start polling at the root");
    }

    // improved method which starts polling at root level
    void improvedPolling() {
        result res;
        improvedPollingRecursive(root, res);
        res.display("Start polling at the root with improved algorithm");
    }

    // improved method which starts polling at specified level
    int improvedPollingAtLevel(node *curr, result &res) {
        vector<node *> nodes = getLevel(curr, 1);
        int ans = 0;
        for (int i = 0; i < nodes.size(); i++) {
            ans += improvedPollingRecursive(nodes[i], res);
        }
        return ans;
    }

    // start polling at best estimated load level
    void smartPolling() {
        result res;
        vector<node *> nodes = getLevel(root, optimalStartLevel());
        for (int i = 0; i < nodes.size(); i++) {
            pollingRecursive(nodes[i], res);
        }
        res.display("Start polling at the estimated load level");
    }

    // improved method which starts polling at best estimated load level
    void improvedSmartPolling() {
        result res;
        vector<node *> nodes = getLevel(root, optimalStartLevel());
        for (int i = 0; i < nodes.size(); i++) {
            improvedPollingRecursive(nodes[i], res);
        }
        res.display("Start polling at the estimated load level with improved algorithm");
    }

    // start polling at leaf level
    void pollingLeaf() {
        result res;
        res.total = (int) leaves.size();
        for (int i = 0; i < leaves.size(); i++) {
            switch (leaves[i]->numOfChild) {
                case (0):
                    res.idle++;
                    break;
                case (1):
                    res.success++;
                    break;
                default:
                    res.collision++;
            }
        }
        res.display("Start polling at the leaves");
    }

    int size() const {
        return root->numOfChild;
    }

private:
    // helper methods for polling tests

    int improvedPollingRecursive(node *curr, result &res) {
        res.total++;
        switch (curr->numOfChild) {
            case (0):
                res.idle++;
                return 0;
            case (1):
                res.success++;
                return 1;
            default:
                res.collision++;
                int ans = improvedPollingRecursive(curr->leftChild, res);
                if (ans == 0) {
                    ans += improvedPollingAtLevel(curr->rightChild, res);
                } else {
                    ans += improvedPollingRecursive(curr->rightChild, res);
                }
                return ans;
        }
    }

    vector<node *> getLevel(node *start, int level) {
        vector<node *> arr;
        getLevelRecursive(arr, start, level);
        return arr;
    }

    void getLevelRecursive(vector<node *> &arr, node *curr, int level) {
        if (level == 0) {
            arr.push_back(curr);
            return;
        }
        getLevelRecursive(arr, curr->leftChild, level - 1);
        getLevelRecursive(arr, curr->rightChild, level - 1);
    }

    void pollingRecursive(node *curr, result &res) {
        res.total++;
        switch (curr->numOfChild) {
            case (0):
                res.idle++;
                return;
            case (1):
                res.success++;
                return;
            default:
                res.collision++;
                pollingRecursive(curr->leftChild, res);
                pollingRecursive(curr->rightChild, res);
                return;
        }
    }


    void resetTree(node *curr) {
        if (curr == nullptr || curr->numOfChild == 0) {
            return;
        }
        curr->numOfChild = 0;
        resetTree(curr->leftChild);
        resetTree(curr->rightChild);
    }

    void insertRecursive(node *curr, int index) {
        if (curr == nullptr) {
            return;
        }
        if (index <= curr->key) {
            insertRecursive(curr->leftChild, index);
        } else {
            insertRecursive(curr->rightChild, index);
        }
        curr->numOfChild++;
    }

    void buildTree(node *curr, int level, int low, int high) {
        if (level >= k) {
            leaves.push_back(curr);
            return;
        }
        int mid = (high + low) / 2;
        curr->leftChild = new node(m, low, mid);
        buildTree(curr->leftChild, level + 1, low, mid);
        curr->rightChild = new node(m, mid + 1, high);
        buildTree(curr->rightChild, level + 1, mid + 1, high);
    }

    void deleteTree(node *curr) {
        if (curr == nullptr) {
            return;
        }
        deleteTree(curr->leftChild);
        deleteTree(curr->rightChild);
        delete curr;
    }

    int optimalStartLevel() const {
        return (int) round(log2(loadEstimate));
    }

    void estimateLoad() {
        loadEstimate = size() * (size() - loadEstimate + m) / m;
        if (loadEstimate <= 0) {
            loadEstimate = m / 4;
        } else if (loadEstimate > m) {
            loadEstimate = 3 * m / 4;
        }
    }

    int k;  // num of levels
    int m;  // num of leaves
    node *root;
    vector<node *> leaves;
    int loadEstimate;
};


int main() {

    // results of test files
    string dir = "data/test/";
    tree mac(3);

    ifstream list(dir + "list.txt");
    if (list.is_open()) {
        string line;
        while (getline(list, line)) {
            if (line[line.length() - 1] == '\r') {
                line[line.length() - 1] = '\0';
            }
            cout << line.c_str() << endl;
            ifstream cart(dir + line);
            if (cart.is_open()) {
                string rfid;
                while (getline(cart, rfid)) {
                    mac.insert(stoi(rfid));
                }
                printf("|Idle   |Success   |Collision   |Total   |Idle %%   |Success %%   |Collision %%   | Description\n");
                mac.pollingRoot();
                mac.pollingLeaf();
                mac.improvedPolling();
                mac.smartPolling();
                mac.improvedSmartPolling();
                mac.reset();
                cout << endl;
                cart.close();
            }
        }
        list.close();
    }

    // results of customer files
    dir = "data/customer/";
    mac.resizeTree(10);
    list.open(dir + "list.txt");
    if (list.is_open()) {
        string line;
        while (getline(list, line)) {
            if (line[line.length() - 1] == '\r') {
                line[line.length() - 1] = '\0';
            }
            cout << line.c_str() << endl;
            ifstream cart(dir + line);
            if (cart.is_open()) {
                string rfid;
                while (getline(cart, rfid)) {
                    mac.insert(stoi(rfid));
                }
                printf("|Idle   |Success   |Collision   |Total   |Idle %%   |Success %%   |Collision %%   | Description\n");
                mac.pollingRoot();
                mac.pollingLeaf();
                mac.improvedPolling();
                mac.smartPolling();
                mac.improvedSmartPolling();
                mac.reset();
                cout << endl;
                cart.close();
            }
        }
        list.close();
    }

    cout << "Current K values is 10" << endl;

    cout << "Menu: \n"
            "1 - Scan File\n"
            "2 - Change K value\n"
            "3 - Quit\n";

    // enters main loop
    while (true) {
        cout << "Select option: ";
        string input;
        getline(cin, input);
        cout << endl;
        ifstream cart;
        switch (input[0]) {
            case '1':
                cout << "Enter relative file path: ";
                getline(cin, input);
                cout << endl;
                cart.open("data/" + input);
                if (cart.is_open()) {
                    while (getline(cart, input)) {
                        mac.insert(stoi(input));
                    }
                    printf("|Idle   |Success   |Collision   |Total   |Idle %%   |Success %%   |Collision %%   | Description\n");
                    mac.pollingRoot();
                    mac.pollingLeaf();
                    mac.improvedPolling();
                    mac.smartPolling();
                    mac.improvedSmartPolling();
                    mac.reset();
                    cout << endl;
                    cart.close();
                }else{
                    cout << "Cant open file" << endl << endl;
                }
                break;
            case '2':
                cout << "New value: ";
                getline(cin, input);
                cout << endl;
                int k;
                try {
                    k = stoi(input);
                } catch (...) {
                    cout << "Enter an integer, try again: ";
                    continue;
                }
                mac.resizeTree(k);
                break;
            case '3':
                cout << "Program Closed - BYE BYE" << endl;
                return 0;
            default:
                cout << "Invalid Input\n" << endl;
        }
    }
}
