let ale_c_gcc_options='-Wall -std=c11 -pedantic
            \ -I./include -D_REENTRANT -DRESOURCE_DIR=""'
let &path.="include"

command -nargs=1 Cmake !cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=<args>
set makeprg=cmake\ --build\ build\ --parallel\ 4\ --
nnoremap <F4> :make!<CR>

augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h set ft=c
augroup END
