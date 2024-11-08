# Process Output Capture (Windows)

This project demonstrates how to capture and display the output of a process (specifically a command prompt command) in a Windows environment using the Windows API.

## Features

- Creates a pipe to capture the output of a command executed by `cmd.exe or powershell`.
- Sets the console's output code page to UTF-8 to ensure proper Unicode handling.
- Reads the output of the process and displays it as UTF-16 if the console supports UTF-8.
- Works with standard commands such as `dir .` for listing directory contents.

## Requirements

- Windows OS (tested on Windows 10)
- Visual Studio (or any Windows-compatible C++ compiler)
- The program uses the Windows API, specifically `CreatePipe`, `CreateProcessW`, and related functions for handling process output and pipes.

## How to Use

1. Clone or download this repository to your local machine.
2. Open the project in Visual Studio or any C++ compiler that supports Windows API.
3. Compile and run the program.

### Example Output

When the program is executed, it will run the `dir .` command in the current directory and capture the output. The output will be displayed in the console.

## Potential issues 

Keep in my the output is correct and if wrote in a file there is no problem but when printing (using wcout) the non-ascii characters will not appear in console. 
