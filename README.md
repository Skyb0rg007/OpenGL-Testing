## Learning SDL + OpenGL

I'm going through tutorials, and learning how OpenGl works
as well as basic input controls through SDL2

### Help with ASAN

Address Sanitizer is amazing, but I guess it may just not work with GLEW

Example program run:

    ./src/main


    ==30498==ERROR: LeakSanitizer: detected memory leaks
    
    Direct leak of 8192 byte(s) in 2 object(s) allocated from:
        #0 0x7fcc559a6c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7fcc28ac8cad  (<unknown module>)
    
    Direct leak of 152 byte(s) in 2 object(s) allocated from:
        #0 0x7fcc559a7088 in __interceptor_realloc (/lib64/libasan.so.5+0xef088)
        #1 0x7fcc53566829  (/lib64/libX11.so.6+0x56829)
    
    Direct leak of 17 byte(s) in 1 object(s) allocated from:
        #0 0x7fcc559a6c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7fcc535703a8 in _XlcDefaultMapModifiers (/lib64/libX11.so.6+0x603a8)
    
    Indirect leak of 1520 byte(s) in 19 object(s) allocated from:
        #0 0x7fcc559a6e50 in calloc (/lib64/libasan.so.5+0xeee50)
        #1 0x7fcc535667fe  (/lib64/libX11.so.6+0x567fe)
    
    Indirect leak of 128 byte(s) in 16 object(s) allocated from:
        #0 0x7fcc559a6c48 in malloc (/lib64/libasan.so.5+0xeec48)
        #1 0x7fcc53567162  (/lib64/libX11.so.6+0x57162)
    
    Indirect leak of 80 byte(s) in 3 object(s) allocated from:
        #0 0x7fcc559a7088 in __interceptor_realloc (/lib64/libasan.so.5+0xef088)
        #1 0x7fcc5356707f  (/lib64/libX11.so.6+0x5707f)
    
    SUMMARY: AddressSanitizer: 10089 byte(s) leaked in 43 allocation(s).

If there's any way to fix this lmk
