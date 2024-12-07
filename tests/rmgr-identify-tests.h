#ifndef RMGR_IDENTIFY_TESTS_H
#define RMGR_IDENTIFY_TESTS_H


#ifdef __cplusplus
void rmgr_identify_compiler_frontend(const char*& name, unsigned& major, unsigned& minor, unsigned& patch);
void rmgr_identify_compiler_backend( const char*& name, unsigned& major, unsigned& minor, unsigned& patch);
bool rmgr_identify_compiler_variant( const char*& name, unsigned& major, unsigned& minor, unsigned& patch);

void rmgr_identify_cpp_runtime(const char*& name, unsigned& major, unsigned& minor, unsigned& patch);

extern "C"
#endif
void rmgr_identify_c_runtime(const char** name, unsigned* major, unsigned* minor, unsigned* patch);


#endif // RMGR_IDENTIFY_TESTS_H
