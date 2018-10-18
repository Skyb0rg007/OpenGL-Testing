function! AddToInclude(dir)
    let g:ale_c_gcc_options .= ' -I' . a:dir
    let &path .= ',' . a:dir
endfunction

let ale_c_gcc_options='-Wall -std=c11 -pedantic -Wvla -Winline -D_REENTRANT -DRESOURCE_DIR=""'
call AddToInclude("/usr/include/SDL2")
call AddToInclude("include")
call AddToInclude("libs/cglm/include")
call AddToInclude("libs/structures")
call AddToInclude("libs/klib/include")

command! -nargs=0 Cmake !cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug
command! -nargs=0 CmakeFinal !cmake -H. -Bfinal-build -DCMAKE_BUILD_TYPE=Release
set makeprg=cmake\ --build\ build\ --parallel\ 4\ --
nnoremap <F5> :make!<CR>

augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h set ft=c
augroup END

