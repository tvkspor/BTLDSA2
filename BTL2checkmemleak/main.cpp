#include "main.h"

/**********Place to build tree**********/
struct Nodeh
{
    char ch;
    int freq;
    Nodeh* left, * right;
    int index;
};
Nodeh* getNode(char ch, int freq, Nodeh* left, Nodeh* right,int n)
{
    Nodeh* node = new Nodeh();
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    node->index = n;
    return node;
}
struct comp
{
    bool operator()(Nodeh* l, Nodeh* r)
    {
        if (l->freq != r->freq) return l->freq > r->freq;
        else if (l->ch >= 97 && l->ch <= 122 && r->ch >= 97 && r->ch <= 122) return l->ch > r->ch;
        else if (l->ch >= 65 && l->ch <= 90 && r->ch >= 65 && r->ch <= 90) return l->ch > r->ch;
        else if (l->ch == 0 || r->ch == 0) return l->index > r->index;
        else return l->ch < r->ch;
    }
};
void encode(Nodeh* root, string str, unordered_map<char, string>& huffmanCode)
{
    if (root == nullptr)
        return; 
    encode(root->left, str + "0", huffmanCode);

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    encode(root->right, str + "1", huffmanCode);
}

int getHeightRec(Nodeh* node) {
    if (!node) return 0;
    else return(1 + max(getHeightRec(node->left), getHeightRec(node->right)) );
}
Nodeh* rotateRight(Nodeh* root) {
    Nodeh* temp = root->left;
    root->left = temp->right;
    temp->right = root;
    return temp;
}
Nodeh* rotateLeft(Nodeh* root) {
    Nodeh* temp = root->right;
    root->right = temp->left;
    temp->left = root;
    return temp;
}
int getbalance(Nodeh* node) {
    if (!node) return 0;
    return (getHeightRec(node->left) - getHeightRec(node->right));
}
Nodeh* rotate(Nodeh* temp, int&a) {
    if (!temp) return temp;
    if (a == 0) return temp;
    if (getbalance(temp) > 1) {
        if (getbalance(temp->left) >= 0) temp = rotateRight(temp);
        else {
            temp->left = rotateLeft(temp->left);
            temp = rotateRight(temp);
        }
        a--;
    }
    else if (getbalance(temp) < -1) {
        if (getbalance(temp->right) <= 0) { temp = rotateLeft(temp); }
        else {
            temp->right = rotateRight(temp->right);
            temp = rotateLeft(temp);
        }
        a--;
    }
    if (temp->left == NULL && temp->right == NULL) return temp;
    Nodeh* newnode = temp;
    newnode->left=rotate(newnode->left, a);
    newnode->right=rotate(newnode->right, a);
    if (abs(getbalance(temp))>1) temp=rotate(temp,a);
    return temp;

}

void preorder(Nodeh* root) {
    if (root == nullptr)
        return;
    if (!root->left && !root->right) {
        cout << root->ch << " " << root->freq << endl;

    }
    preorder(root->left);
    preorder(root->right);
}

// Builds Huffman Tree and decode given input text
void buildHuffmanTree(string text, Nodeh*& root, int& result)
{
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }
    if (freq.size() == 1) {
        root = getNode(text[0], freq[text[0]], nullptr, nullptr, 0); result = 0; return;
    }
    priority_queue<Nodeh*, vector<Nodeh*>, comp> pq;
    int order = 0;
    for (auto pair : freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr,order));
        order++;
    }
    while (pq.size() != 1)
    {
        Nodeh* left = pq.top(); pq.pop();
        Nodeh* right = pq.top(); pq.pop();
        int sum = left->freq + right->freq;
        Nodeh* temp = getNode('\0', sum, left, right, order);

        int rotacount = 3;
        temp = rotate(temp, rotacount);

        temp->index = order;
        pq.push(temp);
        order++;
    }

    // root stores pointer to root of Huffman Tree
    root = pq.top();

    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    string str = "";
    for (char ch : text) {
        str += huffmanCode[ch];
    }
    string b = "";
    int p = min(int(str.size()), 10);

    for (int i = 0; i < p; i++) {
        b += str[str.size() - 1 - i];
    }
    if (b == "") result = 0;
    else result = stoi(b, nullptr, 2);
}
/**********Place to build tree**********/


