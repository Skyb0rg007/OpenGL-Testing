let ale_c_gcc_options='-Wall -std=c11 -pedantic
            \ -I./include -D_REENTRANT -DRESOURCE_DIR=""'
set makeprg=cmake\ --build\ build\ --parallel\ 4\ --
command -nargs=1 Cmake !cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=<args>
