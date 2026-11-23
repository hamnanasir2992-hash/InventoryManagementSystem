#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
using namespace std;

struct Product {
    int id;
    string name;
    int price;
    int qty;
    int minQty;
    Product* next;
    Product* prev;
};

struct Transaction {
    string type;
    int id;
    string name;
    int qty;
    int balance;
    string time;
    Transaction* next;
};

Product* head = NULL;
Product* tail = NULL;
int numProducts = 0;

Transaction* top = NULL;
int numHistory = 0;

bool goBack = false;
bool allowBack = true;

int toNumber(string s) {
    int n = 0;
    for (int i = 0; i < (int)s.length(); i++)
        n = n * 10 + (s[i] - '0');
    return n;
}

string toLower(string s) {
    for (int i = 0; i < (int)s.length(); i++)
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] = s[i] + 32;
    return s;
}

string getTime() {
    time_t t = time(0);
    char buf[20];
    strftime(buf, 20, "%d-%m-%Y %H:%M", localtime(&t));
    return buf;
}

void printLine(int width, char c) {
    for (int i = 0; i < width; i++) cout << c;
    cout << endl;
}

void title(string t) {
    cout << endl;
    printLine(91, '=');
    int space = (91 - t.length()) / 2;
    for (int i = 0; i < space; i++) cout << " ";
    cout << t << endl;
    printLine(91, '=');
}

int getInt(string msg, int low, int high) {
    string s;
    while (true) {
        cout << msg;
        getline(cin, s);

        if (allowBack && (s == "b" || s == "B")) {
            goBack = true;
            return 0;
        }

        bool ok = true;
        if (s.length() == 0 || s.length() > 9) ok = false;
        for (int i = 0; i < (int)s.length(); i++)
            if (s[i] < '0' || s[i] > '9') ok = false;

        if (ok == false) {
            cout << "  [!] Invalid input. Enter a whole number.\n";
        } else {
            int x = toNumber(s);
            if (x < low || x > high)
                cout << "  [!] Value must be between " << low << " and " << high << ".\n";
            else
                return x;
        }
    }
}

string getName(string msg) {
    string s;
    while (true) {
        cout << msg;
        getline(cin, s);

        if (s == "b" || s == "B") {
            goBack = true;
            return "";
        }

        if (s.find_first_not_of(' ') == string::npos)
            cout << "  [!] Name cannot be empty.\n";
        else if (s.length() > 22)
            cout << "  [!] Name too long (max 22 characters).\n";
        else
            return s;
    }
}

bool askYes(string msg) {
    string s;
    while (true) {
        cout << msg << " (y/n): ";
        getline(cin, s);
        if (s == "y" || s == "Y") return true;
        if (s == "n" || s == "N" || s == "b" || s == "B") return false;
        cout << "  [!] Please type y or n.\n";
    }
}

Product* findProduct(int id) {
    Product* p = head;
    while (p != NULL) {
        if (p->id == id) return p;
        p = p->next;
    }
    return NULL;
}

void insertEnd(int id, string name, int price, int qty, int minQty) {
    Product* n = new Product;
    n->id = id;
    n->name = name;
    n->price = price;
    n->qty = qty;
    n->minQty = minQty;
    n->next = NULL;
    n->prev = tail;
    if (head == NULL) head = n;
    else tail->next = n;
    tail = n;
    numProducts++;
}

void deleteNode(Product* p) {
    if (p->prev != NULL) p->prev->next = p->next;
    else head = p->next;
    if (p->next != NULL) p->next->prev = p->prev;
    else tail = p->prev;
    delete p;
    numProducts--;
}

void pushTransaction(string type, Product* p, int qty) {
    Transaction* t = new Transaction;
    t->type = type;
    t->id = p->id;
    t->name = p->name;
    t->qty = qty;
    t->balance = p->qty;
    t->time = getTime();
    t->next = top;
    top = t;
    numHistory++;
}

string getStatus(Product* p) {
    if (p->qty == 0) return "OUT";
    if (p->qty <= p->minQty) return "LOW";
    return "OK";
}

void printHeader() {
    printLine(91, '-');
    cout << left << "| " << setw(5) << "ID" << " | " << setw(22) << "Product Name"
         << " | " << right << setw(10) << "Price" << " | " << left << setw(6) << "Qty"
         << " | " << setw(6) << "Min" << " | " << setw(6) << "Status"
         << " | " << right << setw(14) << "Value" << " |" << endl;
    printLine(91, '-');
}