class customer {
public:
    string name;
    Nodeh* root;
    int result;
public:
    customer(string a) : name(a) { buildHuffmanTree(name, root, result); }
    ~customer() { clrRecursive(root); };
    void clrRecursive(Nodeh* node) {
        if (node == nullptr) {
            return;
        }
        clrRecursive(node->left);
        clrRecursive(node->right);
        delete node;
    }
    void hand(Nodeh* root)
    {
        if (root == NULL) return;
        hand(root->left);
        if (!root->left && !root->right) cout << root->ch << endl;
        else cout << root->freq<<endl;
        hand(root->right);
    }
};
long long  combination(int k, int n)
{
    if (k > n) return 0;
    if (k * 2 > n) k = n - k;
    if (k == 0) return 1;

    long long result = n;
    for (unsigned int i = 2; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}
long long int countWays(vector<long long int>& arr)
{
    int N = int(arr.size());
    if (N <= 2) {
        return 1;
    }
    vector<long long int> leftSubTree;
    vector<long long int> rightSubTree;
    long long int root = arr[0];

    for (int i = 1; i < N; i++) {
        if (arr[i] < root) {
            leftSubTree.push_back(
                arr[i]);
        }
        else {
            rightSubTree.push_back(
                arr[i]);
        }
    }
    long long int N1 = int(leftSubTree.size());
    long long int countLeft = countWays(leftSubTree);
    long long int countRight = countWays(rightSubTree);
    return combination(N1, N-1) * countLeft * countRight;
}

/**********Place to build G_restaurant**********/
class BinarySearchTree {
public:
    class Node {
    public:
        int value;
        Node* left;
        Node* right;

        Node(int value) : value(value), left(nullptr), right(nullptr) {}
    };
public:
    Node* root;
    Node* minvalue(Node* node) {
        Node* current = node;
        if (!current) return NULL;
        while (current->left) current = current->left;
        return current;
    }
    void clearRecursive(Node* node) {
        if (node == nullptr) {
            return;
        }

        clearRecursive(node->left);
        clearRecursive(node->right);

        delete node;
    }
    Node* insertnode(Node* node, int value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (value < node->value) {
            node->left = insertnode(node->left, value);
        }
        else if (value >= node->value) {
            node->right = insertnode(node->right, value);
        }
        else {
            node->value = value;
        }

        return node;
    }
    Node* deletenode(Node* root, int k) {
        if (root == NULL) return root;

        if (root->value > k) {
            root->left = deletenode(root->left, k);
        }
        else if (root->value < k) {
            root->right = deletenode(root->right, k);
        }
        else
        {
            if (root->left == NULL || root->right == NULL) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == NULL) {
                    temp = root;
                    root = NULL;
                }
                else *root = *temp;
                delete temp;
            }
            else {
                Node* temp = minvalue(root->right);
                root->value = temp->value;
                root->right = deletenode(root->right, temp->value);
            }
        }
        return root;
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() {}
    void insert(int value) {
        root = insertnode(root, value);
    }
    void clear() {
        clearRecursive(root);
        root = nullptr;
    }
    void delet(int value) {
        root = deletenode(root, value);
    }
    void printinoder(Node* node) {
        if (!node) return;
        printinoder(node->left);
        cout << node->value << endl;
        printinoder(node->right);
    }
    void preodertoKosuken(Node* node, vector<long long int>& a)
    {
        if (node) {
            a.push_back(long long int(node->value));
            preodertoKosuken(node->left, a);
            preodertoKosuken(node->right, a);
        }
    }
};
class HashTableWithBST {
public:
    int M;
    vector<BinarySearchTree> table;

public:
    HashTableWithBST(int size) : M(size), table(size + 1) {}
    int hashFunction(int value) {
        return value % M + 1;
    }
    void insert(int value) {
        int index = hashFunction(value);
        table[index].insert(value);
    }
    ~HashTableWithBST() { for (int i = 0; i < table.size(); i++) table[i].clear(); table.clear(); }
};
class G_restaurant {
public:
    vector <vector<customer*>> a;
public:
    G_restaurant(int size) : a(size + 1) {}
    ~G_restaurant() 
    {
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < a[i].size(); j++) {
                delete a[i][j];
            }
            a[i].clear();
        }
        a.clear();
    }
};
/**********Place to build G_restaurant**********/


/**********Place to build S_restaurant**********/

class Node {
public:
    int key;
    int value;
    int recentuse;
    Node(int ID) : key(ID), value(1), recentuse(0) {}
    void incrementValue() {
        value++;
    }
};
void preodertraversal(vector<Node> heap, vector<int>& a, int index) {
    if (index < heap.size()) {
        a.push_back(heap[index].key);
        preodertraversal(heap, a, 2 * index + 1);
        preodertraversal(heap, a, 2 * index + 2);
    }
}

class Minheap {
public:
    vector<Node> heap;
    vector<int> a;
    map<int, int> keyToIndexMap;

    void heapifyUp(int index) {
        while (index > 0) {
            int parentIndex = (index - 1) / 2;
            if (heap[index].value < heap[parentIndex].value) {
                swapNodes(index, parentIndex);
                index = parentIndex;
            }
            else if (heap[index].value == heap[parentIndex].value) {
                if (heap[index].recentuse > heap[parentIndex].recentuse) { swapNodes(index, parentIndex); index = parentIndex; }
                else break;
            }
            else {
                break;
            }
        }
    }

