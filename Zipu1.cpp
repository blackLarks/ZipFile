/*
 * Flag Display Application - Zipu1.cpp
 *
 * This C++ Builder (VCL) application extracts flag images from an embedded ZIP resource
 * and displays them randomly. The application:
 * 1. Extracts a ZIP file embedded as a resource (ID: 108) in the executable
 * 2. Extracts the ZIP contents to a temporary directory
 * 3. Loads all image files (PNG, JPG, JPEG, BMP, GIF) from the extracted files
 * 4. Displays random flag images with their names
 * 5. Provides a refresh button to show different random flags
 * 6. Cleans up temporary files when the application closes
 */

//---------------------------------------------------------------------------

#include <vcl.h> // Visual Component Library - C++ Builder's GUI framework
#pragma hdrstop

#include "Zipu1.h" // Header file for this form class
//---------------------------------------------------------------------------
#pragma package(smart_init) // Enable smart initialization for packages
#pragma resource "*.dfm" // Link the form's visual design file
#pragma resource \
    "flags.RES" // Link custom resource file containing flag images ZIP
TForm1* Form1; // Global pointer to the main form instance
//---------------------------------------------------------------------------

/*
 * Form Constructor
 * Initializes the form and sets up the application state
 */
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
    // Initialize random number generator with current system tick count
    // This ensures different random sequences each time the app runs
    randomGenerator.seed(GetTickCount());

    // Initialize temporary directory path (will be set during resource extraction)
    tempDirectory = "";

    // Initialize application after form is fully constructed
    // This ensures all UI components are ready before we start processing
    InitializeApplication();
}
//---------------------------------------------------------------------------

/*
 * Form Destructor
 * Cleans up resources when the form is destroyed
 */
__fastcall TForm1::~TForm1()
{
    // Clean up any temporary files and directories created during execution
    CleanupTempFiles();
}
//---------------------------------------------------------------------------

/*
 * Application Initialization
 * Main setup routine that orchestrates the entire flag loading process
 */
void TForm1::InitializeApplication()
{
    // Update status to show we're starting the extraction process
    LabelStatus->Caption = "Status: Extracting resource...";
    Application
        ->ProcessMessages(); // Force UI update to show status immediately

    // Extract and load flag images from embedded ZIP resource
    if (ExtractResourceAsZip()) {
        // If extraction successful, load all image files from extracted directory
        LoadFlagImages();

        if (flagFiles.size() > 0) {
            // Success: Update status with count of loaded images
            LabelStatus->Caption =
                "Status: Successfully loaded " +
                IntToStr(static_cast<int>(flagFiles.size())) + " flag images";

            // Display the first random flag
            ShowRandomFlag();
        } else {
            // No images found after extraction
            LabelStatus->Caption = "Status: No image files found";
            LabelStatus->Font->Color = clRed; // Red text to indicate error
        }
    } else {
        // Resource extraction failed
        LabelStatus->Caption = "Status: Resource extraction failed";
        LabelStatus->Font->Color = clRed; // Red text to indicate error
    }
}
//---------------------------------------------------------------------------

/*
 * Extract Embedded ZIP Resource
 * Extracts a ZIP file that was embedded as a resource in the executable
 * Resource ID: 1, Type: RT_RCDATA (raw data)
 */
bool TForm1::ExtractResourceAsZip()
{
    try {
        HRSRC hResource = NULL;
		// Find the embedded resource by ID (1) and type (RT_RCDATA)
		hResource = FindResource(HInstance, MAKEINTRESOURCE(1), RT_RCDATA);

        if (!hResource) {
            ShowMessage("No suitable resource found in the executable");
            return false;
        }

        // Load the resource into memory
        HGLOBAL hLoadedResource = LoadResource(HInstance, hResource);
        if (!hLoadedResource) {
            ShowMessage("Unable to load resource");
            return false;
        }

        // Get pointer to resource data and its size
        LPVOID pResourceData = LockResource(hLoadedResource);
        DWORD resourceSize = SizeofResource(HInstance, hResource);

        if (!pResourceData || resourceSize == 0) {
            ShowMessage("Invalid resource data");
            return false;
        }

        // Copy resource data to a memory stream for ZIP processing
        TMemoryStream* zipStream = new TMemoryStream();
        try {
            // Write the raw resource data to the memory stream
            zipStream->WriteBuffer(pResourceData, resourceSize);
            zipStream->Position = 0; // Reset stream position to beginning

            // Extract the ZIP file contents to temporary directory
            bool result = ExtractZipToTemp(zipStream);

            delete zipStream; // Clean up memory stream
            return result;
        } catch (Exception &e) {
            delete zipStream; // Ensure cleanup on exception
            ShowMessage("Error processing ZIP data: " + e.Message);
            return false;
        }
    } catch (Exception &e) {
        ShowMessage("Error extracting resource: " + e.Message);
        return false;
    }
}
//---------------------------------------------------------------------------

/*
 * Extract ZIP Contents to Temporary Directory
 * Takes a memory stream containing ZIP data and extracts all files
 * to a newly created temporary directory
 */
