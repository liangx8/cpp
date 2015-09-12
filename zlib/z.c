#include <stdio.h>
//#include <locale.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384

int def(FILE *src, FILE *dst, int level){
  int ret,flush;
  unsigned have;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  ret = deflateInit(&strm,level);
  if (ret != Z_OK) return ret;
  do {
	strm.avail_in = fread(in,1,CHUNK,src);
	if(ferror(src)){
	  (void)deflateEnd(&strm);
	  return Z_ERRNO;
	}
	flush = feof(src) ? Z_FINISH : Z_NO_FLUSH;
	strm.next_in = in;
	do {
	  strm.avail_out = CHUNK;
	  strm.next_out=out;
	  ret=deflate(&strm,flush);
	  assert(ret != Z_STREAM_ERROR);
	  have=CHUNK - strm.avail_out;
	  if(fwrite(out,1,have,dst) != have || ferror(dst)){
		(void)deflateEnd(&strm);
		return Z_ERRNO;
	  }
	} while(strm.avail_out == 0);
	assert(strm.avail_in == 0);
	  
  }while (flush != Z_FINISH);
  assert(ret == Z_STREAM_END);
  (void)deflateEnd(&strm);
  return Z_OK;
}

int inf(FILE *src,FILE *dst){
  int ret;
  unsigned have;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
  /* allocate inflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;
  ret = inflateInit(&strm);
  if (ret != Z_OK)
	return ret;
  /* decompress until deflate stream ends or end of file */
  do {
	strm.avail_in = fread(in, 1, CHUNK, src);
	if (ferror(src)) {
	  (void)inflateEnd(&strm);
	  return Z_ERRNO;
	}
	if (strm.avail_in == 0)
	  break;
	strm.next_in = in;
	/* run inflate() on input until output buffer not full */
	do {
	  strm.avail_out = CHUNK;
	  strm.next_out = out;
	  ret = inflate(&strm, Z_NO_FLUSH);
	  assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
	  switch (ret) {
	  case Z_NEED_DICT:
		ret = Z_DATA_ERROR;     /* and fall through */
	  case Z_DATA_ERROR:
	  case Z_MEM_ERROR:
		(void)inflateEnd(&strm);
		return ret;
	  }

	  have = CHUNK - strm.avail_out;
	  if (fwrite(out, 1, have, dst) != have || ferror(dst)) {
		(void)inflateEnd(&strm);
		return Z_ERRNO;
	  }
	} while (strm.avail_out == 0);
	/* done when inflate() says it's done */
  } while (ret != Z_STREAM_END);
  /* clean up and return */
  (void)inflateEnd(&strm);
  return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

/* report a zlib or i/o error */
 void zerr(int ret)
 {
   fputs("zpipe: ", stderr);
   switch (ret) {
   case Z_ERRNO:
	 if (ferror(stdin))
	   fputs("error reading stdin\n", stderr);
	 if (ferror(stdout))
	   fputs("error writing stdout\n", stderr);
	 break;
   case Z_STREAM_ERROR:
	 fputs("invalid compression level\n", stderr);
	 break;
   case Z_DATA_ERROR:
	 fputs("invalid or incomplete deflate data\n", stderr);
	 break;
   case Z_MEM_ERROR:
	 fputs("out of memory\n", stderr);
	 break;
   case Z_VERSION_ERROR:
	 fputs("zlib version mismatch!\n", stderr);
   }
 }
 /* compress or decompress from stdin to stdout */
 int main(int argc, char **argv)
 {
   int ret;

   /* avoid end-of-line conversions */
   SET_BINARY_MODE(stdin);
   SET_BINARY_MODE(stdout);

   /* do compression if no arguments */
   if (argc == 1) {
	 ret = def(stdin, stdout, Z_DEFAULT_COMPRESSION);
	 if (ret != Z_OK)
	   zerr(ret);
	 return ret;
   }

   /* do decompression if -d specified */
   else if (argc == 2 && strcmp(argv[1], "-d") == 0) {
	 ret = inf(stdin, stdout);
	 if (ret != Z_OK)
	   zerr(ret);
	 return ret;
   }

   /* otherwise, report usage */
   else {
	 fputs("zpipe usage: zpipe [-d] < source > dest\n", stderr);
	 return 1;
   }
 }
