# 🚗 Fleet Management System (C-Language)

A robust command-line application built in **C** to manage a car rental fleet. This system handles everything from user authentication to financial asset valuation.

## ✨ Features
* **🔒 Secure Access:** User login and registration system.
* **📦 Inventory Management:** Add, delete, and search for vehicles in the database.
* **💳 Rental Module:** Track which cars are currently rented or available.
* **📉 Financial Tools:** Automated depreciation calculation using the formula:  
    $$Value = Price \times 0.95^{age}$$
* **📜 Activity Logging:** Every action is timestamped and logged for security audits.

## 🚀 How to Run
1.  **Clone the repo:**
    ```bash
    git clone [https://github.com/omniaislamm/YOUR_REPO_NAME.git](https://github.com/omniaislamm/YOUR_REPO_NAME.git)
    ```
2.  **Compile:**
    Use any C compiler (GCC, Clang) or open the `exer.cbp` file in **Code::Blocks**.
3.  **Execute:**
    Run the compiled binary. The system will automatically create `cars.txt` and `users.txt` on its first run.

## 🛠️ Technologies
* **Language:** C
* **Storage:** Flat-file database (.txt)
* **Memory Management:** Dynamic allocation (realloc/free)
