#===================== Integration tests configuration ========================#
# Integration tests of Nvic module on target (Nucleo board). Registered in module
# CMakeLists.txt by IntegrationTesting_AddPath() and created by
# IntegrationTesting_Generate() in "IntegrationTest" build
# (see EmBi_Platform/CMake/IntegrationTesting/README.md).

IntegrationTesting_Add_Test(
    NAME            Nvic
    TEST_SOURCE     ItTest_Nvic.c
    LINK_LIBS       Nvic_Lib
                    Rcc_Lib             # SysTick interval, reset source
    TIMEOUT         60
)
