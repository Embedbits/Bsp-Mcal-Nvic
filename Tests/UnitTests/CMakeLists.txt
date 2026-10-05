#======================== Unit tests configuration ============================#
# Unit tests of Nvic module. Registered in module CMakeLists.txt by
# UnitTesting_AddPath() and created by UnitTesting_Generate() in "UnitTest"
# build (see EmBi_Platform/CMake/UnitTesting/README.md).

get_filename_component(NVIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
get_filename_component(BSP_DIR  "${NVIC_DIR}/../.." ABSOLUTE)

UnitTesting_Add_Test(
    NAME            Nvic
    TEST_SOURCE     Test_Nvic.c
    UUT_SOURCES     ${NVIC_DIR}/Nvic.c
    MOCK_HEADERS    ${BSP_DIR}/Startup/StartUp_Port.h
    INCLUDE_DIRS    ${NVIC_DIR}
                    ${BSP_DIR}/Startup
    LINK_LIBS       Ral_Lib
)
