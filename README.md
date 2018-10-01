## Learning SDL + OpenGL

I'm going through tutorials, and learning how OpenGl works
as well as basic input controls through SDL2

### Help with ASAN

Address Sanitizer is amazing, but I guess it may just not work with GLEW

Example program run:

    $ cmake .. -DCMAKE_BUILD_TYPE=Debug
    $ cmake --build .
    $ ./src/main



    =================================================================
    ==22560==ERROR: LeakSanitizer: detected memory leaks
    
    Direct leak of 8192 byte(s) in 2 object(s) allocated from:
        #0 0x7f38d41b7c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7f38a6ec8cad  (<unknown module>)
    
    -----------------------------------------------------
    Suppressions used:
      count      bytes template
         41       1897 libX11
    -----------------------------------------------------
    
    SUMMARY: AddressSanitizer: 8192 byte(s) leaked in 2 allocation(s).


    $ ./tests/asantest



    =================================================================
    ==22577==ERROR: LeakSanitizer: detected memory leaks
    
    Direct leak of 920 byte(s) in 5 object(s) allocated from:
        #0 0x7f3d6af86e50 in calloc (/lib64/libasan.so.5+0xeee50)
        #1 0x7f3d577b6944  (<unknown module>)
    
    Direct leak of 520 byte(s) in 13 object(s) allocated from:
        #0 0x7f3d6af86c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7f3d577d06e5  (<unknown module>)
    
    Direct leak of 96 byte(s) in 3 object(s) allocated from:
        #0 0x7f3d6af86c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7f3d577c7f12  (<unknown module>)
    
    Direct leak of 24 byte(s) in 1 object(s) allocated from:
        #0 0x7f3d6af86c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7f3d577c9dc1  (<unknown module>)
    
    Indirect leak of 2076 byte(s) in 9 object(s) allocated from:
        #0 0x7f3d6af87088 in __interceptor_realloc (/lib64/libasan.so.5+0xef088)
        #1 0x7f3d577ca23c  (<unknown module>)
    
    Indirect leak of 40 byte(s) in 1 object(s) allocated from:
        #0 0x7f3d6af86c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7f3d577ca487  (<unknown module>)
    
    Indirect leak of 24 byte(s) in 1 object(s) allocated from:
        #0 0x7f3d6af86c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7f3d577c9dc1  (<unknown module>)
    
    Indirect leak of 24 byte(s) in 1 object(s) allocated from:
        #0 0x7f3d6af86c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7f3d577c9e29  (<unknown module>)
    
    -----------------------------------------------------
    Suppressions used:
      count      bytes template
         41       1897 libX11
    -----------------------------------------------------
    
    SUMMARY: AddressSanitizer: 3724 byte(s) leaked in 34 allocation(s).

If there's any way to fix this lmk