    void heapifyDown(int index) {
        
        if (index >= heap.size()) return;
        int rchildren, lchildren, children;
        while (index < heap.size()) {
            lchildren = 2 * index + 1;
            rchildren = 2 * index + 2;
            if (lchildren >= heap.size()) break;
            else if ( rchildren >= heap.size())
            {
                if (heap[lchildren].value < heap[index].value) { swapNodes(index, lchildren); index = lchildren; }
                else if (heap[index].value == heap[lchildren].value) {
                    if (heap[index].recentuse < heap[lchildren].recentuse) { swapNodes(index, lchildren); index = lchildren;}
                    else break;
                }
                else break; 
            }
            else {
                
                if (heap[index].value < heap[rchildren].value && heap[index].value < heap[lchildren].value) break;
                if (heap[lchildren].value > heap[rchildren].value) {
                    children = rchildren;
                }
                else if (heap[lchildren].value < heap[rchildren].value) {
                    children = lchildren; 
                }
                else {  
                    if (heap[lchildren].recentuse > heap[rchildren].recentuse) {children = lchildren;}
                    else children = rchildren;
                }

                if (heap[index].value == heap[children].value) {
                    
                    if (heap[index].recentuse < heap[children].recentuse) {swapNodes(index, children); index = children;}
                    else break;
                }
                else {
                    swapNodes(index, children);
                    index = children;
                }
            }
        }
    }

    void swapNodes(int index1, int index2) {
        keyToIndexMap[heap[index1].key] = index2;
        keyToIndexMap[heap[index2].key] = index1;
        swap(heap[index1], heap[index2]);
    }


public:
    Minheap(int n) {for (int i = 1; i <= n; i++) a.push_back(i);}
    ~Minheap() { keyToIndexMap.clear(); heap.clear(); a.clear(); }
    void insert(Node node) {
        heap.push_back(node);
        keyToIndexMap[node.key] = heap.size() - 1;
        updaterecent();
        heapifyUp(heap.size() - 1);
    }
    void incrementValue(int key) {
            int index = keyToIndexMap[key];
            heap[index].incrementValue();
            updaterecent();
            heapifyDown(index);
    }
    void remove(int index) {
            swapNodes(index, heap.size() - 1);
            heap.pop_back();
            if (index<=heap.size() - 1) {
                heapifyDown(index);
                heapifyUp(index);
            }
    }
    void modify(int key, int NUM) {
            int index = keyToIndexMap[key];
            heap[index].value -= NUM;
            updaterecent();
            heapifyUp(index);
    }
    void printHeap() {
        for (const auto& node : heap) {
            std::cout << "(Key: " << node.key << ", Value: " << node.value << ") ";
        }
        std::cout << std::endl;
    }
    void notusedres(int n) {
        auto index = find(a.begin(), a.end(), n);
        a.erase(index, index + 1);
        a.insert(a.begin(), n);
    }
    void updaterecent() {
        int n = heap.size();
        for (int i = 0; i < n; i++) {
            heap[i].recentuse = find(a.begin(), a.end(), heap[i].key) - a.begin();
        }
    }
    vector<Node> keitetiken() {
        updaterecent();
        vector<Node> copyheap(heap);
        sort(copyheap.begin(), copyheap.end(), [](Node b, Node c)
            {
                if (b.value < c.value) return true;
                else if (b.value == c.value) {
                    if (b.recentuse > c.recentuse) return true;
                    else return false;
            }
                else return false; });
        return copyheap;
    }

};
class S_restaurant {
public:
    vector <vector<customer*>> b;
public:
    S_restaurant(int size) : b(size + 1) {}
    ~S_restaurant() 
    {
        for (int i = 0; i < b.size(); i++) {
            for (int j = 0; j < b[i].size(); j++) {
                delete b[i][j];
            }
            b[i].clear();
        }
        b.clear();
    }
};



/**********Place to build S_restaurant**********/

