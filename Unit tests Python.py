Python 3.13.5 (tags/v3.13.5:6cb20a2, Jun 11 2025, 16:15:46) [MSC v.1943 64 bit (AMD64)] on win32
Enter "help" below or click "Help" above for more information.
python
import unittest
import random


def bubble_sort_bad(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr


def merge_sort_good(arr):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort_good(arr[:mid])
    right = merge_sort_good(arr[mid:])
    result = []
    i = j = 0
    while i < len(left) and j < len(right):
        if left[i] < right[j]:
            result.append(left[i]); i += 1
        else:
            result.append(right[j]); j += 1
    result.extend(left[i:]); result.extend(right[j:])
    return result


class TestSorting(unittest.TestCase):

    def test_from_task_examples(self):
        # Примеры прямо из условия
        self.assertEqual(bubble_sort_bad([64, 34, 25, 12, 22, 11, 90]),
                         [11, 12, 22, 25, 34, 64, 90])
        self.assertEqual(merge_sort_good([38, 27, 43, 3, 9, 82, 10]),
                         [3, 9, 10, 27, 38, 43, 82])

    def test_empty_and_single_element(self):
        # Граничные случаи: пустой массив и один элемент
        self.assertEqual(bubble_sort_bad([]), [])
        self.assertEqual(bubble_sort_bad([42]), [42])
        self.assertEqual(merge_sort_good([]), [])
        self.assertEqual(merge_sort_good([42]), [42])

    def test_duplicates_and_negatives(self):
        # Дубликаты и отрицательные числа
        self.assertEqual(bubble_sort_bad([2, 2, 2, 2]), [2, 2, 2, 2])
        self.assertEqual(merge_sort_good([-3, 7, -1, 0, 7]), [-3, -1, 0, 7, 7])

    def test_sorted_reversed_and_in_place(self):
        # Уже отсортированный, обратно отсортированный; merge_sort не мутирует вход
        original = [1, 2, 3, 4, 5]
        self.assertEqual(bubble_sort_bad(list(range(50, 0, -1))), list(range(1, 51)))
        self.assertEqual(merge_sort_good(original), original)
        self.assertEqual(original, [1, 2, 3, 4, 5])

    def test_random_large_arrays_agree_with_reference(self):
        # 1000 случайных массивов: результат обязан совпадать с эталоном sorted()
        random.seed(123)
        for _ in range(1000):
            n = random.randint(0, 50)
            arr = [random.randint(-100, 100) for _ in range(n)]
            self.assertEqual(bubble_sort_bad(list(arr)), sorted(arr))
            self.assertEqual(merge_sort_good(arr), sorted(arr))

    def test_merge_sort_performance_on_large_input(self):
        # Merge sort справляется с 100000 элементов (пузырёк на таком завис бы)
        random.seed(7)
        arr = [random.randint(-10**6, 10**6) for _ in range(100_000)]
        self.assertEqual(merge_sort_good(arr), sorted(arr))


if __name__ == "__main__":
    unittest.main(verbosity=2)

SyntaxError: multiple statements found while compiling a single statement
import unittest
import random
def linear_search_bad(arr, target):
    for i in range(len(arr)):
        if arr[i] == target:
            return i
    return -1
def binary_search_good(arr, target):
    left, right = 0, len(arr) - 1
    while left <= right:
        mid = (left + right) // 2
        if arr[mid] == target:
            return mid
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return -1
class TestSearch(unittest.TestCase):
    def assert_found(self, arr, target, result):
        # При дублях любой найденный индекс валиден, если указывает на цель
        self.assertNotEqual(result, -1)
        self.assertEqual(arr[result], target)
    def test_examples_from_task(self):
        # Примеры прямо из условия
        self.assertEqual(linear_search_bad([1, 3, 5, 7, 9], 7), 3)
        self.assertEqual(linear_search_bad([10, 20, 30, 40], 25), -1)
        self.assertEqual(binary_search_good([1, 3, 5, 7, 9], 7), 3)
        self.assertEqual(binary_search_good([2, 4, 6, 8, 10], 5), -1)
    def test_empty_and_single_element(self):
        # Пустой массив и массив из одного элемента
        self.assertEqual(linear_search_bad([], 5), -1)
        self.assertEqual(binary_search_good([], 5), -1)
        self.assertEqual(linear_search_bad([5], 5), 0)
        self.assertEqual(binary_search_good([5], 5), 0)
        self.assertEqual(binary_search_good([5], 4), -1)
    def test_first_last_and_boundaries(self):
        # Цель на границах: первый, последний, вне диапазона слева и справа
        arr = [1, 3, 5, 7, 9]
        self.assertEqual(linear_search_bad(arr, 1), 0)
        self.assertEqual(binary_search_good(arr, 1), 0)
        self.assertEqual(linear_search_bad(arr, 9), 4)
...         self.assertEqual(binary_search_good(arr, 9), 4)
...         self.assertEqual(linear_search_bad(arr, 0), -1)
...         self.assertEqual(binary_search_good(arr, 0), -1)
...         self.assertEqual(linear_search_bad(arr, 100), -1)
...         self.assertEqual(binary_search_good(arr, 100), -1)
...     def test_duplicates(self):
...         # Линейный поиск обязан вернуть первый вхождение; бинарный — любой валидный
...         arr = [1, 1, 1, 1, 1]
...         self.assertEqual(linear_search_bad(arr, 1), 0)
...         self.assert_found(arr, 1, binary_search_good(arr, 1))
...         arr2 = [-5, -2, -2, 0, 3, 3, 3, 8]
...         self.assertEqual(linear_search_bad(arr2, 3), 4)
...         self.assert_found(arr2, 3, binary_search_good(arr2, 3))
...     def test_negatives_and_agreement_with_reference(self):
...         # Обе реализации согласованы друг с другом на случайных отсортированных массивах
...         random.seed(42)
...         for _ in range(500):
...             n = random.randint(0, 60)
...             arr = sorted(random.randint(-100, 100) for _ in range(n))
...             for target in random.sample(range(-105, 105), 10):
...                 lin = linear_search_bad(arr, target)
...                 bin_ = binary_search_good(arr, target)
...                 self.assertEqual((lin == -1), (bin_ == -1))
...                 if lin != -1:
...                     self.assertEqual(arr[lin], target)
...                     self.assertEqual(arr[bin_], target)
...     def test_binary_search_on_large_sorted_array(self):
...         # Бинарный поиск обязан справляться с миллионом элементов за приемлемое время
...         arr = list(range(0, 2_000_000, 2))          # 1 000 000 элементов
...         self.assertEqual(binary_search_good(arr, 0), 0)
...         self.assertEqual(binary_search_good(arr, 1_999_998), 999_999)
...         self.assertEqual(binary_search_good(arr, 999_999), -1)  # нечётного нет
...         self.assertEqual(linear_search_bad(arr[:1000], 500), 250)  # линейный на срезе
... if __name__ == "__main__":
...     unittest.main(verbosity=2)
