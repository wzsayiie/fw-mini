#include "clambda.h"

const CComparableLambda::Id NoneId = { nullptr };

bool CComparableLambda::operator< (const CComparableLambda &a) const { return cmp(a._id) <  0; }
bool CComparableLambda::operator==(const CComparableLambda &a) const { return cmp(a._id) == 0; }
bool CComparableLambda::operator!=(const CComparableLambda &a) const { return cmp(a._id) != 0; }

bool CComparableLambda::operator==(nullptr_t) const { return cmp(NoneId) == 0; }
bool CComparableLambda::operator!=(nullptr_t) const { return cmp(NoneId) != 0; }
CComparableLambda::operator bool  ()          const { return cmp(NoneId) != 0; }

void CComparableLambda::setId(void *a, void *b, void *c) {
	_id[0] = a;
	_id[1] = b;
	_id[2] = c;
}

void CComparableLambda::setRandomId() {
	static union {
		void   *ptr;
		int32_t num;
	}
	acc = { nullptr };

	acc.num++;

	memset(_id, 0, sizeof(Id));
	_id[0] = acc.ptr;
}

int CComparableLambda::cmp(const Id &that) const {
	size_t count = sizeof(Id) / sizeof(void *);

	for (size_t n = 0; n < count; ++n) {
		if (_id[n] != that[n]) {
			return _id[n] < that[n] ? -1 : 1;
		}
	}
	return 0;
}