void simulate(string filename)
{
    ifstream ss(filename);
    string str, name, maxsize, num, num2, num3;
    int MAXSIZE =0;
    ss >> str;
    if (str == "MAXSIZE")
    {
        ss >> maxsize;
        MAXSIZE = stoi(maxsize);
    }
    vector<customer*> check;
    G_restaurant* G_res = new G_restaurant(MAXSIZE);
    HashTableWithBST* G_hashtable = new HashTableWithBST(MAXSIZE);

    S_restaurant* S_res = new S_restaurant(MAXSIZE);
    Minheap* S_minheap = new Minheap(MAXSIZE);
    while (ss >> str)
    {
        if (str == "LAPSE")
        {
            ss >> name;
            unordered_map<char, int> freq;
            for (char ch : name) {
                freq[ch]++;
            }
            if (freq.size() < 3) continue;
            int n = name.size();
            map<char, int> charFrequency;
            for (char ch : name) {
                if (isalpha(ch)) charFrequency[ch]++;
            }
            for (int i = 0; i < n; i++) {
                int x = 0;
                if (name[i] >= 65 && name[i] <= 90) x = 1;

                int n = name[i] + charFrequency[name[i]];

                if (x == 0) { while (n > 122) n -= 26; }
                else { while (n > 90) n -= 26; }
                name[i] = n;
            }
            customer* p = new customer(name);
            customer* newest = new customer(name);
            check.push_back(newest);
            int ID = (p->result) % MAXSIZE + 1;
            if (p->result % 2 == 1) { G_res->a[ID].push_back(p); G_hashtable->insert(p->result); }
            else {
                S_res->b[ID].push_back(p);
                S_minheap->notusedres(ID);
                if (S_minheap->keyToIndexMap.find(ID) != S_minheap->keyToIndexMap.end())
                {
                    S_minheap->incrementValue(ID);
                }
                else { Node newnode(ID); S_minheap->insert(newnode);}  
            }
        }
        if (str == "KOKUSEN")
        {
            for (int i = 1; i <= MAXSIZE; i++) {
                vector<long long int>c;
                if (G_hashtable->table[i].root) G_hashtable->table[i].preodertoKosuken(G_hashtable->table[i].root, c);
                else continue;
                int N = c.size();
                long long n = countWays(c)%MAXSIZE;
                if (n >= G_res->a[i].size()) {
                    if (G_res->a[i].size() == 0) continue;
                    for (int j = 0; j < G_res->a[i].size(); j++) {
                        delete G_res->a[i][j];
                    }
                    G_res->a[i].clear();
                    G_hashtable->table[i].clear();
                }
                else {
                    for (int j = 0; j < n; j++)
                    {
                        customer* b = G_res->a[i].front();
                        G_hashtable->table[i].delet(b->result);
                        G_res->a[i].erase(G_res->a[i].begin());
                        delete b;
                    }
                }
                
            }
        }
        if (str == "KEITEIKEN")
        {
            ss >> num;
            int NUM = stoi(num);
            int g = S_minheap->heap.size();
            int h = min(NUM, g);
            vector<Node> copyheap;
            copyheap = S_minheap->keitetiken();
            for (int i = 0; i < h; i++)
            {
                int k = copyheap[i].key;
                if (NUM >= S_res->b[k].size())
                {    
                    for (int i = 0; i < S_res->b[k].size(); i++) { cout << S_res->b[k][i]->result <<"-" << k << endl; delete S_res->b[k][i]; }
                    S_res->b[k].clear();
                    S_minheap->remove(S_minheap->keyToIndexMap[k]);
                    S_minheap->keyToIndexMap.erase(k);
                    S_minheap->notusedres(k);
                }
                else {
                    for (int i = 0; i < NUM; i++)
                    {
                        customer* b = S_res->b[k].front();
                        cout << b->result<< "-" << k << endl;
                        S_res->b[k].erase(S_res->b[k].begin());
                        delete b;
                    }
                    S_minheap->notusedres(k);
                    S_minheap->modify(k, NUM);
                }
            }
        }
        if (str == "HAND") {
            if (int(check.size() == 0)) continue;
            else check[int(check.size())-1]->hand(check[int(check.size()) - 1]->root);
        }
        if (str == "LIMITLESS") {
            ss >> num2;
            int NUM2 = stoi(num2);
            if (NUM2 > MAXSIZE) continue;
            if (G_hashtable->table[NUM2].root!=NULL) G_hashtable->table[NUM2].printinoder(G_hashtable->table[NUM2].root);
        }
        if (str == "CLEAVE") {
            ss >> num3;
            int NUM3 = stoi(num3);
            vector<int>a;
            preodertraversal(S_minheap->heap, a, 0);
            for (int i = 0; i < a.size(); i++) {
                int n = S_res->b[a[i]].size();
                int k = 0;  
                if (NUM3 < n) k = n - NUM3;
                for (int j = n - 1; j >= k; j--) {
                    cout << a[i] << "-" << S_res->b[a[i]][j]->result << endl;
                }
            }
        }
    }
    for (int i = 0; i<int(check.size()); i++) delete check[i]; check.clear();
    delete G_res;
    delete G_hashtable;
    delete S_res;
    delete S_minheap;
}


int main(int argc, char* argv[]) {
    string fileName = "text.txt";
    simulate(fileName);
    return 0;

}
