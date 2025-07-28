Noto fonts are provided under the Open Font License.

List of full font files:
- NotoNaskhArabic-Regular.ttf
- NotoSerif-Regular.ttf
- NotoSerifArmenian-Regular.ttf
- NotoSerifDevanagari-Regular.ttf
- NotoSerifEthiopic-Regular.ttf
- NotoSerifGeorgian-Regular.ttf
- NotoSerifHebrew-Regular.ttf
- NotoSerifJP-Regular.ttf
- NotoSerifKR-Regular.ttf
- NotoSerifKhmer-Regular.ttf
- NotoSerifMyanmar-Regular.ttf
- NotoSerifTC-Regular.ttf
- NotoSerifTamil-Regular.ttf
- NotoSerifThai-Regular.ttf
- NotoSerifTibetan-Regular.ttf

In order to save space, large font files were subsetted to the data required to run the example using pyftsubset.

```
pip install fonttools uharfbuzz
pyftsubset <font file> --text-file=../Text.cpp --verbose --harfbuzz-repacker --legacy-kern --passthrough-tables --layout-features='*'
```

List of subsetted font files:
- NotoSerifJP-Regular.subset.ttf
- NotoSerifKR-Regular.subset.ttf
- NotoSerifTC-Regular.subset.ttf

Attempting to use these subsetted font files to display any other text will result in missing glyphs.
The full font files including other face variations can be downloaded from https://fonts.google.com/noto.