void printRow(Product* p) {
    cout << left << "| " << setw(5) << p->id << " | " << setw(22) << p->name
         << " | " << right << setw(10) << fixed << setprecision(2) << (double)p->price
         << " | " << left << setw(6) << p->qty << " | " << setw(6) << p->minQty
         << " | " << setw(6) << getStatus(p) << " | " << right << setw(14)
         << (double)p->price * p->qty << " |" << endl << left;
}

void showAll() {
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    printHeader();
    Product* p = head;
    while (p != NULL) {
        printRow(p);
        p = p->next;
    }
    printLine(91, '-');
    cout << "  Total products: " << numProducts << endl;
}

void saveData() {
    ofstream pf("products.txt");
    Product* p = head;
    while (p != NULL) {
        pf << p->id << endl << p->name << endl << p->price << endl
           << p->qty << endl << p->minQty << endl;
        p = p->next;
    }
    pf.close();

    ofstream tf("transactions.txt");
    Transaction* t = top;
    while (t != NULL) {
        tf << t->type << endl << t->id << endl << t->name << endl
           << t->qty << endl << t->balance << endl << t->time << endl;
        t = t->next;
    }
    tf.close();
}

void loadData() {
    string line, name;
    int id, price, qty, minQty;

    ifstream pf("products.txt");
    while (getline(pf, line)) {
        if (line.length() == 0) continue;
        id = toNumber(line);
        getline(pf, name);
        getline(pf, line);  price = toNumber(line);
        getline(pf, line);  qty = toNumber(line);
        getline(pf, line);  minQty = toNumber(line);
        insertEnd(id, name, price, qty, minQty);
    }
    pf.close();

    Transaction* last = NULL;
    ifstream tf("transactions.txt");
    while (getline(tf, line)) {
        if (line.length() == 0) continue;
        Transaction* t = new Transaction;
        t->type = line;
        getline(tf, line);  t->id = toNumber(line);
        getline(tf, t->name);
        getline(tf, line);  t->qty = toNumber(line);
        getline(tf, line);  t->balance = toNumber(line);
        getline(tf, t->time);
        t->next = NULL;
        if (top == NULL) top = t;
        else last->next = t;
        last = t;
        numHistory++;
    }
    tf.close();

    if (numProducts > 0)
        cout << "  Loaded " << numProducts << " product(s) from products.txt\n";
    if (numHistory > 0)
        cout << "  Loaded " << numHistory << " transaction(s) from transactions.txt\n";
}

void addProduct() {
    title("ADD NEW PRODUCT");
    int id = getInt("  Product ID (1-99999)      : ", 1, 99999);
    if (goBack) { goBack = false; return; }
    if (findProduct(id) != NULL) {
        cout << "  [X] Duplicate ID! Product " << id << " already exists.\n";
        return;
    }
    string name = getName("  Product name              : ");
    if (goBack) { goBack = false; return; }
    int price = getInt("  Unit price                : ", 0, 500000);
    if (goBack) { goBack = false; return; }
    int qty = getInt("  Opening quantity          : ", 0, 100000);
    if (goBack) { goBack = false; return; }
    int minQty = getInt("  Low-stock level           : ", 0, 100000);
    if (goBack) { goBack = false; return; }

    insertEnd(id, name, price, qty, minQty);
    saveData();
    cout << "  [OK] Product added successfully.\n";
}

void updateProduct() {
    title("UPDATE PRODUCT");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    showAll();
    cout << endl;
    int id = getInt("  Enter Product ID: ", 1, 99999);
    if (goBack) { goBack = false; return; }
    Product* p = findProduct(id);
    if (p == NULL) {
        cout << "  [X] Product not found.\n";
        return;
    }
    printHeader();
    printRow(p);
    printLine(91, '-');

    cout << "  1. Change name\n  2. Change price\n  3. Change low-stock level\n";
    int c = getInt("  Choice: ", 1, 3);
    if (goBack) { goBack = false; return; }
    if (c == 1) {
        string n = getName("  New name: ");
        if (goBack) { goBack = false; return; }
        p->name = n;
    }
    if (c == 2) {
        int x = getInt("  New price: ", 0, 500000);
        if (goBack) { goBack = false; return; }
        p->price = x;
    }
    if (c == 3) {
        int x = getInt("  New low-stock level: ", 0, 100000);
        if (goBack) { goBack = false; return; }
        p->minQty = x;
    }

    saveData();
    cout << "  [OK] Product updated.\n";
    printHeader();
    printRow(p);
    printLine(91, '-');
}

