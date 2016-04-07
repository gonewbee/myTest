import unittest
import cfuns


class cfunsTest(unittest.TestCase):
    """docstring for cfunsTest"""

    def test_add(self):
        self.assertEqual(cfuns.add(11, 22), 33)

    def test_getLen(self):
        s = 'abcdef'
        self.assertEqual(cfuns.getLen(s), len(s))

if __name__ == '__main__':
    unittest.main()
