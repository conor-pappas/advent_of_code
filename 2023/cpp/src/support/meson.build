support_include = include_directories('.')
support_files = files([
    'input.cpp'
])

support_lib = library(
    'support',
    sources: support_files,
    include_directories: support_include
)

dep_support_lib = declare_dependency(link_with: support_lib, include_directories: support_include)
