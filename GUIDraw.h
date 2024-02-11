class GUIDraw {
public:
	static void OnSomeActionToRefreshValues(HWND hWnd, wchar_t *wcs);
	static void DrawValues(HDC hdc, wchar_t *wcs);
	static void updateProgressBar(int percentageCounter, HWND hProgressBar, HWND hWnd, wchar_t *hProgressText);
	static void CreateQuestion();
};