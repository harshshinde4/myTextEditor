// ss 257 onwards

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "comdlg32.lib")

#define FILE_MENU_EXIT          0

#define NEW_FILE_SELECTED       11
#define OPEN_SELECTED           12
#define SAVE_SELECTED           13
#define SAVE_AS_SELECTED        14

#define UNDO_SELECTED           21
#define CUT_SELECTED            22
#define COPY_SELECTED           23
#define PASTE_SELECTED          24
#define DELETE_SELECTED         25
#define SEARCH_WITH_BING_SELECTED   26
#define FIND_SELECTED           27
#define FIND_NEXT_SELECTED      28
#define FIND_PREVIOUS_SELECTED  29
#define REPLACE_SELECTED        211
#define GO_TO_SELECTED          212
#define SELECT_ALL_SELECTED     213
#define TIME_DATE_SELECTED      214

#define WORD_WRAP_SELECTED      31
#define FONT_SELECTED           32

#define ZOOM_IN_SELECTED        411
#define ZOOM_OUT_SELECTED       412
#define RESTORE_DEFAULT_ZOOM_SELECTED   413
#define STATUS_BAR_SELECTED     42

#define VIEW_HELP_SELECTED      51
#define SEND_FEEDBACK_SELECTED  52
#define ABOUT_NOTEPAD_SELECTED  53

// Declaration of functions 
void AddMenusAll(HWND hWnd);    // for adding the menu
void AddMenubarMenus(HMENU hMenu);
void AddControls (HWND hWnd, int cxScreen, int cyScreen);

int nOpenFile(HWND hWnd);
int DisplayTextFile(char* path);

