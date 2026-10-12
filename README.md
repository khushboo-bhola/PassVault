# 🔐 PassVault — Terminal-Based Password Manager

PassVault is a simple, menu-driven password manager developed in **C**. It allows users to add, view, search, and delete password records through a terminal interface.

The project demonstrates core C programming concepts, including structures, modular programming, file handling, and persistent local storage. It is designed as an educational project and will be extended with stronger security features in future versions.

> **Security Notice:** This version stores password records without encryption. Use dummy credentials only. Do not use it to store real passwords.

---

## 📌 Table of Contents

- [Project Overview](#-project-overview)
- [Features](#-features)
- [Technologies Used](#-technologies-used)
- [Project Structure](#-project-structure)
- [Prerequisites](#-prerequisites)
- [Installation and Setup](#-installation-and-setup)
- [How to Use](#-how-to-use)
- [Data Storage](#-data-storage)
- [Concepts Demonstrated](#-concepts-demonstrated)
- [Security Limitations](#-security-limitations)
- [Future Improvements](#-future-improvements)
- [Contributing](#-contributing)
- [License](#-license)

---

## 📖 Project Overview

Managing multiple website credentials can be difficult. PassVault provides a simple command-line interface for organizing password records locally.

Each record contains:

- **ID:** A unique identifier assigned to the record.
- **Website:** The website or service associated with the credentials.
- **Username:** The account username.
- **Password:** The password associated with the account.

Users can manage these records through an interactive menu without needing a graphical interface or database server.

### Project Goals

- Practice C programming and modular code organization.
- Understand structures and functions.
- Learn file handling and persistent storage.
- Apply Git and GitHub version control practices.
- Build a foundation for future security improvements.

---

## ✨ Features

### 1. Add Password
Enter a website, username, and password to create a new record. PassVault assigns an ID and saves the record locally.

### 2. View Passwords
Display the saved password records, including their IDs, websites, usernames, and passwords.

### 3. Search Passwords
Search for saved records by website name. The program compares the requested website with the stored website names.

### 4. Delete Passwords
Delete a record using its ID. The program rewrites the data file without the selected record, preserving the other records.

### 5. Unique ID Generation
Assign IDs based on the highest existing ID, helping distinguish records even when an earlier record has been deleted.

### 6. Persistent Local Storage
Save records to a local file so they remain available after the application closes and restarts.

### 7. Menu-Driven Interface
Navigate the available operations through a numbered terminal menu and exit when finished.

---

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C | Core programming language |
| GCC | C compiler |
| GNU Make | Build automation |
| File Handling | Local record storage and retrieval |
| Structures | Represent password records |
| Git | Version control |
| GitHub | Source code hosting |

---

## 📂 Project Structure

```text
PassVault/
├── src/
│   ├── main.c
│   ├── password.c
│   └── file.c
├── include/
│   ├── password.h
│   ├── file.h
│   └── utils.h
├── data/
│   └── .gitkeep
├── .gitignore
├── Makefile
├── README.md
└── LICENSE
```

### Directory and File Responsibilities

- `src/main.c` — Displays the menu and handles user choices.
- `src/password.c` — Implements password operations such as adding, viewing, searching, and deleting records.
- `src/file.c` — Handles file operations and generates record IDs.
- `include/password.h` — Defines the `Password` structure and declares password-related functions.
- `include/file.h` — Declares file-storage functions.
- `include/utils.h` — Reserved for shared utility declarations as the project grows.
- `data/` — Stores runtime password data locally.
- `.gitignore` — Excludes generated files and local password data from Git.
- `Makefile` — Automates compilation.
- `README.md` — Documents the project.
- `LICENSE` — Contains the project's license terms.

---

## ⚙️ Prerequisites

Before building PassVault, make sure you have:

- Windows, Linux, or another compatible environment.
- GCC installed and accessible from your terminal.
- GNU Make installed if you want to use the Makefile.
- Git installed if you want to clone the repository or contribute.

Check your installations:

```bash
gcc --version
make --version
git --version
```

---

## 🚀 Installation and Setup

### 1. Clone the Repository

Replace `YOUR_USERNAME` with your GitHub username and use your actual repository name.

```bash
git clone https://github.com/YOUR_USERNAME/PassVault.git
cd PassVault
```

Alternatively, download the repository ZIP file from GitHub and extract it.

### 2. Build Using Make

From the project's root directory, run:

```bash
make
```

The Makefile compiles the C source files and generates the executable.

### 3. Build Using GCC Directly

If GNU Make is unavailable, compile the project manually.

**Windows PowerShell:**

```powershell
gcc -Wall -Wextra src/main.c src/password.c src/file.c -o passvault.exe
```

**Linux:**

```bash
gcc -Wall -Wextra src/main.c src/password.c src/file.c -o passvault
```

### 4. Run the Application

**Windows PowerShell:**

```powershell
.\passvault.exe
```

**Linux:**

```bash
./passvault
```

Run commands from the project root so the relative data-file path works as expected.

---

## 💻 How to Use

After launching PassVault, an interactive menu appears.

```text

          PASSVAULT
1. Add Password
2. View Passwords
3. Search Password
4. Delete Password
5. Exit

Enter your choice:
```

### Example Workflow

1. Select `1` to add a record.
2. Enter a website, username, and dummy password.
3. Select `2` to view saved records and note their IDs.
4. Select `3` to search for a website.
5. Select `4` and enter a record ID to delete it.
6. Select `5` to exit the program.

The menu allows users to perform the available operations without manually editing the storage file.

---

## 💾 Data Storage

PassVault stores records in:

```text
data/passwords.dat
```

The application uses C file-handling functions to read and write records.

- `fopen()` opens or creates a file.
- `fread()` reads stored records.
- `fwrite()` writes records.
- `fclose()` closes an open file.
- `remove()` and `rename()` support the record-deletion workflow.

The current implementation stores binary representations of the `Password` structure rather than using a database.

**Important:** The data file is not encrypted. Binary storage does not provide password confidentiality or security.

The `.gitignore` file excludes `data/*.dat` so local password records are not accidentally committed to the repository.

---

## 🧠 Concepts Demonstrated

This project provides practical experience with:

- Structures and member access in C.
- Functions and header files.
- Modular programming.
- Conditional statements and switch cases.
- Loops and menu-driven interaction.
- Pointers and file pointers.
- Binary file input and output.
- String comparison using `strcmp()`.
- Unique record ID generation.
- Basic error handling.
- Build automation using GNU Make.
- Version control using Git and GitHub.

---

## 🔒 Security Limitations

PassVault is currently an educational prototype, **not a secure password manager**.

Known limitations include:

- Passwords are stored without encryption.
- Passwords are displayed in the terminal.
- Input validation and error handling are basic.
- The binary file is specific to the current structure layout and is not a portable storage format.
- There is no master-password authentication or protected key management.

Do not enter real credentials into this version.

---

## 🛣️ Future Improvements

Planned improvements may include:

- [ ] Add proper encryption using a reputable cryptographic library.
- [ ] Introduce master-password authentication and secure key derivation.
- [ ] Improve input validation and error handling.
- [ ] Support spaces in website names and usernames.
- [ ] Avoid displaying stored passwords unnecessarily.
- [ ] Add automated tests.
- [ ] Improve portability and data-file handling.
- [ ] Add a secure password generator.
- [ ] Improve the user interface and documentation.

These improvements are planned features and are not necessarily implemented in the current version.

---

## 🤝 Contributing

Contributions and suggestions are welcome.

1. Fork the repository.
2. Create a branch for your changes.
3. Make a focused change.
4. Compile and test the application.
5. Submit a pull request describing your changes.

Please do not commit real credentials, generated password data, or other sensitive information.

---

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
