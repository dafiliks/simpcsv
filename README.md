# simpcsv
CSV Parsing Library in C

This library is not responsible for checking that *YOUR* CSV file is in the correct format.
For example, if you specified that your quote character is `"` and then there was a cell that looked like the following:

```
Data1, ["Example"], Data3

             ^
             |
             |
```

Then the whole file would be parsed incorrectly, as if you are using quote characters, the parser expects the quote (if used) to be immediately after the delimiter.
Instead you want your cells to be correctly formatted:

```
Data1, "["Example"]", Data3
```

This achieves the correct result and gets correctly parsed.

<sub> Copyright (C) David Filiks </sub>
