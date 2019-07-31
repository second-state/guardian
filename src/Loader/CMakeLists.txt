add_library(Loader STATIC
    JsonLoader.cpp
    JsonLoader.h
)
target_link_libraries(Loader Common)
set_target_properties(Loader PROPERTIES LINKER_LANGUAGE CXX)