bool TForm1::ExtractZipToTemp(TMemoryStream* zipStream)
{
    try {
        // Create a unique temporary directory name using current tick count
        tempDirectory = TPath::GetTempPath() + "FlagImages_" +
                        IntToStr(static_cast<int>(GetTickCount()));
        try {
            // Create the temporary directory
            TDirectory::CreateDirectory(tempDirectory);
            if (!TDirectory::Exists(tempDirectory)) {
                ShowMessage("Unable to create temporary directory");
                return false;
            }
        } catch (...) {
            ShowMessage("Unable to create temporary directory");
            return false;
        }

        // Use C++ Builder's TZipFile class to handle ZIP extraction
        TZipFile* zipFile = new TZipFile();
        try {
            // Open the ZIP file from memory stream in read mode
            zipFile->Open(zipStream, zmRead);

            // Extract all files from the ZIP archive
            for (int i = 0; i < zipFile->FileCount; i++) {
                String fileName = zipFile->FileNames[i];
                String fullPath = TPath::Combine(tempDirectory, fileName);

                // Ensure the target directory exists (for nested folder structures)
                String dir = TPath::GetDirectoryName(fullPath);
                if (!TDirectory::Exists(dir)) {
                    TDirectory::CreateDirectory(dir);
                }

                // Extract the individual file
                zipFile->Extract(fileName, dir);
            }

            zipFile->Close(); // Close the ZIP file
            delete zipFile; // Clean up ZIP file object
            return true; // Success
        } catch (Exception &e) {
            delete zipFile; // Ensure cleanup on exception
            ShowMessage("Error extracting ZIP file: " + e.Message);
            return false;
        }
    } catch (Exception &e) {
        ShowMessage("Error creating temporary directory: " + e.Message);
        return false;
    }
}
//---------------------------------------------------------------------------

/*
 * Load Flag Image Files
 * Searches the extracted temporary directory for all supported image formats
 * and builds a list of available flag image files
 */
void TForm1::LoadFlagImages()
{
    // Clear any previously loaded file list
    flagFiles.clear();

    // Validate that temporary directory exists
    if (tempDirectory.IsEmpty() || !TDirectory::Exists(tempDirectory))
        return;

    try {
        // Define supported image file extensions
        TStringDynArray extensions;
        extensions.Length = 5;
        extensions[0] = "*.png"; // Portable Network Graphics
        extensions[1] = "*.jpg"; // JPEG
        extensions[2] = "*.jpeg"; // JPEG (alternative extension)
        extensions[3] = "*.bmp"; // Windows Bitmap
        extensions[4] = "*.gif"; // Graphics Interchange Format

        // Search for files matching each extension pattern
        for (int i = 0; i < extensions.Length; i++) {
            // Get all files matching current extension in all subdirectories
            TStringDynArray files = TDirectory::GetFiles(
                tempDirectory, extensions[i], TSearchOption::soAllDirectories);

            // Add each found file to our flag files collection
            for (int j = 0; j < files.Length; j++) {
                flagFiles.push_back(files[j]);
            }
        }
    } catch (Exception &e) {
        ShowMessage("Error searching for image files: " + e.Message);
    }
}
//---------------------------------------------------------------------------

/*
 * Display Random Flag Image
 * Randomly selects and displays one of the loaded flag images
 */
void TForm1::ShowRandomFlag()
{
    // Check if any flag images are available
    if (flagFiles.size() == 0) {
        LabelFlagName->Caption = "No flag images available";
        return;
    }

    try {
        // Generate random index to select a flag image
        std::uniform_int_distribution<int> dist(0, flagFiles.size() - 1);
        int index = dist(randomGenerator);

        // Get the selected file path
        String selectedFile = flagFiles[index];

        // Load and display the image in the ImageFlag component
        ImageFlag->Picture->LoadFromFile(selectedFile);

        // Extract and display the filename (without path and extension)
        String fileName = TPath::GetFileNameWithoutExtension(selectedFile);
        LabelFlagName->Caption = "Flag: " + fileName;

        // Update status to show current position in collection
        LabelStatus->Caption =
            "Status: Displaying " + IntToStr(index + 1) + "/" +
            IntToStr(static_cast<int>(flagFiles.size())) + " flag";
    } catch (Exception &e) {
        ShowMessage("Error displaying image: " + e.Message);
        LabelFlagName->Caption = "Image loading failed";
    }
}
//---------------------------------------------------------------------------

/*
 * Cleanup Temporary Files
 * Removes the temporary directory and all extracted files
 * Called during application shutdown
 */
void TForm1::CleanupTempFiles()
{
    if (!tempDirectory.IsEmpty() && TDirectory::Exists(tempDirectory)) {
        try {
            // Delete the entire temporary directory and all its contents
            TDirectory::Delete(tempDirectory, true); // true = recursive delete
        } catch (Exception &e) {
            // Ignore cleanup errors - this is called during shutdown
            // and we don't want to show error messages at this point
        }
    }
}

//---------------------------------------------------------------------------

/*
 * Random Button Click Event Handler
 * Triggered when user clicks the "Random" button to display a new flag
 */
void __fastcall TForm1::ButtonRandomClick(TObject* Sender)
{
    // Clear current image first to show loading state
    ImageFlag->Picture = NULL;
    LabelFlagName->Caption = "Loading...";
    LabelStatus->Caption = "Status: Refreshing...";

    // Force UI update to show loading state immediately
    Application->ProcessMessages();

    // Add a small delay to make the refresh visible to user
    // This provides visual feedback that something is happening
    Sleep(100);

    // Display a new random flag
    ShowRandomFlag();
}
//---------------------------------------------------------------------------
