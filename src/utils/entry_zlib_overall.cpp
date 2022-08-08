

#ifdef _MSC_VER
#define ZLIB_DLL_EXPORT	__declspec(dllexport)
#else
#define ZLIB_DLL_EXPORT
#endif



#ifdef __cplusplus
extern "C" {
#endif

ZLIB_DLL_EXPORT void DummyFunctionForZLib(void)
{
}

#ifdef __cplusplus
}
#endif
