# PNG Web Service 
A simple PNG web service to hide and extract files from a PNG file 
<!-- ![alt text](https://imgur.com/a/XL3IWhI) -->
<img src=https://imgur.com/pdwe4Sq.png/ width="750px">

***
## Usage 
Launch the service by starting the flask application:
- Windows: `py -m flask run`
- macOS: `python3 -m flask run` 
***
## How it works
To see the 'chunks' of data stored in the PNG file, run `./png-analyze <filename>` from the microservice 
### Example output of `./png-analyze sample.png`
```
PNG Header: OK
Chunk: IHDR (13 bytes of data)
Chunk: SRGB (1 bytes of data)
Chunk: eXIf (268 bytes of data)
Chunk: pHYs (9 bytes of data)
Chunk: iTXt (8962 bvtes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (4578 bytes of data)
Chunk: IEND (0 bytes of data)
```
To manually insert a file into a PNG, run `./png-hide <pngSrcFilename> <filenameToHide> <pngDstFilename>`
### Example output of `./png-analyze sample.png` with hidden file
``` 
PNG Header: OK
Chunk: IHDR (13 bytes of data)
Chunk: SRGB (1 bytes of data)
Chunk: eXIf (268 bytes of data)
Chunk: pHYs (9 bytes of data)
Chunk: file (20415 bytes of data)
Chunk: iTXt (8962 bvtes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (16384 bytes of data)
Chunk: IDAT (4578 bytes of data)
Chunk: IEND (0 bytes of data)
```
The byte data of the inserted file is stored in the 'file' chunk of the PNG. 
To extract the data back from the inserted file, run `./png-extract <pngFilename> <dstFilename>` 


