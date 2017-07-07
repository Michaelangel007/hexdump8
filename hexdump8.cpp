/*
    Michael Pohoreski
    https://github.com/Michaelangel007/hexdump8

Compile:
    g++ -Wall -Wextra hexdump8.cpp -o hexdump8

Usage:
    hexdump8 [-none | -ascii | -html] [-@ ####] filename

Example:
    hexdump8 -html -@ 0x1B00 dos33.bin
*/

// Includes
    #include <stdio.h>
    #include <string.h>

// Globals
    int gnVersion = 6;

    const size_t   _K = 1024;
    const size_t _64K = 64 * _K;
    unsigned char gaBuffer[ _64K ];

    enum TextOutput_e
    {
         TEXT_NONE = 0
        ,TEXT_ASCII
        ,TEXT_HTML
        ,NUM_TEXT_OUTPUT
        ,OFFSET_AT = NUM_TEXT_OUTPUT
    };

    const char *gaTextOutput[] = 
    {
         "none"
        ,"ascii"
        ,"html"
        ,"@"
    };

    int giTextOutput = TEXT_NONE;

    const char NIB2ASC[] = "0123456789ABCDEF";

    const char BIN2ASC[] = // 7-bit ASCII table with Control Chars as '.'
      // 0123456789ABCDEF  
        "................" // 0x00..0x0F
        "................" // 0x10..0x1F
        " !\"#$%&'()*+,-./"// 0x20..0x2F
        "0123456789:;<=>?" // 0x30..0x3F
        "@ABCDEFGHIJKLMNO" // 0x40..0x4F
        "PQRSTUVWXYZ[\\]^_"// 0x50..0x5F
        "`abcdefghijklmno" // 0x60..0x6F
        "pqrstuvwxyz{|}~." // 0x70..0x7F
    ;

// ========================================================================
void asciidump( const unsigned c )
{
    if ((giTextOutput == TEXT_HTML) && ((c == '<') || (c == '>') || (c == '&')))
    {
        if (c == '<') printf( "&lt;" );
        if (c == '>') printf( "&gt;" );
        if (c == '&') printf( "&amp;");
    }
    else
        printf( "%c", c );
}

// ========================================================================
void hexdump( size_t nOffset, size_t nBytes, const unsigned char *pBuffer )
{
    const unsigned char *a   = pBuffer;
    size_t               rem = nBytes & 7;
    size_t               len = nBytes - rem;
    size_t               i   ;

    for( i = 0; i < len; i+= 8, nOffset += 8 )
    {
        printf( "%04X:", (unsigned int) nOffset );

        printf( "%02X %02X %02X %02X %02X %02X %02X %02X",
            a[ i+0 ], a[ i+1 ], a[ i+2 ], a[ i+3 ],
            a[ i+4 ], a[ i+5 ], a[ i+6 ], a[ i+7 ]
        );
        if( giTextOutput == TEXT_ASCII || giTextOutput == TEXT_HTML )
        {
            printf( " |" );
            for( int j = 0; j < 8; j++ )
            {
                unsigned char c = BIN2ASC[ a[ i + j ] & 0x7F ];
                asciidump( c );
            }
            printf( "|" );
        }

        printf( "\n" );
    }

    if( rem )
    {
        printf( "%04X:", (unsigned int) nOffset );
        for( i = 0; i < rem; i++ )
            printf( "%02X ", a[ i + nBytes - rem ] );

        if( giTextOutput == TEXT_ASCII || giTextOutput == TEXT_HTML )
        {
            for( int j = rem; j < 8; j++ )
                printf( "   " );
            printf( "|" );
            for( unsigned j = 0; j < rem; j++ )
            {
                unsigned char c = BIN2ASC[ a[ nBytes - rem + j ] & 0x7F ];
                asciidump( c );
            }
            for( int j = rem; j < 8; j++ )
                printf( " " );
            printf( "|" );
        }
        printf( "\n" );
    }
}

