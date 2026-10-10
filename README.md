
# PassVault

PassVault is a simple terminal-based password manager written in C.

## Features

- Add a password
- View saved passwords
- Search passwords by website
- Delete passwords using their IDs
- Exit through the menu

## Technologies Used

- C programming language
- GCC compiler
- File handling
- Structures
- Git and GitHub

## Project Structure

- `src/` - C implementation files
- `include/` - Header files
- `data/` - Local password data

## How to Compile

Make sure GCC is installed.

```bash
gcc -Wall -Wextra src/main.c src/password.c src/file.c -o passvault.exe
```

## How to Run

On Windows PowerShell:

```powershell
.\passvault.exe
```

## Security Notice

This is an educational project. Passwords are currently
stored without encryption. Use only dummy data and do not
store real passwords until secure encryption is implemented.

## License

This project is licensed under the MIT License.
