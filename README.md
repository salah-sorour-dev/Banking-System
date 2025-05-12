# 🏦 Bank Management System (C++ Project)

This is an updated version of the **Bank Management System** built in **C++**. In addition to the original banking operations, this version introduces a **Login System** and a **User Management Interface** to enhance security and access control.

## ✨ Features
- 🔐 **Login System** to control access to the system  
- 👥 **Manage Clients Page** to list/add/edit/delete/find clients 
- ✔️ Deposit money into an account  
- ✔️ Withdraw money from an account  
- ✔️ View total balances of all clients  
- 👥 **Manage Users Page** to add/edit/delete users and assign task permissions  
- 🔐 **Logout** 

## 🛠️ Technologies Used
- **C++** (File Handling, Vectors, Structs, and Standard Library)  
- **File-Based Storage** (Data is stored in `.txt` files)  
- **Basic Authentication Logic** (Credential checking via file system)  

## 📂 Project Structure
- `Clients.txt` → Stores client account details  
- `Users.txt` → Stores registered user credentials and permissions  
- `main.cpp` → Contains the core banking and authentication logic  
- `Bank.h` → Header file for struct and function declarations  

## 🚀 How to Use
1. Clone the repository:  
   ```bash
   git clone https://github.com/salah-sorour-dev/bank-management-system.git
   cd bank-management-system
   ```
2. Compile the code using any C++ compiler:  
   ```bash
   g++ main.cpp -o bank.exe
   ```  
3. Run the executable:  
   ```bash
   ./bank.exe
   ```  

## 🔒 User Access
- When launching the program, users will be prompted to log in.
- Only authenticated users can access system features based on their permissions.

## 📌 Notes
- The application uses **file-based storage**; all data persists locally between sessions.  
- If `Clients.txt` or `Users.txt` don't exist, the system will create them automatically.  
- Ensure secure storage and management of `Users.txt` for production use.

---

Developed by [salah-sorour-dev](https://github.com/salah-sorour-dev)