void deleteProduct() {
    title("DELETE PRODUCT");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    showAll();
    cout << endl;
    int id = getInt("  Enter ID of the product to delete: ", 1, 99999);
    if (goBack) { goBack = false; return; }
    Product* p = findProduct(id);
    if (p == NULL) {
        cout << "  [X] Product not found.\n";
        return;
    }
    if (askYes("  Delete \"" + p->name + "\"?") == false) {
        cout << "  Cancelled.\n";
        return;
    }
    deleteNode(p);
    saveData();
    cout << "  [OK] Product deleted.\n";
}

void searchProduct() {
    title("SEARCH PRODUCT");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    cout << "  1. Search by ID\n  2. Search by name\n";
    int c = getInt("  Choice: ", 1, 2);
    if (goBack) { goBack = false; return; }

    if (c == 1) {
        int id = getInt("  Enter Product ID: ", 1, 99999);
        if (goBack) { goBack = false; return; }
        Product* p = findProduct(id);
        if (p == NULL) {
            cout << "  [X] No product with ID " << id << ".\n";
            return;
        }
        printHeader();
        printRow(p);
        printLine(91, '-');
    } else {
        string name = getName("  Enter name (or part of it): ");
        if (goBack) { goBack = false; return; }
        string key = toLower(name);
        int found = 0;
        Product* p = head;
        while (p != NULL) {
            if (toLower(p->name).find(key) != string::npos) {
                if (found == 0) printHeader();
                printRow(p);
                found++;
            }
            p = p->next;
        }
        if (found == 0) {
            cout << "  [X] No product matches \"" << key << "\".\n";
        } else {
            printLine(91, '-');
            cout << "  " << found << " match(es) found.\n";
        }
    }
}

bool wrongOrder(Product* a, Product* b, int field, int order) {
    if (field == 1) {
        if (order == 1) return a->id > b->id;
        return a->id < b->id;
    }
    if (field == 2) {
        string x = toLower(a->name);
        string y = toLower(b->name);
        if (order == 1) return x > y;
        return x < y;
    }
    if (order == 1) return a->qty > b->qty;
    return a->qty < b->qty;
}

void swapData(Product* a, Product* b) {
    int id = a->id;          a->id = b->id;          b->id = id;
    string name = a->name;   a->name = b->name;      b->name = name;
    int price = a->price;    a->price = b->price;    b->price = price;
    int qty = a->qty;        a->qty = b->qty;        b->qty = qty;
    int minQty = a->minQty;  a->minQty = b->minQty;  b->minQty = minQty;
}

void bubbleSort(int field, int order) {
    for (int i = 0; i < numProducts - 1; i++) {
        Product* p = head;
        for (int j = 0; j < numProducts - 1 - i; j++) {
            if (wrongOrder(p, p->next, field, order))
                swapData(p, p->next);
            p = p->next;
        }
    }
}

void sortProducts() {
    title("SORT INVENTORY");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    cout << "  1. By Product ID\n  2. By Name\n  3. By Quantity\n";
    int field = getInt("  Sort by: ", 1, 3);
    if (goBack) { goBack = false; return; }
    cout << "  1. Ascending\n  2. Descending\n";
    int order = getInt("  Order  : ", 1, 2);
    if (goBack) { goBack = false; return; }

    bubbleSort(field, order);
    saveData();
    cout << "  [OK] Inventory sorted.\n\n";
    showAll();
}

void stockIn() {
    title("STOCK IN (RECEIVE)");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    showAll();
    cout << endl;
    int id = getInt("  Enter Product ID: ", 1, 99999);
    if (goBack) { goBack = false; return; }
    Product* p = findProduct(id);
    if (p == NULL) {
        cout << "  [X] Product not found.\n";
        return;
    }
    cout << "  Product: " << p->name << "   Current stock: " << p->qty << endl;
    int q = getInt("  Quantity to add: ", 1, 100000);
    if (goBack) { goBack = false; return; }
    if (p->qty + q > 100000) {
        cout << "  [X] Limit exceeded! Max stock per product is 100000.\n";
        return;
    }
    p->qty = p->qty + q;
    pushTransaction("IN", p, q);
    saveData();
    cout << "  [OK] Stock added. New stock of " << p->name << ": " << p->qty << endl;
}

