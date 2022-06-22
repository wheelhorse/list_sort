#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define TEST_LIST_LEN 8

# define likely(x)	__builtin_expect(!!(x), 1)
# define unlikely(x)	__builtin_expect(!!(x), 0)


#define container_of(ptr, type, member) ({				\
	void *__mptr = (void *)(ptr);					\
	((type *)(__mptr - offsetof(type, member))); })

struct list_head {
	struct list_head *next, *prev;
};

static void dump_list_prev(int count, struct list_head *head, const char *name);
static void dump_list_next(int count, struct list_head *head, const char *name);
static void dump_list(struct list_head *head);

static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}



typedef int __attribute__((nonnull(2,3))) (*cmp_func)(void *,
		struct list_head const *, struct list_head const *);

/*
 * Returns a list organized in an intermediate format suited
 * to chaining of merge() calls: null-terminated, no reserved or
 * sentinel head node, "prev" links not maintained.
 */
__attribute__((nonnull(2,3,4)))
static struct list_head *merge(void *priv, cmp_func cmp,
				struct list_head *a, struct list_head *b)
{
	struct list_head *head, **tail = &head;

	for (;;) {
		/* if equal, take 'a' -- important for sort stability */
		if (cmp(priv, a, b) <= 0) {
			*tail = a;
			tail = &a->next;
			a = a->next;
			if (!a) {
				*tail = b;
				break;
			}
		} else {
			*tail = b;
			tail = &b->next;
			b = b->next;
			if (!b) {
				*tail = a;
				break;
			}
		}
	}
	return head;
}

/*
 * Combine final list merge with restoration of standard doubly-linked
 * list structure.  This approach duplicates code from merge(), but
 * runs faster than the tidier alternatives of either a separate final
 * prev-link restoration pass, or maintaining the prev links
 * throughout.
 */
__attribute__((nonnull(2,3,4,5)))
static void merge_final(void *priv, cmp_func cmp, struct list_head *head,
			struct list_head *a, struct list_head *b)
{
	struct list_head *tail = head;
	u_int8_t count = 0;

	for (;;) {
		/* if equal, take 'a' -- important for sort stability */
		if (cmp(priv, a, b) <= 0) {
			tail->next = a;
			a->prev = tail;
			tail = a;
			a = a->next;
			if (!a)
				break;
		} else {
			tail->next = b;
			b->prev = tail;
			tail = b;
			b = b->next;
			if (!b) {
				b = a;
				break;
			}
		}
	}

	/* Finish linking remainder of list b on to tail */
	tail->next = b;
	do {
		/*
		 * If the merge is highly unbalanced (e.g. the input is
		 * already sorted), this loop may run many iterations.
		 * Continue callbacks to the client even though no
		 * element comparison is needed, so the client's cmp()
		 * routine can invoke cond_resched() periodically.
		 */
		if (unlikely(!++count))
			cmp(priv, b, b);
		b->prev = tail;
		tail = b;
		b = b->next;
	} while (b);

	/* And the final links to make a circular doubly-linked list */
	tail->next = head;
	head->prev = tail;
}