int SaveFile(HWND hWnd);
int nWriteFile(char* path);
char* GetFileName(char* path);
int SetFileName(char* path);
void SaveFileMessagebox(HWND hWnd, int size);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Global variables
HWND hInput = NULL, hShreeGaneshPrasanna = NULL, hFileName = NULL;      // for input
HMENU hMenu = NULL, hFileMenu = NULL, hSetBaseButton = NULL, hSetBaseButtonSub = NULL;     // handler to the menu
char path[100];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szAppName[] = TEXT("The Standard Window");
    static TCHAR szWindowName[] = TEXT("NotePad -by HARSH");

    HWND hWnd = NULL;
    HBRUSH hBrush = NULL;
    HCURSOR hCursor = NULL;
    HICON hIcon = NULL;
    HICON hIconSm = NULL;

    WNDCLASSEX wndEx;
    MSG msg;

    path[0] = '\0';     //setting the first index of an array to \0
    ZeroMemory(&wndEx, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    assert(hBrush);

    hCursor = LoadCursor(NULL, IDC_ARROW);
    assert(hCursor);

    hIcon = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIcon);

    hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    assert(hIconSm);

    wndEx.cbSize = sizeof(WNDCLASSEX);
    wndEx.cbClsExtra = 0;
    wndEx.cbWndExtra = 0;
    wndEx.hbrBackground = hBrush;
    wndEx.hCursor = hCursor;
    wndEx.hIcon = hIcon;
    wndEx.hIconSm = hIconSm;
    wndEx.hInstance = hInstance;
    wndEx.lpfnWndProc = WndProc;
    wndEx.lpszClassName = szAppName;
    wndEx.lpszMenuName = NULL;
    wndEx.style = CS_HREDRAW | CS_VREDRAW;

    ATOM bRet = RegisterClassEx(&wndEx);
    assert(bRet);

    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName, szWindowName, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                NULL, NULL, hInstance, NULL);

    assert(hWnd != NULL);

    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ret = 0;
    HDC hDc;
    PAINTSTRUCT ps;
    static int cxScreen, cyScreen;
    static int size = 0;

    switch(uMsg)
    {
        case WM_CREATE:
            AddMenusAll(hWnd);  //adding menus
            AddControls(hWnd, cxScreen, cyScreen);  // adding control
            break;

        case WM_SIZE:
            cxScreen = LOWORD(lParam);
            cyScreen = HIWORD(lParam);
            MoveWindow(hShreeGaneshPrasanna, 0, 0, cxScreen, cyScreen, TRUE);   // If user change the size of the MAIN WINDOW then 
            MoveWindow(hInput, 21, 50, cxScreen-24, cyScreen-70, TRUE);        // this function will move all child w.r.t. SIZE OF MAIN WINDOW.
            break;

        case WM_COMMAND:
            switch (wParam)
            {
                case FILE_MENU_EXIT:
                    ret = MessageBoxA(hWnd, "Are you sure ?", "wait", MB_YESNO | MB_ICONEXCLAMATION | MB_APPLMODAL);
                    if(ret == IDYES)
                        DestroyWindow(hWnd);
                    break;

                case NEW_FILE_SELECTED:
                    SaveFileMessagebox(hWnd, size);
                    path[0] = '\0';     //setting the first index of array to \0
                    SetWindowTextA(hFileName, "untitled");
                    SetWindowTextA(hInput, NULL);
                    size = 0;
                    break;

                case OPEN_SELECTED:
                    SaveFileMessagebox(hWnd, size);
                    size = nOpenFile(hWnd);
                    SetFileName(path);
                    break;

                case SAVE_SELECTED:
                    if(path[0] != '\0')
                        size = nWriteFile(path);
                    else 
                        size = SaveFile(hWnd);
                    SetFileName(path);
                    break;
                
                case SAVE_AS_SELECTED:
                    size = SaveFile(hWnd);
                    SetFileName(path);
                    break;
                
                case UNDO_SELECTED:
                case CUT_SELECTED:
                case COPY_SELECTED:
                case PASTE_SELECTED:
                case DELETE_SELECTED:
                case SEARCH_WITH_BING_SELECTED:
                case FIND_SELECTED:
                case FIND_NEXT_SELECTED:
                case FIND_PREVIOUS_SELECTED:
                case REPLACE_SELECTED:
                case GO_TO_SELECTED:
                case SELECT_ALL_SELECTED:
                case TIME_DATE_SELECTED:
                case WORD_WRAP_SELECTED:
                case FONT_SELECTED:
                case ZOOM_IN_SELECTED:
                case ZOOM_OUT_SELECTED:
                case RESTORE_DEFAULT_ZOOM_SELECTED:
                case STATUS_BAR_SELECTED:
                case VIEW_HELP_SELECTED:
                case SEND_FEEDBACK_SELECTED:
                case ABOUT_NOTEPAD_SELECTED:
                    MessageBoxA(hWnd, "This feature has not implemented yet! ","wait" , MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
                    break;

                default:
                    break;
                
            }
            break;

        case WM_CLOSE:
            SaveFileMessagebox(hWnd, size);
            PostQuitMessage(0);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

void AddControls(HWND hWnd, int cxScreen, int cyScreen)
{
    hShreeGaneshPrasanna = CreateWindow(TEXT("Static"), TEXT("\n|| Shree Ganesh Prasanna ||"), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
                            0, 0, cxScreen, cyScreen, hWnd, NULL, NULL, NULL);
    hFileName = CreateWindow(TEXT("Edit"), TEXT("Untitled"), WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY ,
                            20, 30, 200, 20, hWnd, NULL, NULL, NULL);
    hInput = CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER | SS_LEFT | ES_AUTOVSCROLL | ES_MULTILINE | WS_VSCROLL,
                            21, 50, cxScreen-24, cyScreen-70, hWnd, NULL, NULL, NULL);

}

void AddMenusAll(HWND hWnd)
{
    hMenu = CreateMenu();

    AddMenubarMenus(hMenu);
    SetMenu(hWnd, hMenu);
}

void AddMenubarMenus(HMENU hMenu)
{
    hFileMenu = CreateMenu();
    HMENU hSubMenu = NULL, hSubZoom = NULL;
    hSubMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "File");
    /* Append file menu and its sub menus */
    AppendMenu(hSubMenu, MF_STRING, NEW_FILE_SELECTED, "New File");
    AppendMenu(hSubMenu, MF_STRING, OPEN_SELECTED, "Open");
    AppendMenu(hSubMenu, MF_STRING, SAVE_SELECTED, "Save");
    AppendMenu(hSubMenu, MF_STRING, SAVE_AS_SELECTED, "Save As");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSubMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

    hSubMenu = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Edit");
    /* Append edit menu and its sub menus */
    AppendMenu(hSubMenu, MF_STRING, UNDO_SELECTED, "Undo");
    AppendMenu(hSubMenu, MF_STRING, CUT_SELECTED, "Cut");
    AppendMenu(hSubMenu, MF_STRING, COPY_SELECTED, "Copy");
    AppendMenu(hSubMenu, MF_STRING, PASTE_SELECTED, "Paste");
    AppendMenu(hSubMenu, MF_STRING, DELETE_SELECTED, "Delete");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSubMenu, MF_STRING, SEARCH_WITH_BING_SELECTED, "search with bing...");
    AppendMenu(hSubMenu, MF_STRING, FIND_SELECTED, "Find...");
    AppendMenu(hSubMenu, MF_STRING, FIND_NEXT_SELECTED, "Find next");
    AppendMenu(hSubMenu, MF_STRING, FIND_PREVIOUS_SELECTED, "previous");
    AppendMenu(hSubMenu, MF_STRING, REPLACE_SELECTED, "Replace...");
    AppendMenu(hSubMenu, MF_STRING, GO_TO_SELECTED, "Go To ...");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSubMenu, MF_STRING, SELECT_ALL_SELECTED, "Select All");
    AppendMenu(hSubMenu, MF_STRING, TIME_DATE_SELECTED, "Time/Date");

    hSubMenu = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Format");
    /* Append Format menu and its sub menus */
    AppendMenu(hSubMenu, MF_STRING, WORD_WRAP_SELECTED, "Word Wrap");
    AppendMenu(hSubMenu, MF_STRING, FONT_SELECTED, "Font...");

    hSubMenu = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "View");
    /* Append view menu and its sub menus */
    
    hSubZoom = CreateMenu();
    AppendMenu(hSubMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Zoom");
    /* Append zoom and its sub menus */
    AppendMenu(hSubZoom, MF_STRING, ZOOM_IN_SELECTED, "Zoom In");
    AppendMenu(hSubZoom, MF_STRING, ZOOM_OUT_SELECTED, "Zoom Out");
    AppendMenu(hSubZoom, MF_STRING, RESTORE_DEFAULT_ZOOM_SELECTED, "Restore default zoom");
    AppendMenu(hSubMenu, MF_STRING, STATUS_BAR_SELECTED, "Status Bar");

    hSubMenu = CreateMenu();
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Help");
    /* Append Help menu and its menus */
    AppendMenu(hSubMenu, MF_STRING, VIEW_HELP_SELECTED, "View Help");
    AppendMenu(hSubMenu, MF_STRING, SEND_FEEDBACK_SELECTED, "Send Feedback");
    AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hSubMenu, MF_STRING, ABOUT_NOTEPAD_SELECTED, "About Notepad");
}