// Convert ASCII in hex to unsigned size_t
// ========================================================================
size_t asc2size( const char *pText )
{
    // Skip hex prefix: 0x
    // Skip hex prefix: $
    if( (pText[0] == '0') && (pText[1] == 'x'))
        pText += 2;
    else
    if( pText[0] == '$')
        pText += 1;

    // Map ASCII to Hex values 0x00..0x0F
    // '0' = 0x30 %0011_0000
    // '9' = 0x39 %0011_1001  +0x09
    // '@' = 0x40 %0100_0000  +0x10
    // 'A' = 0x41 %0100_0001  +0x11
    // 'B' = 0x42 %0100_0010  +0x12
    // 'C' = 0x43 %0100_0011
    // 'D' = 0x44 %0100_0100
    // 'E' = 0x45 %0100_0101
    // 'F' = 0x46 %0100_0110  +0x16
    // 'a' = 0x61 %0110_0001  +0x31
    // 'b' = 0x62 %0110_0010  +0x32
    // 'c' = 0x63 %0110_0011  +0x33
    // 'd' = 0x64 %0110_0100  +0x34
    // 'e' = 0x65 %0110_0101  +0x35
    // 'f' = 0x66 %0110_0110  +0x36
    unsigned char c;
    const unsigned char ASC2HEX_1F[ 0x20 ] = {
        0,10,11,12,13,14,15, 0, // 0x00 .. 0x07:^@^A^B^C^D^E^F^G <- 0x41..0x46
        0, 0, 0, 0, 0, 0, 0, 0, // 0x08 .. 0x0F:^H^I^J^K^L^M^N^O
        0, 1, 2, 3, 4, 5, 6, 7, // 0x10 .. 0x17:^P^Q^R^S^T^U^V^W <- 0x30..0x37
        8, 9, 0, 0, 0, 0, 0, 0  // 0x18 .. 0x1F:^X^Y^Z^[^\^]^^^_ <- 0x38..0x39
//      0, 0, 0, 0, 0, 0, 0, 0, // 0x20 .. 0x27:   ! " # $ % & ' 
//      0, 0, 0, 0, 0, 0, 0, 0, // 0x28 .. 0x2F  ( ) * + , _ . /
//      0, 1, 2, 3, 4, 5, 6, 7, // 0x30 .. 0x37: 0 1 2 3 4 5 6 7
//      8, 9, 0, 0, 0, 0, 0, 0  // 0x38 .. 0x3F: 8 9 : ; < = > ?
    };

    size_t n = 0;
    while( *pText )
    {
        c = ASC2HEX_1F[ *pText & 0x1F ];
        n *= 16; // base 16
        n += c;
        pText++;
    }
    return n;
}

// ========================================================================
int usage()
{
    return printf(
        "hexdump8, Version %d\n"
        "https://github.com/Michaelangel007/hexdump8\n"
        "\n"
        "Usage: hexdump8  [-%s | -%s | -%s]  [-%s HexOffset]  filename\n"
        "\n"
        "   -%-5s  Text output is ommited\n"
        "   -%-5s  Text output in ASCII\n"
        "   -%-5s  Text output in HTML\n"
        "   -%-5s  Display adress starting at this address\n"
        "\n"
        "Text output defaults to: -%s\n"
        "\n"
        "Examples:\n"
        "   hexdump8 -%s        Makefile\n"
        "   hexdump8 -%s       Makefile\n"
        "   hexdump8 -%s -%s 0xA Makefile\n"
        "   hexdump8 -%s    \\$A Makefile\n"
        "\n"
        "ERROR: No binary file to hex dump\n"
        , gnVersion
        , gaTextOutput[ TEXT_NONE  ]
        , gaTextOutput[ TEXT_ASCII ]
        , gaTextOutput[ TEXT_HTML  ]
        , gaTextOutput[ OFFSET_AT  ]

        , gaTextOutput[ TEXT_NONE  ]
        , gaTextOutput[ TEXT_ASCII ]
        , gaTextOutput[ TEXT_HTML  ]
        , gaTextOutput[ OFFSET_AT  ]

        , gaTextOutput[ giTextOutput ]

        // Examples
        , gaTextOutput[ TEXT_NONE  ]
        , gaTextOutput[ TEXT_ASCII ]
        , gaTextOutput[ TEXT_HTML  ]
        , gaTextOutput[ OFFSET_AT  ]

        , gaTextOutput[ TEXT_HTML  ]
    );
}

// ========================================================================
int main( const int nArg, const char *aArg[] )
{
    size_t nOffset = 0;

    /* */ int   iArg = 1;
    const char *pArg = aArg[ iArg ];

    if( nArg < 2 )
        return usage();

    while( iArg < nArg )
    {
        if( pArg[0] == '-' )
        {
            if( strcmp( &pArg[1], gaTextOutput[ OFFSET_AT  ] ) == 0 )
            {
                iArg++;
                if( iArg < nArg )
                {
                    const char *param = aArg[ iArg ]; 
                    nOffset = (size_t) asc2size( param );
                }
                else
                    return usage();
            }
            else
            {
                for( int i = 0; i < NUM_TEXT_OUTPUT; i++ )
                {
                    const char *param   = &pArg[1]; 
                    const char *command = gaTextOutput[ i ];

                    if( strcmp( param, command ) == 0 )
                    {
                        giTextOutput = i;
                        break;
                    }
                }
            }
        }
        else
        if( pArg[0] == '$' )
        {
            nOffset = (size_t) asc2size( pArg ); // pass in '$####'
        }
        else
            break;

        iArg++;
        pArg = aArg[ iArg ];
    }

    FILE *pFile = fopen( pArg, "rb" );
    if( pFile )
    {
        fseek( pFile, 0, SEEK_END );
        size_t nBuffer = ftell( pFile );
        fseek( pFile, 0, SEEK_SET );

        while (nBuffer > 0)
        {
            size_t nBytes = (nBuffer < _64K)
                ? nBuffer
                : _64K;
            fread( gaBuffer, 1, nBytes, pFile );
            hexdump( nOffset, nBytes, gaBuffer );

            nOffset += nBytes;
            nBuffer -= nBytes;
        }

        fclose( pFile );        
    } else
        printf( "ERROR: Couldn't read file: %s\n", pArg );

    return 0;
}
