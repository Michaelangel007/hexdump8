# hexdump8

Are you fed up with hexdump programs that are over-complicated and don't do what you need?

Then `hexdump8` might be what you are looking for!

Features:

* 16-bit address
  * Change the starting address
  * Supports `0x` or `$` prefix for hex numbers
  * Can use `$` as starting offset by itself
* 8 byte output
* optional text column
  * None
  * ASCII
  * HTML friendly -- marked up chars `<`, `>`, `&`
* Non-bloated C++ code

# Examples

Text column ommitted

```asm
hexdump8 -none Makefile
0000:68 65 78 64 75 6D 70 38
0008:3A 20 68 65 78 64 75 6D
0010:70 38 2E 63 70 70 0A 09
0018:67 2B 2B 20 2D 57 61 6C
0020:6C 20 2D 57 65 78 74 72
0028:61 20 24 3C 20 2D 6F 20
0030:24 40 0A 09 63 70 20 24
0038:40 20 2F 75 73 72 2F 6C
0040:6F 63 61 6C 2F 62 69 6E
0048:0A 0A 
```

Text Column in ASCII

```asm
hexdump8 -ascii Makefile
0000:68 65 78 64 75 6D 70 38 |hexdump8|
0008:3A 20 68 65 78 64 75 6D |: hexdum|
0010:70 38 2E 63 70 70 0A 09 |p8.cpp..|
0018:67 2B 2B 20 2D 57 61 6C |g++ -Wal|
0020:6C 20 2D 57 65 78 74 72 |l -Wextr|
0028:61 20 24 3C 20 2D 6F 20 |a $< -o |
0030:24 40 0A 09 63 70 20 24 |$@..cp $|
0038:40 20 2F 75 73 72 2F 6C |@ /usr/l|
0040:6F 63 61 6C 2F 62 69 6E |ocal/bin|
0048:0A 0A                   |..      |
```

Text Column in HTML

```asm
hexdump8 -html Makefile
0000:68 65 78 64 75 6D 70 38 |hexdump8|
0008:3A 20 68 65 78 64 75 6D |: hexdum|
0010:70 38 2E 63 70 70 0A 09 |p8.cpp..|
0018:67 2B 2B 20 2D 57 61 6C |g++ -Wal|
0020:6C 20 2D 57 65 78 74 72 |l -Wextr|
0028:61 20 24 3C 20 2D 6F 20 |a $&lt; -o |
0030:24 40 0A 09 63 70 20 24 |$@..cp $|
0038:40 20 2F 75 73 72 2F 6C |@ /usr/l|
0040:6F 63 61 6C 2F 62 69 6E |ocal/bin|
0048:0A 0A                   |..      |
```

ASCII with starting address of 2000

```asm
hexdump8 -ascii -@ 0x2000 Makefile
2000:68 65 78 64 75 6D 70 38 |hexdump8|
2008:3A 20 68 65 78 64 75 6D |: hexdum|
2010:70 38 2E 63 70 70 0A 09 |p8.cpp..|
2018:67 2B 2B 20 2D 57 61 6C |g++ -Wal|
2020:6C 20 2D 57 65 78 74 72 |l -Wextr|
2028:61 20 24 3C 20 2D 6F 20 |a $< -o |
2030:24 40 0A 09 63 70 20 24 |$@..cp $|
2038:40 20 2F 75 73 72 2F 6C |@ /usr/l|
2040:6F 63 61 6C 2F 62 69 6E |ocal/bin|
2048:0A 0A                   |..      |
```

Can use `$` as a command line argument by itself

```asm
hexdump8 -ascii \$1234 Makefile
1234:68 65 78 64 75 6D 70 38 |hexdump8|
123C:3A 20 68 65 78 64 75 6D |: hexdum|
1244:70 38 2E 63 70 70 0A 09 |p8.cpp..|
124C:67 2B 2B 20 2D 57 61 6C |g++ -Wal|
1254:6C 20 2D 57 65 78 74 72 |l -Wextr|
125C:61 20 24 3C 20 2D 6F 20 |a $< -o |
1264:24 40 0A 09 63 70 20 24 |$@..cp $|
126C:40 20 2F 75 73 72 2F 6C |@ /usr/l|
1274:6F 63 61 6C 2F 62 69 6E |ocal/bin|
127C:0A 0A                   |..      |
```

