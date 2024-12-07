#include "rmgr-identify-tests.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace std;


static bool check_version_numbers(const char* v1, const char* v2)
{
    if (!*v1 || !*v2)
        return false;

    while (*v1 && *v1==*v2)
    {
        ++v1;
        ++v2;
    }

    // It's OK for numbers to differ iff one is more detailed than the other one
    return (!*v1 && !std::isdigit(*v2)) || (!*v2 && !std::isdigit(*v1));
}


static bool rmgr_identify_compiler_tests()
{
    bool success = true;

    const char* frontendName;
    unsigned frontendVersionMajor, frontendVersionMinor, frontendVersionPatch;
    rmgr_identify_compiler_frontend(frontendName, frontendVersionMajor, frontendVersionMinor, frontendVersionPatch);

    const char* backendName;
    unsigned backendVersionMajor, backendVersionMinor, backendVersionPatch;
    rmgr_identify_compiler_backend(backendName, backendVersionMajor, backendVersionMinor, backendVersionPatch);

    const char* variantName;
    unsigned variantVersionMajor, variantVersionMinor, variantVersionPatch;
    const bool variantIdentified = rmgr_identify_compiler_variant(variantName, variantVersionMajor, variantVersionMinor, variantVersionPatch);

    char frontendVersion[64];
    snprintf(frontendVersion, sizeof(frontendVersion), "%u.%u.%u", frontendVersionMajor, frontendVersionMinor, frontendVersionPatch);

    char backendVersion[64];
    snprintf(backendVersion, sizeof(backendVersion), "%u.%u.%u", backendVersionMajor, backendVersionMinor, backendVersionPatch);

    char variantVersion[64] = {};
    if (variantIdentified)
    {
        snprintf(variantVersion, sizeof(variantVersion), "%u.%u.%u", variantVersionMajor, variantVersionMinor, variantVersionPatch);
    }

    printf("Detected front-end: %s %s\n",   frontendName, frontendVersion);
    printf("Expected front-end: %s %s\n\n", EXPECTED_FRONTEND, EXPECTED_FRONTEND_VERSION);
    printf("Detected back-end:  %s %s\n",   backendName, backendVersion);
    printf("Expected back-end:  %s %s\n\n", EXPECTED_BACKEND, EXPECTED_BACKEND_VERSION);
#ifdef EXPECTED_VARIANT
    printf("Detected variant:   %s %s\n",   variantName, variantVersion);
    printf("Expected variant:   %s %s\n\n", EXPECTED_VARIANT, EXPECTED_VARIANT_VERSION);
#endif

    if (strcmp(frontendName, EXPECTED_FRONTEND) != 0)
    {
        fprintf(stderr, "Front-end mismatch\n");
        success = false;
    }
    if (!check_version_numbers(frontendVersion, EXPECTED_FRONTEND_VERSION))
    {
        fprintf(stderr, "Front-end version mismatch\n");
        success = false;
    }

    if (strcmp(backendName, EXPECTED_BACKEND) != 0)
    {
        fprintf(stderr, "Back-end mismatch\n");
        success = false;
    }
    if (!check_version_numbers(backendVersion, EXPECTED_BACKEND_VERSION))
    {
        fprintf(stderr, "Back-end version mismatch\n");
        success = false;
    }

#ifdef EXPECTED_VARIANT
    if (strcmp(variantName, EXPECTED_VARIANT) != 0)
    {
        fprintf(stderr, "Variant mismatch\n");
        success = false;
    }
    if (strcmp(variantVersion, EXPECTED_VARIANT_VERSION) != 0)
    {
        fprintf(stderr, "Variant version mismatch\n");
        success = false;
    }
#endif

    return success;
}


static bool rmgr_identify_c_runtime_tests()
{
    bool success = true;

    const char* crtName;
    unsigned crtVersionMajor, crtVersionMinor, crtVersionPatch;
    rmgr_identify_c_runtime(&crtName, &crtVersionMajor, &crtVersionMinor, &crtVersionPatch);

    char crtVersion[64];
    snprintf(crtVersion, sizeof(crtVersion), "%u.%u.%u", crtVersionMajor, crtVersionMinor, crtVersionPatch);

    printf("Detected C runtime:   %s %s\n", crtName, crtVersion);
    printf("Expected C runtime:   %s\n\n", EXPECTED_CRT);

    if (strcmp(crtName, EXPECTED_CRT) != 0)
    {
        fprintf(stderr, "C library mismatch\n");
        success = false;
    }

    return success;
}



static bool rmgr_identify_cpp_runtime_tests()
{
    bool success = true;

    const char* cpprtName;
    unsigned cpprtVersionMajor, cpprtVersionMinor, cpprtVersionPatch;
    rmgr_identify_cpp_runtime(cpprtName, cpprtVersionMajor, cpprtVersionMinor, cpprtVersionPatch);

    char cpprtVersion[64];
    snprintf(cpprtVersion, sizeof(cpprtVersion), "%u.%u.%u", cpprtVersionMajor, cpprtVersionMinor, cpprtVersionPatch);

    printf("Detected C++ runtime: %s %s\n", cpprtName, cpprtVersion);
    printf("Expected C++ runtime: %s\n\n", EXPECTED_CPPRT);

    if (strcmp(cpprtName, EXPECTED_CPPRT) != 0)
    {
        fprintf(stderr, "C++ library mismatch\n");
        success = false;
    }

    return success;
}


int main()
{
    bool success = true;
    success = rmgr_identify_compiler_tests()     && success;
    success = rmgr_identify_c_runtime_tests()    && success;
    success = rmgr_identify_cpp_runtime_tests()  && success;

    if (success)
    {
        std::printf("Success!\n");
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


