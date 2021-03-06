#define	JEMALLOC_WITNESS_C_
#include "jemalloc/internal/jemalloc_internal.h"

void
witness_init(witness_t *witness, const char *name, witness_rank_t rank,
    witness_comp_t *comp)
{

	witness->name = name;
	witness->rank = rank;
	witness->comp = comp;
}

#ifdef JEMALLOC_JET
#undef witness_lock_error
#define	witness_lock_error JEMALLOC_N(witness_lock_error_impl)
#endif
static void
witness_lock_error(const witness_list_t *witnesses, const witness_t *witness)
{
	witness_t *w;

	malloc_printf("<jemalloc>: Lock rank order reversal:");
	ql_foreach(w, witnesses, link) {
		malloc_printf(" %s(%u)", w->name, w->rank);
	}
	malloc_printf(" %s(%u)\n", witness->name, witness->rank);
	abort();
}
#ifdef JEMALLOC_JET
#undef witness_lock_error
#define	witness_lock_error JEMALLOC_N(witness_lock_error)
witness_lock_error_t *witness_lock_error = JEMALLOC_N(witness_lock_error_impl);
#endif

void
witness_lock(tsd_t *tsd, witness_t *witness)
{
	witness_list_t *witnesses;
	witness_t *w;

	if (tsd == NULL)
		return;
	if (witness->rank == WITNESS_RANK_OMIT)
		return;

	witness_assert_not_owner(tsd, witness);

	witnesses = tsd_witnessesp_get(tsd);
	w = ql_last(witnesses, link);
	if (w != NULL && w->rank >= witness->rank && (w->comp == NULL ||
	    w->comp != witness->comp || w->comp(w, witness) > 0))
		witness_lock_error(witnesses, witness);

	ql_elm_new(witness, link);
	ql_tail_insert(witnesses, witness, link);
}

void
witness_unlock(tsd_t *tsd, witness_t *witness)
{
	witness_list_t *witnesses;

	if (tsd == NULL)
		return;
	if (witness->rank == WITNESS_RANK_OMIT)
		return;

	witness_assert_owner(tsd, witness);

	witnesses = tsd_witnessesp_get(tsd);
	ql_remove(witnesses, witness, link);
}

#ifdef JEMALLOC_JET
#undef witness_owner_error
#define	witness_owner_error JEMALLOC_N(witness_owner_error_impl)
#endif
static void
witness_owner_error(const witness_t *witness)
{

	malloc_printf("<jemalloc>: Should own %s(%u)\n", witness->name,
	    witness->rank);
	abort();
}
#ifdef JEMALLOC_JET
#undef witness_owner_error
#define	witness_owner_error JEMALLOC_N(witness_owner_error)
witness_owner_error_t *witness_owner_error =
    JEMALLOC_N(witness_owner_error_impl);
#endif

void
witness_assert_owner(tsd_t *tsd, const witness_t *witness)
{
	witness_list_t *witnesses;
	witness_t *w;

	if (tsd == NULL)
		return;
	if (witness->rank == WITNESS_RANK_OMIT)
		return;

	witnesses = tsd_witnessesp_get(tsd);
	ql_foreach(w, witnesses, link) {
		if (w == witness)
			return;
	}
	witness_owner_error(witness);
}

#ifdef JEMALLOC_JET
#undef witness_not_owner_error
#define	witness_not_owner_error JEMALLOC_N(witness_not_owner_error_impl)
#endif
static void
witness_not_owner_error(const witness_t *witness)
{

	malloc_printf("<jemalloc>: Should not own %s(%u)\n", witness->name,
	    witness->rank);
	abort();
}
#ifdef JEMALLOC_JET
#undef witness_not_owner_error
#define	witness_not_owner_error JEMALLOC_N(witness_not_owner_error)
witness_not_owner_error_t *witness_not_owner_error =
    JEMALLOC_N(witness_not_owner_error_impl);
#endif

void
witness_assert_not_owner(tsd_t *tsd, const witness_t *witness)
{
	witness_list_t *witnesses;
	witness_t *w;

	if (tsd == NULL)
		return;
	if (witness->rank == WITNESS_RANK_OMIT)
		return;

	witnesses = tsd_witnessesp_get(tsd);
	ql_foreach(w, witnesses, link) {
		if (w == witness)
			witness_not_owner_error(witness);
	}
}

#ifdef JEMALLOC_JET
#undef witness_lockless_error
#define	witness_lockless_error JEMALLOC_N(witness_lockless_error_impl)
#endif
static void
witness_lockless_error(const witness_list_t *witnesses)
{
	witness_t *w;

	malloc_printf("<jemalloc>: Should not own any locks:");
	ql_foreach(w, witnesses, link) {
		malloc_printf(" %s(%u)", w->name, w->rank);
	}
	malloc_printf("\n");
	abort();
}
#ifdef JEMALLOC_JET
#undef witness_lockless_error
#define	witness_lockless_error JEMALLOC_N(witness_lockless_error)
witness_lockless_error_t *witness_lockless_error =
    JEMALLOC_N(witness_lockless_error_impl);
#endif

void
witness_assert_lockless(tsd_t *tsd)
{
	witness_list_t *witnesses;
	witness_t *w;

	if (tsd == NULL)
		return;

	witnesses = tsd_witnessesp_get(tsd);
	w = ql_last(witnesses, link);
	if (w != NULL) {
		witness_lockless_error(witnesses);
	}
}

void
witnesses_cleanup(tsd_t *tsd)
{

	witness_assert_lockless(tsd);

	/* Do nothing. */
}
