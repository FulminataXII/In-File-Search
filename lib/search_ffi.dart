import 'dart:ffi';
import 'package:ffi/ffi.dart';

// const char* string_matching(const char* s_c, const char* dir_c, bool case_s, const char* puncts);
typedef StringMatchingC = Pointer<Utf8> Function(
  Pointer<Utf8> s_c,
  Pointer<Utf8> dir_c,
  Uint8 case_s,
  Pointer<Utf8> puncts,
);
typedef StringMatchingDart = Pointer<Utf8> Function(
  Pointer<Utf8>, Pointer<Utf8>, int, Pointer<Utf8>
);

// void free_result(const char* result);
typedef FreeResultC = Void Function(Pointer<Utf8>);
typedef FreeResultDart = void Function(Pointer<Utf8>);

class SearchFFI {
  static final DynamicLibrary _lib = DynamicLibrary.open('search_dll.dll');

  static final StringMatchingDart stringMatching = _lib
      .lookup<NativeFunction<StringMatchingC>>('string_matching')
      .asFunction();

  static final FreeResultDart freeResult = _lib
      .lookup<NativeFunction<FreeResultC>>('free_result')
      .asFunction();

  static String search({
    required String pattern,
    required String directory,
    required bool caseSensitive,
    required String puncts,
  }) {
    final patternPtr = pattern.toNativeUtf8();
    final dirPtr = directory.toNativeUtf8();
    final punctsPtr = puncts.toNativeUtf8();
    final resultPtr = stringMatching(
      patternPtr,
      dirPtr,
      caseSensitive ? 1 : 0,
      punctsPtr,
    );
    final result = resultPtr.toDartString();
    malloc.free(patternPtr);
    malloc.free(dirPtr);
    malloc.free(punctsPtr);
    freeResult(resultPtr);
    return result;
  }
}