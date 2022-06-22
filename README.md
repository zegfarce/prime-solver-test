# this is just a dumb thing i made for fun

### Running

so in case you actually want to run the thing, you can probably just do
```c
cc prime_test.c -o prime_test
./prime_test
```
if you're on a POSIX.1b-compliant system  
i tested it with:
* `gcc`
  * `--std=c89`
  * `-ansi`
  * `--std=c99`
* [M/o/Vfuscator](https://github.com/xoreaxeaxeax/movfuscator) (for the funny)

it should work in most compilers, 
one problem you may encounter is with `CLOCK_MONOTONIC`, which is technically in POSIX, but not required, but is implemented by practically everyone anyway, but may not be implemented by your POSIX system for whatever reason  

windows users who want to run this may have to find a POSIX environment like msys2, mingw, wsl, cygwin, or the countless other alternatives available  
or edit the file to use windows calls i don't care  

### Background

so i was watching a [cool video by jan Misali about base six](https://www.youtube.com/watch?v=wXeX_XKSNlc) and about [5:20 in the video](https://www.youtube.com/watch?v=wXeX_XKSNlc?t=320) he mentions quick and dirty methods to figure out if a number is prime or not  
so just for fun i implemented them in C (specifically for base six), and it turns out they're about 33% faster than just running through all the odd numbers to find factors!  
idk maybe this will be interesting to someone else too  
i did a minimal amount of testing to see if other bases would be more optimal, but in my quick tests it seems base six may be the most optimal here, but i'm not totally sure if that's just because of my code

### License

the license is 0BSD, have fun  
check LICENSE if you want  
or don't  
i dont care