// CollectTheCoinsGUI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CollectTheCoinsGUI.h"
#include <string>
#include <vector>
#include <iterator>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hCTC;										// "Collect The Coins"
HWND hDim;										// "Dimensions of Field: "
HWND hNRows;									// Number of rows
HWND hNCols;									// Number of columns
HWND hRows;										// "Rows"
HWND hCols;										// "Columns"
HWND hCoins;									// "Coins: " + coins
HWND hStart;									// Start button
HWND hField;									// Show Field
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
unsigned long coins = 0;						// Total amount of coins
unsigned long coinsInRound = 0;					// Coins collected in current round
unsigned long rows = 0;							// Rows
unsigned long cols = 0;							// Columns
std::vector<char> field;						// Field
unsigned long x_coord = 0;						// Current x-coordinate
unsigned long y_coord = 0;						// Current y-coordinate
bool gameStarted = false;						// Game has started? Yes, true : No, false
bool a1, d, e1, e2, t, h, y, i, a2;				// Used to determine if "adeethyia" hack has been used
bool fieldGoodSize = true;						// If field is too large, then false

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void Up();
void Down();
void Left();
void Right();
void printField(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COLLECTTHECOINSGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COLLECTTHECOINSGUI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COLLECTTHECOINSGUI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ADEETHYIA));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT window;
	GetClientRect(hWnd, &window);
	wchar_t r[10], c[10];
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_START:
				if (!gameStarted)
				{
					GetWindowTextW(hNRows, r, 10);
					GetWindowTextW(hNCols, c, 10);
					if (std::wstring(r) == L"" || std::wstring(c) == L"")
					{
						int i = (std::wstring(r) == L"") + (std::wstring(c) == L"");
						MessageBoxW(hWnd, L"Enter a whole number in each box", (L"Boxes left empty: " + std::to_wstring(i)).c_str(), MB_OK);
						break;
					}
					rows = std::stoul(r);
					cols = std::stoul(c);
					DeleteObject(r);
					DeleteObject(c);
					if (rows * cols > 100000000)
					{
						MessageBoxW(hWnd, L"The dimensions entered are too large", L"Field Too Large", MB_ICONWARNING | MB_OK);
						break;
					}
					DestroyWindow(hCTC);
					DestroyWindow(hDim);
					DestroyWindow(hNRows);
					DestroyWindow(hRows);
					DestroyWindow(hNCols);
					DestroyWindow(hCols);
					DestroyWindow(hCoins);
					DestroyWindow(hStart);
					gameStarted = true;
					coinsInRound = 0;
					x_coord = 0;
					y_coord = 0;
					SendMessage(hWnd, (UINT)WM_CREATE, (WPARAM)0, (LPARAM)0);
				}
				else if (gameStarted)
				{
					MessageBoxW(hWnd, (L"Rows: " + std::to_wstring(rows) + L"\nColumns: " + std::to_wstring(cols) + L"\nTotal Coins: " + std::to_wstring(coins + coinsInRound)).c_str(), L"Field", MB_OK);
				}
				break;
			case IDM_HELP:
				if (!gameStarted)
				{
					SetWindowTextW(hNRows, L"10");
					SetWindowTextW(hNCols, L"10");
					int choice = MessageBoxW(hWnd, L"Enter the number of rows and columns for the coin field\n\nPress OK to start", L"Start Game", MB_OKCANCEL);
					if (choice == IDCANCEL)
						break;
					fieldGoodSize = true;
					SendMessage(hWnd, (UINT)WM_COMMAND, (WPARAM)IDM_START, (LPARAM)0);
					break;
				}
				else if (gameStarted && fieldGoodSize && (x_coord + 1) < cols && field[y_coord * cols + (x_coord + 1)] == '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\n\nPress OK to automate a right arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						x_coord++;
						field[y_coord * cols + x_coord] = '<';
						printField(hWnd);
					}
					break;
				}
				else if (gameStarted && fieldGoodSize && (y_coord + 1) < rows && field[(y_coord + 1) * cols + x_coord] == '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\n\nPress OK to automate a down arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						y_coord++;
						field[y_coord * cols + x_coord] = '^';
						printField(hWnd);
					}
					break;
				}
				else if (gameStarted && fieldGoodSize && (x_coord) > 0 && field[y_coord * cols + x_coord - 1] == '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\n\nPress OK to automate a left arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						x_coord--;
						field[y_coord * cols + x_coord] = '>';
						printField(hWnd);
					}
					break;
				}
				else if (gameStarted && fieldGoodSize && (y_coord) > 0 && field[(y_coord - 1) * cols + x_coord] == '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\n\nPress OK to automate a up arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						y_coord--;
						field[y_coord * cols + x_coord] = 'v';
						printField(hWnd);
					}
					break;
				}
				else if (gameStarted && !fieldGoodSize && (x_coord + 1) < cols && field[y_coord * cols + (x_coord + 1)] != '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a right arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						x_coord++;
						field[y_coord * cols + x_coord] = '<';
						printField(hWnd);
						goto update;
					}
				}
				else if (gameStarted && !fieldGoodSize && (y_coord + 1) < rows && field[(y_coord + 1) * cols + x_coord] != '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a down arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						y_coord++;
						field[y_coord * cols + x_coord] = '^';
						printField(hWnd);
						goto update;
					}
				}
				else if (gameStarted && fieldGoodSize && (x_coord) > 0 && field[y_coord * cols + x_coord - 1] != '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\n\nPress OK to automate a left arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						x_coord--;
						field[y_coord * cols + x_coord] = '>';
						printField(hWnd);
					}
					break;
				}
				else if (gameStarted && fieldGoodSize && (y_coord) > 0 && field[(y_coord - 1) * cols + x_coord] != '$')
				{
					int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\n\nPress OK to automate a up arrow key press", L"How to Play", MB_OKCANCEL);
					if (choice == IDOK)
					{
						field[y_coord * cols + x_coord] = '#';
						y_coord--;
						field[y_coord * cols + x_coord] = 'v';
						printField(hWnd);
					}
					break;
				}
				else if (gameStarted && !fieldGoodSize && (x_coord + 1) < cols && field[y_coord * cols + (x_coord + 1)] == '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a right arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					x_coord++;
					field[y_coord * cols + x_coord] = '<';
					printField(hWnd);
					goto update;
				}
				}
				else if (gameStarted && !fieldGoodSize && (y_coord + 1) < rows && field[(y_coord + 1) * cols + x_coord] == '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a down arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					y_coord++;
					field[y_coord * cols + x_coord] = '^';
					printField(hWnd);
					goto update;
				}
				}
				else if (gameStarted && !fieldGoodSize && (x_coord) > 0 && field[y_coord * cols + x_coord - 1] == '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a left arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					x_coord--;
					field[y_coord * cols + x_coord] = '>';
					printField(hWnd);
					goto update;
				}
				}
				else if (gameStarted && !fieldGoodSize && (y_coord) > 0 && field[(y_coord - 1) * cols + x_coord] == '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a up arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					y_coord--;
					field[y_coord * cols + x_coord] = 'v';
					printField(hWnd);
					goto update;
				}
				}
				else if (gameStarted && !fieldGoodSize && (x_coord + 1) < cols && field[y_coord * cols + (x_coord + 1)] != '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a right arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					x_coord++;
					field[y_coord * cols + x_coord] = '<';
					printField(hWnd);
					goto update;
				}
				}
				else if (gameStarted && !fieldGoodSize && (y_coord + 1) < rows && field[(y_coord + 1) * cols + x_coord] != '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a down arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					y_coord++;
					field[y_coord * cols + x_coord] = '^';
					printField(hWnd);
					goto update;
				}
				}
				else if (gameStarted && !fieldGoodSize && (x_coord) > 0 && field[y_coord * cols + x_coord - 1] != '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a left arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					x_coord--;
					field[y_coord * cols + x_coord] = '>';
					printField(hWnd);
					goto update;
				}
				}
				else if (gameStarted && !fieldGoodSize && (y_coord) > 0 && field[(y_coord - 1) * cols + x_coord] != '$')
				{
				int choice = MessageBoxW(hWnd, L"Use the arrow keys to move, or type Adeethyia into the field\nNote: (0, 0) refers to the top-left corner\n\nPress OK to automate a up arrow key press", L"How to Play", MB_OKCANCEL);
				if (choice == IDOK)
				{
					field[y_coord * cols + x_coord] = '#';
					y_coord--;
					field[y_coord * cols + x_coord] = 'v';
					printField(hWnd);
					goto update;
				}
				}
				update:
				{
					SetWindowTextW(hField, (L"Current position: (" + std::to_wstring(x_coord) + L", " + std::to_wstring(y_coord) + L")\n" +
						L"Bottom-right corner: (" + std::to_wstring(rows - 1) + L", " + std::to_wstring(cols - 1) + L")").c_str());
 				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		if (!gameStarted)
		{
			SetWindowTextW(hWnd, L"Collect The Coins");
			hCTC = CreateWindowExW(0L, L"static", L"Collect The Coins", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 0, window.right, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hDim = CreateWindowExW(0L, L"static", L"Dimensions:", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 7 * window.bottom / 16, window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hRows = CreateWindowExW(0L, L"static", L"Rows", WS_VISIBLE | WS_CHILD | ES_CENTER, 5 * window.right / 16, 6 * window.bottom / 16, window.right / 4, window.bottom / 16, hWnd, NULL, NULL, NULL);
			hNRows = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_NUMBER, 5 * window.right / 16, 7 * window.bottom / 16, window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hCols = CreateWindowExW(0L, L"static", L"Columns", WS_VISIBLE | WS_CHILD | ES_CENTER, 11 * window.right / 16, 6 * window.bottom / 16, window.right / 4, window.bottom / 16, hWnd, NULL, NULL, NULL);
			hNCols = CreateWindowExW(0L, L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER | ES_NUMBER, 11 * window.right / 16, 7 * window.bottom / 16, window.right / 4, window.bottom / 4, hWnd, NULL, NULL, NULL);
			hCoins = CreateWindowExW(0L, L"static", (L"Coins: " + std::to_wstring(coins)).c_str(), WS_VISIBLE | WS_CHILD | ES_CENTER, window.right / 4, 11 * window.bottom / 16, window.right / 2, window.bottom / 16, hWnd, (HMENU)IDM_START, NULL, NULL);
			hStart = CreateWindowExW(0L, L"Button", L"Start", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 13 * window.bottom / 16, window.right, 3 * window.bottom / 16, hWnd, (HMENU)IDM_START, NULL, NULL);
			SendMessageW(hNRows, (UINT)EM_SETLIMITTEXT, (WPARAM)10, (LPARAM)0);
			SendMessageW(hNCols, (UINT)EM_SETLIMITTEXT, (WPARAM)10, (LPARAM)0);
		}
		else if (gameStarted)
		{
			if (rows * cols > 900000)
			{
				fieldGoodSize = false;
			}
			else
			{
				fieldGoodSize = true;
			}
			hField = CreateWindowExW(0L, L"static", L"", WS_VISIBLE | WS_CHILD | ES_CENTER, 0, 0, window.right, window.bottom, hWnd, (HMENU)IDM_START, NULL, NULL);
			field = std::vector<char>(rows * cols);
			std::fill(field.begin(), field.begin() + rows * cols, '$');
			field[0] = '<';
			if (fieldGoodSize)
			{
				printField(hWnd);
			}
			else
			{
				SetWindowTextW(hField, (L"Current position: (" + std::to_wstring(x_coord) + L", " + std::to_wstring(y_coord) + L")\n" + 
					L"Bottom-right corner: (" + std::to_wstring(rows - 1) + L", " + std::to_wstring(cols - 1) + L")").c_str());
				SetWindowTextW(hWnd, (L"Collect The Coins: 1/" + std::to_wstring(rows * cols)).c_str());
			}
		}
		break;
    case WM_PAINT:
        {
			if (!gameStarted)
			{
				int length = window.bottom > (1 * window.right / 2) ? (1 * window.right / 2) : window.bottom;
				int font = (int)30 * length / 534;
				HFONT hFont = CreateFont(font, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, UNICODE, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
				HFONT hFont1 = CreateFont(2 * font, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, UNICODE, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
				HFONT hFont2 = CreateFont(4 * font, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, UNICODE, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
				MoveWindow(hCTC, 0, 0, window.right, window.bottom / 4, true);
				MoveWindow(hDim, 0, 6 * window.bottom / 16, window.right / 4, window.bottom / 4, true);
				MoveWindow(hRows, 5 * window.right / 16, 5 * window.bottom / 16, window.right / 4, window.bottom / 16, true);
				MoveWindow(hNRows, 5 * window.right / 16, 6 * window.bottom / 16, window.right / 4, window.bottom / 4, true);
				MoveWindow(hCols, 11 * window.right / 16, 5 * window.bottom / 16, window.right / 4, window.bottom / 16, true);
				MoveWindow(hCoins, window.right / 4, 11 * window.bottom / 16, window.right / 2, 2 * window.bottom / 16, true);
				MoveWindow(hNCols, 11 * window.right / 16, 6 * window.bottom / 16, window.right / 4, window.bottom / 4, true);
				MoveWindow(hStart, 0, 13 * window.bottom / 16, window.right, 3 * window.bottom / 16, true);
				SendMessageW(hRows, (UINT)WM_SETFONT, (WPARAM)hFont, (LPARAM)0);
				SendMessageW(hCols, (UINT)WM_SETFONT, (WPARAM)hFont, (LPARAM)0);
				SendMessageW(hDim, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				SendMessageW(hNRows, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				SendMessageW(hNCols, (UINT)WM_SETFONT, (WPARAM)hFont1, (LPARAM)0);
				SendMessageW(hCTC, (UINT)WM_SETFONT, (WPARAM)hFont2, (LPARAM)0);
			}
			else if (gameStarted)
			{
				int length = min(window.bottom / cols, 2 * window.right / rows);
				int font = 500 * length / 534;
				HFONT hFont = CreateFont(font, 0, 0, 0, FW_REGULAR, FALSE, FALSE, FALSE, UNICODE, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Calibri");
				SendMessageW(hField, (UINT)WM_SETFONT, (WPARAM)hFont, (LPARAM)0);
				MoveWindow(hField, 0, 0, window.right, window.bottom, true);
			}
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
			DeleteObject(hdc);
        }
        break;
	case WM_KEYDOWN:
		if (gameStarted)
		{
			if (LOWORD(wParam) == VK_UP)
				Up();
			else if (LOWORD(wParam) == VK_DOWN)
				Down();
			else if (LOWORD(wParam) == VK_LEFT)
				Left();
			else if (LOWORD(wParam) == VK_RIGHT)
				Right();
			else if (LOWORD(wParam) == 0x41 && !a1)
				a1 = true;
			else if (LOWORD(wParam) == 0x44 && a1)
				d = true;
			else if (LOWORD(wParam) == 0x45 && d && !e1)
				e1 = true;
			else if (LOWORD(wParam) == 0x45 && e1)
				e2 = true;
			else if (LOWORD(wParam) == 0x54 && e2)
				t = true;
			else if (LOWORD(wParam) == 0x48 && t)
				h = true;
			else if (LOWORD(wParam) == 0x59 && h)
				y = true;
			else if (LOWORD(wParam) == 0x49 && y)
				i = true;
			else if (LOWORD(wParam) == 0x41 && i)
				a2 = true;
			if (a1 && d && e1 && e2 && t && h && y && i && a2)
			{
				for (unsigned long i = 0; i < rows; i++)
				{
					for (unsigned long j = 0; j < cols; j++)
					{
						field[i * cols + j] = '#';
					}
				}
				a1 = false;
				d = false;
				e1 = false;
				e2 = false;
				t = false;
				h = false;
				y = false;
				i = false;
				a2 = false;
			}
			if (fieldGoodSize)
			{
				printField(hWnd);
			}
			else if (!fieldGoodSize)
			{
				SetWindowTextW(hField, (L"Current position: (" + std::to_wstring(x_coord) + L", " + std::to_wstring(y_coord) + L")\n" +
					L"Bottom-right corner: (" + std::to_wstring(rows - 1) + L", " + std::to_wstring(cols - 1) + L")").c_str());
				unsigned long coinsIR = 0;
				bool allCollected = true;
				for (unsigned long i = 0; i < rows; i++)
				{
					for (unsigned long j = 0; j < cols; j++)
					{
						if (field[i * cols + j] == '#')
						{
							coinsIR++;
						}
						else
						{
							if (i == y_coord && j == x_coord)
							{
								coinsIR++;
							}
							else
							{
								allCollected = false;
							}
						}
					}
				}
				coinsInRound = coinsIR;
				SetWindowTextW(hWnd, (L"Collect The Coins: " + std::to_wstring(coinsInRound) + L"/" + std::to_wstring(rows * cols)).c_str());
				if (allCollected)
				{
					SendMessageW(hWnd, (UINT)ALL_COINS_COLLECTED, (WPARAM)0, (LPARAM)0);
				}
			}
		}
		break;
	case ALL_COINS_COLLECTED:
		coins = coins + coinsInRound;
		MessageBoxW(hWnd, (L"You collected " + std::to_wstring(coinsInRound) + L" coins").c_str(), L"All Coins Collected", MB_ICONINFORMATION);
		DestroyWindow(hField);
		field.erase(field.begin(), field.begin() + rows * cols);
		gameStarted = false;
		SendMessage(hWnd, (UINT)WM_CREATE, (WPARAM)0, (LPARAM)0);
		break;
	case WM_CTLCOLORSTATIC:
		if (true)
		{
			HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(0, 0, 0));
			SetBkColor(hdcStatic, RGB(255, 255, 255));
			return (INT_PTR)hBrush;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Up()
{
	if (y_coord == 0)
	{
		return;
	}
	else
	{
		field[y_coord * cols + x_coord] = '#';
		y_coord--;
		field[y_coord * cols + x_coord] = 'v';
	}
}

void Down()
{
	if (y_coord == (rows - 1))
	{
		return;
	}
	else
	{
		field[y_coord * cols + x_coord] = '#';
		y_coord++;
		field[y_coord * cols + x_coord] = '^';
	}
}

void Left()
{
	if (x_coord == 0)
	{
		return;
	}
	else
	{
		field[y_coord * cols + x_coord] = '#';
		x_coord--;
		field[y_coord * cols + x_coord] = '>';
	}
}

void Right()
{
	if (x_coord == (cols - 1))
	{
		return;
	}
	else
	{
		field[y_coord * cols + x_coord] = '#';
		x_coord++;
		field[y_coord * cols + x_coord] = '<';
	}
}

void printField(HWND hWnd)
{
	std::wstring fieldwstring = L"";
	for (unsigned long i = 0; i < rows; i++)
	{
		std::wstring line(field.begin() + (i * cols), field.begin() + ((i + 1) * cols));
		fieldwstring = fieldwstring + line + L"\n";
	}
	SetWindowTextW(hField, fieldwstring.c_str());
	unsigned long coinsIR = 0;
	bool allCollected = true;
	for (unsigned long i = 0; i < rows; i++)
	{
		for (unsigned long j = 0; j < cols; j++)
		{
			if (field[i * cols + j] == '#')
			{
				coinsIR++;
			}
			else
			{
				if (i == y_coord && j == x_coord)
				{
					coinsIR++;
				}
				else
				{
					allCollected = false;
				}
			}
		}
	}
	coinsInRound = coinsIR;
	SetWindowTextW(hWnd, (L"Collect The Coins: " + std::to_wstring(coinsInRound) + L"/" + std::to_wstring(rows * cols)).c_str());
	if (allCollected)
	{
		SendMessageW(hWnd, (UINT)ALL_COINS_COLLECTED, (WPARAM)0, (LPARAM)0);
	}
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
