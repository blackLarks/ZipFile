# README

| File Name          | Description                                                                |
| ------------------ | -------------------------------------------------------------------------- |
| `flags.bin`        | Binary data file.                                                          |
| `flags.rc`         | Resource scrip.                                                            |
| `flags.RES`        | Compiled resource file generated from `flags.rc`.                          |
| `README.md`        | Project documentation.                                                     |
| `Zip.cbproj`       | C++ Builder project file for the application.                              |
| `Zipu1.cpp`        | Core implementation file for ZIP functionality.                            |
| `Zip1u1.dfm`       | Delphi Form file containing the UI layout for the application.             |
| `Zip1u1.h`         | Header file with declarations corresponding to `Zip1.cpp`.                 |
| `Zip1.h`           | Main Application Entry Point.                                              |


We embed a file (like a PNG, ZIP, etc.) into your `.exe` as a **resource**.

## Build the flags.RES

- **Get the PNG**
  - You can follow the instructions on this [Github Page](https://github.com/hampusborgos/country-flags) to convert the PNG of all countries from the SVG files.
  - Save in the `flags` folder.
  
- **Pack into RES**
  - Zip `flags` folder into `flags.bin`
  - Suppose the resource ID is *1*. Create a `flags.rc` file.
    ```bash
    1 RCDATA flags.bin
    ```
  - Use the command to pack PNG files into RES
    ```bash
    rc flags.rc
    ```

## Unzip from the Resource

- **Locate the resource**
  - Include in the beginning
    ```c++
    #pragma resource \
        "flags.RES" // Link custom resource file containing flag images ZIP
    ```
  - Find the resource with the corresponding ID(1).
    ```c++
    HRSRC hResource = NULL;
    // Find the embedded resource by ID (1) and type (RT_RCDATA)
    hResource = FindResource(HInstance, MAKEINTRESOURCE(1), RT_RCDATA);
    ```

- **Load and Lock the Resource**
  - Load the resource into memory and get a pointer to its raw data
    ```c++
    HGLOBAL hLoadedResource = LoadResource(HInstance, hResource);
    LPVOID pResourceData = LockResource(hLoadedResource);
    ```
  - Get the size of the data
    ```c++
    DWORD resourceSize = SizeofResource(HInstance, hResource);
    ```

- **Treat the Resource as a Memory Stream**
```c++
TMemoryStream* zipStream = new TMemoryStream();
zipStream->WriteBuffer(pResourceData, resourceSize);
zipStream->Position = 0;
```

- **Process the Stream as a ZIP**
  - Open the ZIP file from memory stream
    ```c++
    TZipFile* zipFile = new TZipFile();
    zipFile->Open(zipStream, zmRead);
    ```
  - Extract the file to the temp path
    ```c++
    zipFile->Extract(fileName, dir);
    ```
  - Close and clean
    ```c++
    zipFile->Close();
    delete zipFile;
    ```

## Load PNG from Temp Directory

```c++
// Generate a random number as the PNG index
int index = dist(randomGenerator);

// Get the selected file path
String selectedFile = flagFiles[index];

// Load and display the image in the ImageFlag component
ImageFlag->Picture->LoadFromFile(selectedFile);
```

## Application Interface
![image](https://github.com/user-attachments/assets/d9b85287-76d6-4fc4-a6fe-abf06bf7cbb7)

