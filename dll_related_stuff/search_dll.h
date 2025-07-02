#pragma once

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C"
#endif

// Performs string matching in files within a directory.
// Parameters:
//   s_c      - pattern string (Type: C string)
//   dir_c    - directory path (Type: C string)
//   case_s   - case sensitive search (Type: boolean)
//   puncts   - punctuation characters to consider in string matching (Type: C string)
// Returns:
//   Pointer to a JSON C string (must be freed with free_result).
DLL_EXPORT const char* string_matching(const char* s_c, const char* dir_c, bool case_s, const char* puncts);

// Frees the result buffer returned by string_matching.
DLL_EXPORT void free_result(const char* result);