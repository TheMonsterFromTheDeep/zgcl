#ifndef ZDATA_H
#define ZDATA_H

extern size_t zsize(const void*);
extern size_t zalloc(const void*);
extern size_t *zsizeptr(void*);
extern size_t *zallocptr(void*);

extern void *znewt(size_t,size_t);
extern void *zrealloct(void*,size_t,size_t);

extern void zfree(void*);

#define znew(size,type) (znewt((size),sizeof(type)))
#define zrealloc(struc,size) (zrealloct((struc),(size),sizeof(struc)))

#endif /* Header guard */
