#ifdef  MEMORY_WATCH
#define DIAGNOSTIC_INC
#include <diagnostic.h>

typedef struct {
  void       *pointer;
  size_t      size;
  const char *filename;
  int         line;
} Allocation;

// Memory table global variables
size_t      _mem_total_size = 0;
int         _mem_table_size = 0;
int         _mem_table_cap  = 2048;
Allocation *_mem_table      = NULL;

#ifdef WIN
/******************************************************************************/
static void *reallocarray(void *ptr, int length, size_t element_size)
{
  return realloc(ptr, length * element_size);
}
#endif

////////////////////////////////////////////////////////////////////////////////
void *__malloc(size_t size, const char *filename, int line)
{
  if (!_mem_table && !(_mem_table = calloc(_mem_table_cap, sizeof(Allocation)))) {
    fprintf(stderr, "Critical memory allocation failure!");
    exit(-1);
  }

  void *mem = malloc(size);

  if (mem) {
    _mem_total_size += size;
    
    for (int i = 0; i < _mem_table_size; i++) {
      Allocation *alloc = &_mem_table[i];

      if (!alloc->pointer) {
        alloc->pointer  = mem;
        alloc->size     = size;
        alloc->filename = filename;
        alloc->line     = line;

        return mem;
      }
    }

    if (_mem_table_size >= _mem_table_cap) {
      _mem_table_cap <<= 1;
      _mem_table = reallocarray(_mem_table, _mem_table_cap, sizeof(Allocation));
    }

    Allocation *alloc = &_mem_table[_mem_table_size++];

    if (alloc) {
      alloc->pointer  = mem;
      alloc->size     = size;
      alloc->filename = filename;
      alloc->line     = line;
    }
  }

  return mem;
}

////////////////////////////////////////////////////////////////////////////////
void *__calloc(size_t nmemb, size_t size, const char *filename, int line)
{
  void *ptr = __malloc(nmemb * size, filename, line);

  if (ptr)
  {
    memset(ptr, 0, nmemb * size);
  }

  return ptr;
}

////////////////////////////////////////////////////////////////////////////////
void __free(void *ptr)
{
  size_t size = 0;

  for (int i = 0; i < _mem_table_size; i++) {
    Allocation *alloc = &_mem_table[i];

    if (alloc->pointer == ptr) {
      size = alloc->size;

      alloc->pointer  = NULL;
      alloc->size     = 0;
      alloc->filename = NULL;
      alloc->line     = 0;

      break;
    }
  }

  _mem_total_size -= size;

  free(ptr);
}

////////////////////////////////////////////////////////////////////////////////
void *__realloc(void *ptr, size_t size, const char *filename, int line)
{
  for (int i = 0; i < _mem_table_size; i++) {
    Allocation *alloc = &_mem_table[i];

    if (alloc->pointer == ptr) {
      _mem_total_size += size - alloc->size;

      alloc->pointer  = realloc(ptr, size);
      alloc->size     = size;
      alloc->filename = filename;
      alloc->line     = line;

      return alloc->pointer;
    }
  }

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *__reallocarray(void *ptr, size_t nmemb, size_t size, const char *filename, int line)
{
  return __realloc(ptr, nmemb * size, filename, line);
}

////////////////////////////////////////////////////////////////////////////////
size_t __memuse()
{
    return _mem_total_size;
}

////////////////////////////////////////////////////////////////////////////////
void __end()
{
  for (int i = 0; i < _mem_table_size; i++) {
    Allocation *alloc = &_mem_table[i];

    if (alloc->pointer) {
      fprintf(stderr, "%s(%d): A block of size %ld was allocated but never recovered!\n",
                      alloc->filename,
                      alloc->line,
                      (long)alloc->size);
    }
  }

  free(_mem_table);
}

#endif