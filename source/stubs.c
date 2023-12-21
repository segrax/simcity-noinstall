#include "stubs.h"

#include <Windows.h>
#include <stdlib.h>
#include <string.h>

#include "array_size.h"

void* winmm_handle = NULL;

int stubs_ctor()
{
  wchar_t buffer[MAX_PATH] = {0};
  wchar_t const* winmm = NULL;
  UINT dir_length = GetSystemDirectoryW(buffer, COUNTOF(buffer));
  if (dir_length == 0 || dir_length >= COUNTOF(buffer)) {
    return 1;
  }

  winmm = L"\\winmm.dll";
  if (buffer[min(dir_length - 1, COUNTOF(buffer) - 2)] == L'\\') {
    winmm += 1;
  }

  (void)wcsncat(buffer + dir_length, winmm, COUNTOF(buffer) - dir_length);

  winmm_handle = LoadLibraryW(buffer);
  if (winmm_handle == NULL) {
    return 1;
  }

  return stubs_load();
}

int stubs_dtor()
{
  if (winmm_handle == NULL) {
    return 0;
  }

  return FreeLibrary(winmm_handle) != 0 ? 0 : 1;
}
