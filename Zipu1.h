/*
 * Zipu1.h - Flag Display Application Header File
 *
 * This header file defines the main form class (TForm1) for a Windows GUI application
 * that extracts flag images from an embedded ZIP resource and displays them randomly.
 * 
 * Key Features:
 * - Extracts ZIP files embedded as Windows resources
 * - Displays random flag images from extracted collection
 * - Manages temporary file cleanup
 * - Provides user interface for flag browsing
 *
 * Architecture:
 * - Uses VCL (Visual Component Library) for Windows GUI components
 * - Follows Model-View pattern with form handling both UI and business logic
 * - Resource management with RAII principles (constructor/destructor cleanup)
 * - Exception-safe temporary file handling
 */

//---------------------------------------------------------------------------

#ifndef Zipu1H
#define Zipu1H
//---------------------------------------------------------------------------

/*
 * VCL Framework Includes
 * These headers provide the core Visual Component Library functionality
 * for building Windows GUI applications with C++ Builder
 */
#include <System.Classes.hpp>     // Base VCL classes: TComponent, TObject, etc.
#include <Vcl.Controls.hpp>       // Base control classes: TControl, TWinControl
#include <Vcl.StdCtrls.hpp>       // Standard controls: TButton, TLabel, etc.
#include <Vcl.Forms.hpp>          // Form classes: TForm, TApplication
#include <Vcl.ExtCtrls.hpp>       // Extended controls: TImage, TPanel, etc.

/*
 * Graphics and Image Processing Includes
 * Support for loading and displaying various image formats
 */
#include <Vcl.Graphics.hpp>       // Core graphics classes: TGraphic, TBitmap, TPicture
#include <Vcl.Imaging.pngimage.hpp>  // PNG image format support
#include <Vcl.Imaging.jpeg.hpp>      // JPEG image format support

/*
 * System and File Management Includes
 * Utilities for ZIP handling, file operations, and system integration
 */
#include <System.zip.hpp>         // ZIP file compression/decompression (TZipFile)
#include <System.IOUtils.hpp>     // Modern file I/O utilities (TPath, TDirectory, TFile)

/*
 * Standard C++ Library Includes
 * Modern C++ containers and utilities for enhanced functionality
 */
#include <vector>                 // Dynamic array container for storing file paths
#include <random>                 // Modern C++ random number generation

//---------------------------------------------------------------------------

/*
 * TForm1 - Main Application Form Class
 *
 * This class represents the main window of the flag display application.
 * It inherits from TForm (VCL's base form class) and implements:
 * 
 * 1. Resource Management: Extracts ZIP files embedded in the executable
 * 2. Image Processing: Loads and displays various image formats
 * 3. User Interface: Provides controls for user interaction
 * 4. Temporary File Management: Handles cleanup of extracted files
 * 5. Random Selection: Displays random flags from the collection
 *
 * Design Pattern: This class follows a hybrid approach combining:
 * - Presentation Layer: UI components and event handling
 * - Business Logic: File extraction, image loading, random selection
 * - Resource Management: Temporary directory and file cleanup
 */
class TForm1 : public TForm
{
  __published: // IDE-managed Components
    /*
     * Visual Components Section
     * These components are managed by the RAD Studio IDE and are automatically
     * created/destroyed with the form. They represent the user interface elements.
     */
    
    TImage* ImageFlag;           // Main image display component for showing flag images
                                 // Supports multiple formats: PNG, JPEG, BMP, GIF
                                 // Automatically handles image scaling and display
    
    TButton* ButtonRandomFlag;   // User action button to trigger random flag selection
                                 // Connected to ButtonRandomClick event handler
                                 // Provides the primary user interaction mechanism
    
    TLabel* LabelFlagName;      // Text display component showing the current flag's name
                                // Displays filename without path/extension for clean presentation
                                // Updates automatically when new flag is selected
    
    TLabel* LabelStatus;        // Status information display for user feedback
                                // Shows: loading states, error messages, success counts
                                // Color-coded: normal (black) vs error (red) states

