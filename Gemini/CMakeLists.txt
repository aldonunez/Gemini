add_library(Gemini
    AlgolyParser.cpp
    BinderVisitor.cpp
    Compiler.cpp
    Disassembler.cpp
    FolderVisitor.cpp
    LispyParser.cpp
    Machine.cpp
    pch.cpp
    Syntax.cpp
    Verify.cpp
    )

set(GEMINI_PUBLIC_HEADERS
    AlgolyParser.h
    Compiler.h
    Disassembler.h
    LangCommon.h
    LispyParser.h
    Machine.h
    Syntax.h
    VmCommon.h
)

set_target_properties(Gemini PROPERTIES PUBLIC_HEADER "${GEMINI_PUBLIC_HEADERS}")

install(TARGETS Gemini
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
)
