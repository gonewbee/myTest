from sort_base import SortBase

class MergeSort(SortBase):

    aux = []

    @classmethod
    def merge(cls, a, lo, mid, hi):
        i = lo
        j = mid + 1
        aux = cls.aux
        aux[lo:hi+1] = a[lo:hi+1]
        # print("%d %d %d" % (lo, mid, hi))
        # print(aux)
        for k in range(lo, hi+1):
            if i > mid:
                a[k] = aux[j]
                j += 1
            elif j > hi:
                a[k] = aux[i]
                i += 1
            elif MergeSort.less(aux[j], aux[i]):
                a[k] = aux[j]
                j += 1
            else:
                a[k] = aux[i]
                i += 1


