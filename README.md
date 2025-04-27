# simpcsv

SimpCSV is a simple and lightweight CSV parsing library in C. It is designed to be easy to use and fast enough for most needs (< 5 GB CSV files).

This library does not check if:
- The CSV file exists.
- The CSV file is properly formatted.
- You have enough memory to parse the CSV file.

Any segmentation faults that occur are probably the result of the above.

# Table of Contents

*    [Example](#example)
*    [Benchmark](#benchmark)
*    [Usage](#usage)
*    [Docs](#docs)
*    [Contributing](#contributing)
*    [License](#license)

# Example

```c
#include <stdio.h>

int main(void)
{
    SimpCSVHandle* handle = simpcsv_open_file("test.csv", '"', ',', '\n');

    simpcsv_count_rows_and_cols(handle);

    for (size_t ri = 0; ri < handle->m_number_of_rows; ri++)
    {
        for (size_t ci = 0; ci < handle->m_number_of_cols; ci++)
        {
        // do something...
        }
    }

    simpcsv_close_file(handle);

    return 0;
}
```

# Benchmark

The benchmark showcases the time it takes to iterate over every cell in the data set on my system.

You can see my system specs below.

| Part            | Info                                      |
| --------------- | ----------------------------------------- |
| Processor       | Intel(R) Core(TM) i5-6600K CPU @ 3.50GHz  |
| Installed RAM   | 8 GB (7.8 GB usable)                      |
| SSD             | SanDisk Ultra II 480GB                    |
| Swap partition  | 2GB                                       |
| OS              | Arch Linux x86_64                         |
| Kernel          | 6.14.2-arch1-1                            |

Below, you can see information about the data set and the time it took to iterate over it.

| Dataset                                                                                                                                                                      | File Size | Time   | Rows       | Columns |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------- | ------ | ---------- | ------- |
| [Amazon reviews](https://www.kaggle.com/datasets/kritanjalijain/amazon-reviews?select=train.csv)                                                                             | 1.59GB    | 0.5s   | 3,600,000  | 3       |
| [Title-Based Semantic Subject Indexing](https://www.kaggle.com/datasets/hsrobo/titlebased-semantic-subject-indexing?select=pubmed.csv)                                       | 3.99GB    | 1.35s  | 12,834,027 | 4       |
| [eCommerce behavior data](https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-data-from-multi-category-store?select=2019-Oct.csv)                                   | 5.67GB    | 10.12s | 42,448,765 | 9       |
| [Chess matches database (Lichess)](https://www.kaggle.com/datasets/aharon1377/lichess-games-played-in-the-first-trimester-2020?select=lichess_db_standard_rated_2020-01.csv) | 6.06GB    | 11.26s | 46,737,780 | 15      |
| [Seattle Checkouts by Title](https://www.kaggle.com/datasets/city-of-seattle/seattle-checkouts-by-title?select=checkouts-by-title.csv)                                       | 7.65GB    | 14.95s | 34,892,624 | 11      |

I was unable to test bigger CSV files as my system memory is insufficient.

As you can see from the times, the library works extremely well on smaller files.
The library iterates through cells linearly, therefore the time taken increases greatly as the size and number of cells also increase.
This does not matter and will not be changed by me (unless needed) as it is a lot faster than required for the purpose that I'm using it for.

# Usage

To use the library, just add the header and source file into your repository, and adjust your build configuration accordingly. 
This project does not provide a built library object or a script to build it into one, as the library is lightweight enough to import directly into your repository.
If you NEED to compile this library into a library object, you would need to do that yourself.

# Docs

Every single function is annotated with comments in both the header file and the source file. The comments in the header file contain a quick brief of what each function does. The comments in the source file
contain more detailed documentation of the function's parameters and return value. The quick example at the top under "Example" also shows you the general layout of a program using SimpCSV and how to iterate through all the cells in a CSV file.

Currently, this project has been developed and tested on Linux (Arch) machines only.

# Contributing

Feel free to contribute to the project and improve it. There are no strict rules regarding contribution.

# License

The project is available under the [MIT](https://opensource.org/licenses/MIT) license.

<sub> Copyright (C) David Filiks </sub>
