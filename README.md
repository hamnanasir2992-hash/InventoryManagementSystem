# Inventory and Stock Management System
A console-based Inventory and Stock Management System written in C++ for a Data Structures and Algorithms Open-Ended Lab. The program manages product records, tracks stock arrivals and issues, generates summary reports, checks user inputs, and saves data to text files so nothing is lost when the program closes.

## Key Features
### Product Catalog Management: 
Add new products, update product names, prices, or low-stock limits, delete items with confirmation, and view all items in a clean table.   
### Stock IN and Stock OUT: 
Add incoming stock or issue outgoing stock with automatic balance updates, timestamps, and alerts when stock is low or out. 
### Search and Sort: 
Search products by exact ID or partial name, and sort the inventory ascending or descending by ID, name, or quantity.   
### Reports and History: 
View a full inventory summary, a low-stock restock list, and a newest-first transaction history.   
### Input Validation: 
Rejects non-numeric entries, empty or overly long names, duplicate product IDs, and values outside allowed limits.   
### File Persistence: 
Automatically loads saved records at startup and writes updates to products.txt and transactions.txt.   
### Data Structures and Algorithms UsedDoubly Linked List (Product): 
Stores each product (id, name, price, qty, minQty) with next and prev pointers. It grows and shrinks dynamically as products are added or removed.   
### Linked Stack (Transaction): 
Stores every Stock IN and Stock OUT action (type, id, name, qty, balance, time) in Last-In, First-Out (LIFO) order so the most recent transactions show up first.   
### Linear Search: 
Checks items one by one from the start of the list to match a product ID or a lowercase part of a product name (O(n) time, O(1) extra space).   
### Bubble Sort: 
Compares neighboring products and swaps their stored values until they match the chosen sort field (ID, name, or quantity) and order (ascending or descending) (O(n^2) time, O(1) extra space).

## How to Compile and Run
Open a terminal or command prompt in the folder containing OELcode.cpp.   
Compile the program using g++:   **g++ -std=c++11 OELcode.cpp -o OELcode**
Run the compiled program:   **./OELcode**
(On Windows, run OELcode.exe or OELcode)   

## How to Use the Program
Choose menu options 1 through 9 from the main menu to manage products, record stock changes, search, sort, view reports, or display all products.   
Inside submenus, you can type b or B to go back without finishing an action.   
Enter 0 to exit the program and save your data.   
products.txt and transactions.txt are created and updated automatically in the working directory.