/**
 * list_sort - sort a list
 * @priv: private data, opaque to list_sort(), passed to @cmp
 * @head: the list to sort
 * @cmp: the elements comparison function
 *
 * The comparison funtion @cmp must return > 0 if @a should sort after
 * @b ("@a > @b" if you want an ascending sort), and <= 0 if @a should
 * sort before @b *or* their original order should be preserved.  It is
 * always called with the element that came first in the input in @a,
 * and list_sort is a stable sort, so it is not necessary to distinguish
 * the @a < @b and @a == @b cases.
 *
 * This is compatible with two styles of @cmp function:
 * - The traditional style which returns <0 / =0 / >0, or
 * - Returning a boolean 0/1.
 * The latter offers a chance to save a few cycles in the comparison
 * (which is used by e.g. plug_ctx_cmp() in block/blk-mq.c).
 *
 * A good way to write a multi-word comparison is::
 *
 *	if (a->high != b->high)
 *		return a->high > b->high;
 *	if (a->middle != b->middle)
 *		return a->middle > b->middle;
 *	return a->low > b->low;
 *
 *
 * This mergesort is as eager as possible while always performing at least
 * 2:1 balanced merges.  Given two pending sublists of size 2^k, they are
 * merged to a size-2^(k+1) list as soon as we have 2^k following elements.
 *
 * Thus, it will avoid cache thrashing as long as 3*2^k elements can
 * fit into the cache.  Not quite as good as a fully-eager bottom-up
 * mergesort, but it does use 0.2*n fewer comparisons, so is faster in
 * the common case that everything fits into L1.
 *
 *
 * The merging is controlled by "count", the number of elements in the
 * pending lists.  This is beautiully simple code, but rather subtle.
 *
 * Each time we increment "count", we set one bit (bit k) and clear
 * bits k-1 .. 0.  Each time this happens (except the very first time
 * for each bit, when count increments to 2^k), we merge two lists of
 * size 2^k into one list of size 2^(k+1).
 *
 * This merge happens exactly when the count reaches an odd multiple of
 * 2^k, which is when we have 2^k elements pending in smaller lists,
 * so it's safe to merge away two lists of size 2^k.
 *
 * After this happens twice, we have created two lists of size 2^(k+1),
 * which will be merged into a list of size 2^(k+2) before we create
 * a third list of size 2^(k+1), so there are never more than two pending.
 *
 * The number of pending lists of size 2^k is determined by the
 * state of bit k of "count" plus two extra pieces of information:
 *
 * - The state of bit k-1 (when k == 0, consider bit -1 always set), and
 * - Whether the higher-order bits are zero or non-zero (i.e.
 *   is count >= 2^(k+1)).
 *
 * There are six states we distinguish.  "x" represents some arbitrary
 * bits, and "y" represents some arbitrary non-zero bits:
 * 0:  00x: 0 pending of size 2^k;           x pending of sizes < 2^k
 * 1:  01x: 0 pending of size 2^k; 2^(k-1) + x pending of sizes < 2^k
 * 2: x10x: 0 pending of size 2^k; 2^k     + x pending of sizes < 2^k
 * 3: x11x: 1 pending of size 2^k; 2^(k-1) + x pending of sizes < 2^k
 * 4: y00x: 1 pending of size 2^k; 2^k     + x pending of sizes < 2^k
 * 5: y01x: 2 pending of size 2^k; 2^(k-1) + x pending of sizes < 2^k
 * (merge and loop back to state 2)
 *
 * We gain lists of size 2^k in the 2->3 and 4->5 transitions (because
 * bit k-1 is set while the more significant bits are non-zero) and
 * merge them away in the 5->2 transition.  Note in particular that just
 * before the 5->2 transition, all lower-order bits are 11 (state 3),
 * so there is one list of each smaller size.
 *
 * When we reach the end of the input, we merge all the pending
 * lists, from smallest to largest.  If you work through cases 2 to
 * 5 above, you can see that the number of elements we merge with a list
 * of size 2^k varies from 2^(k-1) (cases 3 and 5 when x == 0) to
 * 2^(k+1) - 1 (second merge of case 5 when x == 2^(k-1) - 1).
 */
__attribute__((nonnull(2,3)))
void list_sort(void *priv, struct list_head *head,
		int (*cmp)(void *priv, struct list_head *a,
			struct list_head *b))
{
	struct list_head *list = head->next, *pending = NULL;
	size_t count = 0;	/* Count of pending */

	if (list == head->prev)	/* Zero or one elements */
		return;

	/* Convert to a null-terminated singly-linked list. */
	head->prev->next = NULL;

	/*
	 * Data structure invariants:
	 * - All lists are singly linked and null-terminated; prev
	 *   pointers are not maintained.
	 * - pending is a prev-linked "list of lists" of sorted
	 *   sublists awaiting further merging.
	 * - Each of the sorted sublists is power-of-two in size.
	 * - Sublists are sorted by size and age, smallest & newest at front.
	 * - There are zero to two sublists of each size.
	 * - A pair of pending sublists are merged as soon as the number
	 *   of following pending elements equals their size (i.e.
	 *   each time count reaches an odd multiple of that size).
	 *   That ensures each later final merge will be at worst 2:1.
	 * - Each round consists of:
	 *   - Merging the two sublists selected by the highest bit
	 *     which flips when count is incremented, and
	 *   - Adding an element from the input as a size-1 sublist.
	 */
	do {
		size_t bits;
		struct list_head **tail = &pending;

		/* Find the least-significant clear bit in count */
		for (bits = count; bits & 1; bits >>= 1)
			tail = &(*tail)->prev;
		/* Do the indicated merge */
		if (likely(bits)) {
			struct list_head *a = *tail, *b = a->prev;

			a = merge(priv, (cmp_func)cmp, b, a);
			/* Install the merged result in place of the inputs */
			a->prev = b->prev;
			*tail = a;
		}

		/* Move one element from input list to pending */
		list->prev = pending;
		pending = list;
		list = list->next;
		pending->next = NULL;
		count++;
        dump_list_prev(count, pending, "pending lists");
        dump_list_next(count, list, "\e\[0;95;1mremaing elements");
        printf("\n\n");
	} while (list);


