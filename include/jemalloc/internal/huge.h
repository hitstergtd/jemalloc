/******************************************************************************/
#ifdef JEMALLOC_H_TYPES

#endif /* JEMALLOC_H_TYPES */
/******************************************************************************/
#ifdef JEMALLOC_H_STRUCTS

#endif /* JEMALLOC_H_STRUCTS */
/******************************************************************************/
#ifdef JEMALLOC_H_EXTERNS

void	*huge_malloc(tsd_t *tsd, arena_t *arena, size_t usize, bool zero,
    tcache_t *tcache);
void	*huge_palloc(tsd_t *tsd, arena_t *arena, size_t usize, size_t alignment,
    bool zero, tcache_t *tcache);
bool	huge_ralloc_no_move(tsd_t *tsd, void *ptr, size_t oldsize,
    size_t usize_min, size_t usize_max, bool zero);
void	*huge_ralloc(tsd_t *tsd, arena_t *arena, void *ptr, size_t oldsize,
    size_t usize, size_t alignment, bool zero, tcache_t *tcache);
#ifdef JEMALLOC_JET
typedef void (huge_dalloc_junk_t)(tsd_t *, void *, size_t);
extern huge_dalloc_junk_t *huge_dalloc_junk;
#endif
void	huge_dalloc(tsd_t *tsd, void *ptr, tcache_t *tcache);
arena_t	*huge_aalloc(const void *ptr);
size_t	huge_salloc(tsd_t *tsd, const void *ptr);
prof_tctx_t	*huge_prof_tctx_get(tsd_t *tsd, const void *ptr);
void	huge_prof_tctx_set(tsd_t *tsd, const void *ptr, prof_tctx_t *tctx);
void	huge_prof_tctx_reset(tsd_t *tsd, const void *ptr);

#endif /* JEMALLOC_H_EXTERNS */
/******************************************************************************/
#ifdef JEMALLOC_H_INLINES

#endif /* JEMALLOC_H_INLINES */
/******************************************************************************/
