This repository is a small C file that prints all the chunks in a PNG file. It was written for fun in the space of an hour, for the purpose of being able to effectively look inside PNG files, which are very common in computing.

This project demonstrates that the PNG container format is a very simple format, which requires minimal code to parse. A PNG file consists of an 8 byte constant as a header, and a sequence of chunks consisting of fixed size fields and raw byte sequences.

While this does not actually decompress the image data, nor does it print the contents of each chunk, the compression algorithm is quite simple, and the contents of other chunks are usually specified as binary fields, with no complex encoding to complicate a tool with more features than this one.