	/* End of input; merge together all the pending lists. */
	list = pending;
	pending = pending->prev;
    count = 0xFF;
	for (;;) {
		struct list_head *next = pending->prev;

        dump_list_prev(count, pending, "pending lists");
        dump_list_next(count, list, "\e\[0;96;1mgenerate result");
        printf("\n\n");
		if (!next)
			break;
		list = merge(priv, (cmp_func)cmp, pending, list);
		pending = next;
        count--;
	}
	/* The final merge, rebuilding prev links */
	merge_final(priv, (cmp_func)cmp, head, pending, list);
}

/* Test code below */

struct tnode {
	struct list_head list;
	int value; /* char value to sort */
	unsigned int serial; /* index of the original location */
};

static void check(void *test, struct tnode *ela, struct tnode *elb)
{
    /* nothing to do for now */

}

/* `priv` is the test pointer so check() can fail the test if the list is invalid. */
static int my_cmp(void *priv, const struct list_head *a, const struct list_head *b)
{
	struct tnode *ela, *elb;

	ela = container_of(a, struct tnode, list);
	elb = container_of(b, struct tnode, list);

	check(priv, ela, elb);
	return ela->value - elb->value;
}

static void dump_list_prev(int count, struct list_head *head, const char *name)
{
	struct tnode *el;
	struct list_head *cur;
    char subname[64] = "";

            
    printf("\e\[0;32;1m%s start-----cnt:%02x-----\e\[m\n", name, count);
	for (cur = head; (cur != NULL); cur = cur->prev) {
        snprintf(subname, 32, "\e\[0;99;1m    sublist@0x%08x", cur);
        dump_list_next(count, cur, subname);
	}
    printf("\e\[0;32;1m%s  end*****cnt:%02x*****\e\[m\n", name, count);
}

static void dump_list_next(int count, struct list_head *head, const char *name)
{
	struct tnode *el;
	struct list_head *cur;

            
    printf("%s start-----cnt:%02x-----\n", name, count);
	for (cur = head; (cur != NULL); cur = cur->next) {
		el = container_of(cur, struct tnode, list);
        printf("        %3d -> %c\n", el->serial, (char) el->value);
	}
    printf("%s  end*****cnt:%02x*****\e\[m\n", name, count);
}
static void dump_list(struct list_head *head)
{
	struct tnode *el;
	struct list_head *cur;

	for (cur = head->next; (cur != head) && (cur->next != NULL); cur = cur->next) {
		el = container_of(cur, struct tnode, list);
        printf("%3d -> %c\n", el->serial, (char) el->value);
	}
}

static void list_sort_test(void)
{
	int i, count = 1;
	struct tnode *el, **elts;
	struct list_head *cur;
	struct list_head head = {&head, &head};

    elts = calloc(TEST_LIST_LEN, sizeof(*elts));
    if (!elts)
        perror("Null el set\n");

    printf("#################################\n");
    printf("########### Initial #############\n");
	for (i = 0; i < TEST_LIST_LEN; i++) {
        el = calloc(1, sizeof(*el));
        if (!el)
            perror("Null el\n");
		 /* force some equivalencies */
		el->value = 65 + random() % 26;
		el->serial = i;
		elts[i] = el;
		list_add_tail(&el->list, &head);
	}

    dump_list(&head);
	list_sort((void *)0, &head, my_cmp);

    printf("#################################\n");
    printf("########### After Sort ##########\n");
    dump_list(&head);
}

int main(int argc, char **argv)
{
    srandom(1);
    list_sort_test();
    return 0;
}
