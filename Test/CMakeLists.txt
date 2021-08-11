add_executable(Test
    TestMain.cpp
    TestAlgoly.cpp
    TestAlgolyCopyArray.cpp
    TestAlgolyEnum.cpp
    TestAlgolyMultiArray.cpp
    TestAlgolyNegativeLimits.cpp
    TestAlgolyPassRef.cpp
    TestAlgolyRecord.cpp
    TestAlgolyStack.cpp
    TestBase.cpp
    TestLispy.cpp
)

target_link_libraries(Test PUBLIC Gemini)

target_include_directories(Test PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Gemini"
)

add_test(Test Test)
