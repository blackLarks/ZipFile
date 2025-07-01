/*
 * Zip.cpp - Main Application Entry Point
 *
 * This is the main entry point for the Flag Display Windows application.
 * It follows the standard Windows GUI application pattern:
 * 1. Initialize the VCL application framework
 * 2. Configure application-wide settings
 * 3. Create and show the main form (Form1)
 * 4. Start the Windows message loop
 * 5. Handle any unhandled exceptions gracefully
 *
 * This file demonstrates the basic structure of a C++ Builder (VCL) application
 * and shows how Windows GUI applications are bootstrapped.
 */

// ---------------------------------------------------------------------------

#include <vcl.h>           // Visual Component Library - Core VCL framework
#pragma hdrstop            // Stop processing headers at this point for precompiled headers
#include <tchar.h>         // Unicode/ANSI character type definitions (_TCHAR, _tWinMain, etc.)
// ---------------------------------------------------------------------------
USEFORM("Zipu1.cpp", Form1);

// ---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	try {
		/*
		 * Application Initialization Phase
		 * Initialize the VCL application framework and set up core services
		 */
		Application->Initialize();

		/*
		 * Taskbar Integration Setting
		 * MainFormOnTaskBar = true means:
		 * - The main form will appear as a separate button on the Windows taskbar
		 * - Alt+Tab will show this application as a separate entry
		 * - Modern Windows UI behavior (recommended for most applications)
		 *
		 * If false: Only the application would appear in taskbar, not individual forms
		 */
		Application->MainFormOnTaskBar = true;

		/*
		 * Main Form Creation
		 * CreateForm creates an instance of the specified form class and:
		 * - Allocates memory for the form object
		 * - Calls the form's constructor
		 * - Registers it as the main application form
		 * - Prepares it for display (but doesn't show it yet)
		 *
		 * __classid(TForm1) gets the runtime type information for the Form1 class
		 * &Form1 is the global pointer that will point to the created form instance
		 */
		Application->CreateForm(__classid(TForm1), &Form1);

		/*
		 * Start the Application Message Loop
		 * Application->Run() starts the main Windows message loop which:
		 * - Shows the main form (makes it visible)
		 * - Processes Windows messages (mouse clicks, keyboard input, paint requests, etc.)
		 * - Keeps the application running until the user closes it
		 * - Returns when the application is terminated
		 *
		 * This is where the application spends most of its time - waiting for and
		 * responding to user input and system events.
		 */
		Application->Run();
	}
	/*
	 * VCL Exception Handler
	 * Catches any VCL/C++ Builder specific exceptions that weren't handled elsewhere
	 * This includes UI exceptions, component errors, and VCL framework issues
	 */
	catch (Exception &exception) {
		// Display the exception message in a standard Windows error dialog
		Application->ShowException(&exception);
	}
	/*
	 * Generic Exception Handler
	 * Catches any other types of exceptions (non-VCL exceptions)
	 * This is a safety net for system errors, access violations, etc.
	 */
	catch (...) {
		try {
			/*
			 * Convert unknown exception to VCL Exception
			 * Since we don't know what type of exception occurred,
			 * we create a generic VCL Exception to display it properly
			 */
			throw Exception("");
		}
		catch (Exception &exception) {
			// Display the generic exception using VCL's error dialog
			Application->ShowException(&exception);
		}
	}

	/*
	 * Application Exit
	 * Return 0 to indicate successful program termination
	 * Windows uses this value to determine if the application closed normally
	 */
	return 0;
}
// ---------------------------------------------------------------------------

