
## Chrono-cpp
A C++ library for natural language relative date parsing based on chrono.js

This library is designed with rules to enable extracting absolute date information from text.

The library supports formats such as:
* today, tomorrow, yesterday
* last friday, next tuesday
* 7 years ago
* 4 days from now

...with additional functionality in the works.
 


### Building  

##### NOTE: 
Need to have bazel installed.

```bazel build main```

### Running

```./main <phrase with relative time information> <reference date (optional)>```

If the reference date is not provided, the program wil default to the current day and time.


[wip]
