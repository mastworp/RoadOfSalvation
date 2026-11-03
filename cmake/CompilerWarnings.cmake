add_library(warnings INTERFACE)
add_library(RoadOfSalvation::warnings ALIAS warnings)

target_compile_options(warnings
                       INTERFACE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wconversion;-Wnon-virtual-dtor;-Woverloaded-virtual>
                                 $<$<CXX_COMPILER_ID:MSVC>:/W4>)
