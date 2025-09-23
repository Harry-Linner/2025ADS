# 定义主程序的名称和所有源文件
add_executable(tree_analyzer
    main.c
    BST.c
    AVLTree.c
    splay.c
)

target_include_directories(tree_analyzer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(tree_analyzer PRIVATE
    -x c
)