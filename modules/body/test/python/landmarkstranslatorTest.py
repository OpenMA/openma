import ma.body
import unittest

class LandmarksTranslatorTest(unittest.TestCase):
  def test_convert(self):
    translator = ma.body.LandmarksTranslator('LandmarksTranslator',[
      ['LFHD', 'L.HF'],
      ['LBHD', 'L.HB'],
      ['RFHD', 'R.HF'],
      ['RBHD', 'R.HB'],
      ['C7'  , 'C7'  ],
    ])
    self.assertEqual(translator.convert('LFHD'), 'L.HF')
    self.assertEqual(translator.convert('LBHD'), 'L.HB')
    self.assertEqual(translator.convert('RFHD'), 'R.HF')
    self.assertEqual(translator.convert('RBHD'), 'R.HB')
    self.assertEqual(translator.convert('C7'), 'C7')
    self.assertEqual(translator.convert('RBHD_'), 'RBHD_')
    self.assertEqual(translator.convert('T10'), 'T10')
  
  def test_convert_if_exists(self):
    translator = ma.body.LandmarksTranslator('LandmarksTranslator',[
      ['LFHD', 'L.HF'],
      ['LBHD', 'L.HB'],
      ['RFHD', 'R.HF'],
      ['RBHD', 'R.HB'],
      ['C7'  , 'C7'  ],
    ])
    self.assertEqual(translator.convertIfExists('LFHD'), 'L.HF')
    self.assertEqual(translator.convertIfExists('LBHD'), 'L.HB')
    self.assertEqual(translator.convertIfExists('RFHD'), 'R.HF')
    self.assertEqual(translator.convertIfExists('RBHD'), 'R.HB')
    self.assertEqual(translator.convertIfExists('C7'), 'C7')
    self.assertEqual(translator.convertIfExists('RBHD_'), '')
    self.assertEqual(translator.convertIfExists('T10'), '')