int nOpenFile(HWND hWnd)
{
    OPENFILENAMEA ofn;
    /*
        OPENFILENAMEA (struct)-
            Contains information that the GetOpenFileName and GetSaveFileName
            functions use to initialize an Open or save as dialouge box 
            
        Present in commdlg.h (include Windows.h)
    */

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "All(*.*)\0*.*\0Text(.txt)\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;

    GetOpenFileNameA(&ofn);
    /*
        GetOpenFileNameA-
            Crates an Open dialog box that lets the user specify the drive
            directory, and the name of a file or set of files to be opend 
    */

    if(ofn.lpstrFile[0] != '\0')
        return(DisplayTextFile(ofn.lpstrFile));
    
    return(0);
}

int DisplayTextFile(char* path)
{
    FILE *file;
    int size = 0;
    char* data;

    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    data = (char*)calloc(size + 1, sizeof(char));
    fread(data, size, 1, file);
    data[size] = '\0';

    SetWindowTextA(hInput, data);
    fclose(file);
    free(data);
    data = NULL;

    return(size);
}

int SaveFile(HWND hWnd)
{
    OPENFILENAMEA ofn;
    /*  
    OPENFILENAMEA (struct)
        contains info that the GetOpenFileName and GetSaveFileName
        functions are used to initialize an open or save as dialog box

    present in commdlg.h (include windows.h) 
    */

    int size = 0;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = path;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "All(*.*)0*.*\0Text(.txt)\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = "Save As";
    ofn.nMaxFileTitle = 8;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    GetSaveFileNameA(&ofn);
        /*
            Creates a save dialog box that lets the user specify the drive,
            directory, and name of a file to save.
                1. Commdlg.h (include Windows.h)
                2. Comdlg32.lib
                3. Comdlg32.dll
        */

    if (ofn.lpstrFile[0] != '\0')
        size = nWriteFile(ofn.lpstrFile);
        
    return(size);
}
    