void stockOut() {
    title("STOCK OUT (ISSUE)");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    showAll();
    cout << endl;
    int id = getInt("  Enter Product ID: ", 1, 99999);
    if (goBack) { goBack = false; return; }
    Product* p = findProduct(id);
    if (p == NULL) {
        cout << "  [X] Product not found.\n";
        return;
    }
    cout << "  Product: " << p->name << "   Current stock: " << p->qty << endl;
    if (p->qty == 0) {
        cout << "  [X] Zero stock! Nothing available to issue.\n";
        return;
    }
    int q = getInt("  Quantity to issue: ", 1, 100000);
    if (goBack) { goBack = false; return; }
    if (q > p->qty) {
        cout << "  [X] Insufficient stock! Requested " << q << ", available " << p->qty << ".\n";
        return;
    }
    p->qty = p->qty - q;
    pushTransaction("OUT", p, q);
    saveData();
    cout << "  [OK] Stock issued. New stock of " << p->name << ": " << p->qty;
    if (p->qty == 0) cout << "  ** NOW OUT OF STOCK **";
    else if (p->qty <= p->minQty) cout << "  ** LOW STOCK WARNING **";
    cout << endl;
}

void inventoryReport() {
    title("INVENTORY REPORT");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    showAll();

    int units = 0, low = 0, out = 0;
    double value = 0;
    Product* p = head;
    while (p != NULL) {
        units = units + p->qty;
        value = value + (double)p->price * p->qty;
        if (p->qty == 0) out++;
        else if (p->qty <= p->minQty) low++;
        p = p->next;
    }
    cout << "\n  ------------- SUMMARY -------------\n";
    cout << "  Total units in stock : " << units << endl;
    cout << "  Total stock value    : " << fixed << setprecision(2) << value << endl;
    cout << "  Low-stock products   : " << low << endl;
    cout << "  Out-of-stock products: " << out << endl;
}

void lowStockReport() {
    title("LOW STOCK REPORT");
    if (head == NULL) {
        cout << "  [X] Inventory is empty.\n";
        return;
    }
    int found = 0;
    Product* p = head;
    while (p != NULL) {
        if (p->qty <= p->minQty) {
            if (found == 0) printHeader();
            printRow(p);
            found++;
        }
        p = p->next;
    }
    if (found == 0) {
        cout << "  All products are sufficiently stocked.\n";
    } else {
        printLine(91, '-');
        cout << "  " << found << " product(s) need restocking.\n";
    }
}

void transactionHistory() {
    title("TRANSACTION HISTORY (newest first)");
    if (top == NULL) {
        cout << "  [X] No transactions recorded yet.\n";
        return;
    }
    printLine(80, '-');
    cout << left << "| " << setw(16) << "Date & Time" << " | " << setw(4) << "Type"
         << " | " << setw(5) << "ID" << " | " << setw(22) << "Product Name"
         << " | " << setw(6) << "Qty" << " | " << setw(8) << "Balance" << " |" << endl;
    printLine(80, '-');
    Transaction* t = top;
    while (t != NULL) {
        cout << left << "| " << setw(16) << t->time << " | " << setw(4) << t->type
             << " | " << setw(5) << t->id << " | " << setw(22) << t->name
             << " | " << setw(6) << t->qty << " | " << setw(8) << t->balance << " |" << endl;
        t = t->next;
    }
    printLine(80, '-');
    cout << "  Total transactions: " << numHistory << endl;
}

void reportsMenu() {
    title("REPORTS");
    cout << "  1. Full inventory report\n  2. Low-stock report\n  3. Transaction history\n";
    int c = getInt("  Choice: ", 1, 3);
    if (goBack) { goBack = false; return; }
    if (c == 1) inventoryReport();
    if (c == 2) lowStockReport();
    if (c == 3) transactionHistory();
}

void showMenu() {
    cout << endl;
    printLine(91, '=');
    cout << "                 INVENTORY AND STOCK MANAGEMENT SYSTEM\n";
    printLine(91, '=');
    cout << "   1. Add Product            6. Search Product\n"
         << "   2. Update Product         7. Sort Inventory\n"
         << "   3. Delete Product         8. Reports (stock / low stock / history)\n"
         << "   4. Stock IN               9. Display All Products\n"
         << "   5. Stock OUT              0. Exit\n";
    printLine(91, '-');
}

int main() {
    cout << "\nStarting system...\n";
    loadData();

    int choice;
    do {
        showMenu();
        allowBack = false;
        choice = getInt("  Enter choice: ", 0, 9);
        allowBack = true;

        if (choice == 1) addProduct();
        else if (choice == 2) updateProduct();
        else if (choice == 3) deleteProduct();
        else if (choice == 4) stockIn();
        else if (choice == 5) stockOut();
        else if (choice == 6) searchProduct();
        else if (choice == 7) sortProducts();
        else if (choice == 8) reportsMenu();
        else if (choice == 9) { title("ALL PRODUCTS"); showAll(); }
    } while (choice != 0);

    saveData();
    cout << "\n  Data saved. Goodbye!\n";
    return 0;
}