    /*
     * Event Handler Declaration
     * This function is called automatically when ButtonRandomFlag is clicked
     */
    void __fastcall ButtonRandomClick(TObject* Sender);

  private: // User declarations
    /*
     * Private Data Members
     * These variables maintain the application's internal state and are not
     * accessible from outside the class, ensuring data encapsulation.
     */
    
    std::vector<String> flagFiles;  // Dynamic collection of discovered flag image file paths
                                    // Uses std::vector for efficient random access and iteration
                                    // Stores full file paths to extracted images
                                    // Populated during LoadFlagImages() execution
    
    String tempDirectory;           // Path to temporary directory containing extracted files
                                    // Created during resource extraction process
                                    // Used for cleanup during application shutdown
                                    // Format: %TEMP%/FlagImages_<timestamp>
    
    std::mt19937 randomGenerator;   // Modern C++ Mersenne Twister random number generator
                                    // Seeded with system tick count for different sequences
                                    // Used with uniform_int_distribution for fair flag selection
                                    // Provides high-quality randomness for user experience

    /*
     * Private Core Functionality Methods
     * These methods implement the main business logic of the application
     * and are called internally to perform specific operations.
     */
    
    bool ExtractResourceAsZip();    // Extracts embedded ZIP resource from executable
                                    // Resource ID: 108, Type: RT_RCDATA
                                    // Returns: true if extraction successful, false otherwise
                                    // Handles Windows resource API calls and error checking
    
    bool ExtractZipToTemp(TMemoryStream* zipStream);  // Extracts ZIP contents to temp directory
                                                      // Parameter: Memory stream containing ZIP data
                                                      // Creates unique temporary directory
                                                      // Returns: success/failure status
                                                      // Uses TZipFile for actual extraction
    
    void LoadFlagImages();          // Discovers and catalogs all image files in temp directory
                                    // Searches for: *.png, *.jpg, *.jpeg, *.bmp, *.gif
                                    // Populates flagFiles vector with found file paths
                                    // Supports recursive subdirectory searching
    
    void ShowRandomFlag();          // Selects and displays a random flag from the collection
                                    // Updates ImageFlag, LabelFlagName, and LabelStatus
                                    // Uses randomGenerator for fair selection
                                    // Handles image loading errors gracefully
    
    void CleanupTempFiles();        // Removes temporary directory and all extracted files
                                    // Called during form destruction (destructor)
                                    // Implements RAII pattern for resource cleanup
                                    // Ignores errors during shutdown to prevent exceptions
    
    void InitializeApplication();   // Main initialization routine called from constructor
                                    // Orchestrates: resource extraction → image loading → display
                                    // Updates UI status during each phase
                                    // Handles initialization errors with user feedback

  public: // User declarations
    /*
     * Public Interface Methods
     * These methods form the public API of the form class and handle
     * object lifecycle management.
     */
    
    __fastcall TForm1(TComponent* Owner);  // Constructor: Initializes form and application state
                                           // Parameter: Owner component (typically Application)
                                           // Initializes random generator with system tick count
                                           // Calls InitializeApplication() to start flag loading
                                           // Follows VCL constructor convention with __fastcall
    
    __fastcall ~TForm1();                  // Destructor: Cleanup resources before form destruction
                                           // Calls CleanupTempFiles() to remove temporary data
                                           // Ensures no resource leaks when application closes
                                           // Implements RAII pattern for automatic cleanup

    // Note: Event handler functions are declared in __published section above
    // Additional public methods can be added here if needed for external access
};

//---------------------------------------------------------------------------

/*
 * Global Form Instance Declaration
 * This extern declaration makes the main form instance available throughout the application.
 * The actual Form1 object is created in the main application file (Zip.cpp) during startup.
 * 
 * Usage Pattern:
 * - Form1 is created by Application->CreateForm() in WinMain
 * - Other units can access the form through this global pointer
 * - Automatically destroyed when application terminates
 */
extern PACKAGE TForm1* Form1;

//---------------------------------------------------------------------------
#endif // Zipu1H

