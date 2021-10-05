add_library(geminivm
    AlgolyParser.cpp
    BinderVisitor.cpp
    Compiler.cpp
    Disassembler.cpp
    FolderVisitor.cpp
    LangCommon.cpp
    LispyParser.cpp
    Machine.cpp
    pch.cpp
    Syntax.cpp
    Verify.cpp
    )

set(GEMINI_PUBLIC_HEADERS
    AlgolyParser.h
    Common.h
    Compiler.h
    Disassembler.h
    LangCommon.h
    LispyParser.h
    Machine.h
    Syntax.h
)

set_target_properties(geminivm PROPERTIES PUBLIC_HEADER "${GEMINI_PUBLIC_HEADERS}")

install(TARGETS geminivm
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
)
