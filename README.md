# Deflate Compressor

A C++ implementation of the DEFLATE compression algorithm (RFC 1951),
built from scratch as a low-level systems programming exercise.

## Features

- Full DEFLATE pipeline: LZ77 + Huffman coding
- Hash table for LZ77 string matching — reduces complexity from O(n²) 
  to O(n), making compression of multi-MB files practical
- Sliding window implementation for memory-efficient processing of 
  large files
- Purely bit-level I/O — no string manipulation
- Length and distance codes follow RFC 1951 specification
- Both compression and decompression implemented

## Performance

Compression ratio on text files is comparable to 7-Zip.
Binary files are processed without errors but yield minimal 
compression, which is expected behavior for already-dense data.

## Build & Run

Open the solution in Visual Studio and build in Release or Debug mode.

Run the executable — a console menu will appear:
[0] Choose custom file path
[1] Compress file
[2] Decompress file
[3] Compress and decompress file
[4] Exit

A default input path is shown at the bottom of the menu.
To use a custom file, select [0] and provide the full path —
the compressed and decompressed output files will appear 
in the same folder as the input file.

## Project Structure

- `kompresja.cpp / .h` — compression logic
- `dekompresja.cpp / .h` — decompression logic  
- `algorytmKodujacy.cpp / .h` — LZ77 encoding algorithm
- `funkcje_pomocnicze_AK/DK` — helper functions for encoding/decoding
- `Tabele_RFC_1951.cpp / .h` — RFC 1951 length and distance tables
- `UI.cpp / .h` — console user interface

## Implementation Notes

This was an early project — the code is procedural rather than 
object-oriented. The focus was on correctly implementing the 
algorithm at the bit level, including the full RFC 1951 
length/distance table.

A future rewrite would introduce proper class structure and unit 
tests for individual compression stages.
