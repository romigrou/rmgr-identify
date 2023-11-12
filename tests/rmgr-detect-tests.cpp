#include <cctype>
#include <cstdio>
#include <cstdlib>


bool rmgr_detect_compiler_tests();
bool rmgr_detect_runtime_tests();


int main()
{
    bool success = true;
    success = rmgr_detect_compiler_tests() && success;
    success = rmgr_detect_runtime_tests()  && success;

    if (success)
    {
        printf("Success!\n");
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


bool check_version_numbers(const char* v1, const char* v2)
{
    while (*v1 && *v1==*v2)
    {
        ++v1;
        ++v2;
    }

    // It's OK for numbers to differ iff one is more detailed than the other one
    return (!*v1 && !isdigit(*v2)) || (!*v2 && !isdigit(*v1));
}