int nWriteFile (char* path)
{
    FILE *file;
    int size = 0;
    char* data = NULL;

    file = fopen(path, "w");    //opens the filename pointed to, by fileName using the w(write) mode.
    size = GetWindowTextLength(hInput);
    data = (char*)realloc(data, (size+1)*sizeof(char));
    GetWindowTextA(hInput, data, size+1); // collecting/catching the text from window whose handler is hInput

    fwrite(data, size, 1, file);
    fclose(file); 
    free(data);
    data = NULL;

    return(size);
}

int SetFileName(char* path)
{
    char* FileName = NULL;
    FileName = GetFileName(path);
    if(FileName == NULL)
        return(0);
    SetWindowTextA(hFileName, FileName);
    // display the string present at FileName on the window whose handler is fileName
    free(FileName);
    FileName = NULL;
    return(1);
}

char* GetFileName(char* str)
{
    int i = 0;
    char* FileName = NULL;
    int length = 0;
    char temp;

    length = strlen(str);
    if(length < 2)
    {
        FileName = (char*)calloc(strlen("Untitled\0"), sizeof(char));
        strcpy(FileName, "Untitled");
        return(FileName);
    }
    // collecting the characters from str to FileName
    for(i = 0; str[length - i - 1] != '\0'; ++i)
    {
        FileName = (char*)realloc(FileName, (i*1)*sizeof(char));
        FileName[i] = str[length - i - 1];
    }
    FileName = (char*)realloc(FileName, (i + 1)*sizeof(char));
    FileName[i] = '\0';

    length = strlen(FileName);

    // reversing the string present at FileName
    for(i = 0; i < (length/2); ++i)
    {
        temp = FileName[i];
        FileName[i] = FileName[length - i - 1];
        FileName[length - i - 1] = temp;
    }
    return(FileName);
}

void SaveFileMessagebox(HWND hWnd, int size)
{
    char msg[300] = "Do you want to save the changes you made to the file";
    char* FileName = NULL;
        
    if((size) != GetWindowTextLength(hInput)) // comparing the size of input text && the text present on input child
    {
        FileName = GetFileName(path);
        strcat(msg, FileName);
        strcat(msg, " ?");
        free(FileName);
        FileName = NULL;

        if(IDYES == MessageBox(hWnd, TEXT(msg), TEXT("Notepad - by HARSH !"), MB_YESNO | MB_ICONEXCLAMATION |MB_APPLMODAL))
        {
            if(path[0] != '\0')
                size = nWriteFile(path);
            else 
                size = SaveFile(hWnd);
        }
    }
}




